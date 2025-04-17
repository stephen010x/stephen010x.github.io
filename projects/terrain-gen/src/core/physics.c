#include <stdio.h>
#include "../main.h"
#include "physics.h"


// can change, so don't set to const
vec3 gravity = GRAVITY;


extern bool behave_flag;


void printvec(const char* name, vec3 v) {
    printf("%s (%f, %f)\n", name, v[0], v[1]);
}

#define PRINTVAR(__t, __n) do {         \
        printf(#__n " " __t "\n", __n); \
    } while(0)




// calculate projected values
// can be used to set them too
void setprojected(BEHAVE* b, float dt, vec3 vel, vec3 pos) {

    vec3 deltap, deltav;
    
    // scale delta-v by dt and add it to velocity
    // actually nevermind, scaling it might be messingit up
    //glm_vec3_scale(b->deltav, dt, deltav);
    //glm_vec3_add(b->vel, deltav, vel);
    glm_vec3_add(b->vel, b->deltav, vel);

    // add impulse to velocity
    glm_vec3_add(vel, b->impulse, vel);

    // add shift to position
    glm_vec3_add(b->pos, b->shift, pos);
    
    // scale velocity by dt and add to position
    glm_vec3_scale(vel, dt, deltap);
    glm_vec3_add(pos, deltap, pos);
}




// for some reason this works better than cglm's distance calculator?
float getdist(vec3 p1, vec3 p2) {
    float dx = p2[0] - p1[0];
    float dy = p2[1] - p1[1];
    float dz = p2[2] - p1[2];
    return sqrt(dx*dx + dy*dy + dz*dz);
}




void behave_apply(BEHAVE* b, double t, float dt) {

    /*printf("---------setprojected---------\n");
    printvec("pvel", b->vel);
    printvec("ppos", b->pos);*/

    // apply vectors to states
    /*glm_vec3_add(b->vel, b->deltav,  b->vel);
    glm_vec3_add(b->vel, b->impulse, b->vel);
    glm_vec3_add(b->pos, b->shift,   b->pos);
    vec3 vel;
    glm_vec3_scale(b->vel, dt, vel);
    glm_vec3_add(b->pos, vel, b->pos);*/
    // apply vectors to states
    setprojected(b, dt, b->vel, b->pos);

    /*printvec("deltav", b->deltav);
    printvec("impulse", b->impulse);
    printvec("nvel", b->vel);
    printvec("npos", b->pos);*/
    
    // reset vectors
    glm_vec3_zero(b->impulse);
    glm_vec3_zero(b->deltav);
    glm_vec3_zero(b->shift);

    // TODO: I could have objects have a list of vector forces that
    //       apply to them. Only they are pointer, so many different
    //       objects could be affected by the same force.
    // apply gravity for next frame
    glm_vec3_add(b->deltav, gravity, b->deltav);
}




// TODO:
// maybe assume that there will only ever be one collision
// for each object. And decide which collision that would be
// by aslecting the largest offset.
// TODO: try to figure out why this one isn't working as well as old one.
// you will notice I just repurposed the old one until I get this one working
// It is basically suppose to do the exact same thing as the old one, only 
// with less steps
void collision_impulse(BEHAVE* b1, BEHAVE* b2, vec3 normin) {
    // get masses
    register float m1 = b1->mass;
    register float m2 = b2->mass;
    
    // enforce normal to be unit vector
    vec3 norm;
    glm_vec3_normalize_to(normin, norm);

    // perform dot product between velocities and normal
    float dot1 =  glm_vec3_dot(b1->vel, norm);
    float dot2 = -glm_vec3_dot(b2->vel, norm);

    // If difference of velocity is same direction as normal, then
    // ignore collision impulse
    //if (dot1 + dot2 >= 0)
    //    return;

    // calculate magnitudes of new velocity
    // https://en.wikipedia.org/wiki/Inelastic_collision
    float v1x, v2x;
    if (m2 == INFINITY) {
        v1x = -(ELASTICITY*(dot2-dot1) + dot2);
        v2x = -dot2;
    } else {
        dot2 = -dot2;
        v1x = -(ELASTICITY*m2*(dot2-dot1) + m1*dot1 + m2*dot2) / (m1+m2);
        v2x = -(ELASTICITY*m1*(dot1-dot2) + m2*dot2 + m1*dot1) / (m1+m2);
    }

    // create output sum vector using new velocity minus old normal
    // this cancels out the old normal velocity for the new one via impulse
    vec3 v1add, v2add;
    glm_vec3_scale(norm, -ABS(v1x) - dot1, v1add);
    glm_vec3_scale(norm, -ABS(v2x) - dot2, v2add);

    // add our adjusted velocity normals back into the output impulse
    glm_vec3_add(b1->impulse, v1add, b1->impulse);
    glm_vec3_add(b2->impulse, v2add, b2->impulse);
}



