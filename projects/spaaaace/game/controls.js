

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



//##########################
// Key Interrupts
//##########################
keyPressed = function() {
    var key = keycode;
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
        if ([key.down, key.s].contains(keyCode)) {
            game.control.brake = true;
        }

    }
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
    if ([key.down, key.s].contains(keyCode)) {
            game.control.brake = false;
    }
};

keyboardFrameEnd = function() {
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