#ifndef PHYSICS_H
#define PHYSICS_H

#include <cglm/cglm.h>
#include <stdbool.h>



// TODO: I think air drag might be messing up perfectly inelastic collisions
//#define DRAG_COEFF 0.2
#define DRAG_COEFF 0
#define DENSITY 1
//#define ELASTICITY 0.5
#define ELASTICITY 0.3
//#define ELASTICITY 0
//#define ELASTICITY 1
//#define BOUNCE_THRESH 0.01
#define BOUNCE_THRESH 0

#define DT_DIVISOR 8


// device is assumed to be at rest
#define GRAVITY {0, 0, -0.001}
//#define GRAVITY {0, 0, 0}


typedef struct {
    // states
    vec3 pos;
    vec3 vel;

    // params
    float scale;
    float mass;

    // reset each frame
    vec3 impulse;
    vec3 deltav;
    vec3 shift;

} BEHAVE;


extern vec3 gravity;


float getdist(vec3 p1, vec3 p2);
void setprojected(BEHAVE* b, float dt, vec3 vel, vec3 pos);


// two pass system.
// the first pass is object dependant. 
// the second pass is the apply and reset below

void behave_apply(BEHAVE* b, double t, float dt);

// NOTE all of these add on to output vectors, not set them.
// ALSO, make sure to call each of these functions in the order listed here
void collision_shift  (BEHAVE* b1, BEHAVE* b2, vec3 norm, float overlap);
void collision_impulse(BEHAVE* b1, BEHAVE* b2, vec3 norm);
void collision_deltav (BEHAVE* b1, BEHAVE* b2, vec3 norm);
/*void collision_shift  (BEHAVE* b1, BEHAVE* b2, vec3 norm, vec3 s1o, vec3 s2o);
void collision_impulse(BEHAVE* b1, BEHAVE* b2, vec3 norm, vec3 i1o, vec3 i2o);
void collision_deltav
    (BEHAVE* b1, BEHAVE* b2, vec3 norm, float overlap, float dt, vec3 d1o, vec3 d2o);*/

bool sphere_collision_detect
    (BEHAVE* b1, BEHAVE* b2, float dt, float* overlap, vec3 normout);

void sphere_collide_eval(BEHAVE* b1, BEHAVE* b2, float dt);
void wall_collide_eval(BEHAVE* b, vec3 start, vec3 end, float dt);




#endif






      ///////////////////////////////////////////
     ///////////////////////////////////////////
    
         /*  NOTES  */
  
 ///////////////////////////////////////////
///////////////////////////////////////////




/* Alright, I figured out why the collisions were failing. 
 * The problem is that the force of gravity exherts a velocity that is 
 * basically canceled out in an infintesimal timestep system.
 * This is because the force of a collision is almost always several
 * orders of magnitude larger than other forces such as gravity.
 * In an idealistic system, the force of a collision is infinitely
 * greater than any other force due to the instantaneous change of 
 * velocity. So we will be using that metric.
 * So basically, the solution is have two changes of velocity, both
 * only applied at the end of the frame. The first is the velocity change
 * due to acceleration, and the second is the velocity change due to an impulse.
 *
 * so basically to summarize
 * 1) Create an impulse system that cancels out acceleration system on a normal
 * 2) Only apply changes to states at end of frame (position and velocity)
 */







/* I wonder if I should add something similar to impulse that is more position based
 * Also, remember to use projected velocity and position for collision detections
 * Well, maybe not. The goal is to use the current velocity for collision detection,
 * While also being able to cancel out forces inferior to the impulse.
 * If I don't use projected velocity and position, then the velocity applied
 * by gravity will be applied in the next frame, justifying an unnatural bounce.
 * But if I do use projected velocity, it will cancel out the force in that frame.
 * But then it would apply it's own impulse having it bounce up anyway.
 * What I seem to need is to have collisions due to delta v to be canceled,
 * And for collisions due to true velocity to apply impulse
 * 
 * The position adjuster can perhaps be the one that cancels the rest?
 *
 * The position version of impulse can just be a "shift".
 * So the shift is weirder than impulse, because the impulse is a force
 * and the force isn't a state.
 * However, the velocity is a state, and applying a "shift" would technically
 * generate infinite velocity, which is unideal.
 * So the "shift" is more of a magic property than a realistic one.
 * And I can thereby justify it fixing what the other collision can't
 */


// Note that the latter looks better for block commenting than the previous.
// Just think about that for future block comments.

