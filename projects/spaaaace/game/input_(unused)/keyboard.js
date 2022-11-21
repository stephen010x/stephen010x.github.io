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