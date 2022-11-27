
//#############################################################
//#############################################################
//
//		THIS IS ALL JUNK CODE
//
//	This is all pratcially garbage, but mostly to
//	help me with an ease of mind when I delete or change code.
//
//#############################################################
//#############################################################



//##########################
// STAR OBJECT
//##########################

function Star(chunk) { //x,y,distance
	chunk.items.push(this);
    this.x = randInt(chunk.x, chunk.x + chunk.size.x);
    this.y = randInt(chunk.y, chunk.y + chunk.size.y);
	this.z = randInt(chunk.z, chunk.z + chunk.size.z); //Math.pow(randFloat(-3, 3), 1/3)
    this.light = randInt(0, 255);
	this.px = 0;
	this.py = 0;
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
	// Remember to modulous around a literal position rather than the screen position
	// A star distribution would be awesome. Maybe I should actually just generate stars. 
    this.style();
	var x = (this.x - cam.x) / this.z;
    var y = (this.y - cam.y) / this.z;
    var star = cam.position_to_screen([x, y])[0];
    var x2 = x// % width;
    var y2 = y// % height;
    //x2 += width  * (x2 < 0);
    //y2 += height * (y2 < 0);
    var x1 = x2 + (this.px - x)
    var y1 = y2 + (this.py - y)
	line(x2,y2,this.px,this.py);
	this.px = x2
	this.py = y2
    //Fix line wraparound at high speeds. 
};
Star.prototype.update = function(dt) {
    //this.p_x = this.x;
    //this.p_y = this.y;
    //this.x = this._x - cam.x * this.distance;
    //this.y = this._y - cam.y * this.distance;
};

Star.prototype.shader = function(px,py) {}





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





//##########################
// SKY OBJECT
//##########################
/*
function Sky(count) {
	this.chunks = [];
	world.items.push(this);
	world.layer[0].push(this);
	//world.logic[0].push(this);
}

//I should probably input chunk numbers as a chunk rectangle range.
// chunk = {xnum,ynum,znum,width}
// I should not have each chunk store their own width. I should try to establish something more universal.
// Make sure chunk positions are integers
Sky.prototype.createStars = function(count,chunk) {
	for (var i = 0; i < count; i++) {
		this.stars.push(new Star(chunk));
	}
}

Sky.prototype.draw = function() {
	for (var i = 1; i < this.stars.length; i++) {
		this.stars[i].draw();
	}
}

Sky.prototype.update = function(dt) {
	for (var i = 1; i < this.stars.length; i++) {
		this.stars[i].update(dt);
	}
}
*/