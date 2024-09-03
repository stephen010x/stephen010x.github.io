

//##########################
// GAME OBJECT
//##########################

var game = {
	width: window.innerWidth,
	height: window.innerHeight,
    dt: 0,
    time: 0,
	frame: 0,
	fps: 60,
    //frametime: 0,
	//static_dt: 0.03,
	
    control: {
        thrust: false,
        thrust_start: false,
        turn_right: false,
        turn_left: false,
        brake: false,
        zoom: false,
    },
}


//##########################
// WORLD OBJECT
//##########################

function arrayfill(count) {
	var _list = []
	for (var i = 0; i < count; i++) {
		_list.push([])
	}
	return _list;
}


//Should I make world a class (therefore duplicatable), or should it be just a global object?
// for now I will make it a global object
var world = {
	items: [],
    layer: arrayfill(4), //Array(4).fillcopy([])
	logic: arrayfill(10), //Array(10).fillcopy([])
}


/////////////////////////
// Generate World
/////////////////////////
world.build = function() {
	// Consider stars building themselves. May be faster to draw too.
	//this.sky = new Sky();
	//this.stars = this.sky.createStars(100)
	
	//ChunkHandler();
    
    //var distance = 1;
    //this.planet = new Planet(200, 200, 100/distance, distance);
	
	this.planets = []
	for (var i = 0; i < 10; i++) {
		// not as big
		//*
		var x = randInt(-50000,50000); //randInt(-500,500);
		var y = randInt(-50000,50000); //randInt(-500,500);
		var z = (1-Math.pow(randFloat(0,1),0.25))*10000; //console.log(z);
		var size = randInt(100,200);
		//this.planets.push(new Planet(x*z, y*z, z, size*Math.pow(z,0.5)))
		this.planets.push(new Planet(x, y, z*10, size*Math.sqrt(z,0.5)))//*/
		// big planets
		/*
		var x = randInt(-150000,150000); //randInt(-500,500);
		var y = randInt(-150000,150000); //randInt(-500,500);
		var z = (1-Math.pow(randFloat(0,1),0.25))*100000; //console.log(z);
		var size = randInt(100,200);
		//this.planets.push(new Planet(x*z, y*z, z, size*Math.pow(z,0.5)))
		this.planets.push(new Planet(x, y, z*10, size*Math.sqrt(z,0.5)))//*/
	}

	this.decorplanets = [];
	for (let i = 0; i < 100; i++) {
		var x = randInt(-50000,50000); //randInt(-500,500);
		var y = randInt(-50000,50000); //randInt(-500,500);
		var z = Math.sqrt(randInt(0,5000*2000)); //console.log(z);
		var size = randInt(10,20);
		this.decorplanets.push(new Planet(x, y, z*10, size*Math.sqrt(z,0.5)))
	}
    
    this.player = new Player(0,0);
    
    cam.x = this.player.x;
    cam.y = this.player.y;
    cam.width = cam._width;
    cam.height = cam._height;
	
	this.chunker = new ChunkHandler();

    temp_static_dt = 60 * 0.03 / game.fps;
}
