var width = window.innerWidth;
var height = window.innerHeight;

/// jshint ignore : start
// ^^^^^^ ^^^^^^   ^^^^^
//     DO NOT DELETE!
// (Disables Error Buddy warnings) 




//##########################
// INITILIZATION
//##########################

/////////////////////////
// Global Initilizers
/////////////////////////

var cam = {
    x: 0,
    y: 0,
    rot: 0,
    width: width,
    height: height,
    fps: 60,
    dt: 0,
    time: 0,
    distance: 1,
};
cam.init_width = cam.width;
cam.init_height = cam.height;
cam.ratio = cam.height/cam.width;

var mouse = {
    x: 0,
    y: 0,
    click: false,
    screen: {
        x: 0,
        y: 0,
    },
    hover: [],
    select: [],
};

var keyboard = {
    press: [],
    tap: [],
    release: [],
};
var k = keyboard;

var keycode;

var world = {
    items: [],
	logic: [],
	layer: [],
};

var game = {
    dt: 0,
    time: 0,
    frametime: 0,
    control: {
        thrust: false,
        thrust_start: false,
        turn_right: false,
        turn_left: false,
    },
};


/////////////////////////
// Function Declarators
/////////////////////////
var setup;
var main;

var gamelogic;

var render;
var renderSetup;

var randInt;
var randFloat;
var distance;
var either;

var keyFrameEnd;


/////////////////////////
// Class Declarators
/////////////////////////
var Simulatable;
var Polygon;
var Planet;
var Player;
var Stream;
var Star;




//##########################
// SETUP
//##########################

function setup() {
    createCanvas(window.innerWidth, window.innerHeight);
  
    world.stars = [];
    for (var i = 1; i < 100; i++) {
        world.stars.push(new Star());
    }
    world.planet = new Planet(200,200,100,1);
    world.player = new Player(10,10);
    
    //world.items.push(world.planet);
    //world.items.push(world.player);
    
    cam.x = world.player.x;
    cam.y = world.player.y;
    cam.width = cam.init_width;
    cam.height = cam.init_height;
	
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
    
    gamelogic(0.03); //game.dt //0.03
    render();
    
    keyFrameEnd();
}




//##########################
// GAME LOGIC
//##########################

function gamelogic(dt) {
    for (var i = 0; i < world.logic.length; i++) {
        world.logic[i].update(dt);
    }
    cam.update(dt);
}




//##########################
// GAME RENDERING
//##########################

function renderSetup() {
    frameRate(cam.fps);
    //stroke(255, 255, 255);
    //strokeWeight(1);
    noStroke();
}

function render() {
    background(0, 0, 0);
    for (var i = 0; i < world.layer.length; i++) {
        var item = world.layer[i];
        //println(item.poly[0]);
        item.draw();
    }
}




//##########################
// CLASS DEFINITIONS
//##########################

/////////////////////////
// Simulatable Class
/////////////////////////
function Simulatable(x,y,scale,distance) {
    this.scale = either(scale,1);
    this.x = x;
    this.y = y;
    this.vx = 0;
    this.vy = 0;
    this.angle = 0;
    this.distance = either(distance,1);
    world.logic.push(this); //Why did I not think of this sooner???
}
/*Simulatable.prototype.update = function(dt) {
    return null;
};*/
Simulatable.prototype.friction = function(dt, friction) {
    this.vx -= this.vx * friction * dt;
    this.vy -= this.vy * friction * dt;
};


/////////////////////////
// Polygon Class
/////////////////////////
function Polygon(color) {
    this.color = color;
    this.poly = [];
	world.layer.push(this);
}
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


/////////////////////////
// Planet Class
/////////////////////////

function Planet(x,y,size,distance){
    Simulatable.call(this,x,y,1,distance);
    Object.assign(Planet.prototype, Simulatable.prototype);
    
    Polygon.call(this, color(255,255,255));
    Object.assign(Planet.prototype, Polygon.prototype);
    
    this.radius = size;
    this.poly = [];
    var poly_count = 30;
    for (var i = 0; i < poly_count; i++) {
        var angle = 2 * Math.PI / poly_count * i;
        x = Math.cos(angle) * size;
        y = Math.sin(angle) * size;
        this.poly.push([x,y]);
    }
}
Planet.prototype.design = function() {
    fill(this.color);
};
Planet.prototype.update = function(dt) {};


