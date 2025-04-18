
 /*
  *    ==== DO NOT EDIT THIS FILE ====
  */

const char terrain_frag[]="#version 300 es\n"
"precision mediump float;\n"
"//#extension GL_OES_standard_derivatives : enable\n"
"//#extension GL_EXT_shader_texture_lod : enable\n"
"\n"
"uniform vec3 u_light_norm;\n"
"uniform float u_light_amb;\n"
"uniform float u_light_bright;\n"
"uniform float u_light_spec_bright;\n"
"uniform float u_light_spec_pow;\n"
"uniform vec4 u_light_diff_color;\n"
"uniform vec4 u_light_amb_color;\n"
"uniform vec3 u_cam_pos;\n"
"\n"
"uniform bool u_enable_vfog;\n"
"uniform bool u_enable_texture;\n"
"\n"
"uniform float u_tex0_scale;\n"
"uniform float u_tex0_strength;\n"
"uniform sampler2D u_tex0_tex;\n"
"uniform sampler2D u_tex0_norm;\n"
"// TODO: rename occl to amb\n"
"uniform sampler2D u_tex0_occl;\n"
"uniform sampler2D u_tex0_disp;\n"
"\n"
"uniform float u_tex1_scale;\n"
"uniform float u_tex1_strength;\n"
"uniform sampler2D u_tex1_tex;\n"
"uniform sampler2D u_tex1_norm;\n"
"uniform sampler2D u_tex1_occl;\n"
"uniform sampler2D u_tex1_disp;\n"
"\n"
"uniform float u_tex2_scale;\n"
"uniform float u_tex2_strength;\n"
"uniform sampler2D u_tex2_tex;\n"
"uniform sampler2D u_tex2_norm;\n"
"uniform sampler2D u_tex2_occl;\n"
"uniform sampler2D u_tex2_disp;\n"
"\n"
"uniform float u_tex3_scale;\n"
"uniform float u_tex3_strength;\n"
"uniform sampler2D u_tex3_tex;\n"
"uniform sampler2D u_tex3_norm;\n"
"uniform sampler2D u_tex3_occl;\n"
"uniform sampler2D u_tex3_disp;\n"
"\n"
"in vec4 v_color;\n"
"in vec2 v_tex_coord;  // interpolated per vertex norm image coords\n"
"in vec3 v_coords3;     // interpolated fragment 3d coords\n"
"in float v_depth;\n"
"\n"
"in vec3 T;\n"
"in vec3 B;\n"
"in vec3 N;\n"
"\n"
"\n"
"in vec4 v_texmap;\n"
"uniform mat4 u_dithermat;\n"
"uniform float u_ditherscale;\n"
"\n"
"out vec4 o_color;\n"
"\n"
"\n"
"\n"
"vec3 color_to_norm(vec4 color);\n"
"vec4 norm_to_color(vec3 norm);\n"
"\n"
"//float round(float n);\n"
"//vec2 round(vec2 n);\n"
"//vec3 round(vec3 n);\n"
"vec2 vfloor(vec2 n);\n"
"\n"
"\n"
"\n"
"void main() {\n"
"\n"
"int tex_mode = 0;\n"
"vec4 norm_color;\n"
"vec4 tex_color;\n"
"vec4 occl_color;\n"
"float tex_scale;\n"
"float tex_strength;\n"
"\n"
"//float wiggle = sin((v_tex_coord.x + v_tex_coord.y)/10.0)*3.0;\n"
"\n"
"/*if (v_coords3.z < -25.0 + wiggle)\n"
"if (N.z > 0.8)\n"
"tex_mode = 2;\n"
"else\n"
"tex_mode = 3;\n"
"else {\n"
"if (N.z > 0.6 + wiggle/100.0)\n"
"//if (v_coords3.z < -15.0 && N.z > 0.7 || v_coords3.z < -21.0)\n"
"if (v_coords3.z < -15.0 + wiggle) //&& N.z > 0.65)\n"
"tex_mode = 2;\n"
"else\n"
"tex_mode = 0;\n"
"else\n"
"tex_mode = 1;\n"
"}*/\n"
"\n"
"/*if (tex_mode == 0) {\n"
"vec2 tex_coord = v_tex_coord / u_tex0_scale;\n"
"tex_strength = u_tex0_strength;\n"
"norm_color = texture2D(u_tex0_norm, tex_coord);\n"
"tex_color  = u_enable_texture ? texture2D(u_tex0_tex, tex_coord) : vec4(1.0);\n"
"occl_color = texture2D(u_tex0_occl, tex_coord);\n"
"} else if (tex_mode == 1) {\n"
"vec2 tex_coord = v_tex_coord / u_tex1_scale;\n"
"tex_strength = u_tex1_strength;\n"
"norm_color = texture2D(u_tex1_norm, tex_coord);\n"
"tex_color  = u_enable_texture ? texture2D(u_tex1_tex, tex_coord) : vec4(1.0);\n"
"occl_color = texture2D(u_tex1_occl, tex_coord);\n"
"} else if (tex_mode == 2) {\n"
"vec2 tex_coord = v_tex_coord / u_tex2_scale;\n"
"tex_strength = u_tex2_strength;\n"
"norm_color = texture2D(u_tex2_norm, tex_coord);\n"
"tex_color  = u_enable_texture ? texture2D(u_tex2_tex, tex_coord) : vec4(1.0);\n"
"occl_color = texture2D(u_tex2_occl, tex_coord);\n"
"} else if (tex_mode == 3) {\n"
"vec2 tex_coord = v_tex_coord / u_tex3_scale;\n"
"tex_strength = u_tex3_strength;\n"
"norm_color = texture2D(u_tex3_norm, tex_coord);\n"
"tex_color  = u_enable_texture ? texture2D(u_tex3_tex, tex_coord) : vec4(1.0);\n"
"occl_color = texture2D(u_tex3_occl, tex_coord);\n"
"}*/\n"
"\n"
"float max1 = 0.0;\n"
"float max2 = 0.0;\n"
"int tex1, tex2;\n"
"\n"
"// find two biggest elements\n"
"for (int i = 0; i < 4; i++) {\n"
"if (v_texmap[i] > max1){\n"
"max2 = max1;\n"
"max1 = v_texmap[i];\n"
"tex2 = tex1;\n"
"tex1 = i;\n"
"} else if (v_texmap[i] > max2) {\n"
"max2 = v_texmap[i];\n"
"tex2 = i;\n"
"}\n"
"}\n"
"\n"
"// sort tex1 and tex2\n"
"if (tex2 < tex1) {\n"
"int temp = tex2;\n"
"tex2 = tex1;\n"
"tex1 = temp;\n"
"max1 = max2;\n"
"}\n"
"\n"
"// calculate lod\n"
"vec2 dithercoord = v_tex_coord.xy / u_ditherscale;\n"
"float dmul = 1.0/1.5;\n"
"vec2 dudx = dFdx(dithercoord/dmul);\n"
"vec2 dldy = dFdy(dithercoord/dmul);\n"
"float delta_max = max(dot(dudx, dudx), dot(dldy, dldy));\n"
"float lod = 0.5 * log2(delta_max);\n"
"float flod2 = exp2(floor(max(lod+0.5, 0.0)));\n"
"\n"
"//gl_FragColor = vec4(vec3(1.0/(1.0+max(lod, 0.0))), 1.0);\n"
"//gl_FragColor = vec4(vec3(1.0/flod2), 1.0);\n"
"//gl_FragColor = vec4(vec3(1.0/exp2(max(lod, 0.0))), 1.0);\n"
"//gl_FragColor = vec4(vec3(1.0/lod), 1.0);\n"
"//gl_FragColor = vec4(vec3(1.0/(1.0+max(floor(lod+1.0), 0.0))), 1.0);\n"
"//return;\n"
"\n"
"// calculate dither\n"
"int k;\n"
"//index = max1 * 16.0;\n"
"/// /*float(1<<int(lod))*/ exp2(floor(lod)\n"
"ivec2 index = ivec2(mod(dithercoord/flod2, vec2(4.0)));\n"
"/*for (int x = 0; x < 4; x++)\n"
"if (x == index.x)\n"
"for (int y = 0; y < 4; y++)\n"
"if (y == index.y)\n"
"k = int(u_dithermat[x][y]);*/\n"
"k = int(u_dithermat[index.x][index.y]);\n"
"\n"
"\n"
"int test = int(max1 * 16.0);\n"
"\n"
"\n"
"//gl_FragColor = norm_to_color(normalize(vec3(max1, max2, 0.0)));\n"
"//gl_FragColor = vec4(float(test)/16.0, float(test)/16.0, float(test)/16.0, 1.0);\n"
"//gl_FragColor = vec4(float(k)/16.0, float(k)/16.0, float(k)/16.0, 1.0);\n"
"//return;\n"
"\n"
"vec2 tex_coord0 = v_tex_coord / u_tex0_scale;\n"
"vec2 tex_coord1 = v_tex_coord / u_tex1_scale;\n"
"vec2 tex_coord2 = v_tex_coord / u_tex2_scale;\n"
"vec2 tex_coord3 = v_tex_coord / u_tex3_scale;\n"
"\n"
"vec2 dx0 = dFdx(tex_coord0);\n"
"vec2 dy0 = dFdy(tex_coord0);\n"
"vec2 dx1 = dFdx(tex_coord1);\n"
"vec2 dy1 = dFdy(tex_coord1);\n"
"vec2 dx2 = dFdx(tex_coord2);\n"
"vec2 dy2 = dFdy(tex_coord2);\n"
"vec2 dx3 = dFdx(tex_coord3);\n"
"vec2 dy3 = dFdy(tex_coord3);\n"
"\n"
"//k = 8;\n"
"\n"
"\n"
"if ((test > k) ? (tex1 == 0) : (tex2 == 0)) {\n"
"//vec2 tex_coord = v_tex_coord / u_tex0_scale;\n"
"tex_strength = u_tex0_strength;\n"
"norm_color = textureGrad(u_tex0_norm, tex_coord0, dx0, dy0);\n"
"tex_color  = u_enable_texture ? textureGrad(u_tex0_tex, tex_coord0, dx0, dy0) : vec4(1.0);\n"
"occl_color = textureGrad(u_tex0_occl, tex_coord0, dx0, dy0);\n"
"} else if ((test > k) ? (tex1 == 1) : (tex2 == 1)) {\n"
"//vec2 tex_coord = v_tex_coord / u_tex1_scale;\n"
"tex_strength = u_tex1_strength;\n"
"norm_color = textureGrad(u_tex1_norm, tex_coord1, dx0, dy0);\n"
"tex_color  = u_enable_texture ? textureGrad(u_tex1_tex, tex_coord1, dx0, dy0) : vec4(1.0);\n"
"occl_color = textureGrad(u_tex1_occl, tex_coord1, dx0, dy0);\n"
"} else if ((test > k) ? (tex1 == 2) : (tex2 == 2)) {\n"
"//vec2 tex_coord = v_tex_coord / u_tex2_scale;\n"
"tex_strength = u_tex2_strength;\n"
"norm_color = textureGrad(u_tex2_norm, tex_coord2, dx0, dy0);\n"
"tex_color  = u_enable_texture ? textureGrad(u_tex2_tex, tex_coord2, dx0, dy0) : vec4(1.0);\n"
"occl_color = textureGrad(u_tex2_occl, tex_coord2, dx0, dy0);\n"
"} else if ((test > k) ? (tex1 == 3) : (tex2 == 3)) {\n"
"//vec2 tex_coord = v_tex_coord / u_tex3_scale;\n"
"tex_strength = u_tex3_strength;\n"
"norm_color = textureGrad(u_tex3_norm, tex_coord3, dx0, dy0);\n"
"tex_color  = u_enable_texture ? textureGrad(u_tex3_tex, tex_coord3, dx0, dy0) : vec4(1.0);\n"
"occl_color = textureGrad(u_tex3_occl, tex_coord3, dx0, dy0);\n"
"}\n"
"\n"
"////////////////////////////\n"
"// translate normal vector to model space\n"
"\n"
"mat3 TBN = mat3(normalize(T), normalize(B), normalize(N));\n"
"//vec4 norm_color = texture2D(u_norm0, vfloor(v_tex_coord/v_depth/1000.0)*v_depth);\n"
"\n"
"/*vec4 norm_color = texture2D(tex_norm, v_tex_coord);\n"
"vec4 tex_color = u_enable_texture ? texture2D(tex_tex, v_tex_coord) : vec4(1.0);\n"
"vec4 occl_color = texture2D(tex_occl, v_tex_coord);*/\n"
"\n"
"//vec4 norm_color = texture2DLod(u_norm0, v_tex_coord/100.0, 1);\n"
"vec3 prenorm = color_to_norm(norm_color);\n"
"vec3 norm = normalize(TBN * normalize(vec3(prenorm.xy, prenorm.z / tex_strength)));\n"
"//norm = vec3(norm.zy, norm.z * u_tex_scale);\n"
"//norm = N;\n"
"//norm = TBN * vec3(0.0,0.0,1.0);\n"
"\n"
"\n"
"\n"
"////////////////////////////\n"
"// calculate lighting\n"
"\n"
"vec3 light_norm = normalize(u_light_norm);\n"
"\n"
"// calculate diffuse light\n"
"vec3 diffuse = max(dot(norm, light_norm), 0.0) * u_light_bright * u_light_diff_color.rgb;\n"
"\n"
"// calculate specular light\n"
"vec3 specular = vec3(0.0);\n"
"if (diffuse.x > 0.0 || diffuse.y > 0.0 || diffuse.z > 0.0) {\n"
"vec3 half_norm = normalize(light_norm + normalize(-v_coords3+u_cam_pos));\n"
"float spec_angle = max(dot(half_norm, norm), 0.0);\n"
"specular = pow(spec_angle, u_light_spec_pow) * u_light_spec_bright * u_light_diff_color.rgb;\n"
"}\n"
"\n"
"// calculate ambience\n"
"vec3 ambient = u_light_amb * u_light_amb_color.rgb * occl_color.rgb;\n"
"\n"
"// calculate backlight\n"
"//float backlight = max(dot(norm, vec3(-light_norm.xy*2.0, light_norm.z)), 0.0)/4.0;\n"
"//float backlight = 0.0;\n"
"\n"
"\n"
"\n"
"////////////////////////////\n"
"// calculate final color value\n"
"\n"
"//vec4 color = norm_to_color(normalize(T));\n"
"//vec4 color = norm_to_color(norm);\n"
"vec4 color = v_color * tex_color;\n"
"//color = vec4((color.rgb * (diffuse + backlight) * u_light_bright +\n"
"//color = vec4(color.rgb * diffuse + specular + ambient, color.a);\n"
"color = vec4(color.rgb * (diffuse + ambient) + specular, color.a);\n"
"color = clamp(color, 0.0, 1.0);\n"
"\n"
"//color = texture2D(test[tex_mode], v_tex_coord);\n"
"\n"
"o_color = color;\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"\n"
"vec3 color_to_norm(vec4 color) {\n"
"vec3 norm = normalize(color.rgb * 2.0 - 1.0);\n"
"return vec3(norm.x, norm.y, norm.z);\n"
"//return vec3(norm.x, norm.y, norm.z);\n"
"}\n"
"\n"
"\n"
"vec4 norm_to_color(vec3 norm) {\n"
"vec3 _norm = vec3(norm.x, norm.y, norm.z);\n"
"return vec4(clamp((_norm + 1.0) / 2.0, 0.0, 1.0), 1.0);\n"
"//return vec4(clamp((norm + 1.0) / 2.0, 0.0, 1.0), 1.0);\n"
"}\n"
"\n"
"vec2 vfloor(vec2 n) {\n"
"return vec2(floor(n.x), floor(n.y));\n"
"}\n"
;