void __collision_impulse(BEHAVE* b1, BEHAVE* b2, vec3 normin) {

    float coeff = ELASTICITY;

    // get masses
    register float m1 = b1->mass;
    register float m2 = b2->mass;

    //printf("mass %f, %f\n", m1, m2);
    
    // enforce normal to be unit vectors
    vec3 n1, n2;
    glm_vec3_normalize_to(normin, n2);
    glm_vec3_negate_to(n2, n1);

    // perform dot product between velocities and normal
    float dot1 = glm_vec3_dot(b1->vel, n1);
    float dot2 = glm_vec3_dot(b2->vel, n2);

    // gah! Why is bounce threshold still needed???
    // T/ODO: find a way to get rid fo this!
    // Good news! The bounce threshold does absolutely
    // nothing to fix the jitter. So it is not needed!
    //printf("dot sum %f+%f=%f\n", dot1, dot2, dot1+dot2);
    //if (BOUNCE_THRESH != 0 && ABS(dot1+dot2) < BOUNCE_THRESH)
    //    coeff = 0;
    //    return;
    
    // calculate velocities parallel to normal 
    vec3 v1n, v2n;
    glm_vec3_scale(n1, dot1, v1n);
    glm_vec3_scale(n2, dot2, v2n);
    
    // subtract normal velocity from original velocity
    // we add in the *adjusted* normal velocity back in later
    glm_vec3_sub(b1->impulse, v1n, b1->impulse);
    glm_vec3_sub(b2->impulse, v2n, b2->impulse);

    // get magnitudes of NORMAL velocities for collision formula
    float v1s = glm_vec3_norm(v1n);
    float v2s = glm_vec3_norm(v2n);

    // calculate magnitudes of new velocity
    // https://en.wikipedia.org/wiki/Inelastic_collision
    float v1x, v2x;
    if (m2 == INFINITY) {
        v1x = -(coeff*(v2s-v1s) + v2s);
        v2x = -v2s; // probably adjust back to positive due to how I handle normals
    } else {
        v1x = -(coeff*m2*(v2s-v1s) + m1*v1s + m2*v2s) / (m1+m2);
        v2x = -(coeff*m1*(v1s-v2s) + m2*v2s + m1*v1s) / (m1+m2);
    }

    // create normal velocity vector with new scalar values
    glm_vec3_scale(n1, v1x, v1n);
    glm_vec3_scale(n2, v2x, v2n);

    // add our adjusted velocity normals back into the output velocity
    glm_vec3_add(b1->impulse, v1n, b1->impulse);
    glm_vec3_add(b2->impulse, v2n, b2->impulse);
}




// this uses previous deltav vector and adjusts it
// so make sure to call this after other changes to deltav
// TODO: I realize that I need to cancel out unshared delta-v
// rather than all delta-v
void collision_deltav(BEHAVE* b1, BEHAVE* b2, vec3 norm) {
    // I mean, this is pretty straightforward. It just cancels out the 
    // delta-v on the normal.

    // get relative delta-v
    vec3 rdv;
    glm_vec3_sub(b2->deltav, b1->deltav, rdv);

    // get magnitude of delta-v normal
    float dot = glm_vec3_dot(b1->deltav, norm);


    /////////
    // old
    
    // get magnitude of delta-v normal
    float dot1 = glm_vec3_dot(b1->deltav, norm);
    float dot2 = glm_vec3_dot(b2->deltav, norm);

    // scale normal units by magnitude
    vec3 nd1, nd2;
    glm_vec3_scale_as(norm, dot1, nd1);
    glm_vec3_scale_as(norm, dot2, nd2);

    /*printf("---------collision_deltav---------\n");
    printvec("b1->deltav", b1->deltav);
    PRINTVAR("%f", dot1);
    printvec("nd1", nd1);*/

    // subtract inverted normal delta-v from original delta-v
    glm_vec3_sub(b1->deltav, nd1, b1->deltav);
    glm_vec3_sub(b2->deltav, nd2, b2->deltav);
}




