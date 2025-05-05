precision mediump float;

uniform sampler2D tex0;

varying vec4 color;
varying vec3 norm;
varying vec2 tex_coord;


void main() {
    vec4 tex = texture2D(tex0, tex_coord);
    if (tex.a < 0.5)
        discard;
    if (color.a > 0.0)
        tex = tex * color;
    gl_FragColor = vec4(tex.rgb, 1.0);
}
