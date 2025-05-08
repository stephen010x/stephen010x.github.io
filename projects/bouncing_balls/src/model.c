#include "model.h"
#include <stdio.h>


MODEL* MODEL_init(MODEL* model) {
    // create buffer
    glGenBuffers(1, &model->vert_buff);
    glBindBuffer(GL_ARRAY_BUFFER, model->vert_buff);
    glBufferData(
        GL_ARRAY_BUFFER, 
        (GLsizeiptr)MODEL_MESH_sizeof(model),
        model->mesh->data,
        GL_STATIC_DRAW
    );

    // store locations for values
    // TODO: this is the sort of thing to be stored in a shader object
    model->u_color_loc   = glGetUniformLocation(model->shader_prog, "u_color");
    model->u_mod_mat_loc = glGetUniformLocation(model->shader_prog, "u_mod_mat");
    // TODO: add error checking for this and others. As it can return -1 if invalid
    model->vert_pos_loc  = glGetAttribLocation(model->shader_prog,  "vert_pos");


    switch (model->drawtype) {
        case DRAWTYPE_2D_PLAIN:
            break;
        case DRAWTYPE_3D_PLAIN:
            // get u_light_norm location
            model->u_light_norm_loc = 
                glGetUniformLocation(model->shader_prog, "u_light_norm");

            // get u_light_map location
            model->u_light_map_loc = 
                glGetUniformLocation(model->shader_prog, "u_light_map");
            break;
    }

    return model;
}



int MODEL_draw(MODEL* model){
    if (!model->visable)
        return 0;

    //*DEBUG*/printf("%p, %p, %p\n", &model->mesh->data[0], &model->mesh->data[1], &model->mesh->data[2]);

    // bind buffer
    //glBindBuffer(GL_ARRAY_BUFFER, model->vert_buff);
    /*glBufferData(
        GL_ARRAY_BUFFER, 
        (GLsizeiptr)MESH_sizeof(model->mesh),
        model->mesh->data,
        GL_STATIC_DRAW
    );*/

    // set vertex attributes
    // can potentially be different for each model
    // so it has to be set each draw call.
    // TODO: in a more efficient system, this would only
    // need to be set up once for each shader
    // so you would divide models up per-shader
    switch (model->drawtype) {
        case DRAWTYPE_2D_PLAIN:
            glVertexAttribPointer(
                model->vert_pos_loc,
                2,
                GL_FLOAT,
                GL_FALSE,
                0*sizeof(GLfloat),
                (void*)0            // offset from vertex. The cast is confusing
            );
            break;
        case DRAWTYPE_3D_PLAIN:
            glVertexAttribPointer(
                model->vert_pos_loc,
                3,
                GL_FLOAT,
                GL_FALSE,
                0*sizeof(GLfloat),
                (void*)0            // offset from vertex. The cast is confusing
            );
            break;
    }
    // TODO: Why does this need to be enabled? Is it possible to have
    // multiple vertex attribute arrays, and then just activate one of them?
    glEnableVertexAttribArray(model->vert_pos_loc);

    // set uniform u_color
    glUniform4fv(model->u_color_loc, 1, model->color.raw);

    // set uniform model view matrix
    glUniformMatrix4fv(model->u_mod_mat_loc, 1, GL_FALSE, (GLfloat*)model->view_mat);


    switch (model->drawtype) {
        case DRAWTYPE_2D_PLAIN:
            break;
        case DRAWTYPE_3D_PLAIN: {

            // set uniform u_color
            //glUniform4fv(model->u_color_loc, 1, model->color.raw);

            // set u_light_norm
            glUniform3fv(model->u_light_norm_loc, 1, world.light.dir.norm);
            //printf("bongs\n");
            //glUniform3f(model->u_light_norm_loc, 0.5, 0.7, 0.1);

            // set u_light_map
            glUniform2fv(model->u_light_map_loc, 1, world.light.dir.range);

            //printf("%d %d\n", model->u_light_norm_loc, model->u_light_map_loc);
            
        } break;
    }
    
    // send draw to queue
    //printf("verts: %d\n", model->mesh->verts);
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
