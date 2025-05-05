precision mediump float;

attribute vec3 vert_pos;
attribute vec3 vert_norm;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;
uniform mat3 u_norm_mat;
uniform vec4 u_color;

varying vec4 color;


void main() {
    // translate vertex for rasterization
    gl_Position = u_proj_mat * u_mod_mat * vec4(vert_pos, 1.0);

    // rotate normal according to model matrix
    // apparently cutting off the mat4 into a mat3 will isolate the rotation and scaling
    // turns out this is not supported in glsl 1 and 2, so this is being done cpu side
    //mat3 norm_mat = mat3(transpose(inverse(u_mod_mat)));
    //vec3 norm = normalize(norm_mat * vert_norm);
    vec3 norm = normalize(u_norm_mat * vert_norm);

    /*// create shade colors for fragment shader
    // the vertex normal happens to be the vertex position
    // horray for the simplicify of spheres!
    float l_dot = dot(normalize(vert_pos), normalize(u_light_norm));
    float l_mul = (l_dot + 1.0) * (u_light_map[1] - u_light_map[0])/2.0 + u_light_map[0];
    // clamp to handle saturation
    color = clamp(u_color * l_mul, 0.0, 1.0);
    // make sure opacity is unaffected by shadow
    color[3] = 1.0;*/

    if (all(equal(norm, vec3(0.0, 0.0, 1.0)))) {
        color = u_color;
    } else {
        color = u_color / 1.5;
        color[3] = u_color[3];
    }

    //color = vec4(1.0, 0.5, 0.2, 1.0);
    //color = clamp(vec4(vert_pos.xyz, 1.0), 0.0, 1.0);
    //color = clamp(vec4(norm.xyz/2.0 + 0.5, 1.0), 0.0, 1.0);
}
