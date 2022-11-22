

//##########################
// Global Method Definitions
//##########################

/////////////////////////
// Returns a random integer in a range. Don't remember if inclusive or not.
function randInt(begin, end) {
    return Math.floor(Math.random()*(end - begin)) + begin;
}

/////////////////////////
// Returns a random float in a range.
function randFloat(begin, end) {
    return Math.random()*(end - begin) + begin;
}

/////////////////////////
// Returns the distance between two points. #CONSIDER MAKING IT A LIST OF POINTS
function distance(p1, p2) {
    return Math.sqrt( Math.pow( p1.x - p2.x , 2) + Math.pow( p1.y - p2.y , 2) );
}

/////////////////////////
// For default variables. Returns value if variable is undefined.
function either(variable, value) {
    return typeof variable !== "undefined" ? variable : value;
}




//##########################
// Object Prototypes
//##########################

/////////////////////////
// Search and remove item from list
Array.prototype.remove = function(item){
    return this.splice(this.indexOf(item), 1)[0];
};

/////////////////////////
// Search and remove every item from list of another lists
Array.prototype.strip = function(list){
    for (var i = 0; i < list.length; i++) {
        this.splice(this.indexOf(list[i]), 1);
    }
};

/////////////////////////
// Append to list only if it doesn't already contain it.
Array.prototype.add = function(item){
    if (!this.includes(item)) {
        this.push(item);
    }
};

/////////////////////////
// Convert every item in array to a method with specified format
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

/////////////////////////
// Multiplies and returns every item in list by constant
Array.prototype.multiply = function(scale) {
    var newlist = [];
    for (var i = 0; i < this.length; i++) {
        var scaled_item = this[i] * scale;
        newlist.push(scaled_item);
    }
    return newlist;
};

Array.prototype.fillcopy = function(object) {
	_list = Array(this.length)
	for (var i = 0; i < this.length; i++) {
        //this[i] = Object.assign({}, object);
		_list[i] = Object.assign({}, object)
    }
	console.log(_list);
	return _list;
}




// ##### OBSOLETE ##### //replaced by includes() Method
/////////////////////////
/*/ Check if item exists within list
Array.prototype.contains = function(item) {
    for (var i = 0; i < this.length; i++) {
        if (this[i] === item) {
            return true;
        }
    }
};*/


//##### OBSOLETE #####
/////////////////////////
/*/ Can't even tell what is different between this and the assign method
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
};*/