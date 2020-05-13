'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 
/*ps aux | grep ltranc | grep node
  kill -9 pid*/


let sharedLib = ffi.Library('./libsvgparse', {
  'SVGtoJSONwrapper': [ 'string', ['string'] ],
  'SVGtitlewrapper': [ 'string', ['string'] ],
  'SVGdescriptionwrapper': [ 'string', ['string'] ],
  'SVGrectListtoJSONwrapper': [ 'string', ['string'] ],
  'rectListtoJSONwrapper': [ 'string', ['string'] ],
  'SVGcircListtoJSONwrapper': [ 'string', ['string'] ],
  'circListtoJSONwrapper': [ 'string', ['string'] ],
  'SVGpathListtoJSONwrapper': [ 'string', ['string'] ],
  'SVGgroupListtoJSONwrapper': [ 'string', ['string'] ],
  'rectAttrListToJSONwrapper': [ 'string', ['string', 'int'] ],
  'circAttrListToJSONwrapper': [ 'string', ['string', 'int'] ],
  'pathAttrListToJSONwrapper': [ 'string', ['string', 'int'] ],
  'groupAttrListToJSONwrapper': [ 'string', ['string', 'int'] ],
  'updateAttributewrapper': [ 'bool', ['string', 'string', 'string', 'int', 'int'] ],
  'setTDwrapper': [ 'string', ['string', 'string', 'string'] ],
  'addRectanglewrapper' : [ 'bool', ['string', 'float', 'float', 'float', 'float', 'string'] ],
  'addCirclewrapper' : [ 'bool', ['string', 'float', 'float', 'float', 'string'] ],
  'circScalewrapper' : [ 'bool', ['string', 'int', 'float'] ],
  'rectScalewrapper' : [ 'bool', ['string', 'int', 'float'] ],
  'createNewSVGwrapper' : [ 'bool', ['string'] ]

});

//Call a C function that takes no arguments, returns a string, and display its value

//Sample endpoint
app.get('/startup', function(req , res){

  var datalist = [];
  var filenames = [];

  console.log("STARTING");

  fs.readdir('uploads/', function (err, files) {
    if (err) {
        return console.log('Unable to scan directory: ' + err);
    } 
    files.forEach(function (file) {

      console.log("get data for:");
      console.log(file);
      var json = JSON.parse(sharedLib.SVGtoJSONwrapper('uploads/' + file));
      if (json != null && (file.search(".svg") != -1)) {
        filenames.push(file);
        json.filename = file;
        var stats = fs.statSync('uploads/' + file)
        let filesize = stats['size'];
        json.size = filesize;
        datalist.push(json);
      }
      
    });

    res.send({
      jsonlist: datalist,
      files: filenames
      
    });
    
  }); 

});

app.get('/imagedropdown', function(req, res){

  var json;
  var newfile;
  var thetitle;
  var thedesc;
  var rectList;
  var circList;
  var pathList;
  var groupList;
  var rectAttrList = [];
  var circAttrList = [];
  var pathAttrList = [];
  var groupAttrList = [];
  fs.readdir('uploads/', function (err, files) {
    if (err) {
        return console.log('Unable to scan directory: ' + err);
    } 
    files.forEach(function (file) {

        if (file === req.query.name) {
          console.log("match");
          json = JSON.parse(sharedLib.SVGtoJSONwrapper('uploads/' + file));
          newfile = file;
          thetitle = sharedLib.SVGtitlewrapper('uploads/' + file);
          thedesc = sharedLib.SVGdescriptionwrapper('uploads/' + file);
          rectList = JSON.parse(sharedLib.SVGrectListtoJSONwrapper('uploads/' + file));
          circList = JSON.parse(sharedLib.SVGcircListtoJSONwrapper('uploads/' + file));
          pathList = JSON.parse(sharedLib.SVGpathListtoJSONwrapper('uploads/' + file));
          groupList = JSON.parse(sharedLib.SVGgroupListtoJSONwrapper('uploads/' + file));

          for (var i = 0; i < rectList.length; i++){            
            var temprectattrs = JSON.parse(sharedLib.rectAttrListToJSONwrapper('uploads/' + file, i));
            var rectx = {"name": "x", "value": rectList[i].x};
            var recty = {"name": "y", "value": rectList[i].y};
            var rectw = {"name": "w", "value": rectList[i].w};
            var recth = {"name": "h", "value": rectList[i].h};
            temprectattrs.push(rectx);
            temprectattrs.push(recty);
            temprectattrs.push(rectw);
            temprectattrs.push(recth);
            rectAttrList[i] = temprectattrs;
          }
          console.log("rectattrlist:");
          console.log(rectAttrList);
          for (var i = 0; i < circList.length; i++){            
            var tempcircattrs = JSON.parse(sharedLib.circAttrListToJSONwrapper('uploads/' + file, i));
            var circcx = {"name": "cx", "value": circList[i].cx};
            var circcy = {"name": "cy", "value": circList[i].cy};
            var circr = {"name": "r", "value": circList[i].r};
            tempcircattrs.push(circcx);
            tempcircattrs.push(circcy);
            tempcircattrs.push(circr);
            circAttrList[i] = tempcircattrs;
          }
          console.log("circattrlist:");
          console.log(circAttrList);
          for (var i = 0; i < pathList.length; i++){            
            var temppathattrs = JSON.parse(sharedLib.pathAttrListToJSONwrapper('uploads/' + file, i));
            var pathd = {"name": "d", "value": pathList[i].d};
            temppathattrs.push(pathd);
            pathAttrList[i] = temppathattrs;

          }
          console.log("pathattrlist:");
          console.log(pathAttrList);
          for (var i = 0; i < groupList.length; i++){            
            groupAttrList[i] = JSON.parse(sharedLib.groupAttrListToJSONwrapper('uploads/' + file, i));
          }
          console.log("groupattrlist:");
          console.log(groupAttrList);
        
        }
        else {
          console.log("no match");
        }
        
    });

    res.send({
      returnFile: json,
      filename: newfile,
      title: thetitle,
      description: thedesc,
      rects: rectList,
      circs: circList,
      paths: pathList,
      groups: groupList,
      rectAttrs: rectAttrList,
      circAttrs: circAttrList,
      pathAttrs: pathAttrList,
      groupAttrs: groupAttrList
    });
    
  }); 
  
});