// this one might actually be a bit complicated, as it needs to use the proposed
// position for both objects before shifting them.
// as a result, this needs to be called first.
void collision_shift(BEHAVE* b1, BEHAVE* b2, vec3 norm, float overlap) {

    register float m1 = b1->mass;
    register float m2 = b2->mass;

    float r1, r2;

    if (m2 == INFINITY) {
        r1 = overlap;
        r2 = 0;
    } else if (m1 == m2) {
        r2 = r1 = overlap / 2;
    } else {
        float k = overlap/ABS(m2+m1);
        r1 = m2*k;
        r2 = m1*k;
    }

    // scale norm by magnintude of r's
    vec3 p1o, p2o;
    glm_vec3_scale_as(norm, -r1, p1o);
    glm_vec3_scale_as(norm,  r2, p2o);

    // add scaled norm to shift
    glm_vec3_add(p1o, b1->shift, b1->shift);
    glm_vec3_add(p2o, b2->shift, b2->shift);
}




// remember to use projected position, not current position
bool sphere_collision_detect
    (BEHAVE* b1, BEHAVE* b2, float dt, float* overlap, vec3 normout) {

    // get projected states
    vec3 vel1, vel2, pos1, pos2;
    setprojected(b1, dt, vel1, pos1);
    setprojected(b2, dt, vel2, pos2);

    // calculate distance
    float dist = getdist(pos1, pos2);

    // get radius for each
    register float rad1 = b1->scale;
    register float rad2 = b2->scale;

    // calculate overlap
    *overlap = rad1 + rad2 - dist;

    if (*overlap < 0 || isnan(*overlap))
        return false;

    //*DEBUG*/ behave_flag = false;

    // calculate normal of collision
    glm_vec3_sub(pos1, pos2, normout);

    return true;
}



void sphere_collide_eval(BEHAVE* b1, BEHAVE* b2, float dt) {
    float overlap;
    // TODO: get rid of anorm. It is just ducktape
    vec3 norm, anorm;
    // if sphere collision is detected, then apply collision transformations
    if (sphere_collision_detect(b1, b2, dt, &overlap, norm)) {
        // TODO Not really sure why we need to invert the norm here
        // please figure this out
        glm_vec3_negate_to(norm, anorm);
        collision_shift(b1, b2, anorm, overlap);
        collision_impulse(b1, b2, norm);
        // TODO: collision doesn't seem to fix jitter.
        // it behaves very similarly without it.
        // FIX THIS!
        //collision_deltav(b1, b2, norm);
    }
}



void _wall_collide(BEHAVE* b, vec3 norm, float overlap);


// TODO: find a more vectorized solution to this rather than expanding them
void wall_collide_eval(BEHAVE* b, vec3 start, vec3 end, float dt) {

    // get projected state
    vec3 vel, pos;
    setprojected(b, dt, vel, pos);

    // expand start and ends to convenient values
    register float sx = start[0];
    register float sy = start[1];
    register float sz = start[2];
    register float ex = end[0];
    register float ey = end[1];
    register float ez = end[2];

    // expand behave values to convenient values
    register float rad = b->scale;
    register float x = pos[0];
    register float y = pos[1];
    register float z = pos[2];

    // test wall collisions
    if (x + rad > ex) _wall_collide(b, (vec3){ 1, 0, 0},  x+rad-ex);
    if (x - rad < sx) _wall_collide(b, (vec3){-1, 0, 0}, -x+rad+sx);
    if (y + rad > ey) _wall_collide(b, (vec3){ 0, 1, 0},  y+rad-ey);
    if (y - rad < sy) _wall_collide(b, (vec3){ 0,-1, 0}, -y+rad+sy);
    if (z + rad > ez) _wall_collide(b, (vec3){ 0, 0, 1},  z+rad-ez);
    if (z - rad < sz) _wall_collide(b, (vec3){ 0, 0,-1}, -z+rad+sz);
}


void _wall_collide(BEHAVE* b, vec3 norm, float overlap) {

    // dummy wall behavior
    static BEHAVE wall = {
        .mass = INFINITY,
    };

    // ducktape. remove later
    vec3 anorm;
    glm_vec3_negate_to(norm, anorm);

    // apply collision transformations
    collision_shift(b, &wall, norm, overlap);
    collision_impulse(b, &wall, norm);
    // TODO: collision doesn't seem to fix jitter.
    // it behaves very similarly without it.
    // FIX THIS!
    //collision_deltav(b, &wall, norm);
}
