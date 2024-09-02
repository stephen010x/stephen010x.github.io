/////////////////////////
// Camera Update
/////////////////////////


/////////////////////////
// Camera Object
/////////////////////////
var cam = {
    x: 0,
    y: 0,
    z: 0,
    rot: 0,
    width: game.width,
    height: game.height,
    depth: game.height,
    //fps: 10,
    //dt: 0,
    //time: 0,
    dw: 0,
};
cam._width = cam.width;
cam._height = cam.height;
cam.ratio = cam.height/cam.width;

world.logic[0].push(cam)


cam.update = function(dt) {
    var player = world.player;
    var speed = Math.sqrt(Math.pow(player.vx,2) + Math.pow(player.vy,2));
    var dx = player.x - cam.x;
    var dy = player.y - cam.y;
    var mx = dx * 0.1;
    var my = dy * 0.1;
    
    cam.x = cam.x + mx + player.vx/15;
    cam.y = cam.y + my + player.vy/15;
    
    /*var dw = ((0 + speed/3) + cam.z*500)*dt*1;
    
	cam.z -= dw/100;*/

    cam.dw += speed/1e4;
    cam.dw *= 0.9;

	cam.z = player.z - cam.depth - cam.dw;
};

// Convert screen position to literal. Returns object
cam.screen_to_position = function(object, z) {
	var x = ((object.x / game.width  - 0.5) * cam.width  - 0.5) * (z - cam.z)/cam.depth + cam.x;
	var y = ((object.y / game.height - 0.5) * cam.height - 0.5) * (z - cam.z)/cam.depth + cam.y;
    return {x:x, y:y, z:z};
};

// Convert a list of literal positions to screen positions
// Change this to accept existing objects so that I don't have to create a new
// array or object for this. 
cam.position_to_screen = function(array) {
    var _temp = [];
    for (var i = 0; i < array.length; i++) {
        var object = {x:array[i][0], y:array[i][1], z:either(array[i][2],1)};
        // as a sort of temporary solution, returns Infinity if past the camera
        if (object.z - cam.z <= 0) {
            _temp.push({x:Infinity, y:Infinity});
        } else {
            var x = (((object.x - cam.x) / (object.z - cam.z) * cam.depth + 0.5) / cam.width  + 0.5) * game.width; 
            var y = (((object.y - cam.y) / (object.z - cam.z) * cam.depth + 0.5) / cam.height + 0.5) * game.height;
            _temp.push({x:x, y:y});
        }
    }
    return _temp;
};
