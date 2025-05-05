precision mediump float;

uniform sampler2D tex0;

//varying vec4 color;
//varying vec3 norm;
//varying vec2 tex_coord;

uniform float u_swidth;
uniform float u_sheight;

void main() {
    vec2 index = vec2(gl_FragCoord.x / u_swidth, gl_FragCoord.y / u_sheight);
    vec4 tex = texture2D(tex0, index);
    gl_FragColor = vec4(tex.rgb, 1.0);
    //float k = gl_FragCoord.z;
    //gl_FragColor = vec4(k, k, k, 1.0);
}
