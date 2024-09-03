

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
// STAR OBJECT
//##########################
// CHANGE STARS FROM CIRCLES TO TRIANGLES!

function Star(chunk) { //x,y,distance
	var chunkPos = chunk.truePosition();
	
	var n = 1;
	var r = 0;

	var k = 200;
	noiseDetail(1,1); //1,1.5
	//while (n*2 > r) {
		this.x = randInt(chunkPos.x, chunkPos.x + chunk.size.x);
		this.y = randInt(chunkPos.y, chunkPos.y + chunk.size.y);
		this.z = randFloat(chunkPos.z, chunkPos.z + chunk.size.z); //Math.pow(randFloat(-3, 3), 1/3)
		var p = Math.PI;
		n = Math.max(0,Math.min(1,(noise(this.x/k+p,this.y/k+p,this.z*400/k+p) - 0.08) / 0.3));
		r = random();
	//}
	// this is what distributes them in a pattern
	if (n*2 > r) {return};
	
	//var n = Math.max(0,Math.min(1,(noise(this.x/k,this.y/k) - 0.08) / 0.3));
	//console.log(n);
    //this.light = Math.round(n*255*4) //Math.round(Math.pow(randFloat(0, 1),1/3)*255);
	this.light = 255;
	//console.log(this.light);
	var screen = cam.position_to_screen([[this.x, this.y, this.z]])[0]
	this.psx = screen.x;
	this.psy = screen.y;
	//this._dx = 0;
	//this._dy = 0;
	chunk.items.push(this);

	//console.log(this.x, this.y, this.z);
}
//Should I make light the same thing as opacity?
Star.prototype.style = function() {
	//var opacity = Math.max(255/Math.pow(distance,4), 255)
};
Star.prototype.draw = function() {
	this.style();
	var screen = cam.position_to_screen([[this.x, this.y, this.z]])[0];

	var distance = (this.z - cam.z)/1000
    var n = this.light;
	var opacity = 1/distance / (distance2(screen.x,screen.y, this.psx,this.psy)/4 + 1/distance) * 255
	//console.log("opacity: ", opacity);
    //strokeWeight(1/distance);
    // use actual camera functions to calculate a proper stroke weight
    // and perhaps actual volume ratio math to calculate lower opacity due to blur
	strokeWeight(2/distance);
	var color = game.color(n,n,n,opacity)
    stroke(color);
	
	line(screen.x,screen.y, this.psx,this.psy);
	this.psx = screen.x;
	this.psy = screen.y;
};

Star.prototype.update = function(dt) {};

Star.prototype.shader = function(px,py) {};

function createStars(count,chunk) {
	var _list = [];
	for (var i = 0; i < count; i++) {
		_list.push(new Star(chunk));
	}
	return _list;
}



// For generative chunks only, not storative chunks. So no chunk heiarchy
// also this used to be generic, but I kinda mutated it into a star exclusive chunk
function Chunk(x,y,z) {
	this.x = x;
	this.y = y;
	this.z = z;
	this.items = [];
	//console.log(this);

	// mutation starts here
	createStars(20,this);
}

Chunk.prototype = { //Lets just have it start at origin
	size: {
		x: 500,
		y: 500,
		z: 500,
	},
}
Chunk.prototype.truePosition = function() {
	return {
		x: this.x * this.size.x, 
		y: this.y * this.size.y, 
		z: this.z * this.size.z, 
	}
}
Chunk.prototype.update = function(dt, dep) {
	let depth = either(dep, 500);
	// vectors would make this way nicer, just sayin'
	// added mutation that will delete chunk when no longer visable
	let pos = this.truePosition();
	let size = this.size;
	let temp = [[pos.x, pos.y, pos.z + size.z], [pos.x + size.x, pos.y + size.y, pos.z + size.z]]
	let screen = cam.position_to_screen(temp);
	if (screen[0].x > game.width || screen[0].y > game.height ||
		screen[1].x < 0 || screen[1].y < 0 ||
		pos.z - cam.z > depth || pos.z + size.z - cam.z < 0)
		//console.log("gop:", screen, game.width, game.height, pos, cam.z, depth)
		return -1;
	for (var i = 0; i < this.items.length; i++) {
		this.items[i].update(dt);
	}
	return 0;
}

Chunk.prototype.draw = function() {
	for (var i = 0; i < this.items.length; i++) {
		this.items[i].draw();
	}
}

// It may actually be overkill to create classes for all of these. 
// Since these won't be repeated, I should just create custom objects. Not classes.
/*
function Plane(start,end) {
	this.rows = [Row()]
	this.width = start[0] - end[0]
	this.height = start[1] - end[1]
}

function Row(start,end) {
	this.column = [Column()];
}

function Column(start,end) {
	this.chunks = [];
}*/




