#version 300 es
precision mediump float;

in float depth;
//varying vec4 position;

//vec4 encode_float_to_color(float value);
//float decode_color_to_float(vec4 color);

//out float o_depth;
out vec4 o_color;


void main() {
    o_color.r = depth;
    o_color.a = 1.0;
    //gl_FragColor.a = 1.0;
    //o_depth = depth;
}


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
}
*/
