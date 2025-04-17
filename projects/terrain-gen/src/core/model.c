#include <stdio.h>

#include "model.h"
#include "shader.h"
#include "texture.h"





// TODO use this to create a function that updates all models, maybe.

/*void model_update_pipeline(double t, float dt) {
    // Alright. It looks like the multi-pass model is still
    // the most stable. 
    // Also, the jitteryness seems to be a product of 
    // perfectly elastic collisions in an approximate
    // enviroment with a ball pushing another ball into
    // a wall and demanding 

    //dt /= BEHAVE_PASSES;
    dt = 0.3;
    
    for (int i = 0; i < BEHAVE_PASSES; i++) {
        #if 0
        // This is the multi-pass model in order to 
        // remove bias from order of execution
        for (int i = 0; i < NUM_MODELS; i++)
            MODEL_update(models+i, t, dt);
        for (int i = 0; i < NUM_MODELS; i++) {
            // TODO: actually add this to a model callback or something
            // perhaps implement a multi-pass physics system (layers)
            behave_apply(behave+i, t, dt);
            model_transform(models+i);
        }
        #else
        // this is the single-pass model. Behavior is less equal,
        // but it should avoid some of the jank created by the previous model
        for (int i = 0; i < NUM_MODELS; i++) {
            MODEL_update(models+i, t, dt);
            behave_apply(behave+i, t, dt);
            model_transform(models+i);
        }
        /\*for (int i = 0; i < NUM_MODELS; i++) {
            behave_apply(behave+i, t, dt);
            model_transform(models+i);
        }*\/
        #endif
    }
}*/


int _model_init(MODEL* model);




int model_init(MODEL* model, MESH* mesh, TEXTURE* texture, bool stream) {
    static int count = 0;
    
    *model = (MODEL){
        .color = (COLOR){1.0f, 1.0f, 1.0f, 1.0f},
        .mesh = mesh,
        .id = count++,
        .data = NULL,
        .visable = true,
        .gl_usage = GL_STATIC_DRAW,
        .update_call = NULL,
        .draw_call = NULL,
        .view_mat = GLM_MAT4_IDENTITY_INIT,
        .texture = texture,
    };

    if (!stream)
        model->gl_usage = GL_STATIC_DRAW;
    else
        model->gl_usage = GL_STREAM_DRAW;

    _model_init(model);
        
    return 0;
}


int _model_init(MODEL* model) {
    // select shader program to use
    //glUseProgram(model->shader_prog);
    
    // create buffer
    glGenBuffers(1, &model->vert_buff);

    // store locations for values
    // T/ODO: this is the sort of thing to be stored in a shader object
    /*model->u_mod_mat_loc = glGetUniformLocation(model->shader_prog, "u_mod_mat");
    model->u_color_loc   = glGetUniformLocation(model->shader_prog, "u_color");
    // T/ODO: add error checking for this and others. As it can return -1 if invalid
    model->vert_pos_loc  = glGetAttribLocation(model->shader_prog,  "vert_pos");
    ASSERT(model->vert_pos_loc >= 0, NULL, "no location for vert_pos_loc\n");
    model->vert_norm_loc  = glGetAttribLocation(model->shader_prog,  "vert_norm");

    // get u_light_norm and map location (if it exists)
    model->u_light_norm_loc = glGetUniformLocation(model->shader_prog, "u_light_norm");
    model->u_light_map_loc = glGetUniformLocation(model->shader_prog, "u_light_map");*/

    /*GLenum data_usage;

    switch (model->type) {
    
        case MODELTYPE_STATIC:
            data_usage = GL_STATIC_DRAW;
            break;
            
        case MODELTYPE_DYNAMIC,
            data_usage = GL_STREAM_DRAW;
            break;
    }*/

    glBindBuffer(GL_ARRAY_BUFFER, model->vert_buff);
    glBufferData(
        GL_ARRAY_BUFFER,
        (GLsizeiptr)mesh_data_sizeof(model->mesh),
        model->mesh->data,
        model->gl_usage /*data_usage*/
    );
    
    return 0;
}





// NOTICE: only call this within a shader draw callback
int model_draw(MODEL* model, double t){
    if (!model->visable)
        return 0;

    glBindBuffer(GL_ARRAY_BUFFER, model->vert_buff);
    
    shader_setup_attributes(current_shader);

    // call object specific draw function
    // TODO eventually get rid of this.
    // objects should call this, not the other way around
    if (model->draw_call != NULL)
        model->draw_call(model, t);

    switch (model->gl_usage) {
            
        case GL_STREAM_DRAW:
            glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                (GLsizeiptr)mesh_data_sizeof(model->mesh),
                model->mesh->data
            );
        break;
    }




    // TODO: Why does this need to be enabled? Is it possible to have
    // multiple vertex attribute arrays, and then just activate one of them?
    //glEnableVertexAttribArray(model->vert_pos_loc);
    GLint vert_pos_loc = glGetAttribLocation(current_shader->program, "vert_pos");
    glEnableVertexAttribArray(vert_pos_loc);

    // set uniform u_color
    if (current_shader->u_color_loc >= 0) 
        glUniform4fv(current_shader->u_color_loc, 1, model->color.raw);

    // set uniform model view matrix
    if (current_shader->u_mod_mat_loc >= 0)
        glUniformMatrix4fv(current_shader->u_mod_mat_loc,
            1, GL_FALSE, (GLfloat*)model->view_mat);

    // set uniform normal matrix
    if (current_shader->u_norm_mat_loc > 0) {
        mat3 norm_mat;
        glm_mat4_pick3(model->view_mat, norm_mat);
        glm_mat3_inv(norm_mat, norm_mat);
        glm_mat3_transpose(norm_mat);
        
        glUniformMatrix3fv(current_shader->u_norm_mat_loc, 
            1, GL_FALSE, (GLfloat*)norm_mat);
    }

    if (model->texture != NULL)
        texture_bind(model->texture, current_shader, "tex0", GL_TEXTURE0);
    
    // send draw to queue
    switch (current_shader->drawtype) {
        case SHADER_DRAW_DEFAULT:
            glDrawArrays(model->mesh->mode, 0, model->mesh->verts);
            break;
        case SHADER_DRAW_WIREFRAME:
            glDrawArrays(GL_LINES, 0, model->mesh->verts);
            break;
        case SHADER_DRAW_POINTS:
            glDrawArrays(GL_POINTS, 0, model->mesh->verts);
            break;
    }

    return 0;
}



//*DEBUG*/ print_array_buff(model->mesh->verts, GL_FLOAT);

// void print_array_buff(GLint verts, GLenum type);

/*void print_array_buff(GLint verts, GLenum type) {
    if (verts <= 0)
        return;
    switch (type) {
        case GL_FLOAT: {
            GLfloat data[verts];
            glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(data), *data);
            printf("{");
            for (int i = 0; i < verts-1; i++)
                printf("%f, ", (float)data[i]);
            printf("%f}\n", data[verts-1]);
            break;
        }
    }
}*/