/////////////////////////
// Player Class
/////////////////////////
function Player(x,y){
    Simulatable.call(this,x,y,1);
    Object.assign(Player.prototype, Simulatable.prototype);
    
    Polygon.call(this, color(255,255,255));
    Object.assign(Player.prototype, Polygon.prototype);
    
    this.speed = 300;
    this.rotspeed = 5;
    
    this.poly = [[10,0],[-7,7],[-3,0],[-7,-7]];
    this.streams = [];
    this.stream_index = 0;
    //Perhaps turn stream into an array with a start pointer. Use modulus in loops.
}
Player.prototype.design = function() {
    stroke(0,0,0);
    strokeWeight(1);
    fill(this.color);
};
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


/////////////////////////
// Stream Class
/////////////////////////
function Stream() {
    Simulatable.call(this,0,0);
    Object.assign(Stream.prototype, Simulatable.prototype);
    
    Polygon.call(this, color(99, 171, 199, 200));
    Object.assign(Stream.prototype, Polygon.prototype);

    this.poly = [];
    this.deltas = [];
    this.age = 0;
}
Stream.prototype.design = function() {
    var scale = (width / cam.width + height/cam.height*0) / 2*2;
    stroke(this.color);
    strokeWeight(5*scale);
    noFill();
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
        world.logic.remove(this);
		world.layer.remove(this);
        world.player.streams.remove(this);
    }
};
Stream.prototype.pointfriction = function(dt,friction,target) {
    target[0] -= target[0] * friction * dt;
    target[1] -= target[1] * friction * dt;
};


/////////////////////////
// Star Class
/////////////////////////
function Star() { //x,y,distance
    this.x = randInt(0, width);
    this.y = randInt(0, width);
    this.p_x = this.x;
    this.p_y = this.y;
    this._x = this.x;
    this._y = this.y;
    this.distance = Math.pow(randFloat(-3, 3), 1/3);
    //distance of 1 is middleground. Positive is foreground. Negative is background.
    //Positions are middleground regardless of distance.
    this.light = randInt(0, 255);
    world.logic.push(this);
	world.layer.push(this);
}
Star.prototype.style = function() {
    var n = this.light;
    strokeWeight(1);
    stroke(n, n, n);
};
Star.prototype.draw = function() {
    this.style();
    var star = cam.position_to_screen([this.x, this.y])[0];
    var x2 = this.x % width;
    var y2 = this.y % height;
    x2 += width  * (x2 < 0);
    y2 += height * (y2 < 0);
    var x1 = x2 + (this.x - this.p_x);
    var y1 = y2 + (this.y - this.p_y);
    line(x2,y2,x1,y1);
    //Fix line wraparound at high speeds. 
};
Star.prototype.update = function(dt) {
    this.p_x = this.x;
    this.p_y = this.y;
    this.x = this._x - cam.x * this.distance;
    this.y = this._y - cam.y * this.distance;
};


/////////////////////////
// Camera Update
/////////////////////////
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



//##########################
// Global Method Definitions
//##########################

/////////////////////////
// Returns a random integer in a range. Don't remember if inclusive or not.
function randInt(begin, end) {
    return Math.floor(Math.random()*(end - begin)) + begin;
}

function randFloat(begin, end) {
    return Math.random()*(end - begin) + begin;
}

function distance(p1, p2) {
    return Math.sqrt( Math.pow( p1.x - p2.x , 2) + Math.pow( p1.y - p2.y , 2) );
}

// For default variables
function either(variable, value) {
    return typeof variable !== "undefined" ? variable : value;
}




//##########################
// Key Interrupts
//##########################

