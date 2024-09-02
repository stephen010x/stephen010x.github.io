/////////////////////////
// Planet Class
/////////////////////////

function Planet(x,y,z,size){
	// Locatable class broken. Fix it.
    Movable.call(this,x,y,z,1);
    Object.assign(Planet.prototype, Movable.prototype);
    
    Polygon.call(this, color(255,255,255));
    Object.assign(Planet.prototype, Polygon.prototype);
    
    this.radius = size;
    this.poly = [];
    var poly_count = 100;
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

//Delete this when locatable class fixed
Planet.prototype.update = function() {}

//Planet.prototype.update = function(dt) {};
