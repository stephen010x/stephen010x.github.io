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
    world.items.push(this); //Why did I not think of this sooner???
}


/////////////////////////
// Simulatable Class
/////////////////////////
function Movable(x,y,scale,distance,layer) {
	Locatable.call(this, x,y,1);
    Object.assign(Simulatable.prototype, Simulatable.prototype);
	
    this.scale = either(scale,1);
    this.vx = 0;
    this.vy = 0;
    this.angle = 0;
    world.logic[layer].push(this); //Why did I not think of this sooner???
}