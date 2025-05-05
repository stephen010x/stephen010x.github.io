precision mediump float;

uniform sampler2D tex0;
uniform mat4 dithermat4;

varying vec4 color;
varying vec3 norm;
varying vec2 tex_coord;

// good heavens, it is such a pain just to create and index a matrix
/*float dither[16] = float[16](
     0.0/16.0,  8.0/16.0,  2.0/16.0, 10.0/16.0,
    12.0/16.0,  4.0/16.0, 14.0/16.0,  6.0/16.0,
     3.0/16.0, 11.0/16.0,  1.0/16.0,  9.0/16.0,
    15.0/16.0,  7.0/16.0, 13.0/16.0,  5.0/16.0
);*/

/*
    // this creates line bias, so lets do some rotations
    06 12 08 10
    14 04 16 02
    05 11 07 09
    13 03 15 01

    06 12 08 10
    14 04 16 02
    05 11 07 09
    13 03 15 01
*/

// Alright, so there are two ways to discard data here
// the first is discard. The second is just minimizing or maximizing the alpha
// if discard doesn't work, then use the other. (with blending enabled)

void dither(vec4 color);


void main() {
    vec4 tex = texture2D(tex0, tex_coord);
    dither(tex);
}


void dither(vec4 color) {
    // I will burn in hell for this. But so will WebGL1.0
    //mat2 dithermat;
    mat4 dithermat;

    vec2 coord = gl_FragCoord.xy;
    int alpha = int(floor(color.a * 16.0 + 0.5));
    //alpha = int(((coord.x - 50.0) / 100.0)*16.0);



    // more efficient if gpu supports true unanimous branching
    if (alpha == 16) {
        gl_FragColor = vec4(color.rgb, 1.0);
        return;
    } else if (alpha == 0) {
        discard;
        return;
    }

    // good god!
    // the best I can do is pass this as a uniform,
    // but there is also the bigass loop later on just to index it
    dithermat[0][0] = 00.0;
    dithermat[0][1] = 08.0;
    dithermat[0][2] = 02.0;
    dithermat[0][3] = 10.0;
    dithermat[1][0] = 12.0;
    dithermat[1][1] = 04.0;
    dithermat[1][2] = 14.0;
    dithermat[1][3] = 06.0;
    dithermat[2][0] = 03.0;
    dithermat[2][1] = 11.0;
    dithermat[2][2] = 01.0;
    dithermat[2][3] = 09.0;
    dithermat[3][0] = 15.0;
    dithermat[3][1] = 07.0;
    dithermat[3][2] = 13.0;
    dithermat[3][3] = 05.0;


    
    ivec2 index = ivec2(mod(coord, vec2(4.0)));

    // this is madness!
    // this could be optimized by passing a matrix as a 4x4 texture
    int k;
    for (int x = 0; x < 4; x++)
        for (int y = 0; y < 4; y++)
            if (x == index.x && y == index.y)
                k = int(dithermat[x][y]);

    if (alpha > k)
        gl_FragColor = vec4(color.rgb, 1.0);
    else
        discard;
}






    /*dithermat[0][0] = 0.0/4.0;
    dithermat[0][1] = 2.0/4.0;
    dithermat[1][0] = 3.0/4.0;
    dithermat[1][1] = 1.0/4.0;*/



    /*06 12 08 10
    14 04 16 02
    05 11 07 09
    13 03 15 01*/

    /*dithermat[0][0] = 05.0/16.0;
    dithermat[0][1] = 11.0/16.0;
    dithermat[0][2] = 07.0/16.0;
    dithermat[0][3] = 09.0/16.0;
    dithermat[1][0] = 13.0/16.0;
    dithermat[1][1] = 03.0/16.0;
    dithermat[1][2] = 15.0/16.0;
    dithermat[1][3] = 01.0/16.0;
    dithermat[2][0] = 04.0/16.0;
    dithermat[2][1] = 10.0/16.0;
    dithermat[2][2] = 06.0/16.0;
    dithermat[2][3] = 08.0/16.0;
    dithermat[3][0] = 12.0/16.0;
    dithermat[3][1] = 02.0/16.0;
    dithermat[3][2] = 14.0/16.0;
    dithermat[3][3] = 00.0/16.0;*/
