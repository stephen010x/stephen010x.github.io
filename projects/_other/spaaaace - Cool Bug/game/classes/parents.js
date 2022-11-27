//##########################
// CLASS DEFINITIONS
//##########################

/////////////////////////
// Locatable Class
/////////////////////////
function Locatable(x,y,z) {
    this.x = x;
    this.y = y;
    this.z = z;
    world.items.push(this);
}


/////////////////////////
// Movable Class
/////////////////////////
function Movable(x,y,z,scale,layer) {
	Locatable.call(this, x,y,z);
    Object.assign(Movable.prototype, Locatable.prototype);
	
    this.scale = either(scale,1);
    this.vx = 0;
    this.vy = 0;
    this.angle = 0;
	
	layer = either(layer,0)
    world.logic[layer].push(this);
}

/*Movable.prototype.update = function(dt) {
    return null;
};*/

Movable.prototype.friction = function(dt, friction) {
    this.vx -= this.vx * friction * dt;
    this.vy -= this.vy * friction * dt;
};



