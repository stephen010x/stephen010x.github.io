precision mediump float;

uniform vec2 u_pos;
uniform float u_scale;
uniform float u_rot;
uniform float u_width;
uniform float u_height;

vec2 F(vec2 Z);
int mod(int x, int y);


const int type = 1;


const int ITTER = 100;


int colors_length = 2;
vec4 colors[2];


void main() {

    colors[0] = vec4(0.9, 0.7, 0.2, 1.0);
    colors[1] = vec4(0.2, 0.3, 0.7, 1.0);

    //float color;

    //vec2 coord = gl_FragCoord.xy * u_scale / 200.0 + u_pos;
    vec2 coord = (vec2(gl_FragCoord[0], gl_FragCoord[1]) - vec2(u_width/2.0, u_height/2.0)) * u_scale / 200.0;

    vec2 c;

    // I should've just passed the translation matrix and then multipled
    // it here, rather than just do this manually.
    c.x = coord.x * cos(u_rot) + coord.y * sin(u_rot);
    c.y = coord.x * sin(u_rot) - coord.y * cos(u_rot);

    c += u_pos;

    vec2 Z = vec2(0,0);

    int k = ITTER;
    for (int i = ITTER; (i > 0); i--) {
        Z = F(Z) + c;
        if (length(Z) < 1.0) {
            k--;
        }
    }

    if (type == 0) {
    
        float shade = clamp(float(k)/float(ITTER), 0.0, 1.0);
        gl_FragColor = vec4(shade, shade, shade, 1.0);
    
    } else if (type == 1) {

        vec4 color;

        if (k == 0)
        //    color = vec4(0.0, 0.0, 0.0, 0.0);
            ;
        else {
        
            float div = float(ITTER)/float(colors_length-1);
            int index = int(float(k)/div);
            float mixv = mod(float(k), div) / div;

            // good lord
            if (index == 0) color = mix(colors[0], colors[1], mixv);
            //if (index == 1) vec4 color = mix(colors[1], colors[2], mixv);
            //if (index == 2) vec4 color = mix(colors[2], colors[3], mixv);
            //if (index == 3) vec4 color = mix(colors[3], colors[4], mixv);
        }

        gl_FragColor = color;
        

    }
    

    //k = int(mod(gl_FragCoord.x + gl_FragCoord.y, 100.0));



    /*float shade1 = clamp(float(k)/float(ITTER), 0.0, 1.0);
    float shade2 = clamp(mod(float(k+50)/float(ITTER), 100.0), 0.0, 1.0);
    float shade3 = clamp(mod(float(k+30)/float(ITTER), 100.0), 0.0, 1.0);

    gl_FragColor = vec4(shade1, shade2, shade3, 1.0);*/
}





vec2 F(vec2 Z) {
    return vec2(Z.x*Z.x - Z.y*Z.y, 2.0*Z.x*Z.y);
}


// teardrop
/*
vec2 F(vec2 Z) {
    return vec2(Z.x*Z.x, -Z.y*Z.y);
}
*/
