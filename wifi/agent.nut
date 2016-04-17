server.log("Give an x: " + http.agenturl() + "?x=speed");
server.log("Give a y: " + http.agenturl() + "?y=speed");
server.log("Give an r: " + http.agenturl() + "?r=speed");


function httpHandler(request, response) {
    try {
        // Check if the user sent led as a query parameter
        if ("x" in request.query) {
            local X = request.query.x.tointeger();
            device.send("changeX", X); 
            //response.send(200, "{\"Success\":\"OK\"}");
        }
        if("y" in request.query) {
            local Y = request.query.y.tointeger();
            device.send("changeY", Y); 
            //response.send(200, "{\"Success\":\"OK\"}");
        }
        if("r" in request.query) {
            local R = request.query.r.tointeger();
            device.send("changeR", R); 
            //response.send(200, "{\"Success\":\"OK\"}"); 
        }
        else{
            // Send a response back to the browser saying everything was OK.
            response.send(200, "{\"Success\":\"OK\"}");
        }
    } catch (ex) {
        response.send(500, "Internal Server Error: " + ex);
    }
}


/* REGISTER HTTP HANDLER -----------------------------------------------------*/
http.onrequest(httpHandler);