keyPressed = function() {
    var key = keycode;
    //Add code here that calls a function specific to each object. Actually, maybe that is a bad idea.
    if (!k.press.contains(keyCode)) {
        k.press.push(keyCode);
        k.tap.push(keyCode);
    
        if ([key.up, key.w, key.space].contains(keyCode)) {
            game.control.thrust_start = true;
        }
        
        if ([key.up, key.w, key.space].contains(keyCode)) {
            game.control.thrust = true;
        }
        if ([key.left, key.a].contains(keyCode)) {
            game.control.turn_left = true;
        }
        if ([key.right, key.d].contains(keyCode)) {
            game.control.turn_right = true;
        }
    
    }
    
    //k.press.push(keyCode);
    //k.tap.push(keyCode);
    //println(k.press);
};
keyReleased = function() {
    var key = keycode;
    
    k.press.remove(keyCode);
    k.release.push(keyCode);
    
    if ([key.up, key.w, key.space].contains(keyCode)) {
        game.control.thrust = false;
    }
    if ([key.left, key.a].contains(keyCode)) {
        game.control.turn_left = false;
    }
    if ([key.right, key.d].contains(keyCode)) {
        game.control.turn_right = false;
    }
};
keyFrameEnd = function() {
    k.tap = [];
    k.release = [];
    game.control.thrust_start = false;
};




//##########################
// Mouse Interrupts
//##########################

mousePressed = function() { 
    mouse.click = true;
};

mouseReleased = function() { 
    mouse.click = false;
};

mouseMoved = function() {
    mouse.screen.x = mouseX;
    mouse.screen.y = mouseY;
    mouse = Object.assign(mouse, cam.screen_to_position(mouse.screen.x));
};

mouseDragged = function() { mouseMoved(); };

mouseClicked = function() {  };
mouseOver = function() { };
mouseOut = function() {
    mouse.click = false;
};




//##########################
// Object Prototypes
//##########################

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
        var x = (object.x - cam.x + cam.width/2 ) * width  / cam.width * (distance); // + cam.distance
        var y = (object.y - cam.y + cam.height/2) * height / cam.height * (distance); // + cam.distance
        _temp.push({x:x, y:y});
    }
    return _temp;
};


Array.prototype.remove = function(item){
    return this.splice(this.indexOf(item), 1)[0];
};

Array.prototype.strip = function(list){
    for (var i = 0; i < list.length; i++) {
        this.splice(this.indexOf(list[i]), 1);
    }
};

Array.prototype.add = function(item){
    if (!this.includes(item)) {
        this.push(item);
    }
};

Array.prototype.assign = function(methods){
    if (this.length === methods.length) {
        var object = {};
        for (var i = 0; i < this.length; i++) {
            object[methods[i]] = this[i];
        }
        return object;
    }
    else { throw "Error: Array is not the same size as provided list!"; }
};

//##### OBSOLETE #####
Array.prototype.massAssign = function(methods){
    var array = [];
    for (var i = 0; i < this.length; i++) {
        var object = {};
        var point = this[i];
        for (var j = 0; j < point.length; j++) {
            object[methods[j]] = point[j];
        }
        array.push(object);
    }
    return array;
};

Array.prototype.scale = function(scale) {
    var newlist = [];
    for (var i = 0; i < this.length; i++) {
        var scaled_item = this[i] * scale;
        newlist.push(scaled_item);
    }
    return newlist;
};

Array.prototype.contains = function(item) {
    for (var i = 0; i < this.length; i++) {
        if (this[i] === item) {
            return true;
        }
    }
};




//##########################
// Program Initilization
//##########################

// Run setup
//setup();

// Redirect loop entry point
function draw() { main(); }


keycode = {
backspace:8, tab:9, enter:13, shift:16,
ctrl:17, alt:18, caps:20, esc:27,
left:37, up:38, right:39, down:40,
0:48, 1:49, 2:50, 3:51, 4:52, 
5:53, 6:54, 7:55, 8:56, 9:57,
a:65, b:66, c:67, d:68, e:69, f:70, 
g:71, h:72, i:73, j:74, k:75, l:76, 
m:77, n:78, o:79, p:80, q:81, r:82, 
s:83, t:84, u:85, v:86, w:87, x:88, 
y:89, z:90,
f1:112, f2:113, f3:114, f4:115, 
f5:116, f6:117, f7:118, f8:119, 
f9:120, f10:121, f11:122, f12:123, 
comma:188, period:190,
forward_slash:191, open_bracket:219,
back_slash:220, close_braket:221,
quote:222,
};




