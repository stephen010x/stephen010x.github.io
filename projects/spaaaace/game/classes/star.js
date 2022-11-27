

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

function Star(chunk) { //x,y,distance
	var chunkPos = chunk.truePosition();
	
	var n = 1;
	var r = 0;

	var k = 200
	noiseDetail(1,1); //1,1.5
	//while (n*2 > r) {
		this.x = randInt(chunkPos.x, chunkPos.x + chunk.size.x);
		this.y = randInt(chunkPos.y, chunkPos.y + chunk.size.y);
		this.z = randFloat(chunkPos.z, chunkPos.z + chunk.size.z); //Math.pow(randFloat(-3, 3), 1/3)
		var p = Math.PI;
		n = Math.max(0,Math.min(1,(noise(this.x/k+p,this.y/k+p,this.z/k+p) - 0.08) / 0.3));
		r = random();
	//}
	if (n*2 > r) {return};
	
	//var n = Math.max(0,Math.min(1,(noise(this.x/k,this.y/k) - 0.08) / 0.3));
	console.log(n);
    //this.light = Math.round(n*255) //Math.round(Math.pow(randFloat(0, 1),1/3)*255);
	this.light = 255;
	//console.log(this.light);
	var screen = cam.position_to_screen([[this.x, this.y, this.z]])[0]
	this.psx = screen.x;
	this.psy = screen.y;
	//this._dx = 0;
	//this._dy = 0;
	chunk.items.push(this);
}
//Should I make light the same thing as opacity?
Star.prototype.style = function() {
	//var opacity = Math.max(255/Math.pow(distance,4), 255)
};
Star.prototype.draw = function() {
	this.style();
	var screen = cam.position_to_screen([[this.x, this.y, this.z]])[0];

	var distance = this.z - cam.z
    var n = this.light;
	var opacity = 1/distance / (distance2(screen.x,screen.y, this.psx,this.psy)/4 + 1/distance) * 255
    //strokeWeight(1/distance);
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
function Chunk(x,y,z) {
	this.x = x;
	this.y = y;
	this.z = z;
	this.items = [];
	//console.log(this);
}

Chunk.prototype = { //Lets just have it start at origin
	size: {
		x: 400,
		y: 400,
		z: 0.5,
	},
}
Chunk.prototype.truePosition = function() {
	return {
		x: this.x * this.size.x, 
		y: this.y * this.size.y, 
		z: this.z * this.size.z, 
	}
}
Chunk.prototype.update = function(dt) {
	for (var i = 0; i < this.items.length; i++) {
		this.items[i].update(dt);
	}
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
	
	//this.layers = [];
	//this.chunks = [];

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
	
	this.layers = [];
	this.chunks = [];
	
	var depth = 2;
	var size = Chunk.prototype.size
	
	for (var z = cam.z; z - cam.z < depth; z+= size.z) {
		var k = Math.floor(z / size.z);
		var start = cam.screen_to_position({x:0, y:0}, z + size.z);
		var end = cam.screen_to_position({x:game.width, y:game.height}, z + size.z);
		
		var row = []
		for (var y = start.y; y < end.y; y += size.y) {
				var j = Math.floor(y / size.y);
			
			var column = []
			for (var x = start.x; x < end.x; x += size.x) {
			var i = Math.floor(x / size.x);
				var chunk = new Chunk(i,j,k);
				this.chunks.push(chunk);
				column.push(chunk)
			}
			row.push(column);
		}
		this.layers.push(row);
	}
	
	console.log(this.layers);
	
	
	
	/*for (var z = 0; z < 3/0.2; z++) {
		this.chunks.push(new Chunk(0,0,z));
	}*/
	
	//this.chunks.push(new Chunk(0,0,0));
	
	for (var i = 0; i < this.chunks.length; i++) {
		createStars(100,this.chunks[i]);
	}
};

ChunkHandler.prototype.draw = function() {
	for (var i = 0; i < this.chunks.length; i++) {
		this.chunks[i].draw();
	}
};

ChunkHandler.prototype.update = function(dt) {
	for (var i = 0; i < this.chunks.length; i++) {
		this.chunks[i].update(dt);
	}
};



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





