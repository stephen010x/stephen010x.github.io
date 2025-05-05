precision mediump float;

attribute float vert_pos;

uniform mat4 u_proj_mat;
uniform mat4 u_mod_mat;

uniform vec4 u_color;
uniform vec3 u_points[4];

varying vec4 color;



void main() {
    // assume only 64 vertices called to generate
    float t = vert_pos / 64.0;

    float pow0 = 1.0;
    float pow1 = pow0*(1.0-t);
    float pow2 = pow1*(1.0-t);
    float pow3 = pow2*(1.0-t);

    float t0 = 1.0;
    float t1 = t0*t;
    float t2 = t1*t;
    float t3 = t2*t;

    vec3 newpoint = 1.0 * pow3 * u_points[0] * t0 + 
                    3.0 * pow2 * u_points[1] * t1 +
                    3.0 * pow1 * u_points[2] * t2 +
                    1.0 * pow0 * u_points[3] * t3;

    // translate vertex for rasterization
    gl_Position = u_proj_mat * u_mod_mat * vec4(newpoint, 1.0);
    //gl_Position = u_proj_mat * u_mod_mat * vec4(t,t*t,0, 1.0);
    //gl_Position = u_proj_mat * u_mod_mat * vec4(u_points[3], 1.0);
    //gl_Position = u_proj_mat * u_mod_mat * vec4(-t,2.0*t,0, 1.0);
    //gl_Position = u_proj_mat * vec4(t,t,t, 1.0);
    //gl_Position = vec4(0,0,0,1) + 0.01*gl_Position;

    //gl_PointSize = 1.0/gl_Position[3];

    //color = clamp(u_color * t, 0.1, 1.0);
    color = u_color;
}
