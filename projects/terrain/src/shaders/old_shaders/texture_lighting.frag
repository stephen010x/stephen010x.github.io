precision mediump float;

// TODO: rename these to u_tex0, etc.
uniform sampler2D tex0;
uniform sampler2D norm0;

uniform mat3 u_norm_mat;

uniform float u_tex_scale;

varying vec4 v_color;
varying vec3 v_norm;
varying vec2 v_tex_coord;
varying vec2 v_norm_coord;
varying vec4 v_coords3;     // i feel smart for this one

// TODO: rename these to u_point_light_pos, etc.
uniform vec3 point_light_pos;
uniform float point_light_int;

uniform int u_f_mode;


//uniform vec3 u_light_norm_debug;


vec3 color_to_norm(vec4 color);
vec4 norm_to_color(vec3 norm);


void main() {
    vec4 tex = texture2D(tex0, v_tex_coord);
    //vec3 norm = normalize((texture2D(norm0, v_norm_coord).rgb - 0.5) * 2.0);
    // get normal from texture map, and rotate according to normal matrix
    vec3 _norm = color_to_norm(texture2D(norm0, v_norm_coord));
    // NOTE: the scaling properties of u_norm_mat will water down our normal!
    // So use a u_norm_mat that has been built with no scaling!
    //vec3 norm = normalize(u_norm_mat * _norm);
    vec3 norm = _norm;
    //vec3 norm = color_to_norm(texture2D(norm0, v_norm_coord));

    if (v_color.a > 0.5)
        tex = tex * vec4(v_color.rgb, 1.0);
    else //if (color.a != 1.0)
        discard;    // does discard also return? Imma assume yes.

    vec3 adj_norm;

    vec3 tnorm = vec3(floor(v_norm.x+0.5), floor(v_norm.y+0.5), floor(v_norm.z+0.5));

    // we are only doing math for axis aligned face normals
    // because I am lazy. Also it is cheaper (probably)
    if (tnorm == vec3(0.0, 0.0, 1.0))
        adj_norm = vec3(-norm.y, norm.x, norm.z);
    else if (tnorm == vec3(0.0, 0.0, -1.0))
        adj_norm = vec3(-norm.y, norm.x, -norm.z);
    else if (tnorm == vec3(0.0, 1.0, 0.0))
        adj_norm = vec3(norm.x, -norm.z, norm.y);
    else if (tnorm == vec3(0.0, -1.0, 0.0))
        adj_norm = vec3(norm.x, norm.z, norm.y);
    else if (tnorm == vec3(1.0, 0.0, 0.0))
        adj_norm = vec3(-norm.z, norm.x, norm.y);
    else if (tnorm == vec3(-1.0, 0.0, 0.0))
        adj_norm = vec3(norm.z, norm.x, norm.y);
    else {
        // debug
        //gl_FragColor = vec4(norm, 1.0);
        gl_FragColor = norm_to_color(v_norm);
        //gl_FragColor = norm_to_color(vec3(0.0, 0.0, -1.0));
        return;
    }

    if (u_f_mode == 1) {
        if (abs(tnorm.z) > 0.5)
            tnorm.z = -tnorm.z;
        adj_norm = tnorm;
        //gl_FragColor = norm_to_color(tnorm);
        //return;
    }

    vec3 light_vect = v_coords3.xyz - point_light_pos.xyz;
    // TODO: not sure why the Z needs to be inverted here. It might be because
    // the color_to_norm etc. functions reverse the z. So consider 
    // undoing that
    vec3 light_norm = -normalize(vec3(light_vect.xy, -light_vect.z));
    float light_dist = length(light_vect);

    //*DEBUG*/ light_norm = vec3(1.0, 1.0, 1.0);
    //*DEBUG*/ light_norm = u_light_norm_debug;

    float intense = point_light_int / (light_dist * light_dist);
    //intense = log(1.0 + intense * 2.0) * 1.0;
    
    float l_dot = dot(adj_norm, light_norm);
    //*DEBUG*/ float l_dot = dot(tnorm, light_norm);

    float mult;
    if (u_f_mode == 2)
        mult = clamp(l_dot * intense, 0.08, 1000.0);
    else
        mult = clamp(l_dot * intense, 0.05, 1000.0);
        //mult = l_dot * intense;
    //mult = log(1.0+mult*2.0)*1.3;

    vec4 frag;
    if (u_f_mode == 3)
        /*DEBUG*/ frag = clamp(tex * l_dot, 0.0, 1.0);
    else
        frag = clamp(tex * mult, 0.0, 1.0);

    if (u_f_mode == 4)
        gl_FragColor = tex;
    else if (u_f_mode == 5)
        gl_FragColor = vec4(norm_to_color(light_norm).rgb * tex.rgb, 1.0);
    else if (u_f_mode == 6)
        gl_FragColor = norm_to_color(adj_norm);
    else
        gl_FragColor = vec4(frag.rgb, 1.0);

    // FOR DEBUGGING
    //gl_FragColor = vec4(norm_to_color(light_norm).rgb * tex.rgb, 1.0);
    //gl_FragColor = norm_to_color(adj_norm);
    //gl_FragColor = norm_to_color(norm);
    //gl_FragColor = norm_to_color(tnorm);
}


vec3 color_to_norm(vec4 color) {
    //return normalize((color.rgb - 0.5) * 2.0);
    vec3 norm = normalize(color.rgb * 2.0 - 1.0);
    return vec3(norm.x, norm.y, -norm.z/u_tex_scale);
}


vec4 norm_to_color(vec3 norm) {
    vec3 _norm = vec3(norm.x, norm.y, -norm.z*u_tex_scale);
    return vec4(clamp((_norm + 1.0) / 2.0, 0.0, 1.0), 1.0);
}
