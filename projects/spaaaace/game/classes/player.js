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