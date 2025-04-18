
 /*
  *    ==== DO NOT EDIT THIS FILE ====
  */

const char wireframe_frag[]="#version 300 es\n"
"precision mediump float;\n"
"\n"
"uniform vec4 u_wire_color;\n"
"uniform float u_wire_range;\n"
"\n"
"in float v_depth;\n"
"\n"
"out vec4 out_color;\n"
"\n"
"void main() {\n"
"//gl_FragColor = vec4(u_wire_color.rgb, 1.0 - u_wire_color.a * v_depth / u_wire_range);\n"
"out_color = vec4(1.0 - u_wire_color.rgb * v_depth / u_wire_range, 1.0);\n"
"}\n"
;
