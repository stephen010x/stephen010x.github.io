#include <math.h>
#include <stdint.h>

#include "shapes.h"
#include "../main.h"




void init_circle(void);
void init_sphere(void);
void init_triangle(void);
void init_polygon(void);
void init_square(void);
void init_cube(void);
void init_tex_square(void);

int shape_setup_attributes(int type, SHADER* shader);




GLuint triangle_vert_buff;
GLuint polygon_vert_buff;
GLuint circle_vert_buff;
GLuint square_vert_buff;
GLuint sphere_vert_buff;
GLuint cube_vert_buff;
GLuint tex_square_vert_buff;
GLuint tex_square_flipped_vert_buff;



// TODO:
// I'm realizing if two models are using the same mesh, they
// should reference the opengl context for it rather than make a new
// one. In other words, the mesh should create a buffer, not the model
TRIANGLE_MESH_TYPE triangle_mesh = {
    .verts = 3,
    .type = MESHTYPE_2D_PACKED,
    .mode = GL_TRIANGLES,
    .data = {
         0.0,  0.5,
        -0.5, -0.5,
         0.5, -0.5,
    },
};

// TODO: Change OpenGL default winding order to be clockwise
SQUARE_MESH_TYPE square_mesh = {
    .verts = 4,
    .type = MESHTYPE_2D_PACKED,
    .mode = GL_TRIANGLE_FAN,
    /*.data = {
        -0.5,  0.5,
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
    },*/
    // I decided that the math was easier like this
    .data = {
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
    },
};
    
CIRCLE_MESH_TYPE circle_mesh = {
    .verts = CIRCLE_RES,
    .type = MESHTYPE_2D_PACKED,
    .mode = GL_TRIANGLE_FAN,
    .data = {0},
};


SPHERE_MESH_TYPE sphere_mesh = {
    .verts = SPHERE_VERTS,
    .type = MESHTYPE_3D_PACKED,
    .mode = GL_TRIANGLE_STRIP,
    .data = {0},
};


/*
    Cube Point Diagram
    ==================

      6____7
    4/___5/|
    | 2  | 3
    0____1/

    y
    |  z
    | /
    |/_____x


    Valid formats that won't get culled
    (point 0 equivalent must always be even)
    ===================================

    2---3  3---1  1---0  0---2
    | \ |  | / |  | \ |  | / |
    0---1  2---0  3---2  1---3


    Valid connecting patterns that 
    prevent extra triangles
    ==============================
     
    2---3--4---#
    | \ | \| / |
    0---1  5---#

    --------+--------
            |     
    #---5   |   #---#
    | \ |   |   | \ |
    #---4   |   4---5
       /|   |    |\|
    2---3   |   2---3
    | \ |   |   | \ |
    0---1   |   0---1


    Cube vertex layout (triangle strips)
    ====================================

                         M---N
                         | \ |
                         K---L
                          |\|
    6---7--8---A  C---E  I---J
    | \ | \| / |//| / |\ | \ |
    4---5  9---B  D---F--G---H
     |\|
    2---3
    | \ |
    0---1


    Cube point layout
    =================

                3---7
                |BAC|
    6---7---3---2---6
    |TOP|RIG|BOT|LEF|
    4---5---1---0---4
    |FRO|
    0---1


    Reverse Winding
    Cube point layout
    =================

                1---5
                |FRO|
    4---5---1---0---4
    |TOP|RIG|BOT|LEF|
    6---7---3---2---6
    |BAC|
    2---3
*/

//               x    y    z
#define CUBE_P0 0.0, 0.0, 0.0
#define CUBE_P1 1.0, 0.0, 0.0
#define CUBE_P2 0.0, 0.0, 1.0
#define CUBE_P3 1.0, 0.0, 1.0
#define CUBE_P4 0.0, 1.0, 0.0
#define CUBE_P5 1.0, 1.0, 0.0
#define CUBE_P6 0.0, 1.0, 1.0
#define CUBE_P7 1.0, 1.0, 1.0

// remember, in a gl-triangle-strip, each vertex connects to the two before it, and the
// two after it. a certain order can be followed that will perfectly render this cube in 
// one draw call

