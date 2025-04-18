
 /*
  *    ==== DO NOT EDIT THIS FILE ====
  */

const char water_frag[]="#version 300 es\n"
"precision mediump float;\n"
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
"/*uniform float u_tex_scale;\n"
"uniform float u_tex_strength;\n"
"uniform sampler2D u_norm0;*/\n"
"uniform sampler2D u_depth0;\n"
"\n"
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
"\n"
"uniform float u_swidth;\n"
"uniform float u_sheight;\n"
"uniform mat4 u_dithermat;\n"
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
"out vec4 o_color;\n"
"\n"
"\n"
"vec3 color_to_norm(vec4 color);\n"
"vec4 norm_to_color(vec3 norm);\n"
"float monochrome(vec4 color);\n"
"float monochrome(vec3 color);\n"
"\n"
"//vec4 encode_float_to_color(float value);\n"
"//float decode_color_to_float(vec4 color);\n"
"\n"
"//float round(float n);\n"
"//vec2 round(vec2 n);\n"
"//vec3 round(vec3 n);\n"
"//vec2 vfloor(vec2 n);\n"
"\n"
"\n"
"\n"
"void main() {\n"
"//float near =\n"
"\n"
"////////////////////////////\n"
"// translate normal vector to model space\n"
"\n"
"mat3 TBN = mat3(normalize(T), normalize(B), normalize(N));\n"
"\n"
"vec2 tex_coord = v_tex_coord / u_tex0_scale;\n"
"\n"
"vec4 norm_color = texture(u_tex0_norm, tex_coord);\n"
"vec4 tex_color = u_enable_texture ? texture(u_tex0_tex, tex_coord) : vec4(1.0);\n"
"vec4 occl_color = texture(u_tex0_occl, tex_coord);\n"
"\n"
"vec3 prenorm = color_to_norm(norm_color);\n"
"vec3 norm = normalize(TBN * normalize(vec3(prenorm.xy, prenorm.z / u_tex0_strength)));\n"
"\n"
"\n"
"\n"
"////////////////////////////\n"
"// calculate depth difference\n"
"\n"
"vec2 screen_to_coord = vec2(gl_FragCoord.x / u_swidth, gl_FragCoord.y / u_sheight);\n"
"float depth = texture(u_depth0, screen_to_coord).r - v_depth;\n"
"\n"
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
"//vec3 ambient = u_light_amb * u_light_amb_color.rgb;\n"
"vec3 ambient = u_light_amb * u_light_amb_color.rgb * occl_color.rgb;\n"
"\n"
"// calculate backlight\n"
"//float backlight = max(dot(norm, vec3(-light_norm.xy*2.0, light_norm.z)), 0.0)/4.0;\n"
"//float backlight = 0.0;\n"
"\n"
"\n"
"\n"
"\n"
"////////////////////////////\n"
"// calculate dither\n"
"\n"
"ivec2 index = ivec2(mod(gl_FragCoord.xy, vec2(4.0)));\n"
"\n"
"/*int k;\n"
"for (int x = 0; x < 4; x++)\n"
"if (x == index.x)\n"
"for (int y = 0; y < 4; y++)\n"
"if (y == index.y)\n"
"k = int(u_dithermat[x][y]);*/\n"
"int k = int(u_dithermat[index.x][index.y]);\n"
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
"//color = vec4(color.rgb * (diffuse + ambient) + specular, color.a);\n"
"color = vec4(color.rgb * (diffuse + ambient) + specular, color.a);\n"
"\n"
"//color = vec4(vec3(depth*10.0), 1.0);\n"
"//o_color = clamp(vec4(color.rgb/2.0, color.a), 0.0, 1.0);\n"
"//return;\n"
"\n"
"if (depth/1.88 + monochrome(specular)*8.0 <= float(k) /*&& monochrome(specular) < 0.5*/)\n"
"//if (sqrt(sqrt(depth))/1.88*6.0*2.0 <= float(k))\n"
"o_color = clamp(vec4(color.rgb, color.a*0.5), 0.0, 1.0);\n"
"//discard;\n"
"else\n"
"o_color = clamp(vec4(color.rgb/2.0, color.a), 0.0, 1.0);\n"
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
"/*vec2 vfloor(vec2 n) {\n"
"return vec2(floor(n.x), floor(n.y));\n"
"}*/\n"
"\n"
"\n"
"/*vec4 encode_float_to_color(float value) {\n"
"uint bits = floatBitsToUint(value);\n"
"\n"
"float b0 = float((bits >> 0)  & 0xFF);\n"
"float b1 = float((bits >> 8)  & 0xFF);\n"
"float b2 = float((bits >> 16) & 0xFF);\n"
"float b3 = float((bits >> 24) & 0xFF);\n"
"\n"
"return vec4(b0, b1, b2, b3) / 256.0;\n"
"}\n"
"\n"
"\n"
"float decode_color_to_float(vec4 color) {\n"
"uint b0 = uint(color[0] * 256.0);\n"
"uint b1 = uint(color[1] * 256.0);\n"
"uint b2 = uint(color[2] * 256.0);\n"
"uint b3 = uint(color[3] * 256.0);\n"
"\n"
"uint bits = (b0<<0) | (b1<<8) | (b2<<16) | (b3<<24);\n"
"\n"
"return uintBitsToFloat(bits);\n"
"}*/\n"
"\n"
"\n"
"\n"
"float monochrome(vec4 color) {\n"
"return (color.r*0.299 + color.g*0.587 + color.b*0.114) * color.a;\n"
"}\n"
"\n"
"\n"
"float monochrome(vec3 color) {\n"
"return color.r*0.299 + color.g*0.587 + color.b*0.114;\n"
"}\n"
;
