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
		_list.push([x,y,z])
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



/////////////////////////
// Trail3D Class
/////////////////////////
function Trail3D(color,weight,layer) {
	color = either(color, game.color(255,255,255)) //Uhhh....
	Drawable.call(this, color, layer);

	this.weight = either(weight, 10);
    this.trail = [];
}

Trail3D.prototype.adjust = function() {
    let poly = new Array(this.trail.length * 2);
    let len = this.trail.length;
    let weight = this.weight/2;
    //console.log(this.trail);
    for (let i = 0; i < len; i++) {
        //console.log(i);
        if (i == 0 || i == len-1) {
            let pp = this.trail[i];
            poly[i] = [pp[0], pp[1], pp[2]];
            poly[len*2-1-i] = poly[i];
        } else {
            let pp = this.trail[i];
            //let pn = either(either(this.trail[i+1], this.trail[i-1]), this.trail[i]);
            let pn = this.trail[i+1];
            pp = {x: pp[0], y: pp[1], z: pp[2]};
            pn = {x: pn[0], y: pn[1], z: pn[2]};
            let dx = pn.x - pp.x;
            let dy = pn.y - pp.y;
            let r = Math.sqrt(dx*dx + dy*dy);
            dx /= r;
            dy /= r;
            //poly[i] = {x: pp.x - dy*this.weight, y: pp.y + dx*this.weight};
            //poly[(len-1)*2-i] = {x: pp.x + dy*this.weight, y: pp.y - dx*this.weight};
            poly[i] = [pp.x - dy*weight, pp.y + dx*weight, pp.z];
            //console.log(len, i, (len-1)*2-i, this.trail.length * 2);
            poly[len*2-1-i] = [pp.x + dy*weight, pp.y - dx*weight, pp.z];
        }

    }
    return poly;
}

//# Consider having _x be screen x, and normal x to be literal x.
Trail3D.prototype.draw = function() {
    //var parent = cam.position_to_screen(this);
    // Consider having camera take x, y, and z rather than distance.
    //var n_poly = s_poly.massAssign(['x','y']);
    var poly = this.adjust();
    //console.log(poly);
    var s_poly = cam.position_to_screen(poly);

    //print(poly, s_poly);
    
    this.design();
    
    beginShape();
    for (var i = 0; i < s_poly.length; i++) {
        var p = s_poly[i];
        vertex(p.x, p.y);
    }
    endShape();
};
