

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
	this.sky = new Sky();
	this.stars = this.sky.createStars(100)
    
    var distance = 1;
    this.planet = new Planet(200, 200, 100/distance, distance);
    
    this.player = new Player(10,10);
    
    cam.x = this.player.x;
    cam.y = this.player.y;
    cam.width = cam._width;
    cam.height = cam._height;

    temp_static_dt = 60 * 0.03 / game.fps;   
}