#version 300 es
precision mediump float;

uniform vec4 u_wire_color;
uniform float u_wire_range;

in float v_depth;

out vec4 out_color;

void main() {
    //gl_FragColor = vec4(u_wire_color.rgb, 1.0 - u_wire_color.a * v_depth / u_wire_range);
    out_color = vec4(1.0 - u_wire_color.rgb * v_depth / u_wire_range, 1.0);
}
