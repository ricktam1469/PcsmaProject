var http = require("http");
var express = require("express");
var app = express();
var bodyParser = require('body-parser');

var fs = require('fs');



console.log("started");
app.set('port', 3000);

app.use(function (req, res, next) {

	// Website you wish to allow to connect
	res.setHeader('Access-Control-Allow-Origin', '*');

	// Request methods you wish to allow
	res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, PATCH, DELETE');

	// Request headers you wish to allow
	res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');

	// Set to true if you need the website to include cookies in the requests sent
	// to the API (e.g. in case you use sessions)
	res.setHeader('Access-Control-Allow-Credentials', true);

	// Pass to next layer of middleware
	next();
});

app.use(bodyParser.json());
var server = http.createServer(app);
server.listen(3000, "0.0.0.0");

app.get('/', function (req, res) {
	console.log('hit rcvd!');
	res.send("Running!!!");
});

app.get('/file', function (req, res) {
	fs.readFile('./test.txt', function read(err, data) {
		if (Buffer.isBuffer(data)) {
			data = data.toString('utf8');
		}
		if (err) {
			console.log(err);
		} else {
			console.log(data);
			res.send(data);
		}

	});
});