

//##########################
// GAME OBJECT
//##########################

var game = {
	width: window.innerWidth,
	height: window.innerHeight,
    dt: 0,
    time: 0,
	frame: 0,
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

var world = {
	items: [],
    layer: Array(4).fill([]),
    logic: Array(10).fill([]),
}


/////////////////////////
// Generate World
/////////////////////////
world.build = function() {
	// Consider stars building themselves. May be faster to draw too.
	this.sky = new Sky();
	this.stars = sky.createStars(100)
    
    var distance = 1;
    this.planet = new Planet(200, 200, 100/distance, distance);
    
    this.player = new Player(10,10);
    
    cam.x = this.player.x;
    cam.y = this.player.y;
    cam.width = cam.init_width;
    cam.height = cam.init_height;

    temp_static_dt = 60 * 0.03 / game.fps;   
}