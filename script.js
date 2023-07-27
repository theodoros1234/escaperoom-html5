var splashScreenInterval;
var splashScreenIntervalCount = 0;
var lcdCursorBlinkInverted = false;
var displayContainer;
var prevLedColor = "";
var congratulationsEnabledPoll;

function keyCodeToKeypad(code) {
    if (48 <= code && code <= 57)
        return code - 48;
    else if (96 <= code && code <= 105)
        return code - 96;
    else switch (code) {
        case 37: // Left
        case 65:
            return 4;
        case 38: // Up
        case 87:
            return 2;
        case 39: // Right
        case 68:
            return 6;
        case 40: // Down
        case 83:
            return 8;
        case 8: // Back/Del
        case 46:
        case 27:
            return 10;
        case 13: // Enter
            return 11;
        default:
            return -1;
    }
}

function keyDownHandler(event) {
    code = keyCodeToKeypad(event.keyCode);
    //console.log("Down: " + code);
    if (code!=-1)
        Module.keyDown(code);
}

function changeLEDColor() {
    var newColor = Module.getLEDs();
    if (prevLedColor != newColor) {
        prevLedColor = newColor;
        document.querySelector(':root').style.setProperty('--led-color', 'rgba(' + newColor + ', 1)');
    }
}

/*
function keyUpHandler(event) {
    code = keyCodeToKeypad(event.keyCode);
    console.log("Up: " + code);
    if (code!=-1)
        Module.keyUp(code);
}*/

window.onload = function() {
    displayContainer = document.getElementById("display-container");
    window.onresize = function() {
        var size = Math.min(window.innerWidth/1872, window.innerHeight/624, 1);
        displayContainer.style.scale = size;
    };
    window.onresize();
};

function drawScreen(pixels) {
    var row=0, col=0;
    const canvas = document.getElementById("user-display").getContext("2d");
    canvas.fillStyle = "white";

    if (pixels === "") {
        canvas.clearRect(0, 0, 1520, 272);
        return;
    }

    for (const c of pixels) {
        if (c === '\n') {
            row++;
            col=0;
            continue;
        }
        if (c === 'X') {
            canvas.fillRect(16*(col + parseInt(col/5)), 16*(row + parseInt(row/8)), 16, 16);
            // canvas.fillRect(col, row, 1, 1);
        } else {
            canvas.clearRect(16*(col + parseInt(col/5)), 16*(row + parseInt(row/8)), 16, 16);
            // canvas.clearRect(col, row, 1, 1);
        }
        col++;
    }
}

Module["onRuntimeInitialized"] = function () {
    console.log("wasm loaded");
    setInterval(function() {Module.millisTick(100);}, 100);
    Module.loadDisplayFont();

    setInterval(changeLEDColor, 16);
    setInterval(function() {drawScreen(Module.getDisplayContentsBitmap());}, 33);
    setInterval(function() {
        Module.lcdSetBlinkInversion(lcdCursorBlinkInverted);
        lcdCursorBlinkInverted = !lcdCursorBlinkInverted;
    }, 500);

    Module.setup();
    splashScreenInterval = setInterval(function() {
        if (splashScreenIntervalCount < 17) {
            Module.splashScreen(splashScreenIntervalCount);
        }
        if (splashScreenIntervalCount == 47) {
            clearInterval(splashScreenInterval);
            setInterval(Module.loop, 10);
        }
        splashScreenIntervalCount++;
    }, 33);

    setInterval(Module.ISR2, 250);
    congratulationsEnabledPoll = setInterval(function() {
        if (Module.isCongratulationsEnabled()) {
            clearInterval(congratulationsEnabledPoll);
            setInterval(Module.congratulations, 15);
        }
    }, 100);

    document.getElementsByTagName("html")[0].onkeydown = keyDownHandler;
    // document.getElementsByTagName("html")[0].onkeyup = keyUpHandler;
}
