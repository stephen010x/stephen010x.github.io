

//##########################
// SETUP
//##########################

function setup() {
	console.log("setup started");
	game.color = color;
    Object.assign(world, new world.build());
    renderSetup();
	console.log("setup finished");
}



//##########################
// MAIN LOOP
//##########################
// frametime or game.frametime?
var frametime = 0;
var static_dt = 0.03;
function main() {
    game.time = millis()/1000;
    game.dt = game.time - frametime;
    
    gamelogic(static_dt * 60 / game.fps); //game.dt //0.03
    render();
    
    keyboardFrameEnd();
	game.frame += 1
}
// I should probably be fine with using more global variables defined next to theh 
// functions that plan to use them.




//##########################
// GAME LOGIC
//##########################

function gamelogic(dt) {
	// update every object in every world.logic layer
    for (var i = 0; i < world.logic.length; i++) {
        var layer = world.logic[i];
        for (var j = 0; j < layer.length; j++) {
            layer[j].update(dt);
        }
    }
    //cam.update(dt);
}




//##########################
// GAME RENDERING
//##########################

function renderSetup() {
	createCanvas(window.innerWidth, window.innerHeight);
    frameRate(game.fps);
}

function render() {
	//noSmooth()
    background(0, 0, 0);
    fill(255,255,255);
    noStroke();
    
	// Draw every object in every world.layer
    for (var i = 0; i < world.layer.length; i++) {
        var layer = world.layer[i];
        for (var j = 0; j < layer.length; j++) {
            layer[j].draw();
        }
    }
    
    fill(255,255,255);
    stroke(0,0,0);
    strokeWeight(1);
    let p = {x: world.player.x, y: world.player.y, z: world.player.z};
    //let m = {x: world.planets[0].x.toFixed(2), y: world.planets[0].y.toFixed(2), z: world.planets[0].z.toFixed(2)};
    text(`Player:\n\t\tx: ${p.x | 0}\n\t\ty: ${p.y | 0}\n\t\tz: ${p.z | 0}`, 40, 40); 
    //text(`Planet 1: ${m.x}, ${m.y}, ${m.z}, ${world.planets[0].radius.toFixed(2)}`, 40, 60); 
}




//##########################
// Program Initilization
//##########################

// Redirect loop entry point
function draw() { main(); }

console.log("game script initilization finished")