#define CUBE_V0 CUBE_P2
#define CUBE_V1 CUBE_P3
#define CUBE_V2 CUBE_P6
#define CUBE_V3 CUBE_P7
#define CUBE_V4 CUBE_P6
#define CUBE_V5 CUBE_P7
#define CUBE_V6 CUBE_P4
#define CUBE_V7 CUBE_P5
#define CUBE_V8 CUBE_P5
#define CUBE_V9 CUBE_P7
#define CUBE_VA CUBE_P1
#define CUBE_VB CUBE_P3
#define CUBE_VC CUBE_P1
#define CUBE_VD CUBE_P3
#define CUBE_VE CUBE_P0
#define CUBE_VF CUBE_P2
#define CUBE_VG CUBE_P2
#define CUBE_VH CUBE_P6
#define CUBE_VI CUBE_P0
#define CUBE_VJ CUBE_P4
#define CUBE_VK CUBE_P0
#define CUBE_VL CUBE_P4
#define CUBE_VM CUBE_P1
#define CUBE_VN CUBE_P5

//                   x     y     z
#define NORM_BOT    0.0, -1.0,  0.0
#define NORM_TOP    0.0,  1.0,  0.0
#define NORM_LEF   -1.0,  0.0,  0.0
#define NORM_RIG    1.0,  0.0,  0.0
#define NORM_FRO    0.0,  0.0, -1.0
#define NORM_BAC    0.0,  0.0,  1.0

/*const GLfloat cube_norms[6*3] = {
    NORM_BOT,
    NORM_TOP,
    NORM_LEF,
    NORM_RIG,
    NORM_FRO,
    NORM_BAC,
};*/

// This is designed for a single draw call (the original idea was to render each face
// independantly with 6 calls to glDrawArrays, making good use of that offset parameter.
CUBE_MESH_TYPE cube_mesh = {
    .verts = 4*6,
    .type = MESHTYPE_3D_VERT_NORM,
    .mode = GL_TRIANGLE_STRIP,
    .data = {
        // back face
        CUBE_V0, NORM_BAC,
        CUBE_V1, NORM_BAC,
        CUBE_V2, NORM_BAC,
        CUBE_V3, NORM_BAC,
        
        // top face
        CUBE_V4, NORM_TOP,
        CUBE_V5, NORM_TOP,
        CUBE_V6, NORM_TOP,
        CUBE_V7, NORM_TOP,
            
        // right face
        CUBE_V8, NORM_RIG,
        CUBE_V9, NORM_RIG,
        CUBE_VA, NORM_RIG,
        CUBE_VB, NORM_RIG,
        
        // bottom face
        CUBE_VC, NORM_BOT,
        CUBE_VD, NORM_BOT,
        CUBE_VE, NORM_BOT,
        CUBE_VF, NORM_BOT,
        
        // left face
        CUBE_VG, NORM_LEF,
        CUBE_VH, NORM_LEF,
        CUBE_VI, NORM_LEF,
        CUBE_VJ, NORM_LEF,
        
        // front face
        CUBE_VK, NORM_FRO,
        CUBE_VL, NORM_FRO,
        CUBE_VM, NORM_FRO,
        CUBE_VN, NORM_FRO,
    },
};



TEX_SQUARE_MESH_TYPE tex_square_flipped_mesh = {
    .verts = 4,
    .type = MESHTYPE_3D_VERT_NORM_TEX2,
    .mode = GL_TRIANGLE_FAN,
    // using texture 3d coordinates for simplicity.
    // The third coord axis is ignored.
    .data = {
        0.0, 1.0, 0.0, // vert
        0.0, 0.0, 1.0, // norm
        0.0, 0.0,      // tex
        
        0.0, 0.0, 0.0, // vert
        0.0, 0.0, 1.0, // norm
        0.0, 1.0, // tex
        
        1.0, 0.0, 0.0, // vert
        0.0, 0.0, 1.0, // norm
        1.0, 1.0,      // tex
        
        1.0, 1.0, 0.0, // vert
        0.0, 0.0, 1.0, // norm
        1.0, 0.0,      // tex
    },
};


