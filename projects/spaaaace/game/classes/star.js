

//# Consider making all stars part of this class, and add in a star slave class
//# If I just want to make stars not bloat the layer pool, then I can create
// a star handler class that creates the stars, and also draws them when 
// drawn called on itself.
//# Consider having stars wrap around from literal position rather than
// screen position. That way uncomfortably close particles won't be always on 
// the screen once I add sized particles. 
//# On that note, I should probably have the particles wrap around from the camera, 
// rather than needing to move them individually
//# Consider the partiles moving on their own like dust. Like Stardust. 


//##########################
// SKY OBJECT
//##########################

function Sky(count) {
	var stars = [];
	world.items.push(this);
	world.layer[0].push(this);
	//world.logic[0].push(this);
}

Sky.prototype.createStars = function(count) {
	for (var i = 1; i < count; i++) {
		this.stars.push(new Star());
	}
}

Sky.prototype.draw = function() {
	for (var i = 1; i < count; i++) {
		this.stars[i].draw();
	}
}

Sky.prototype.update = function(dt) {
	for (var i = 1; i < count; i++) {
		this.stars[i].update(dt);
	}
}




//##########################
// STAR OBJECT
//##########################

function Star() { //x,y,distance
	
    this.x = randInt(0, width);
    this.y = randInt(0, width);
	this.z = randFloat(0.01, 3); //Math.pow(randFloat(-3, 3), 1/3)
    this.light = randInt(0, 255);
    //world.items.push(this);
	//this.p_x = this.x;
    //this.p_y = this.y;
    //this._x = this.x;
    //this._y = this.y;
}
Star.prototype.style = function() {
    var n = this.light;
    strokeWeight(1);
    stroke(n, n, n);
};
Star.prototype.draw = function() {
    this.style();
	var x = this.x - cam.x * this.distance;
    var y = this.y - cam.y * this.distance;
    var star = cam.position_to_screen([x, y])[0];
    var x2 = x % width;
    var y2 = y % height;
    x2 += width  * (x2 < 0);
    y2 += height * (y2 < 0);
    var x1 = x2 + (x - this.x);
    var y1 = y2 + (y - this.y);
    line(x2,y2,x1,y1);
    //Fix line wraparound at high speeds. 
};
Star.prototype.update = function(dt) {
    //this.p_x = this.x;
    //this.p_y = this.y;
    //this.x = this._x - cam.x * this.distance;
    //this.y = this._y - cam.y * this.distance;
};

Star.prototype.shader = function(px,py) {}

/*
Note for creating a sort of "shader", which is basically a prototype modifier that is executed
each time it is drawn.
If only I could find a way to overwrite prototypes
*/

// I need a way to customize the shape being drawn. Maybe that is what the shader can be used for.
// For the shader, don't have it call itself every time. If possible, have it only call itself once for objects
// that have lots of children, and they all just use it. 

// Then again maybe a fucntion call won't be so bad, depending on the overhead. Perhaps a conditional is 
// more appropriate





//////////////////////
// BACKUP
//////////////////////
/*
function Star() { //x,y,distance
    this.x = randInt(0, width);
    this.y = randInt(0, width);
    this.p_x = this.x;
    this.p_y = this.y;
    this._x = this.x;
    this._y = this.y;
    this.distance = Math.pow(randFloat(-3, 3), 1/3);
    this.z = distance;
    //distance of 1 is middleground. Positive is foreground. Negative is background.
    //Positions are middleground regardless of distance.
    this.light = randInt(0, 255);
    world.items.push(this);
}
Star.prototype.style = function() {
    var n = this.light;
    strokeWeight(1);
    stroke(n, n, n);
};
Star.prototype.draw = function() {
    this.style();
    var star = cam.position_to_screen([this.x, this.y])[0];
    var x2 = this.x % width;
    var y2 = this.y % height;
    x2 += width  * (x2 < 0);
    y2 += height * (y2 < 0);
    var x1 = x2 + (this.x - this.p_x);
    var y1 = y2 + (this.y - this.p_y);
    line(x2,y2,x1,y1);
    //Fix line wraparound at high speeds. 
};
Star.prototype.update = function(dt) {
    this.p_x = this.x;
    this.p_y = this.y;
    this.x = this._x - cam.x * this.distance;
    this.y = this._y - cam.y * this.distance;
};
*/