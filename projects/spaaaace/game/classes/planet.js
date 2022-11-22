/////////////////////////
// Planet Class
/////////////////////////

function Planet(x,y,size,distance){
    Locatable.call(this,x,y,1,distance);
    Object.assign(Planet.prototype, Locatable.prototype);
    
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

//Planet.prototype.update = function(dt) {};