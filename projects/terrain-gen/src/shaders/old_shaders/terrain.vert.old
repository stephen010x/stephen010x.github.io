precision mediump float;

attribute vec3 vert_pos;
attribute vec3 vert_norm;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;
uniform mat3 u_norm_mat;
uniform vec4 u_color;

uniform vec3 u_light_norm;
uniform float u_light_amb;
uniform float u_light_bright;
uniform float u_light_spec_bright;
uniform float u_light_spec_pow;
uniform vec3 u_cam_pos;

varying vec4 color;
varying float depth;



vec3 color_to_norm(vec4 color);
vec4 norm_to_color(vec3 norm);


void main() {
    // translate vertex for rasterization
    gl_Position = u_proj_mat * u_mod_mat * vec4(vert_pos, 1.0);

    // calculate vertex position
    vec4 vertpos = u_mod_mat * vec4(vert_pos, 1.0);
    vec3 vertex = vec3(vertpos) / vertpos.w;

    // rotate normal according to model matrix
    vec3 norm = normalize(u_norm_mat * vert_norm);


    vec3 light_norm = normalize(u_light_norm);


    // calculate diffuse light
    float diffuse = max(dot(norm, light_norm), 0.0); 
    
    // calculate specular light
    float specular = 0.0;
    if (diffuse > 0.0) {
        vec3 half_norm = normalize(light_norm + normalize(-vertex+u_cam_pos));
        float spec_angle = max(dot(half_norm, norm), 0.0);
        specular = pow(spec_angle, u_light_spec_pow);
    }
    
    // calculate final color value
    //color = norm_to_color(norm);
    color = u_color;
    //color = norm_to_color(normalize(u_cam_pos));
    color = vec4(u_light_bright * (color.rgb * diffuse * u_light_bright + specular*u_light_spec_bright) + u_light_amb, color.a);
    color = clamp(color, 0.0, 1.0);
    

    // set depth to be logrithmic
    //gl_Position.z = 1.0/gl_Position.z;
    gl_Position.z = log2(gl_Position.z);
}




vec3 color_to_norm(vec4 color) {
    //return normalize((color.rgb - 0.5) * 2.0);
    vec3 norm = normalize(color.rgb * 2.0 - 1.0);
    return vec3(norm.x, norm.y, -norm.z);
}


vec4 norm_to_color(vec3 norm) {
    vec3 _norm = vec3(norm.x, norm.y, -norm.z);
    return vec4(clamp((_norm + 1.0) / 2.0, 0.0, 1.0), 1.0);
}
