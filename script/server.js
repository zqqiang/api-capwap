'use strict';

var dgram = require('dgram');
var util = require('util');
var server = dgram.createSocket('udp4');

server.on('listening', function() {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', function(message, remote) {
    var obj = JSON.parse(message);
    console.log(util.inspect(obj, { showHidden: true, depth: null }));
    console.log('--------------------------------------------------------------------------');
});

server.on("error", function(err) {
    console.trace("Server Error:\n" + err.stack);
    server.close();
});

server.on("close", function(err) {
    console.log("Server close");
});

server.bind(10090, '172.16.95.50');
