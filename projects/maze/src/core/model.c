#include "model.h"
#include <stdio.h>





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







MODEL* MODEL_easy(MODEL* model, MESH* mesh, void* data, GLuint shader, bool stream) {
    static int count = 0;
    
    *model = (MODEL){
        .color = (COLOR){1.0f, 1.0f, 1.0f, 1.0f},
        .mesh = mesh,
        .id = count++,
        .data = data,
        .visable = true,
        .gl_usage = GL_STATIC_DRAW,
        .update_call = NULL,
        .draw_call = NULL,
        .shader_prog = shader,
        .view_mat = GLM_MAT4_IDENTITY_INIT,
    };

    if (!stream)
        model->gl_usage = GL_STATIC_DRAW;
    else
        model->gl_usage = GL_STREAM_DRAW;
        
    return model;
}


MODEL* MODEL_init(MODEL* model) {
    // select shader program to use
    glUseProgram(model->shader_prog);
    
    // create buffer
    glGenBuffers(1, &model->vert_buff);

    // store locations for values
    // TODO: this is the sort of thing to be stored in a shader object
    model->u_mod_mat_loc = glGetUniformLocation(model->shader_prog, "u_mod_mat");
    model->u_color_loc   = glGetUniformLocation(model->shader_prog, "u_color");
    // TODO: add error checking for this and others. As it can return -1 if invalid
    model->vert_pos_loc  = glGetAttribLocation(model->shader_prog,  "vert_pos");
    ASSERT(model->vert_pos_loc >= 0, NULL, "no location for vert_pos_loc\n");

    // get u_light_norm and map location (if it exists)
    model->u_light_norm_loc = glGetUniformLocation(model->shader_prog, "u_light_norm");
    model->u_light_map_loc = glGetUniformLocation(model->shader_prog, "u_light_map");

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
        (GLsizeiptr)MESH_sizeof(model->mesh),
        model->mesh->data,
        model->gl_usage /*data_usage*/
    );
    
    return model;
}






int MODEL_draw(MODEL* model, double t){
    if (!model->visable)
        return 0;

    // select shader program to use
    // disabled because I think setting it will refresh all of the 
    // uniforms, including one that is being set before this function 
    // is called.
    glUseProgram(model->shader_prog);

    glBindBuffer(GL_ARRAY_BUFFER, model->vert_buff);

    // call object specific draw function
    if (model->draw_call != NULL)
        model->draw_call(model, t);

    switch (model->gl_usage) {
            
        case GL_STREAM_DRAW:
            glBufferSubData(
                GL_ARRAY_BUFFER,
                0,
                (GLsizeiptr)MESH_sizeof(model->mesh),
                model->mesh->data
            );
        break;
    }

    // set vertex attributes
    // can potentially be different for each model
    // so it has to be set each draw call.
    // TODO: in a more efficient system, this would only
    // need to be set up once for each shader
    // so you would divide models up per-shader


    GLint attrib_size = 0;
    GLenum attrib_type = GL_FLOAT;
    GLsizei attrib_stride = 0*sizeof(GLfloat);
    

    switch (model->mesh->type) {
        case MESHTYPE_1D_PACKED:
            attrib_size = 1;
            break;
        case MESHTYPE_2D_PACKED:
            attrib_size = 2;
            break;
        case MESHTYPE_3D_PACKED:
            attrib_size = 3;
            break;
    }
    
    glVertexAttribPointer(
        model->vert_pos_loc,
        attrib_size,
        attrib_type,
        GL_FALSE,
        attrib_stride,
        (void*)0            // offset from vertex. The cast is confusing
    );


    // TODO: Why does this need to be enabled? Is it possible to have
    // multiple vertex attribute arrays, and then just activate one of them?
    glEnableVertexAttribArray(model->vert_pos_loc);

    // set uniform u_color
    if (model->u_color_loc >= 0) 
        glUniform4fv(model->u_color_loc, 1, model->color.raw);

    // set uniform model view matrix
    if (model->u_mod_mat_loc >= 0)
        glUniformMatrix4fv(model->u_mod_mat_loc, 
            1, GL_FALSE, (GLfloat*)model->view_mat);

    // set u_light_norm
    if (model->u_light_norm_loc >= 0)
        glUniform3fv(model->u_light_norm_loc, 1, world.light.dir.norm);

    // set u_light_map
    if (model->u_light_map_loc >= 0)
        glUniform2fv(model->u_light_map_loc, 1, world.light.dir.range);
    
    // send draw to queue
    glDrawArrays(model->mesh->mode, 0, model->mesh->verts);

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
