#version 300 es
precision mediump float;
//#extension GL_OES_standard_derivatives : enable
//#extension GL_EXT_shader_texture_lod : enable

uniform vec3 u_light_norm;
uniform float u_light_amb;
uniform float u_light_bright;
uniform float u_light_spec_bright;
uniform float u_light_spec_pow;
uniform vec4 u_light_diff_color;
uniform vec4 u_light_amb_color;
uniform vec3 u_cam_pos;

uniform bool u_enable_vfog;
uniform bool u_enable_texture;

uniform float u_tex0_scale;
uniform float u_tex0_strength;
uniform sampler2D u_tex0_tex;
uniform sampler2D u_tex0_norm;
// TODO: rename occl to amb
uniform sampler2D u_tex0_occl;
uniform sampler2D u_tex0_disp;

uniform float u_tex1_scale;
uniform float u_tex1_strength;
uniform sampler2D u_tex1_tex;
uniform sampler2D u_tex1_norm;
uniform sampler2D u_tex1_occl;
uniform sampler2D u_tex1_disp;

uniform float u_tex2_scale;
uniform float u_tex2_strength;
uniform sampler2D u_tex2_tex;
uniform sampler2D u_tex2_norm;
uniform sampler2D u_tex2_occl;
uniform sampler2D u_tex2_disp;

uniform float u_tex3_scale;
uniform float u_tex3_strength;
uniform sampler2D u_tex3_tex;
uniform sampler2D u_tex3_norm;
uniform sampler2D u_tex3_occl;
uniform sampler2D u_tex3_disp;

in vec4 v_color;
in vec2 v_tex_coord;  // interpolated per vertex norm image coords
in vec3 v_coords3;     // interpolated fragment 3d coords
in float v_depth;

in vec3 T;
in vec3 B;
in vec3 N;


in vec4 v_texmap;
uniform mat4 u_dithermat;
uniform float u_ditherscale;

out vec4 o_color;



vec3 color_to_norm(vec4 color);
vec4 norm_to_color(vec3 norm);

//float round(float n);
//vec2 round(vec2 n);
//vec3 round(vec3 n);
vec2 vfloor(vec2 n);