app.get('/attrsubmit', function(req, res){

    console.log("id: " + req.query.id);
    console.log("value: " + req.query.value);
    var parsed = req.query.id.split("_");
    console.log(parsed);
  
    var type = 0;
    var num = parseInt(parsed[1]);
    var name = parsed[3];
    var value = req.query.value;

    if (parsed[0] === "Rectangle")
      type = 1;
    else if (parsed[0] === "Circle")
      type = 2;
    else if (parsed[0] === "Path")
      type = 3;
    else if (parsed[0] === "Group")
      type = 4;

    console.log("name: " + name);
    console.log("value: " + value);
    console.log("type: " + type);
    console.log("num: " + num);
    console.log("file: " + req.query.file)

    

    var returnbool = sharedLib.updateAttributewrapper('uploads/' + req.query.file, name, value, num, type);

    res.send({

      output: returnbool

    });
    

});


app.get('/newattrsubmit', function(req, res){

    console.log("id: " + req.query.id);
    console.log("value: " + req.query.value);
    var parsed = req.query.id.split("_");
    console.log(parsed);
  
    var type = 0;
    var num = parseInt(parsed[1]);
    var name = req.query.name;
    var value = req.query.value;

    if (parsed[0] === "Rectangle")
      type = 1;
    else if (parsed[0] === "Circle")
      type = 2;
    else if (parsed[0] === "Path")
      type = 3;
    else if (parsed[0] === "Group")
      type = 4;

    console.log("name: " + name);
    console.log("value: " + value);
    console.log("type: " + type);
    console.log("num: " + num);
    console.log("file: " + req.query.file)

    var returnstring = sharedLib.updateAttributewrapper('uploads/' + req.query.file, name, value, num, type);

    res.send({

      output: returnstring

    });
    

});

app.get('/titleeditsubmit', function(req, res){

    var returnstring = sharedLib.setTDwrapper('uploads/' + req.query.file, req.query.title, "title");

    res.send({

      output: returnstring

    });
    

});

app.get('/descriptioneditsubmit', function(req, res){

    var returnstring = sharedLib.setTDwrapper('uploads/' + req.query.file, req.query.description, "description");

    res.send({

      output: returnstring

    });
    

});


app.get('/newsvgsubmit', function(req, res) {

    var returnstring = "svgreturnval";

    var filename = req.query.file;

    var returnstring = sharedLib.createNewSVGwrapper('uploads/' + filename)

    console.log(filename);

    res.send({

      output: returnstring

    });
    

});


app.get('/newcircsubmit', function(req, res) {

  console.log(req.query.file);
  console.log(req.query.cx);
  console.log(req.query.cy);
  console.log(req.query.r);
  console.log(req.query.u);

  var returnval = sharedLib.addCirclewrapper('uploads/' + req.query.file, req.query.cx, req.query.cy, req.query.r, req.query.u);

  res.send({

    output: returnval

  });

});
app.get('/newrectsubmit', function(req, res) {

    console.log(req.query.file);
    console.log(req.query.x);
    console.log(req.query.y);
    console.log(req.query.w);
    console.log(req.query.h);
    console.log(req.query.u);

    var returnval = sharedLib.addRectanglewrapper('uploads/' + req.query.file, req.query.x, req.query.y, req.query.w, req.query.h, req.query.u);

    res.send({

      output: returnval

    });
    

});
app.get('/scaleimagedropdown', function(req, res) {

  var rectList;
  var circList;

  fs.readdir('uploads/', function (err, files) {
    if (err) {
        return console.log('Unable to scan directory: ' + err);
    } 
    files.forEach(function (file) {

      if (file === req.query.file) {
        console.log("match");
        rectList = JSON.parse(sharedLib.rectListtoJSONwrapper('uploads/' + file));
        circList = JSON.parse(sharedLib.circListtoJSONwrapper('uploads/' + file));
      }
      
    });

    res.send({

      rects: rectList,
      circs: circList

    });

  });

});
app.get('/scalerectanglesubmit', function(req, res) {

  console.log("SCALING RECTANGLE")
  
  var index = req.query.index;
  var filename = req.query.file;
  var scalefactor = parseFloat(req.query.value);

  console.log(index);
  console.log(filename);
  console.log(scalefactor);

  var returnval = sharedLib.rectScalewrapper('uploads/' + filename, index-1, scalefactor);


  res.send({

    output: returnval

  });


});
app.get('/scalecirclesubmit', function(req, res) {

  console.log("SCALING CIRCLE")

  var index = req.query.index;
  var filename = req.query.file;
  var scalefactor = parseFloat(req.query.value);

  console.log(index);
  console.log(filename);
  console.log(scalefactor);

  var returnval = sharedLib.circScalewrapper('uploads/' + filename, index-1, scalefactor);


  res.send({

    output: returnval

  });


});







app.listen(portNum);
console.log('Running app at localhost: ' + portNum);