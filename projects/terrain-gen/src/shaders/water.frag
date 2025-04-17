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
/*uniform float u_tex_scale;
uniform float u_tex_strength;
uniform sampler2D u_norm0;*/
uniform sampler2D u_depth0;

uniform bool u_enable_texture;

uniform float u_tex0_scale;
uniform float u_tex0_strength;
uniform sampler2D u_tex0_tex;
uniform sampler2D u_tex0_norm;
// TODO: rename occl to amb
uniform sampler2D u_tex0_occl;
uniform sampler2D u_tex0_disp;


uniform float u_swidth;
uniform float u_sheight;
uniform mat4 u_dithermat;

in vec4 v_color;
in vec2 v_tex_coord;  // interpolated per vertex norm image coords
in vec3 v_coords3;     // interpolated fragment 3d coords
in float v_depth;

in vec3 T;
in vec3 B;
in vec3 N;

out vec4 o_color;


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



void main() {
    //float near = 

    ////////////////////////////
    // translate normal vector to model space
    
    mat3 TBN = mat3(normalize(T), normalize(B), normalize(N));

    vec2 tex_coord = v_tex_coord / u_tex0_scale;
    
    vec4 norm_color = texture(u_tex0_norm, tex_coord);
    vec4 tex_color = u_enable_texture ? texture(u_tex0_tex, tex_coord) : vec4(1.0);
    vec4 occl_color = texture(u_tex0_occl, tex_coord);

    vec3 prenorm = color_to_norm(norm_color);
    vec3 norm = normalize(TBN * normalize(vec3(prenorm.xy, prenorm.z / u_tex0_strength)));



    ////////////////////////////
    // calculate depth difference
    
    vec2 screen_to_coord = vec2(gl_FragCoord.x / u_swidth, gl_FragCoord.y / u_sheight);
    float depth = texture(u_depth0, screen_to_coord).r - v_depth;




    ////////////////////////////
    // calculate lighting
    
    vec3 light_norm = normalize(u_light_norm);

    // calculate diffuse light
    vec3 diffuse = max(dot(norm, light_norm), 0.0) * u_light_bright * u_light_diff_color.rgb; 
    
    // calculate specular light
    vec3 specular = vec3(0.0);
    if (diffuse.x > 0.0 || diffuse.y > 0.0 || diffuse.z > 0.0) {
        vec3 half_norm = normalize(light_norm + normalize(-v_coords3+u_cam_pos));
        float spec_angle = max(dot(half_norm, norm), 0.0);
        specular = pow(spec_angle, u_light_spec_pow) * u_light_spec_bright * u_light_diff_color.rgb;
    }

    // calculate ambience
    //vec3 ambient = u_light_amb * u_light_amb_color.rgb;
    vec3 ambient = u_light_amb * u_light_amb_color.rgb * occl_color.rgb;

    // calculate backlight
    //float backlight = max(dot(norm, vec3(-light_norm.xy*2.0, light_norm.z)), 0.0)/4.0;
    //float backlight = 0.0;




    ////////////////////////////
    // calculate dither

    ivec2 index = ivec2(mod(gl_FragCoord.xy, vec2(4.0)));

    /*int k;
    for (int x = 0; x < 4; x++)
        if (x == index.x)
            for (int y = 0; y < 4; y++)
                if (y == index.y)
                    k = int(u_dithermat[x][y]);*/
    int k = int(u_dithermat[index.x][index.y]);



    ////////////////////////////
    // calculate final color value
    
    //vec4 color = norm_to_color(normalize(T));
    //vec4 color = norm_to_color(norm);
    vec4 color = v_color * tex_color;
    //color = vec4((color.rgb * (diffuse + backlight) * u_light_bright + 
    //color = vec4(color.rgb * (diffuse + ambient) + specular, color.a);
    color = vec4(color.rgb * (diffuse + ambient) + specular, color.a);

    //color = vec4(vec3(depth*10.0), 1.0);
    //o_color = clamp(vec4(color.rgb/2.0, color.a), 0.0, 1.0);
    //return;

    if (depth/1.88 + monochrome(specular)*8.0 <= float(k) /*&& monochrome(specular) < 0.5*/)
    //if (sqrt(sqrt(depth))/1.88*6.0*2.0 <= float(k))
        o_color = clamp(vec4(color.rgb, color.a*0.5), 0.0, 1.0);
        //discard;
    else
        o_color = clamp(vec4(color.rgb/2.0, color.a), 0.0, 1.0);
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
