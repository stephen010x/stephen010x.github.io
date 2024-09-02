

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

    this.vz = 0;
    
    this.x += this.vx * dt;
    this.y += this.vy * dt;
    this.z += this.vz * dt;
    //this.z += Math.abs(this.vx) * dt/1000;

    // I should create an actual helper function or class so that other
    // classes can utilize this predefined Z plane
    /*let k = 1e9;
    
    let _k = 1/k
    let z_plane = function(x, h) {
        return 1/(_k*x*x + 1/h)
    }

    this.z = 0;
    for (let i = 0; i < world.planets.length; i++) {
        let planet = world.planets[i];
        let x = this.x - planet.x;
        let y = this.y - planet.y;
        let r = Math.sqrt(x*x + y*y);
        this.z += z_plane(r, planet.z);
        //console.log(planet);
    }

    this.z *= 0.75;

    console.log(this.z);*/

    // deepest cone solution
    // the goal here is to always be in view of the target planet
    // and ideally in view of most other planets as well
    let k = 0.4;

    let _k = 1/k;
    let cone = function(x, z, r) {
        let temp = r - Math.abs(x);
        return (temp < 0) ? (z + _k*temp) : (z);
    }
    let tz = 0;
    for (let i = 0; i < world.planets.length; i++) {
        let planet = world.planets[i];
        let x = this.x - planet.x;
        let y = this.y - planet.y;
        let r = Math.sqrt(x*x + y*y);
        let z = cone(r, planet.z, planet.radius);
        /*if (i == 0) {
            console.log(x.toFixed(2), y.toFixed(2), z.toFixed(6));
        }*/
        if (z > tz) {
            tz = z;
        }
    }

    this.z = tz;
    
    
    if (game.control.thrust_start) {
		stream = new Stream();
		this.streams.push(stream);
        //this.streams.push(new Stream());
        //index += 1;
        //stream = this.streams[index];
    }
    
    if (game.control.thrust) {
		//console.log(this.streams.length,stream)
		let vector = {x: Math.cos(this.angle), y: Math.sin(this.angle), z:0}
        this.vx += dt * this.speed * vector.x;
        this.vy += dt * this.speed * vector.y;
		if (game.frame % 1 == 0) {
			// Remember to put out points based derivative, not framerate
			// actually, just cull points when their physics are destroyed
			
			stream.burn(-500, vector, this, dt);
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
    Movable.call(this,0,0,0); // Movable.call(this,0,0,1);
    Object.assign(Stream.prototype, Movable.prototype);
	// BUG!!! This doesn't work if locatable rather than movable
	
	//Locatable.call(this,0,0);
    //Object.assign(Stream.prototype, Locatable.prototype);
    
    Trail3D.call(this, color(99, 171, 199, 200));
    Object.assign(Stream.prototype, Trail3D.prototype);

    //this.poly = [];
    this.deltas = [];
    this.age = 0;
	this.layer = 0;
}

Stream.prototype.burn = function(velocity,vector,carry,dt) { //function(velocity,angle,carry,dt) {
    /*var c = Math.cos(angle);
    var s = Math.sin(angle);
    var vx = velocity * c + carry.vx;
    var vy = velocity * s + carry.vy;
    var x = carry.x - vx*dt - 10*c;
    var y = carry.y - vy*dt - 10*s;
    this.deltas.push([vx,vy]);
    this.poly.push([x, y]);*/
    let vx = velocity * vector.x + carry.vx;
    let vy = velocity * vector.y + carry.vy;
    let vz = velocity * vector.z + carry.vz;
    let x = carry.x - vx*dt - 5*vector.x;
    let y = carry.y - vy*dt - 5*vector.y;
    let z = carry.z - vz*dt - 5*vector.z;
    this.deltas.push([vx, vy, vz]);
    this.trail.push([x, y, z]);
};

Stream.prototype.update = function(dt) {
    var streams = world.player.streams;
	var abs = Math.abs;
    
    if (this !== streams[streams.length - 1] || !game.control.thrust) {
        this.age += dt;
        this.color = game.color(99, 171, 199, 200 - this.age*40);
    }
	for (var i = this.trail.length - 1; i >= 0; i--) {
		var point = this.trail[i];
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
    target[2] -= target[2] * friction * dt;
};

Stream.prototype.destroy = function() {
	world.items.remove(this);
	world.layer[this.layer].remove(this);
	world.logic[this.layer].remove(this);
	world.player.streams.remove(this);
}




Stream.prototype.design = function() {
    var scale = (width / cam.width + height/cam.height*0) / 2*2;
    //stroke(this.color);
    //strokeWeight(5*scale/(this.z - cam.z));
    //noFill();
    //noStroke();
    stroke(this.color);
    strokeWeight(1);
    fill(this.color);
};
