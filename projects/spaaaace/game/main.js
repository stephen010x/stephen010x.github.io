

var temp_static_dt;


//##########################
// SETUP
//##########################

function setup() {
    world.build();
    renderSetup();
}




//##########################
// MAIN LOOP
//##########################
var frametime = 0;
function main() {
    game.time = millis()/1000;
    game.dt = game.time - frametime;
    frametime = game.time;
    
    gamelogic(temp_static_dt); //game.dt //0.03
    render();
    
    keyboardFrameEnd();
}




//##########################
// GAME LOGIC
//##########################

function gamelogic(dt) {
    for (var i = 0; i < world.logic.length; i++) {
        var layer = world.logic[i];
        for (var j = 0; j < layer.length; j++) {
            layer[j].update(dt);
        }
    }
    //cam.update(dt);
}

/*Simulatable.prototype.update = function(dt) {
    return null;
};*/
Simulatable.prototype.friction = function(dt, friction) {
    this.vx -= this.vx * friction * dt;
    this.vy -= this.vy * friction * dt;
};

Planet.prototype.update = function(dt) {};

Polygon.prototype.updatedpoly = function() {
    var poly = this.poly;
    var _list = [];
    for (var i = 0; i < poly.length; i++) {
        var nx = poly[i][0] * this.scale;
        var ny = poly[i][1] * this.scale;
        var x = nx * Math.cos(this.angle) - ny * Math.sin(this.angle);
        var y = ny * Math.cos(this.angle) + nx * Math.sin(this.angle);
        x += this.x;
        y += this.y;
        _list.push([x,y]);
    }
    return _list;
};
//##### CONSIDER changing polygons to object points rather than array points

Player.prototype.update = function(dt) {
    var index = this.streams.length - 1;
    var stream = this.streams[index];
    
    this.x += this.vx * dt;
    this.y += this.vy * dt;
    
    if (game.control.thrust_start) {
        this.streams.push(new Stream());
        index += 1;
        stream = this.streams[index];
    }
    
    if (game.control.thrust) {
        this.vx += dt * this.speed * Math.cos(this.angle);
        this.vy += dt * this.speed * Math.sin(this.angle);
        stream.burn(-500, this.angle, this, dt);
    }
    if (game.control.turn_right) {
        this.angle += dt * this.rotspeed;
    }
    if (game.control.turn_left) {
        this.angle -= dt * this.rotspeed;
    }
    
    this.friction(dt, 0.5);
};


Stream.prototype.burn = function(velocity,angle,carry,dt) {
    var c = Math.cos(angle);
    var s = Math.sin(angle);
    var vx = velocity * c + carry.vx;
    var vy = velocity * s + carry.vy;
    var x = carry.x - vx*dt - 10*c;
    var y = carry.y - vy*dt - 10*s;
    this.deltas.push([vx,vy]);
    this.poly.push([x, y]);
};
Stream.prototype.update = function(dt) {
    var streams = world.player.streams;
    
    if (this !== streams[streams.length - 1] || !game.control.thrust) {
        this.age += dt;
        this.color = color(99, 171, 199, 200 - this.age*40);
    }
    
    for (var i = 0; i < this.poly.length; i++) {
        var point = this.poly[i];
        var delta = this.deltas[i];
        point[0] += delta[0]*dt;
        point[1] += delta[1]*dt;
        this.pointfriction(dt, 3, delta);
    }
    if (this.age > 5) {
        world.items.remove(this);
        world.player.streams.remove(this);
    }
};
Stream.prototype.pointfriction = function(dt,friction,target) {
    target[0] -= target[0] * friction * dt;
    target[1] -= target[1] * friction * dt;
};




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
    
    for (var i = 0; i < world.layer.length; i++) {
        var layer = world.layer[i];
        for (var j = 0; j < layer.length; j++) {
            layer[j].draw();
        }
    }
}

Planet.prototype.design = function() {
    fill(this.color);
};

//# Consider having _x be screen x, and normal x to be literal x.
Polygon.prototype.draw = function() {
    //var parent = cam.position_to_screen(this);
    var poly = this.updatedpoly();
    var s_poly = cam.position_to_screen(poly,this.distance);
    // Consider having camera take x, y, and z rather than distance.
    //var n_poly = s_poly.massAssign(['x','y']);
    
    this.design();
    beginShape();
    
    for (var i = 0; i < s_poly.length; i++) {
        var p = s_poly[i];
        vertex(p.x, p.y);
    }
    endShape();
};

Player.prototype.design = function() {
    stroke(0,0,0);
    strokeWeight(1);
    fill(this.color);
};

Stream.prototype.design = function() {
    var scale = (width / cam.width + height/cam.height*0) / 2*2;
    stroke(this.color);
    strokeWeight(5*scale);
    noFill();
};





//##########################
// Program Initilization
//##########################


// Redirect loop entry point
function draw() { main(); }