TEX_SQUARE_MESH_TYPE tex_square_mesh = {
    .verts = 4,
    .type = MESHTYPE_3D_VERT_NORM_TEX2,
    .mode = GL_TRIANGLE_FAN,
    // using texture 3d coordinates for simplicity.
    // The third coord axis is ignored.
    .data = {
        0.0, 1.0, 0.0, // vert
        0.0, 0.0, 1.0, // norm
        0.0, 1.0,      // tex
        
        0.0, 0.0, 0.0, // vert
        0.0, 0.0, 1.0, // norm
        0.0, 0.0, // tex
        
        1.0, 0.0, 0.0, // vert
        0.0, 0.0, 1.0, // norm
        1.0, 0.0,      // tex
        
        1.0, 1.0, 0.0, // vert
        0.0, 0.0, 1.0, // norm
        1.0, 1.0,      // tex
    },
};






SHADER* shader_init(SHADER* shader, GLuint program, SHADER_USE_CALL call, void* data) {
    *shader = (SHADER){
        .program = program,
        .callback = call,
        .data = data,
    };

    // select current shader to get value locations
    glUseProgram(program);

    shader->vert_pos_loc  = glGetAttribLocation(program, "vert_pos");
    shader->vert_norm_loc = glGetAttribLocation(program, "vert_norm");
    shader->vert_tex_loc  = glGetAttribLocation(program, "vert_tex");
    ASSERT(shader->vert_pos_loc >= 0, NULL, "no location for vert_pos_loc\n");

    // note, these may return -1, meaning they are not in the program
    shader->u_mod_mat_loc    = glGetUniformLocation(program, "u_mod_mat");
    shader->u_norm_mat_loc   = glGetUniformLocation(program, "u_norm_mat");
    shader->u_color_loc      = glGetUniformLocation(program, "u_color");
    shader->u_light_norm_loc = glGetUniformLocation(program, "u_light_norm");
    shader->u_light_map_loc  = glGetUniformLocation(program, "u_light_map");

    #ifdef DEBUG_MODE

    mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
    
    // set uniform model view matrix
    if (shader->u_mod_mat_loc > 0)
        glUniformMatrix4fv(shader->u_mod_mat_loc, 
            1, GL_FALSE, (GLfloat*)viewmat);

    // what the heck is this for? I already implemented this below,
    // and yet this is broken code anyway
    /*if (shader->u_mod_mat_loc > 0)
        glUniformMatrix4fv(shader->u_norm_mat_loc, 
            1, GL_FALSE, (GLfloat*)viewmat);*/

    // debug color
    if (shader->u_color_loc > 0) 
        glUniform4fv(shader->u_color_loc, 1, (GLfloat[]){0.0, 1.0, 0.0, 1.0});

    // set uniform normal matrix
    if (shader->u_norm_mat_loc > 0) {
        //mat3 norm_mat = mat3(transpose(inverse(u_mod_mat)));
        mat3 norm_mat;
        glm_mat4_pick3(viewmat, norm_mat);
        glm_mat3_inv(norm_mat, norm_mat);
        glm_mat3_transpose(norm_mat);
        
        glUniformMatrix3fv(shader->u_norm_mat_loc, 
            1, GL_FALSE, (GLfloat*)norm_mat);
    }

    #endif

    // deselect shader to undefined shader
    glUseProgram(NULL_SHADER);

    return shader;
}

SHADER* shader_use(SHADER* shader) {
    glUseProgram(shader->program);

    if (shader->callback)
        shader->callback(shader, shader->data);

    return shader;
}


// TODO: create an error message system that collects different types of the same error
// and then have a handler that prints out the error and how many instances of it every interval
// to reduce error bloat
// that way I can let the programmer know that the uniforms arent being set if they wish
int shader_set_float(SHADER* shader, char* vname, float val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform1f(u_loc, (GLfloat)val);
        return 0;
    }
    return -1;
}

int shader_set_int(SHADER* shader, char* vname, int val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform1i(u_loc, (GLint)val);
        return 0;
    }
    return -1;
}

int shader_set_mat4(SHADER* shader, char* vname, mat4 val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniformMatrix4fv(u_loc, 1, GL_FALSE, (GLfloat*)val);
        return 0;
    }
    return -1;
}

