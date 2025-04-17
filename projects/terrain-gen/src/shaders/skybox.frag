#version 300 es
precision mediump float;

uniform vec3 u_cam_dir;
uniform vec3 u_cam_pos;

in vec4 v_color;
//varying float depth;

in vec3 v_coords3;
out vec4 out_color;

void main() {
    vec3 sky_dir = normalize(v_coords3 - u_cam_pos);
    float grad = 2.5*(1.0-dot(sky_dir, vec3(0.0,0.0,1.0))/1.5);
    
    out_color = vec4(vec3(v_color * grad), 1.0);
    
    gl_FragDepth = 0.9999999;
}
