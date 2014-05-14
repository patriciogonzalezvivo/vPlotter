//  TODO:
//        - ADD OSC mode to website and API

//  Modules
//
var http = require("http");   // http server
var fs = require('fs');       // filesystem.
var path = require('path');   // used for traversing your OS.
var url = require('url');     // utility for URLs
var exec = require('child_process').exec;  // running cmd

var formidable = require('formidable');  // uploading files;
// var mongo = require('mongodb');  // MongoDB

// Settings
//
var WWW_ROOT = "./www/";
var HTTP_PORT = 8080;
var MONGO_PORT = 27017;

var vPlotterPIPE;
var status = {'printing' : false,
              'queue': [],
              'options' : {'motorsDistance' : 1384,
                           'pulleyRadius' : 6,
                           'stepsPerRotation' : 800,
                           'stepDelay' : 2,
                           'penDownAngle' : 140,
                           'penUpAngle' : 70,
                           'penDelay' : 200,
                           'OSCPort' : 101010,
                           'debugDisplay' : false
                          }
             };

//  MongoDB
//
// var Server = mongo.Server,
//     Db = mongo.Db,
//     assert = require('assert')
//     BSON = mongo.BSONPure;
// var server = new Server('localhost', MONGO_PORT, {auto_reconnect: true});
// db = new Db('vPlotter', server);
// db.open(function(err, db) {
//   db.collection('pictures', function(err, collection){
//   }
// });

//  vPlotter App
//
function printFirstInLine() {
    console.log('There are ' + status.queue.length + ' objects on the queue');
    console.log('Printer is ' + status.printing);
    if (status.printing === false && status.queue.length > 0) {
        var actualFile = status.queue[0],
        command = './vPlotter -d ' + status.options.motorsDistance +
            ' -pr ' + status.options.pulleyRadius +
            ' -spr ' + status.options.stepsPerRotation +
            ' -sd ' + status.options.stepDelay +
            ' -pd ' + status.options.penDownAngle +
            ' -pu ' + status.options.penUpAngle +
            ' -delay ' + status.options.penDelay;

        if (status.options.debugDisplay) {
            command += ' -x';
        }

        if (actualFile.file === 'OSC') {
            command += ' -o ' + status.options.OSCPort;
        } else {
            command += ' -s ' + actualFile.scale +
                 ' -r ' + actualFile.rotate +
                 ' -i ' + actualFile.file;
        }

    console.log(command);
    status.printing = true;
    vPlotterPIPE = exec(command, function (error, stdout, stderr) {
      // console.log('stdout: '+stdout);
      // console.log('stderr: '+stderr);
      // if (error !== null) {
        // console.log('exec error: ' + error);
      // }

      //  Erase the job from the queue and the file
      //
      status.printing = false;
      if (actualFile.file !== 'OSC') {
        fs.unlink('www/data/' + actualFile.file, function (err) {
          if (err) { console.log(err); }
          console.log(actualFile.file + ' successfully deleted');
        });
      }
      status.queue.shift();

      if (status.queue.length > 0) {
        printFirstInLine();
      }
    });
  }
}

// WEB SERVER
//
var server = http.createServer( function( req , res ) {
  var parsedReq = url.parse(req.url);

  if (req.url === "/status.json") {
    //  ECHO STATUS
    //
    res.writeHead(200, { 'Content-Type':'application/json'}); //, "Access-Control-Allow-Origin":"*"
    res.write(JSON.stringify(status));
    res.end();
  }
  else if(parsedReq.pathname === "/set") {
    //  SET OPTIONS
    //
    var url_parts = url.parse(req.url, true);
    var query = url_parts.query;
    status.options = query;
    if(query.debugDisplay==null){
      status.options.debugDisplay = false;
    } else {
      status.options.debugDisplay = true;
    }
    console.log("Options now are: ");
    console.log(status.options);
  }
  else if(parsedReq.pathname == "/addOsc" && req.method.toLowerCase() == 'post'){
    var queue_obj = {
      'file': 'OSC',
      'rotate': 0,
      'scale': 100
    }
    status.queue.push(queue_obj);
    printFirstInLine();
  }
  else if(parsedReq.pathname == "/addFile" && req.method.toLowerCase() == 'post'){
      console.log("File recived");
    //  UPLOAD FILE & ADD TO QUEUE
    //
    // parse a file upload
    var form = new formidable.IncomingForm();
    var files = [];
    var fields = [];

    form.uploadDir = "www/data";
    form.keepExtensions = true;

    form
        .on('error', function(err) {
            console.log("File upload err");
            console.log(err);
        })
        .on('field', function(field, value) {
            // console.log(field, value);
            fields.push([field, value]);
        })
        .on('fileBegin', function(name, file){
            var filename = "img_" + Date.now()+".svg";
            file.name = filename;
            file.path = form.uploadDir + "/" + filename;//file.name;
        })
        .on('file', function(field, file) {
          console.log([field,file]);
          var queue_obj = {
            'file': file.name,
            'rotate': fields[0][1],
            'scale': fields[1][1]
          }
          status.queue.push(queue_obj);
        })
        .on('progress', function(bytesReceived, bytesExpected) {
            //self.emit('progess', bytesReceived, bytesExpected)
            var percent = (bytesReceived / bytesExpected * 100) | 0;
            process.stdout.write('Uploading: %' + percent + '\r');
        })
        .on('end', function() {
          console.log('-> upload done');
          printFirstInLine();
        });

      form.parse(req);
  }
  else {
    //  REGULAR WEB SERVER
    //
    var mimeTypes = {
        "html":  "text/html",
        "jpeg":  "image/jpeg",
        "jpg":   "image/jpeg",
        "png":   "image/png",
        "svg":   "image/svg+xml",
        "svgz":  "image/svg+xml",
        "js":    "text/javascript",
        "css":   "text/css"
    };

    var fileToLoad;

    if(req.url == "/") {
      fileToLoad = "index.html";
    } else {
      fileToLoad = url.parse(req.url).pathname.substr(1);
    }

    console.log("[HTTP] :: Loading :: " + WWW_ROOT + fileToLoad);

    var fileBytes;
    var httpStatusCode = 200;

    // check to make sure a file exists...
    fs.exists(WWW_ROOT + fileToLoad,function(doesItExist) {

      // if it doesn't exist lets make sure we load error404.html
      if(!doesItExist) {
        console.log("[HTTP] :: Error loading :: " + WWW_ROOT + fileToLoad);

        httpStatusCode = 404;
        fileToLoad = "error404.html";
      }

      var fileBytes = fs.readFileSync(WWW_ROOT + fileToLoad);
      var mimeType = mimeTypes[path.extname(fileToLoad).split(".")[1]]; // complicated, eh?

      res.writeHead(httpStatusCode,{'Content-type':mimeType});
      res.end(fileBytes);
    });
	}
}).listen(HTTP_PORT);
console.log("Server started at http://localhost:" + HTTP_PORT);