int shader_set_vec2(SHADER* shader, char* vname, vec2 val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform2fv(u_loc, 1, (GLfloat*)val);
        return 0;
    }
    return -1;
}

int shader_set_vec3(SHADER* shader, char* vname, vec3 val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform3fv(u_loc, 1, (GLfloat*)val);
        return 0;
    }
    return -1;
}

int shader_set_vec4(SHADER* shader, char* vname, vec4 val) {
    glUseProgram(shader->program);
    GLint u_loc = glGetUniformLocation(shader->program, vname);
    if (u_loc > 0) {
        glUniform4fv(u_loc, 1, (GLfloat*)val);
        return 0;
    }
    return -1;
}






void shapes_init(void) {
    init_circle();
    init_sphere();
    init_triangle();
    init_polygon();
    init_square();
    init_tex_square();
    init_cube();
}


void init_circle(void) {
    for (int i = 0; i < CIRCLE_RES; i++) {
        circle_mesh.data[i*2+0] = (float)cos( (float)i * (2*MATH_PI/CIRCLE_RES) );
        circle_mesh.data[i*2+1] = (float)sin( (float)i * (2*MATH_PI/CIRCLE_RES) );
    }

    // create buffer
    glGenBuffers(1, &circle_vert_buff);

    // upload buffer data
    glBindBuffer(GL_ARRAY_BUFFER, circle_vert_buff);
    glBufferData(
        GL_ARRAY_BUFFER, 
        (GLsizeiptr)MESH_sizeof(&circle_mesh),
        circle_mesh.data,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, NULL_BUFFER);
}


void init_sphere(void) {
    #define STRIP_VERT_INDEX(__V, __D) ((__V) + (__D)*STRIP_VERTS)
    #define X_ANG_OFFSET ((MATH_PI*1/(float)SPHERE_V_RES))
    // 4??? Yeah... This will probably be a problem later
    #define Y_ANG_OFFSET ((MATH_PI*4/(float)SPHERE_D_RES))
        
    // generate sphere mesh
    for (int i = 0; i < SPHERE_D_RES; i++) {
        float yang = Y_ANG_OFFSET*i;
        for (int j = 0; j < SPHERE_V_RES+1; j++) {
            float xang = X_ANG_OFFSET*j;
            // y extruding point
            // rotate on x first and then y, and then y again for extra offset
            vec3 v = {0, -1, 0};
            glm_vec3_rotate(v, xang, (vec3){1, 0, 0});
            glm_vec3_rotate(v, yang, (vec3){0, 1, 0});
            glm_vec3_copy(v, sphere_mesh.v3[STRIP_VERT_INDEX(j*2, i)]);
            //printf("vec1 %f, %f, %f\n", v[0], v[1], v[2]);
            glm_vec3_rotate(v, Y_ANG_OFFSET, (vec3){0, 1, 0});
            glm_vec3_copy(v, sphere_mesh.v3[STRIP_VERT_INDEX(j*2+1, i)]);
            //printf("vec2 %f, %f, %f\n", v[0], v[1], v[2]);
        }
        //glm_vec3_copy((vec3){0, 1,0}, sphere_mesh.v3[STRIP_VERT_INDEX(0, i)]);
        //glm_vec3_copy((vec3){0,-1,0},
        //    sphere_mesh.v3[STRIP_VERT_INDEX(STRIP_VERTS-1, i)]);
    }
    /*for (int i = 0; i < SPHERE_VERTS; i++) {
        vec3* v = sphere_mesh.v3+i;
        if (i%STRIP_VERTS == 0)
            printf("#### strip %d ####\n", i/STRIP_VERTS);
        printf("vec[%d] %f, %f, %f\n", i, v[0][0], v[0][1], v[0][2]);
    }*/

    // create buffer
    glGenBuffers(1, &sphere_vert_buff);

    // upload buffer data
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vert_buff);
    glBufferData(
        GL_ARRAY_BUFFER, 
        (GLsizeiptr)MESH_sizeof(&sphere_mesh),
        sphere_mesh.data,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, NULL_BUFFER);
}





// TODO: implement these
void init_triangle(void) {return;}
void init_polygon(void) {return;}






