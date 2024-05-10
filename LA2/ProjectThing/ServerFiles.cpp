#include "ProjectThing.h"

const char HTML_INDEX[] = R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Robot Car</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <h1>Robot Car</h1>
    <div class="connection-status">
        <span>•</span>
        <p class="connection-text">No connection</p>
    </div>

    <p id="distance">Distance to obstacle in front: </p>
    
    <div class="controls">
        <div class="button-group">
            <button id="left" title="Left">˂</button>
            <button id="right" title="Right">˃</button>
        </div>
        
        <div class="button-group">
            <button id="backward" title="Backward"></button>
            <button id="forward" title="Forward"></button>
        </div>
    </div>

    <script src="control.js"></script>
</body>
</html>
)";

const char CSS_STYLE[] = R"(
*, *::before, *::after {
    box-sizing: border-box;
}

body {
    position: relative;
    display: flex;
    flex-direction: column;
    justify-content: space-between;
    align-items: center;
    height: 100dvh;
    margin: 0;
    padding: 2rem;
    font-family: sans-serif;
    background-color: hsl(0, 0%, 10%);
    color: hsl(0, 0%, 90%);
}

.connection-status {
    position: absolute;
    display: flex;
    flex-direction: row;
    align-items: center;
    gap: 0.5rem;
    top: 0;
    right: 0;
    padding: 1rem 2rem;
}

.connection-status span {
    color: red;
    font-size: 2rem;
    animation: pulse 1.2s infinite;
}

@keyframes pulse {
    0% {
        transform: scale(1);
    }
    50% {
        transform: scale(1.3);
    }
    100% {
        transform: scale(1);
    }
}

.controls {
    display: flex;
    justify-content: space-between;
    width: 100%;
}

button {
    align-self: flex-end;
    padding: 2rem;
    cursor: pointer;
    border: none;
}

.button-group {
    display: flex;
    gap: 1.5rem;
}

#forward, #backward {
    background: repeating-linear-gradient(
        transparent,
        transparent 10px,
        hsla(0, 0%, 100%, 0.6) 5px,
        hsla(0, 0%, 100%, 0.5) 20px
    ),
    linear-gradient(to bottom, #a9a9a9 0%, #696969 100%);
    box-shadow: 0 5px 15px rgba(0, 0, 0, 0.2);
    border-radius: 0.1rem 0.1rem 0.5rem 0.5rem;
    padding: 2.5rem;
}

#forward {
    padding-block: 4rem;
}

#forward:hover, #backward:hover{
    background: repeating-linear-gradient(
        transparent,
        transparent 10px,
        rgba(255, 255, 255, 0.3) 5px,
        rgba(255, 255, 255, 0.5) 20px
    ),
    linear-gradient(to bottom, #696969 0%, #a9a9a9 100%);
}

#left, #right {
    border: 0.15rem solid hsl(0, 0%, 40%);
    border-radius: 30%;
    font-size: 1.5rem;
    color: hsl(0, 0%, 90%);
    background: transparent;
}

#left:hover, #right:hover {
    background: hsl(0, 0%, 20%);
}
)";

const char JS_CONTROL[] = R"(
var socket = new WebSocket("ws://" + window.location.hostname + "/ws");
var distance = Number.MAX_SAFE_INTEGER;

socket.onopen = function(e) {
    document.querySelector(".connection-text").textContent = "Connected";
    document.querySelector(".connection-status span").style.color = "lightgreen";
};

socket.onclose = function(e) {
    document.querySelector(".connection-text").textContent = "No connection";
    document.querySelector(".connection-status span").style.color = "red";
};

socket.onmessage = function(e) {
    distance = Number(e.data.trim());

    if (distance < 20) {
        document.getElementById("distance").textContent = "Obstacle in front too close, cannot move";
    } else {
        document.getElementById("distance").textContent = "Distance to obstacle in front: " + distance + " cm";
    }
};


function sendCommand(command) { 
    if (command === "forward" && distance < 20) {
        command = "stop";
    }

    if (socket.readyState === WebSocket.OPEN) {
        socket.send(command);
    }
}


var commands = ["forward", "backward", "left", "right"];
commands.forEach(function(command) {
    var button = document.getElementById(command);

    button.addEventListener("mousedown", function(e) {
        e.preventDefault();
        sendCommand(command);
    });

    button.addEventListener("touchstart", function(e) {
        e.preventDefault();
        sendCommand(command);
    });

    button.addEventListener("mouseup", function(e) {
        e.preventDefault();
        sendCommand("stop");
    });

    button.addEventListener("touchend", function(e) {
        e.preventDefault();
        sendCommand("stop");
    });
});


document.addEventListener('keydown', function(e) {
    e.preventDefault();
    switch(e.which) {
        case 37: // left
            sendCommand("left");
            break;

        case 38: // up
            if (distance < 10) {
                sendCommand("stop");
                break;
            }
            sendCommand("forward");
            break;

        case 39: // right
            sendCommand("right");
            break;

        case 40: // down
            sendCommand("backward");
            break;

        default: return;
    }
});


document.addEventListener('keyup', function(e) {
    e.preventDefault();
    sendCommand("stop");
});
)";