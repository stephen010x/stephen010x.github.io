precision mediump float;

attribute vec3 vert_pos;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;

uniform vec4 u_color;

varying vec4 color;



void main() {
    // translate vertex for rasterization
    gl_Position = u_proj_mat * u_mod_mat * vec4(vert_pos, 1.0);

    //gl_PointSize = 30.0/(gl_Position[3]*gl_Position[3]);
    gl_PointSize = 10.0/(gl_Position[3]);

    color = u_color;
}