/*
Object is stationary, delta-v is applied to it
Collision is detected to occur as a result of delta-v (projected position)
    Collision uses current velocity to generate impulse
        If there is no velocity, but collision occurs, then 
        there is no impulse. Collision would have been caused by projected position
    Object is shifted to be just barely touching device rather than inside it.
    So when a collision occurs, the next frame pretends that it begins at the 
    exact moment of collision.

    Perhaps that is a philosophy that I can take adavantage of, 
    "the next frame pretends to begin at the exact moment of collision"
    Of course we should never see this beginning, as the behave function should
    move it visably by the end of that frame... unless the object is at rest.
    We basically need just enough information to be able to tell the accurate state
    of an object to adjust it for the next frame to be accurate.

    Lets say we reach the end of a frame. We should have these things:
    A shift
    An impulse
    A delta v
    A velocity
    A position

    First, what do we do if there is a shift?
    I imagine the shift could be applied either at the beginning or the
    end. It shouldn't matter which order. The position is only useful for 
    collisions anyway.

    Lets look at what the frame of reference would look like
    An object resting atop another with gravity.
    It would have no previous velocity. It would have no current velocity
    It would have a force pushing down, and a force resisting pushing up
    It would be placed right at infintesimal contact

    Now an object colliding with another with gravity
    It would have previous velocity. I would have no current velocity
    It would have a force pushing down, and an IMPULSE pushing up. (infinite force)
    It would be placed at infintesimal contact

    The differences:
    First has force up, whereas second has impulse up.
        Note that impulse overwhelms any force parallel
    The first has no previous velocity, whereas the second does

    So basically, when collision occurs, three things happen
    If it is overlapping with the object, shove it to the edge
    If there is a current normal velocity, then apply an impulse to basically reverse it
    If there is a current normal delta v, then oppose it to zero it out.


////////////////////////////
///////////////////////////
////    So to finalize
////     - The starting velocity is zero, and the delta v is reset to gravity.
////     - Next we detect if a collision occurs in the next frame using 
////       projected position.
////        - We perform the shift collision using current position and normal
////        - We perform impulse collision using current velocity and normal
////        - We perform delta-v collision using current delta-v and normal
/////////////////////
////////////////////

    To test this, lets say we have an object resting atop another with gravity
    Projected position is calculated using current delta-v and velocity and position
    A collision is found to be immenent, so we perform the three collision adjustments.
        We first shift it so that it is outside of the other object
        The current velocity is zero, so there is no impulse.
        There is current delta-v, so we oppose that, calceling out gravity for frame.
    So this yields an adjusted position at contact, no impulse, and no normal
    delta-v. This means that the object will be moved to contact location, and 
    will remain stationary.


    Now how is multi-collision going to be handled?
    Say we have a box that is in a corner, and gravity is 
    forcing it into that corner. That box is inevitably 
    colliding with two different other objects.

    The first thing is that the collision algorithm only needs to be 
    ran once for every two objects.

    My main concern is multi-collision at rest, as that is mainly where
    it would occur, although it can occur while in motion too due to 
    the non-infintesimal timestep.
    Lets say an object collides with two objects, one after the other.

    I think this requires me to have a three pass physics engine.
    The first pass to reset stuff. The second pass to set stuff. And a 
    third pass to apply stuff. This way objects can apply sum changes to 
    another without being erased once the next object needs to be reset
    upon update.

    One philosophy of this physics system is that everything is only ever 
    summed. never directly changed. And it all mainly has to be based on their 
    current states rather than their future states so that there is no bias 
    in order of processing for each object. (Although this can potentially make 
    shifts get out of hand. I should see if that would negatively affect the 
    collision shift function).

    So the shift is obvious. But it is also dangerous.
    Luckily the collision shift doesn't perform a huge shift, but instead
    a small enough shift just to offset it from intersecting.
    And most of the time multi-collision shifts will be on nearly
    perpendicular axis anyway.
    A multi-collision shift has the potential to push an object such that
    is no longer in contact, or sometimes even overlapping into another object.
    This will create some oscillation. However, I believe it is an over-damped
    oscillation, so it should subside.
    One legitimate danger I see though is an object colliding with many tiny objects
    all at once. Though the difference in mass should heavily reduce this effect,
    if all of the smaller objects were much denser, and equal mass to the larger
    mass, then the shift would compound into an uncomfortably large offset.
    And I am not confident this would be damped enough to be stable and come to
    a rest.

    Next up the delta-v. Hmm...
    This is an equal and opposite 

    Actually, this three pass system can be reduced to two, as the 
    reset and the apply pass can be combined into one pass.
    And then we add another pass that determines the number of colliding objects.

    So the passes go like this,
     - Detection pass
     - Resolution pass
     - Apply pass

     The detection creates a list of all objects that you collide with.
     The resolution has all of the collisions add their component to the vectors
     And the apply pass divides each vector by the number of collisions to 
     get an average, and then apply it.

     Now, this is already a bad system. Because what if you get slammed by one object
     while tapped by another in one frame? The big collision effectively gets
     cut in half.

     I suppose the impulse vector doesn't need to be averaged.
     And the delta-v? Probably not either.
     And what about the shift vector? I suppose the shift

     Yeah... Lets just keep this to the two pass and face the consiquences.
     Besides, having more than two collisions at once is *very hard to do* in
     2D space. And similarly more than three in 3D space.
*/