void init_square(void) {

    // create buffer
    glGenBuffers(1, &square_vert_buff);

    // upload buffer data
    glBindBuffer(GL_ARRAY_BUFFER, square_vert_buff);
    glBufferData(
        GL_ARRAY_BUFFER,
        (GLsizeiptr)MESH_sizeof((void*)&square_mesh), // why the heck am I getting a warning here?
        square_mesh.data,                            // I specifically made this void to avoid this!
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, NULL_BUFFER);
}




void init_tex_square(void) {
    // create buffer
    glGenBuffers(1, &tex_square_vert_buff);

    // upload buffer data
    glBindBuffer(GL_ARRAY_BUFFER, tex_square_vert_buff);
    glBufferData(
        GL_ARRAY_BUFFER,
        (GLsizeiptr)MESH_sizeof((void*)&tex_square_mesh),
        tex_square_mesh.data,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, NULL_BUFFER);


    // create buffer
    glGenBuffers(1, &tex_square_flipped_vert_buff);

    // upload buffer data
    glBindBuffer(GL_ARRAY_BUFFER, tex_square_flipped_vert_buff);
    glBufferData(
        GL_ARRAY_BUFFER,
        (GLsizeiptr)MESH_sizeof((void*)&tex_square_flipped_mesh),
        tex_square_flipped_mesh.data,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, NULL_BUFFER);
}