/*

Idea for the game.

So when flying away from massive distant objects, the ship actually flies both away and out from the planet, so it remains on screen, but gets farther and farther away.
I think all collisions are maintained. The player still travels on the same axis


Take some inspiration from Spaceflight Simulator, and how it designed the world as if it was a 3D object, and we can just land on a plane of it.

I should have three planes. The foreground plane, the middleground plane (player's plane), and the background plane.

Asteroids will still (probably) not scale like the planets. 


The thing is, that I can't implement a frictionless spaceflight system unless the player can see all obsticles ahead of them.
It very well may be more pleasing to have space normally extended in-front of you. But if I do that, I will have to cap player speed.

I can still make it frictionless, or relatively frictionless. But I can't have the player go whatever speed they want.

If I want to have the player go lightspeeds, then perhaps disable all small enough objects, and then zoom out a lot. 


I wonder if I should add a car mode for driving on planets....
Say.... What if I made the whole planet an open world for driving in a car. (So you can fly in space, and then drive on land, and then perhaps also swim in water)

I should not make it combat based, but instead have it be exploration and ecological based, where you chart the universe, figure out the orbit of planets, discover new species, etc. You start out with a blank map of the solar system, and then explore from there. (Though, I will want to create ways for the player to find planets.)

I kind of have to consider here the advantage of non-distant objects. The thing is, that it would be nice to provide a sort of background imagery and whatnot.


I should make turning slower when thruster is on. (It will make high speed turning more intuitive, especially if it is hard to see which direction your ship is pointing.

I should have forward particles that are closer to the camera, and speed past the camera faster than the middleground. It will really give a 3D aesthetic to it. 
If I can, I should make it transparent. If not, then perhaps use a circle dot transparency method.

If possible, make it so that jet streams vary in transparency and size. 

Bubble gradients would make for a really cool aesthetic theme. Expecially if we can see them slowly shrink and whatnot for something like the jetstream

Maybe I can make the jetstream a sort of weapon or tool.

It would be cool if the jetstream was a liquid paint that could paint other objects. And imagine enemies that hunt you that you could just push away or blind with your paint.


I suddenly want a place where you can collect orbs, all of them physics based. 


Due to the new stream mechanics, (which is really satisfying), it would be a shame to eliminate speed turning. I should make it so that the player can turn faster simply when the stream isn't on, and to be able to maintain that turn speed when the stream is reignighted. That way I can technically have both mechanics, and make it more skill based. Though the speed cap is now low enough that maybe I can just have the player turn slow when going fast and the stream is on.


It would be cool if the player is capable of navigating without a minimap, and without markers. That zoom out planet idea is a fun idea. 

I should probably add relative and objective layers.

I should add layers to the world, rather than to the objects. That sounds much more like an efficient layering. I will probably start with 3 objective layers, then 1 subjective layer behind them all.

I wonder if I can use the star particles to give the illusion of the player flying towards a planet to explain why it is getting bigger. It probably wouldn't be hard to do if I implement a zoom mechanic, as I can just add the stars to the subjective layer, or even their own special layer, and just have their modulous be a 3 dimensional modulous. That way they actually move around like in a 3D world.

Should a world always be in view of the player? Or perhaps should they only become subjective once you leave their atmosphere?

Add a paint ring around one of the planets. It would be cool if the paint ring was an actual paint ring that you had to swim through to bypass.


The planets aren't subject to objective friction, though the player is. So I should probably make it so that either the planets stay still, or so that the player inherits the velocity of the planet as objective space. 
Though honestly, I can't really see much use of simulated planets. So I will probably have the planets be motionless.

*/
