// RENAME MODULE TO "SHAPES", maybe




/////////////////////////
// Drawable Class
/////////////////////////
function Drawable(color,layer) { //(x,y,z,color)
    //this.x = x;
    //this.y = y;
    //this.z = z;
	this.color = color;
	layer = either(layer,0)
	world.layer[layer].push(this);
}




/////////////////////////
// Polygon Class
/////////////////////////
function Polygon(color,layer) {
	color = either(color, game.color(255,255,255)) //Uhhh....
	Drawable.call(this, color, layer);
	
    this.poly = [];
}

Polygon.prototype.updatedpoly = function(z) {
    var poly = this.poly;
    var _list = [];
    for (var i = 0; i < poly.length; i++) {
        var nx = poly[i][0] * this.scale;
        var ny = poly[i][1] * this.scale;
        var x = nx * Math.cos(this.angle) - ny * Math.sin(this.angle);
        var y = ny * Math.cos(this.angle) + nx * Math.sin(this.angle);
        x += this.x;
        y += this.y;
        //_list.push({x:x,y:y,z:z});
		_list.push([x,y])
    }
    return _list;
};
//##### CONSIDER changing polygons to object points rather than array points


//# Consider having _x be screen x, and normal x to be literal x.
Polygon.prototype.draw = function() {
    //var parent = cam.position_to_screen(this);
    // Consider having camera take x, y, and z rather than distance.
    //var n_poly = s_poly.massAssign(['x','y']);
	var poly = this.updatedpoly(either(this.z, 1)); 
    var s_poly = cam.position_to_screen(poly);
    
    this.design();
    beginShape();
    
    for (var i = 0; i < s_poly.length; i++) {
        var p = s_poly[i];
        vertex(p.x, p.y);
    }
    endShape();
};