void init_cube(void) {
    // create buffer
    glGenBuffers(1, &cube_vert_buff);

    // upload buffer data
    glBindBuffer(GL_ARRAY_BUFFER, cube_vert_buff);
    glBufferData(
        GL_ARRAY_BUFFER,
        (GLsizeiptr)MESH_sizeof((void*)&cube_mesh),
        cube_mesh.data,
        //sphere_mesh.data,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, NULL_BUFFER);
}






// TODO: finish implementing this
int draw_triangle(vec2 points[3], vec2 pos, SHADER* shader) {
    // TODO: add a default shader if shader is null
    if (shader)
        shader_use(shader);
    else
        return -1;
    /*else
        shader_use();*/

    return -1;
}



// TODO: move this to models or something
int shape_setup_attributes(int type, SHADER* shader) {

    int attribs;

    switch (type) {
        case MESHTYPE_3D_VERT_NORM:
            attribs = 2;
            break;
        case MESHTYPE_3D_VERT_NORM_TEX2:
            attribs = 3;
            break;
        default:
            attribs = 1;
    }


    GLint attrib_size[4];
    GLenum attrib_type[4];
    intptr_t attrib_off[4];
    GLint attrib_loc[4];
    
    GLsizei attrib_stride = 0*sizeof(GLfloat);

    for (int i = 0; i < attribs; i++) {
        attrib_type[i] = GL_FLOAT;
        attrib_off[i] = i * (3*sizeof(GLfloat));
        attrib_loc[i] = shader->attrib_loc[i];
    }
    

    switch (type) {
        case MESHTYPE_1D_PACKED:
            attrib_size[0] = 1;
            break;
        case MESHTYPE_2D_PACKED:
            attrib_size[0] = 2;
            break;
        case MESHTYPE_3D_PACKED:
            attrib_size[0] = 3;
            break;
        case MESHTYPE_3D_VERT_NORM:
            attrib_size[0] = 3;
            attrib_size[1] = 3;
            attrib_stride = 6 * sizeof(GLfloat);
            break;
        case MESHTYPE_3D_VERT_NORM_TEX2:
            attrib_size[0] = 3;
            attrib_size[1] = 3;
            attrib_size[2] = 2;
            //attrib_off[3] = attrib_off[2] + 2*sizeof(GLfloat);
            // TODO: add a function to mesh that will get vertex size
            attrib_stride = (3 + 3 + 2) * sizeof(GLfloat);
            break;
    }


    for (int i = 0; i < attribs; i++) {
        if (attrib_loc[i] < 0)
            continue;
            
        glVertexAttribPointer(
            attrib_loc[i],
            attrib_size[i],
            attrib_type[i],
            GL_FALSE,
            attrib_stride,
            (void*)attrib_off[i]
        );
        glEnableVertexAttribArray(attrib_loc[i]);
    }

    return 0;
}




int draw_rectangle(float width, float height, float rot, vec2 pos, COLOR color, SHADER* shader, int layer) {

    shader_use(shader);

    // I wonder if it is okay to bind this before binding shader?
    glBindBuffer(GL_ARRAY_BUFFER, square_vert_buff);

    shape_setup_attributes(square_mesh.type, shader);

    mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
    // blegh. I forgot about the opengl ordering here. These need to be in reverse
    glm_translate(viewmat, (vec3){pos[0], pos[1], /*(float)layer/100.0-1.0}*/ -(float)layer/1000.0});
    glm_rotate_z(viewmat, rot, viewmat);
    glm_scale(viewmat, (vec3){width, height, 1});

    // set uniform model view matrix
    if (shader->u_mod_mat_loc > 0)
        glUniformMatrix4fv(shader->u_mod_mat_loc, 
            1, GL_FALSE, (GLfloat*)viewmat);

    // set uniform color
    if (shader->u_color_loc > 0) 
        glUniform4fv(shader->u_color_loc, 1, color.raw);
    
    // send draw to queue
    glDrawArrays(square_mesh.mode, 0, square_mesh.verts);

    return 0;
}




int draw_rect2(vec3 pos, vec2 scale, vec3 norm, float rot, COLOR color, SHADER* shader) {

    shader_use(shader);

    // I wonder if it is okay to bind this before binding shader?
    glBindBuffer(GL_ARRAY_BUFFER, square_vert_buff);

    shape_setup_attributes(square_mesh.type, shader);

    mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
    //mat4 lookmat;
    glm_translate(viewmat, pos);

    if (norm != NULL && (norm[0] != 0 || norm[1] != 0 || norm[2] != 0)) {
        // get the cross product to find axis of rotation and angle of rotation
        vec3 axis;
        glm_vec3_crossn(norm, (vec3){0,0,1}, axis);
        float angle = glm_vec3_angle(norm, (vec3){0,0,1});
        // rotate matrix on axis of rotation
        glm_rotate(viewmat, angle, axis);
        glm_rotate(viewmat, rot, (vec3){0,0,1});
    }

    glm_scale(viewmat, (vec3){scale[0], scale[1], 1});

    // set uniform model view matrix
    if (shader->u_mod_mat_loc > 0)
        glUniformMatrix4fv(shader->u_mod_mat_loc, 
            1, GL_FALSE, (GLfloat*)viewmat);

    if (shader->u_color_loc > 0) 
        glUniform4fv(shader->u_color_loc, 1, color.raw);

    // set uniform normal matrix
    // the matrix has to be completely rebuilt to avoid scaling affecting normals
    if (shader->u_norm_mat_loc > 0) {
        mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
        glm_translate(viewmat, pos);
        if (norm != NULL && (norm[0] != 0 || norm[1] != 0 || norm[2] != 0)) {
            // get the cross product to find axis of rotation and angle of rotation
            vec3 axis;
            glm_vec3_crossn(norm, (vec3){0,0,1}, axis);
            float angle = glm_vec3_angle(norm, (vec3){0,0,1});
            // rotate matrix on axis of rotation
            glm_rotate(viewmat, angle, axis);
            glm_rotate(viewmat, rot, (vec3){0,0,1});
        }
        // no scaling
    
        //mat3 norm_mat = mat3(transpose(inverse(u_mod_mat)));
        mat3 norm_mat;
        glm_mat4_pick3(viewmat, norm_mat);
        glm_mat3_inv(norm_mat, norm_mat);
        glm_mat3_transpose(norm_mat);
        
        glUniformMatrix3fv(shader->u_norm_mat_loc, 
            1, GL_FALSE, (GLfloat*)norm_mat);
    }
    
    // send draw to queue
    // TODO: offload this function to the shader with a shader function call
    // have shader override modes, like draw points or draw lines for wireframe.
    glDrawArrays(square_mesh.mode, 0, square_mesh.verts);

    return 0;
}



// TODO: implement these later
int draw_polygon(vec2* points, float rot, vec2 pos, SHADER* shader) {return -1;}
int draw_oval(      float width, float height, float rot, vec2 pos, SHADER* shader) {return -1;}
int draw_sphere(vec3 size, vec3 pos, SHADER* shader) {return -1;}






int draw_rect3(vec3 pos, vec3 scale, vec3 dir, float roll, COLOR color, SHADER* shader) {

    // setup
    shader_use(shader);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vert_buff);
    shape_setup_attributes(cube_mesh.type, shader);

    // stretch, rotate, and translate
    mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
    mat4 lookmat;
    // blegh. Remember that these need to stay in reverse order. I think.
    glm_translate(viewmat, pos);

    if (dir != NULL && (dir[0] != 0 || dir[1] != 0 || dir[2] != 0)) {
        // get the cross product to find axis of rotation and angle of rotation
        vec3 axis;
        glm_vec3_crossn(dir, (vec3){0,0,1}, axis);
        float angle = glm_vec3_angle(dir, (vec3){0,0,1});

        // rotate matrix on axis of rotation
        glm_rotate(viewmat, angle, axis);
        glm_rotate(viewmat, roll, (vec3){0,0,1});
        
        //glm_scale(viewmat, (vec3){scale[0], scale[1], 1});
    }
    glm_scale(viewmat, scale);

    // set uniform model view matrix
    if (shader->u_mod_mat_loc > 0)
        glUniformMatrix4fv(shader->u_mod_mat_loc, 
            1, GL_FALSE, (GLfloat*)viewmat);

    // set uniform u_color
    if (shader->u_color_loc > 0) 
        glUniform4fv(shader->u_color_loc, 1, color.raw);

    // set uniform normal matrix
    if (shader->u_norm_mat_loc > 0) {
        mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
        glm_translate(viewmat, pos);
        if (dir != NULL && (dir[0] != 0 || dir[1] != 0 || dir[2] != 0)) {
            // get the cross product to find axis of rotation and angle of rotation
            vec3 axis;
            glm_vec3_crossn(dir, (vec3){0,0,1}, axis);
            float angle = glm_vec3_angle(dir, (vec3){0,0,1});
            // rotate matrix on axis of rotation
            glm_rotate(viewmat, angle, axis);
            glm_rotate(viewmat, roll, (vec3){0,0,1});
        }
        // no scaling
    
        //mat3 norm_mat = mat3(transpose(inverse(u_mod_mat)));
        mat3 norm_mat;
        glm_mat4_pick3(viewmat, norm_mat);
        glm_mat3_inv(norm_mat, norm_mat);
        glm_mat3_transpose(norm_mat);
        
        glUniformMatrix3fv(shader->u_norm_mat_loc, 
            1, GL_FALSE, (GLfloat*)norm_mat);
    }
    
    // send draw to queue
    glDrawArrays(cube_mesh.mode, 0, cube_mesh.verts);

    return 0;
}





