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
    width: width,
    height: height,
    //fps: 10,
    //dt: 0,
    //time: 0,
};
cam._width = cam.width;
cam._height = cam.height;
cam.ratio = cam.height/cam.width;



cam.update = function(dt) {
    var player = world.player;
    var speed = Math.sqrt(Math.pow(player.vx,2) + Math.pow(player.vy,2));
    var dx = player.x - cam.x;
    var dy = player.y - cam.y;
    var mx = dx * 0.1;
    var my = dy * 0.1;
    
    cam.x = cam.x + mx + player.vx/25;
    cam.y = cam.y + my + player.vy/25;
    
    var dw = ((cam.init_width + speed/3) - cam.width)*dt*1;
    
    cam.width += dw;
    cam.height = cam.width * cam.ratio;
};


// Convert literal position to screen. Returns object
cam.screen_to_position = function(object) {
    var x = (object.x + cam.x - cam.width/2) * cam.width  / width;
    var y = (object.y + cam.y - cam.height/2) * cam.height / height;
    return {x:x, y:y};
};

cam.position_to_screen = function(array, distance) {
    var _temp = [];
    var distance = either(distance, 1);
    for (var i = 0; i < array.length; i++) {
        var object = {x:array[i][0], y:array[i][1]};
        var x = (object.x - cam.x + cam.width/2 ) * width  / cam.width * (distance); 
        var y = (object.y - cam.y + cam.height/2) * height / cam.height * (distance);
        _temp.push({x:x, y:y});
    }
    return _temp;
};