server.log("Device Started");

function arduinoData() {
    // Read the UART for data sent by Arduino to indicate the state of its LE 
}

function changeX(x) {
    server.log("X: " + x);
    arduino.write("x"+x);
}
function changeY(y) {
    server.log("Y: " + y);
    arduino.write("y"+y);
}
function changeR(r) {
    server.log("R: " + r);
    arduino.write("r"+r);
}
agent.on("changeX", changeX);
agent.on("changeY", changeY);
agent.on("changeR", changeR);

// Alias UART to which Arduino is connected and configure UART
arduino <- hardware.uart57;
arduino.configure(19200, 8, PARITY_NONE, 1, NO_CTSRTS, arduinoData);