int draw_texture_plane(vec3 pos, vec2 scale, vec3 norm, float rot, TEXTURE* texture, SHADER* shader, bool flip) {

    GLuint buff = tex_square_vert_buff;
    MESH* mesh = (MESH*)&tex_square_mesh;

    if (flip) {
        buff = tex_square_flipped_vert_buff;
        mesh = (MESH*)&tex_square_flipped_mesh;
    }

    shader_use(shader);

    texture_bind(texture, shader, "tex0", GL_TEXTURE0);

    // I wonder if it is okay to bind this before binding shader?
    // which I am not doing. But I do wonder.
    glBindBuffer(GL_ARRAY_BUFFER, buff);

    shape_setup_attributes(mesh->type, shader);

    mat4 viewmat = GLM_MAT4_IDENTITY_INIT;
    mat4 lookmat;
    glm_translate(viewmat, pos);
    /*glm_look((vec3){0,0,0}, norm, (vec3){0,0,1}, lookmat);
    glm_rotate_z(lookmat, rot, lookmat);
    glm_mat4_mul(lookmat, viewmat, viewmat);*/

    if (norm != NULL) {
        vec3 axis;
        // get the cross product to find axis of rotation and angle of rotation
        glm_vec3_crossn(norm, (vec3){0,0,1}, axis);
        float angle = glm_vec3_angle(norm, (vec3){0,0,1});

        // rotate matrix on axis of rotation
        glm_rotate(viewmat, angle, axis);
        glm_rotate(viewmat, rot, (vec3){0,0,1});
    }
    
    glm_scale(viewmat, (vec3){scale[0], scale[1], 1});

    // set uniform model view matrix
    if (shader->u_mod_mat_loc > 0)
        glUniformMatrix4fv(shader->u_mod_mat_loc, 
            1, GL_FALSE, (GLfloat*)viewmat);

    #ifdef DEBUG_MODE
    if (shader->u_color_loc > 0) 
        glUniform4fv(shader->u_color_loc, 1, (GLfloat[]){0.0f, 1.0f, 0.0f, 1.0f});
    #endif

    // set uniform normal matrix
    if (shader->u_norm_mat_loc > 0) {
        //mat3 norm_mat = mat3(transpose(inverse(u_mod_mat)));
        mat3 norm_mat;
        glm_mat4_pick3(viewmat, norm_mat);
        glm_mat3_inv(norm_mat, norm_mat);
        glm_mat3_transpose(norm_mat);
        
        glUniformMatrix3fv(shader->u_norm_mat_loc, 
            1, GL_FALSE, (GLfloat*)norm_mat);
    }
    
    // send draw to queue
    // TODO: offload this function to the shader with a shader function call
    // have shader override modes, like draw points or draw lines for wireframe.
    glDrawArrays(mesh->mode, 0, mesh->verts);

    return 0;
}




































