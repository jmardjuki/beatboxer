"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

var fs = require('fs');
var socketio = require('socket.io');
var io;

var dgram = require('dgram');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');
	
	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});
};

function handleCommand(socket) {
    var errorTimer = setTimeout(function() {
        socket.emit('daError', "Cannot connect to the server");
    }, 5000);
    
    socket.on('proc', function(fileName) {
        var absPath = "/proc/" + fileName;
        console.log('accessing ' + absPath);

        fs.exists(absPath, function(exists) {
            if (exists) {
                fs.readFile(absPath, function(err, fileData) { 
                    if (err) {
                        emitSocketData(socket, fileName,
                            "ERROR: Unable to read file " + absPath);
                    } else {
                        emitSocketData(socket, fileName,
                            fileData.toString('utf8'));
                    }
                });
            } else {
                emitSocketData(socket, fileName,
                    "ERROR: File " + absPath + " not found.");
            }
        });
        clearTimeout(errorTimer);
       
    });

	// Pased string of comamnd to relay
	socket.on('beat', function(data) {
		console.log('beat command: ' + data);
		
		// Info for connecting to the local process via UDP
		var PORT = 12345;
		var HOST = '192.168.2.2';
		var buffer = new Buffer(data);

		var client = dgram.createSocket('udp4');
		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
		    if (err) 
		    	throw err;
		    console.log('UDP message sent to ' + HOST +':'+ PORT);
		});
		
		client.on('listening', function () {
		    var address = client.address();
		    console.log('UDP Client: listening on ' + address.address + ":" + address.port);
		});
		// Handle an incoming message over the UDP from the local application.
		client.on('message', function (message, remote) {
		    console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);
		    
		    var reply = message.toString('utf8')
		    socket.emit('commandReply', reply);
		    
		    client.close();
            clearTimeout(errorTimer);          

		});
		client.on("UDP Client: close", function() {
		    console.log("closed");
		});
		client.on("UDP Client: error", function(err) {
		    console.log("error: ",err);
		});
	});  

};

function emitSocketData(socket, fileName, contents){
    var result = {
        fileName: fileName,
        contents: contents
    }
    socket.emit('fileContents', result);
}