void main() {

    int tex_mode = 0;
    vec4 norm_color;
    vec4 tex_color;
    vec4 occl_color;
    float tex_scale;
    float tex_strength;

    //float wiggle = sin((v_tex_coord.x + v_tex_coord.y)/10.0)*3.0;

    /*if (v_coords3.z < -25.0 + wiggle)
        if (N.z > 0.8)
            tex_mode = 2;
        else
            tex_mode = 3;
    else {
        if (N.z > 0.6 + wiggle/100.0)
            //if (v_coords3.z < -15.0 && N.z > 0.7 || v_coords3.z < -21.0)
            if (v_coords3.z < -15.0 + wiggle) //&& N.z > 0.65)
                tex_mode = 2;
            else
                tex_mode = 0;
        else
            tex_mode = 1;
    }*/

    /*if (tex_mode == 0) {
        vec2 tex_coord = v_tex_coord / u_tex0_scale;
        tex_strength = u_tex0_strength;
        norm_color = texture2D(u_tex0_norm, tex_coord);
        tex_color  = u_enable_texture ? texture2D(u_tex0_tex, tex_coord) : vec4(1.0);
        occl_color = texture2D(u_tex0_occl, tex_coord);
    } else if (tex_mode == 1) {
        vec2 tex_coord = v_tex_coord / u_tex1_scale;
        tex_strength = u_tex1_strength;
        norm_color = texture2D(u_tex1_norm, tex_coord);
        tex_color  = u_enable_texture ? texture2D(u_tex1_tex, tex_coord) : vec4(1.0);
        occl_color = texture2D(u_tex1_occl, tex_coord);
    } else if (tex_mode == 2) {
        vec2 tex_coord = v_tex_coord / u_tex2_scale;
        tex_strength = u_tex2_strength;
        norm_color = texture2D(u_tex2_norm, tex_coord);
        tex_color  = u_enable_texture ? texture2D(u_tex2_tex, tex_coord) : vec4(1.0);
        occl_color = texture2D(u_tex2_occl, tex_coord);
    } else if (tex_mode == 3) {
        vec2 tex_coord = v_tex_coord / u_tex3_scale;
        tex_strength = u_tex3_strength;
        norm_color = texture2D(u_tex3_norm, tex_coord);
        tex_color  = u_enable_texture ? texture2D(u_tex3_tex, tex_coord) : vec4(1.0);
        occl_color = texture2D(u_tex3_occl, tex_coord);
    }*/

    float max1 = 0.0;
    float max2 = 0.0;
    int tex1, tex2;

    // find two biggest elements
    for (int i = 0; i < 4; i++) {
        if (v_texmap[i] > max1){
            max2 = max1;
            max1 = v_texmap[i];
            tex2 = tex1;
            tex1 = i;
        } else if (v_texmap[i] > max2) {
            max2 = v_texmap[i];
            tex2 = i;
        }
    }

    // sort tex1 and tex2
    if (tex2 < tex1) {
        int temp = tex2;
        tex2 = tex1;
        tex1 = temp;
        max1 = max2;
    }

    // calculate lod
    vec2 dithercoord = v_tex_coord.xy / u_ditherscale;
    float dmul = 1.0/1.5;
    vec2 dudx = dFdx(dithercoord/dmul);
    vec2 dldy = dFdy(dithercoord/dmul);
    float delta_max = max(dot(dudx, dudx), dot(dldy, dldy));
    float lod = 0.5 * log2(delta_max);
    float flod2 = exp2(floor(max(lod+0.5, 0.0)));

    //gl_FragColor = vec4(vec3(1.0/(1.0+max(lod, 0.0))), 1.0);
    //gl_FragColor = vec4(vec3(1.0/flod2), 1.0);
    //gl_FragColor = vec4(vec3(1.0/exp2(max(lod, 0.0))), 1.0);
    //gl_FragColor = vec4(vec3(1.0/lod), 1.0);
    //gl_FragColor = vec4(vec3(1.0/(1.0+max(floor(lod+1.0), 0.0))), 1.0);
    //return;

    // calculate dither
    int k;
    //index = max1 * 16.0;
    /// /*float(1<<int(lod))*/ exp2(floor(lod)
    ivec2 index = ivec2(mod(dithercoord/flod2, vec2(4.0)));
    /*for (int x = 0; x < 4; x++)
        if (x == index.x)
            for (int y = 0; y < 4; y++)
                if (y == index.y)
                    k = int(u_dithermat[x][y]);*/
    k = int(u_dithermat[index.x][index.y]);


    int test = int(max1 * 16.0);

    
    //gl_FragColor = norm_to_color(normalize(vec3(max1, max2, 0.0)));
    //gl_FragColor = vec4(float(test)/16.0, float(test)/16.0, float(test)/16.0, 1.0);
    //gl_FragColor = vec4(float(k)/16.0, float(k)/16.0, float(k)/16.0, 1.0);
    //return;

    vec2 tex_coord0 = v_tex_coord / u_tex0_scale;
    vec2 tex_coord1 = v_tex_coord / u_tex1_scale;
    vec2 tex_coord2 = v_tex_coord / u_tex2_scale;
    vec2 tex_coord3 = v_tex_coord / u_tex3_scale;
    
    vec2 dx0 = dFdx(tex_coord0);
    vec2 dy0 = dFdy(tex_coord0);
    vec2 dx1 = dFdx(tex_coord1);
    vec2 dy1 = dFdy(tex_coord1);
    vec2 dx2 = dFdx(tex_coord2);
    vec2 dy2 = dFdy(tex_coord2);
    vec2 dx3 = dFdx(tex_coord3);
    vec2 dy3 = dFdy(tex_coord3);

    //k = 8;
    

    if ((test > k) ? (tex1 == 0) : (tex2 == 0)) {
        //vec2 tex_coord = v_tex_coord / u_tex0_scale;
        tex_strength = u_tex0_strength;
        norm_color = textureGrad(u_tex0_norm, tex_coord0, dx0, dy0);
        tex_color  = u_enable_texture ? textureGrad(u_tex0_tex, tex_coord0, dx0, dy0) : vec4(1.0);
        occl_color = textureGrad(u_tex0_occl, tex_coord0, dx0, dy0);
    } else if ((test > k) ? (tex1 == 1) : (tex2 == 1)) {
        //vec2 tex_coord = v_tex_coord / u_tex1_scale;
        tex_strength = u_tex1_strength;
        norm_color = textureGrad(u_tex1_norm, tex_coord1, dx0, dy0);
        tex_color  = u_enable_texture ? textureGrad(u_tex1_tex, tex_coord1, dx0, dy0) : vec4(1.0);
        occl_color = textureGrad(u_tex1_occl, tex_coord1, dx0, dy0);
    } else if ((test > k) ? (tex1 == 2) : (tex2 == 2)) {
        //vec2 tex_coord = v_tex_coord / u_tex2_scale;
        tex_strength = u_tex2_strength;
        norm_color = textureGrad(u_tex2_norm, tex_coord2, dx0, dy0);
        tex_color  = u_enable_texture ? textureGrad(u_tex2_tex, tex_coord2, dx0, dy0) : vec4(1.0);
        occl_color = textureGrad(u_tex2_occl, tex_coord2, dx0, dy0);
    } else if ((test > k) ? (tex1 == 3) : (tex2 == 3)) {
        //vec2 tex_coord = v_tex_coord / u_tex3_scale;
        tex_strength = u_tex3_strength;
        norm_color = textureGrad(u_tex3_norm, tex_coord3, dx0, dy0);
        tex_color  = u_enable_texture ? textureGrad(u_tex3_tex, tex_coord3, dx0, dy0) : vec4(1.0);
        occl_color = textureGrad(u_tex3_occl, tex_coord3, dx0, dy0);
    }

    ////////////////////////////
    // translate normal vector to model space
    
    mat3 TBN = mat3(normalize(T), normalize(B), normalize(N));
    //vec4 norm_color = texture2D(u_norm0, vfloor(v_tex_coord/v_depth/1000.0)*v_depth);
    
    /*vec4 norm_color = texture2D(tex_norm, v_tex_coord);
    vec4 tex_color = u_enable_texture ? texture2D(tex_tex, v_tex_coord) : vec4(1.0);
    vec4 occl_color = texture2D(tex_occl, v_tex_coord);*/
        
    //vec4 norm_color = texture2DLod(u_norm0, v_tex_coord/100.0, 1);
    vec3 prenorm = color_to_norm(norm_color);
    vec3 norm = normalize(TBN * normalize(vec3(prenorm.xy, prenorm.z / tex_strength)));
    //norm = vec3(norm.zy, norm.z * u_tex_scale);
    //norm = N;
    //norm = TBN * vec3(0.0,0.0,1.0);



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
    vec3 ambient = u_light_amb * u_light_amb_color.rgb * occl_color.rgb;

    // calculate backlight
    //float backlight = max(dot(norm, vec3(-light_norm.xy*2.0, light_norm.z)), 0.0)/4.0;
    //float backlight = 0.0;



    ////////////////////////////
    // calculate final color value
    
    //vec4 color = norm_to_color(normalize(T));
    //vec4 color = norm_to_color(norm);
    vec4 color = v_color * tex_color;
    //color = vec4((color.rgb * (diffuse + backlight) * u_light_bright + 
    //color = vec4(color.rgb * diffuse + specular + ambient, color.a);
    color = vec4(color.rgb * (diffuse + ambient) + specular, color.a);
    color = clamp(color, 0.0, 1.0);

    //color = texture2D(test[tex_mode], v_tex_coord);
    
    o_color = color;
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

vec2 vfloor(vec2 n) {
    return vec2(floor(n.x), floor(n.y));
}
