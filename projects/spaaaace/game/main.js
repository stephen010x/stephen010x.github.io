

//##########################
// SETUP
//##########################

function setup() {
	console.log("setup started");
    world.build();
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
    frametime = game.time;
    
    gamelogic(static_dt * 90 / game.fps); //game.dt //0.03
    render();
    
    keyboardFrameEnd();
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
    frameRate(cam.fps);
}

function render() {
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
}




//##########################
// Program Initilization
//##########################

// Redirect loop entry point
function draw() { main(); }

console.log("game script initilization finished")