// I really need to remove this as a class, and turn it into a global object.
function ChunkHandler() {
	world.logic[0].push(this);
	world.layer[0].push(this);

	// captures last place chunks were generated
	//this.p = {x:0, y:0, z:0};
	
	//this.layers = [];
	//this.chunks = [];
	this.chunks = {};

	/*
	for (var z = cam.z; z - cam.z < depth; z+= Chunk.prototype.size.z) {
		var start = cam.screen_to_position({x:0, y:0}, z);
		var end = cam.screen_to_position({x:game.width, y:game.height}, z);
		
		for (var x = start.x; start.x + x < end.x; x += Chunk.prototype.size.x) {
			
			for (var y = start.y; start.y + y < end.y; y += Chunk.prototype.size.y) {
				this.chunks.push(new Chunk(x,y,z));
			}
		}
	}*/
	
	//this.chunker = []
	
	//chunker[z][y][x]
	/*
	
	this.layers = [];
	this.chunks = [];
	
	this.depth = 500;
	let depth = this.depth;
	
	var size = Chunk.prototype.size;
	
	for (var z = cam.z; z - cam.z < depth; z+= size.z) {
		var k = Math.floor(z / size.z);
		var start = cam.screen_to_position({x:0, y:0}, z + size.z);
		var end = cam.screen_to_position({x:game.width, y:game.height}, z + size.z);
		
		var row = [];
		for (var y = start.y; y < end.y; y += size.y) {
				var j = Math.floor(y / size.y);
			
			var column = []
			for (var x = start.x; x < end.x; x += size.x) {
			var i = Math.floor(x / size.x);
				var chunk = new Chunk(i,j,k);
				this.chunks.push(chunk);
				column.push(chunk);
			}
			row.push(column);
		}
		this.layers.push(row);
	}*/
	
	//console.log(this.layers);
	
	
	
	/*for (var z = 0; z < 3/0.2; z++) {
		this.chunks.push(new Chunk(0,0,z));
	}*/
	
	//this.chunks.push(new Chunk(0,0,0));
	
	/*for (var i = 0; i < this.chunks.length; i++) {
		createStars(20,this.chunks[i]);
	}*/
}

ChunkHandler.prototype.draw = function() {
	/*for (var i = 0; i < this.chunks.length; i++) {
		this.chunks[i].draw();
	}*/
	for (let key in this.chunks) {
		this.chunks[key].draw();
	}
}

ChunkHandler.prototype.update = function(dt) {
	// What if instead of generating chunks like this, 
	// I instead just update chunks only in the viewport
	// sort of like a hashed array with positions
	// and if a chunk doesn't exist there, then I create one.
	/*for (var i = 0; i < this.chunks.length; i++) {
		if (this.chunks[i].update(dt)) {
			this.chunks.splice(i, 1);
			i--;
			//console.log("chunk deleted");
		}
	}
	let size = Chunk.prototype.size;
	*/
	//while (p.x - cam.x > size.x) {}

	let size = Chunk.prototype.size;
	let depth = 3*size.z;

	let snap = function(n) {
		return {
			x: ((n.x / size.x) >> 0) * size.x, 
			y: ((n.y / size.y) >> 0) * size.y, 
			z: ((n.z / size.z) >> 0) * size.z,
			width: ((n.width / size.x) >> 0) * size.x,
			height: ((n.height / size.y) >> 0) * size.y,
			depth: ((n.depth / size.z) >> 0) * size.z
		};
	}

	let scam = snap(cam);

	// I forgot to cast to screen coordiantes for the x and y of each z
	for (let z = scam.z; z < scam.z + depth; z += size.z) {
		for (let y = scam.y - size.y; y < scam.y + scam.height; y += size.y) {
			for (let x = scam.x - size.x; x < scam.x + scam.width; x += size.x) {
				let index = [x, y, z].toString();
				if (!(index in this.chunks)) {
					//console.log("gep:", x, y, z);
					var chunk = new Chunk((x/size.x)>>0, (y/size.y)>>0, (z/size.z)>>0);
					this.chunks[index] = chunk;
					//this.layers.push(chunk);
					//console.log(chunk);
					//console.log(chunk);
				}
			}
		}
	}
	for (let key in this.chunks) {
		if (this.chunks[key].update(dt, depth)) {
			delete this.chunks[key];
		}
	}
}



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





//##########################
// DUMB STAR OBJECT
//##########################
// to be used until I get the better one working
/*
function DumbStars() {
	let stars = [];
	for (let i = 0; i < 100; i++) {
		
	}
}

function DumbStar(ux, uy, uz) {
	
}*/
