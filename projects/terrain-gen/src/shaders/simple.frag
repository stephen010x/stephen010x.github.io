#version 300 es
precision mediump float;

in vec4 v_color;
//varying float depth;

out vec4 out_color;

void main() {
    out_color = v_color;
    //gl_FragDepth = depth;
}
