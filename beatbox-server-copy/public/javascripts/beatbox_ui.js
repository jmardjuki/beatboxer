"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {
    window.setInterval(function() {sendRequest('uptime')}, 1000);
    window.setInterval(function() {askRequest('get 1')}, 1000);
    window.setInterval(function() {askRequest('get 2')}, 1000);
    window.setInterval(function() {askRequest('get 3')}, 1000);
    socket.on('fileContents', function(result) {
        var fileName = result.fileName;
        var contents = result.contents

        var domObj;
        switch(fileName) {
        case 'uptime':
            var time = contents.split(" ");
            time = time[0];
            var hours = Math.floor(time/3600);
            var min = Math.floor((time - hours*3600)/60);
            var sec = Math.floor(time - min*60 - hours *3600);
            contents = hours + ":" + min + ":" + sec + "(H:M:S)";
            domObj = $('#status');
            break;
        default:
            console.log("Unknown DOM object: " + fileName);
            return;
        }
        contents = replaceAll(contents, "\n", "<br/>")
        domObj.html(contents);

    });
	$('#modeNone').click(function(){
		sendBeatCommand("beat 1");
        askRequest('get 1');
	});
	$('#modeRock').click(function(){
		sendBeatCommand("beat 2");
        askRequest('get 1');
	});
	$('#modeJanet').click(function(){
		sendBeatCommand("beat 3");
        askRequest('get 1');
	});
	$('#volumeDown').click(function(){
		sendBeatCommand("volume 2");
        askRequest('get 3');
	});
    $('#volumeUp').click(function(){
		sendBeatCommand("volume 1");
        askRequest('get 3');
	});
	$('#tempoDown').click(function(){
		sendBeatCommand("tempo 2");
        askRequest('get 2');
	});
    $('#tempoUp').click(function(){
		sendBeatCommand("tempo 1");
        askRequest('get 2');
	});
	$('#drumHihat').click(function(){
		sendBeatCommand("drum 1");
	});
    $('#drumSnare').click(function(){
		sendBeatCommand("drum 2");
	});
	$('#drumBase').click(function(){
		sendBeatCommand("drum 3");
	});

    
	socket.on('commandReply', function(result) {
		if (result.indexOf("beat") > -1){
            var res = result.split(" ");
            $('#modeid').text(res[1]);
        } else if (result.indexOf("tempo") > -1) {
            var res = result.split(" ");
            $('#tempoid').val(res[1]);
        } else if (result.indexOf("volume") > -1) {
            var res = result.split(" ");
            $('#volumeid').val(res[1]);
        }
	});
    socket.on('daError', function(result) {
        var msg = 'SERVER ERROR: ' + result;
        console.log("Error detected");
        $('#error-text').text(msg);
        $('#error-box').show();
        setTimeout(function(){
            $('#error-box').hide();
        }, 10000);       
    });
	
});

function sendBeatCommand(message) {
	socket.emit('beat', message);
};

function sendRequest(file) {
    socket.emit('proc', file);
};

function askRequest(whichOne){
    socket.emit('beat', whichOne);
};

function replaceAll(str, find, replace) {
    return str.replace(new RegExp(find, 'g'), replace);
};
