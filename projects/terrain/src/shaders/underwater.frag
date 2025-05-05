#version 300 es
precision mediump float;

uniform vec3 u_light_norm;
uniform float u_light_amb;
uniform float u_light_bright;
uniform float u_light_spec_bright;
uniform float u_light_spec_pow;
uniform vec4 u_light_diff_color;
uniform vec4 u_light_amb_color;
uniform vec3 u_cam_pos;

uniform bool u_enable_vfog;
uniform float u_tex_scale;
uniform float u_tex_strength;
uniform sampler2D u_norm0;
uniform sampler2D u_depth0;

uniform float u_swidth;
uniform float u_sheight;
uniform mat4 u_dithermat;

uniform float u_water_height;

uniform vec4 u_fog_color;

uniform int u_mode;

in vec4 v_color;
in vec2 v_tex_coord;  // interpolated per vertex norm image coords
in vec3 v_coords3;     // interpolated fragment 3d coords
in float v_depth;

in vec3 T;
in vec3 B;
in vec3 N;


vec3 color_to_norm(vec4 color);
vec4 norm_to_color(vec3 norm);
float monochrome(vec4 color);
float monochrome(vec3 color);

//vec4 encode_float_to_color(float value);
//float decode_color_to_float(vec4 color);

//float round(float n);
//vec2 round(vec2 n);
//vec3 round(vec3 n);
//vec2 vfloor(vec2 n);

out vec4 o_color;



void main() {
    //o_color = vec4(0.0, 1.0, 0.0, 1.0);
    //return;

    if (u_mode == 2) {
        o_color = u_fog_color;
        return;
    }

    ////////////////////////////
    // translate normal vector to model space
    
    /*mat3 TBN = mat3(normalize(T), normalize(B), normalize(N));
    vec4 norm_color = texture(u_norm0, v_tex_coord);
    vec4 tex_color = vec4(1.0);
    vec3 prenorm = color_to_norm(norm_color);
    vec3 norm = normalize(TBN * normalize(vec3(prenorm.xy, prenorm.z / u_tex_strength)));
    */


    ////////////////////////////
    // calculate depth difference
    
    vec2 screen_to_coord = vec2(gl_FragCoord.x / u_swidth, gl_FragCoord.y / u_sheight);
    float depth = texture(u_depth0, screen_to_coord).r;

    //o_color = vec4(vec3(1.0/depth), 1.0);
    //return;

    if (depth != v_depth || v_coords3.z > u_water_height) discard;
    



    ////////////////////////////
    // calculate dither

    ivec2 index = ivec2(mod(gl_FragCoord.xy, vec2(4.0)));
    int k = int(u_dithermat[index.x][index.y]);


    /*
    ////////////////////////////
    // calculate final color value
    
    //vec4 color = norm_to_color(normalize(T));
    //vec4 color = norm_to_color(norm);
    vec4 color = v_color * tex_color;
    //color = vec4((color.rgb * (diffuse + backlight) * u_light_bright + 
    color = vec4(color.rgb * (diffuse + ambient) + specular, color.a);

    //color = vec4(vec3(depth*10.0), 1.0);
    //o_color = clamp(vec4(color.rgb/2.0, color.a), 0.0, 1.0);
    //return;*/

    //float colormul = (depth/1.88/2.0 <= float(k)) ? (1.0) : (1.0);
    //float alphamul = (depth/1.88/2.0 <= float(k)) ? (0.5) : (1.0);

    //if (depth != v_depth && v_coords3.z < -20.0) discard;
    //if (depth != v_depth || v_coords3.z > -20.0) discard;

    //if (depth != v_depth || (v_coords3.z > -20.0 && u_mode == 1)) discard;
    //if (depth != v_depth && u_mode == 1) discard;

    /*if (v_depth > depth) {
        o_color = vec4(1.0,0.0,1.0,1.0);
        return;
    }*/
    
    //o_color = vec4(vec3(1.0/(depth-v_depth)), 1.0);
    //o_color = vec4(vec3(depth!=v_depth), 1.0);
    //o_color = vec4(vec3(10.0/v_depth), 1.0);
    //o_color = vec4(vec3(v_coords3.z > -20.0 - 0.01 ? 1.0 : 0.0), 1.0);
    //return;

    //return;

    vec3 colormul = vec3(1.0);
    float alphamul = 1.0;
    float kmul = 0.5;


    /*if (v_coords3.z > -20.0) {
        //alphamul /= 2.0;
        kmul /= 2.0;
    }*/

    /*if (v_coords3.z > -20.0 - 0.01) {
        depth = v_depth;
    }*/

    /*if (u_mode == 0)
        colormul = vec3(8.0, 8.0, 4.0);*/
        
    if (v_depth/1.88/2.0 <= float(k))
        /*if (u_mode == 0) {
            alphamul /= 2.0;
        } else {
            alphamul /= 2.0;
        }*/
        if (u_mode == 0)
            alphamul = 0.75;
        else
            alphamul /= 2.0;
    else
        ;


    /*if (depth/1.88/2.0 <= float(k))
    //if (sqrt(sqrt(depth))/1.88*6.0*2.0 <= float(k))
        o_color = clamp(vec4(u_fog_color.rgb, u_fog_color.a/2.0), 0.0, 1.0);
        //discard;
    else
        o_color = clamp(vec4(u_fog_color.rgb, u_fog_color.a), 0.0, 1.0);*/

    o_color = clamp(vec4(u_fog_color.rgb*colormul, u_fog_color.a*alphamul), 0.0, 1.0);
    //gl_FragDepth = 0.0;

    //o_color = vec4(1.0);
}






vec3 color_to_norm(vec4 color) {
    vec3 norm = normalize(color.rgb * 2.0 - 1.0);
    return vec3(norm.x, norm.y, norm.z);
    //return vec3(norm.x, norm.y, norm.z);
}


vec4 norm_to_color(vec3 norm) {
    vec3 _norm = vec3(norm.x, norm.y, norm.z);
    return vec4(clamp((_norm + 1.0) / 2.0, 0.0, 1.0), 1.0);
    //return vec4(clamp((norm + 1.0) / 2.0, 0.0, 1.0), 1.0);
}

/*vec2 vfloor(vec2 n) {
    return vec2(floor(n.x), floor(n.y));
}*/


/*vec4 encode_float_to_color(float value) {
    uint bits = floatBitsToUint(value);

    float b0 = float((bits >> 0)  & 0xFF);
    float b1 = float((bits >> 8)  & 0xFF);
    float b2 = float((bits >> 16) & 0xFF);
    float b3 = float((bits >> 24) & 0xFF);

    return vec4(b0, b1, b2, b3) / 256.0;
}


float decode_color_to_float(vec4 color) {
    uint b0 = uint(color[0] * 256.0);
    uint b1 = uint(color[1] * 256.0);
    uint b2 = uint(color[2] * 256.0);
    uint b3 = uint(color[3] * 256.0);

    uint bits = (b0<<0) | (b1<<8) | (b2<<16) | (b3<<24);

    return uintBitsToFloat(bits);
}*/



float monochrome(vec4 color) {
    return (color.r*0.299 + color.g*0.587 + color.b*0.114) * color.a;
}


float monochrome(vec3 color) {
    return color.r*0.299 + color.g*0.587 + color.b*0.114;
}
