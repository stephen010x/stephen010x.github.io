

/////////////////////////
// Player Class
/////////////////////////
function Player(x,y){
    Movable.call(this,x,y,1,1,0);
    Object.assign(Player.prototype, Movable.prototype);
    
    Polygon.call(this, color(255,255,255));
    Object.assign(Player.prototype, Polygon.prototype);
    
    this.speed = 300;
    this.rotspeed = 5;
    
    this.poly = [[10,0],[-7,7],[-3,0],[-7,-7]];
    this.streams = [];
    this.stream_index = 0;
    //Perhaps turn stream into an array with a start pointer. Use modulus in loops.
}

Player.prototype.update = function(dt) {
    var index = this.streams.length - 1;
    var stream = this.streams[index];
    
    this.x += this.vx * dt;
    this.y += this.vy * dt;
    
    if (game.control.thrust_start) {
		stream = new Stream();
		this.streams.push(stream);
        //this.streams.push(new Stream());
        //index += 1;
        //stream = this.streams[index];
    }
    
    if (game.control.thrust) {
		//console.log(this.streams.length,stream)
        this.vx += dt * this.speed * Math.cos(this.angle);
        this.vy += dt * this.speed * Math.sin(this.angle);
		if (game.frame % 1 == 0) {
			stream.burn(-500, this.angle, this, dt);
		}
    }
    if (game.control.turn_right) {
        this.angle += dt * this.rotspeed;
    }
    if (game.control.turn_left) {
        this.angle -= dt * this.rotspeed;
    }
    
    this.friction(dt, 0.5);
	
};


Player.prototype.design = function() {
    stroke(0,0,0);
    strokeWeight(1);
    fill(this.color);
};




/////////////////////////
// Stream Class
/////////////////////////
function Stream() {
    Movable.call(this,0,0);
    Object.assign(Stream.prototype, Movable.prototype);
	// BUG!!! This doesn't work if locatable rather than movable
	
	Locatable.call(this,0,0);
    Object.assign(Stream.prototype, Locatable.prototype);
    
    Polygon.call(this, color(99, 171, 199, 200));
    Object.assign(Stream.prototype, Polygon.prototype);

    this.poly = [];
    this.deltas = [];
    this.age = 0;
	this.layer = 0;
}

Stream.prototype.burn = function(velocity,angle,carry,dt) {
    var c = Math.cos(angle);
    var s = Math.sin(angle);
    var vx = velocity * c + carry.vx;
    var vy = velocity * s + carry.vy;
    var x = carry.x - vx*dt - 10*c;
    var y = carry.y - vy*dt - 10*s;
    this.deltas.push([vx,vy]);
    this.poly.push([x, y]);
};

Stream.prototype.update = function(dt) {
    var streams = world.player.streams;
	var abs = Math.abs;
    
    if (this !== streams[streams.length - 1] || !game.control.thrust) {
        this.age += dt;
        this.color = game.color(99, 171, 199, 200 - this.age*40);
    }
	for (var i = this.poly.length - 1; i >= 0; i--) {
		var point = this.poly[i];
		var delta = this.deltas[i];
		if (abs(delta[0]) + abs(delta[1]) < 35) {break;}
		point[0] += delta[0]*dt;
		point[1] += delta[1]*dt;
		this.pointfriction(dt, 3, delta);
	}
    if (this.age > 5) {
		this.destroy();
    }
};

Stream.prototype.pointfriction = function(dt,friction,target) {
    target[0] -= target[0] * friction * dt;
    target[1] -= target[1] * friction * dt;
};

Stream.prototype.destroy = function() {
	world.items.remove(this);
	world.layer[this.layer].remove(this);
	world.logic[this.layer].remove(this);
	world.player.streams.remove(this);
}




Stream.prototype.design = function() {
    var scale = (width / cam.width + height/cam.height*0) / 2*2;
    stroke(this.color);
    strokeWeight(5*scale);
    noFill();
};
