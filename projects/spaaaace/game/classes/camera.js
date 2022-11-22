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
    //fps: 10,
    //dt: 0,
    //time: 0,
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
    
    cam.x = cam.x + mx + player.vx/25;
    cam.y = cam.y + my + player.vy/25;
    
    var dw = ((0 + speed/3) + cam.z*500)*dt*1;
    
    //cam.width += dw;
    //cam.height = cam.width * cam.ratio;
	
	cam.z -= dw/500;
};

// Convert screen position to literal. Returns object
cam.screen_to_position = function(object) {
    var x = (object.x + cam.x - cam.width/2) * cam.width  / width;
    var y = (object.y + cam.y - cam.height/2) * cam.height / height;
    return {x:x, y:y};
};

// Convert a list of literal positions to screen positions
cam.position_to_screen = function(array) {
    var _temp = [];
    for (var i = 0; i < array.length; i++) {
        var object = {x:array[i][0], y:array[i][1], z:either(array[i][2],1)};
        var x = (object.x - cam.x + cam.width/2 ) / (object.z - cam.z) * width  / cam.width; 
        var y = (object.y - cam.y + cam.height/2) / (object.z - cam.z) * height / cam.height;
        _temp.push({x:x, y:y});
    }
    return _temp;
};