// TODO: paste this code into the relevant above functions

/*

    // create model
    // 2D Circle
    #if 0
    for (int i = 0; i < NUM_MODELS; i++) {
        models[i] = (MODEL){
            .color = {
                .r = FRAND(),
                .g = FRAND(),
                .b = FRAND(),
                .w = 1.0
            },
            .id = i,
            .mesh = (MESH*)&circle_mesh,
            .visable = true,
            .drawtype = DRAWTYPE_2D_PLAIN,
            .update_call = (UPDATE_CALLBACK)circle_update,
            .shader_prog = poly_program,
            .view_mat = GLM_MAT4_IDENTITY_INIT,
        };

        float scale = (FRAND()*0.9 + 0.2)/4;
        
        behave[i] = (BEHAVE){
            .vel = {
                (FRAND()*2-1)/10/10,
                (FRAND()*2-1)/10/10,
                0,
            },
            .pos = {
                FRANDRANGE(wmin[0]+scale, wmax[0]-scale),
                FRANDRANGE(wmin[1]+scale, wmax[1]-scale),
                0,
            },
            .scale = scale,
            .mass = (4/3)*MATH_PI*scale*scale*scale*DENSITY,
        };

        model_transform(models+i);

        MODEL_init(models+i);
    }
    #else
    // 3D Sphere
    for (int i = 0; i < NUM_MODELS; i++) {
        models[i] = (MODEL){
            .color = {
                .r = FRAND(),
                .g = FRAND(),
                .b = FRAND(),
                .w = 1.0
            },
            .id = i,
            .mesh = (MESH*)&sphere_mesh,
            .visable = true,
            .drawtype = DRAWTYPE_3D_PLAIN,
            .update_call = (UPDATE_CALLBACK)circle_update,
            .shader_prog = sphere_program,
            .view_mat = GLM_MAT4_IDENTITY_INIT,
        };

        //float scale = (FRAND()*0.9 + 0.6)/4;
        float scale = FRANDRANGE(0.1, 0.4);
        
        behave[i] = (BEHAVE){
            .vel = {
                (FRAND()*2-1)/10/10,
                (FRAND()*2-1)/10/10,
                (FRAND()*2-1)/10/10,
            },
            .pos = {
                FRANDRANGE(wmin[0]+scale, wmax[0]-scale),
                FRANDRANGE(wmin[1]+scale, wmax[1]-scale),
                FRANDRANGE(wmin[2]+scale, wmax[2]-scale),
            },
            .scale = scale,
            .mass = (4/3)*MATH_PI*scale*scale*scale*DENSITY,
        };

        model_transform(models+i);

        MODEL_init(models+i);
    }
    #endif
}
*/
