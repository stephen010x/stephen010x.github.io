precision mediump float;

attribute vec3 vert_pos;
uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;

uniform vec4 u_color;
uniform vec3 u_light_norm;  // make sure this is normalized!!!
uniform vec2 u_light_map;

varying vec4 color;

void main() {
    // translate vertex for rasterization
    gl_Position = u_proj_mat * u_mod_mat * vec4(vert_pos, 1.0);

    // create shade colors for fragment shader
    // the vertex normal happens to be the vertex position
    // horray for the simplicify of spheres!
    float l_dot = dot(normalize(vert_pos), normalize(u_light_norm));
    //float l_mul = (l_dot + 1) * (u_light_map[1] - u_light_map[0])/2 + u_light_map[0];
    float l_mul = (l_dot + 1.0) * (u_light_map[1] - u_light_map[0])/2.0 + u_light_map[0];
    // clamp to handle saturation
    color = clamp(u_color * l_mul, 0.0, 1.0);
    // make sure opacity is unaffected by shadow
    color[3] = 1.0;

    //color = vec4(l_dot, l_dot, l_dot, 1.0);
    //color = vec4(normalize(vert_pos), 1.0);
    //color = vec4(clamp(u_light_norm, 0.0, 1.0), 1.0);
}
