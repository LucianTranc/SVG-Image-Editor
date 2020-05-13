// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {

    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/startup',   //The server endpoint we are connecting to
        data: {
            name1: "Value 1",
            name2: "Value 2"
        },
        
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
                */
               //$('#blah').html("On page load, received string '"+data.foo+"' from server");
               //We write the object to the console to show that the request was successful
               console.log("success");
               populateFileLog(data.jsonlist);
               
               populateDropDown(data.files);

               populateComponentDropDown(data.files);

               populateScaleDropDown(data.files);
               
               let img = $("#imagedropdown option:first").text();
   
        
               
        },
        fail: function(error) {
            // Non-200 return, do something with error
            //$('#blah').html("On page load, received error from server");
            console.log("fail");
            console.log(error); 
        }
    });

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });

    $('#imagedropdown').change(function(e) {

        let img = $("#imagedropdown option:selected").text();
        console.log(img);

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/imagedropdown',   //The server endpoint we are connecting to
            data: {
                name: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data.returnFile);
                updateViewPanel(data);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

    $('#scaleimagedropdown').change(function(e) {

        let img = $("#scaleimagedropdown option:selected").text();
        console.log(img);

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/scaleimagedropdown',   //The server endpoint we are connecting to
            data: {
                file: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                updateScaleModal(data);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }
        });

    });

 

    $('#componentList').on("click", ".attrsubmit", function(e) {

        e.preventDefault();

        var buttonid = $(this).attr("id");
        var paresedbuttonid = buttonid.split("_");
        console.log(paresedbuttonid);
        console.log(buttonid);
        console.log("getting data from: " + buttonid + "_form");
        var inputvalue = $("#" + buttonid + "_form").val();
        console.log(inputvalue);
        let img = $("#imagedropdown option:selected").text();
        console.log(img);

        /* if (paresedbuttonid[3] === "x" && inputvalue < 0) {
            alert("Negative x not allowed");
            return;
        }
        if (paresedbuttonid[3] === "y" && inputvalue < 0) {
            alert("Negative y not allowed");
            return;
        }
        if (paresedbuttonid[3] === "w" && inputvalue < 0) {
            alert("Negative w not allowed");
            return;
        }
        if (paresedbuttonid[3] === "h" && inputvalue < 0) {
            alert("Negative h not allowed");
            return;
        }
        if (paresedbuttonid[3] === "cx" && inputvalue < 0) {
            alert("Negative cx not allowed");
            return;
        }
        if (paresedbuttonid[3] === "cy" && inputvalue < 0) {
            alert("Negative cx not allowed");
            return;
        }
        if (paresedbuttonid[3] === "r" && inputvalue < 0) {
            alert("Negative r not allowed");
            return;
        } */
        


        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/attrsubmit',   //The server endpoint we are connecting to
            data: {
                id: buttonid,
                value: inputvalue,
                file: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                if (data.output == false) {
                    alert("Invalid Attribute");
                }
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });

    });
    $('#RectangleScale').on("click", ".rectanglescalesubmit", function(e) {

        e.preventDefault();

        var buttonid = $(this).attr("id");
        var paresedbuttonid = buttonid.split("_");
        console.log(paresedbuttonid);
        console.log(buttonid);
        console.log("getting data from: " + buttonid + "_form");
        var inputvalue = $("#" + buttonid + "_form").val();
        console.log(inputvalue);
        let img = $("#scaleimagedropdown option:selected").text();
        console.log(img);
        var i = paresedbuttonid[1];


        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/scalerectanglesubmit',   //The server endpoint we are connecting to
            data: {
                index: i,
                value: inputvalue,
                file: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                if (data.output == false) {
                    alert("Invalid Attribute");
                }
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });

    });
    $('#CircleScale').on("click", ".circlescalesubmit", function(e) {

        e.preventDefault();

        var buttonid = $(this).attr("id");
        var paresedbuttonid = buttonid.split("_");
        console.log(paresedbuttonid);
        console.log(buttonid);
        console.log("getting data from: " + buttonid + "_form");
        var inputvalue = $("#" + buttonid + "_form").val();
        console.log(inputvalue);
        let img = $("#scaleimagedropdown option:selected").text();
        console.log(img);
        var i = paresedbuttonid[1];

        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/scalecirclesubmit',   //The server endpoint we are connecting to
            data: {
                index: i,
                value: inputvalue,
                file: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                if (data.output == false) {
                    alert("Invalid Attribute");
                }
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });

    });

    $('#componentList').on("click", ".newattrsubmit", function(e) {

        e.preventDefault();

        var buttonid = $(this).attr("id");
        console.log(buttonid);
        console.log("getting data from: " + buttonid + "_form");
        var inputvaluename = $("#" + buttonid + "_formname").val();
        var inputvaluevalue = $("#" + buttonid + "_formvalue").val();
        console.log("name: "+inputvaluename);
        console.log("value: "+inputvaluevalue);
        let img = $("#imagedropdown option:selected").text();
        console.log(img);


        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/newattrsubmit',   //The server endpoint we are connecting to
            data: {
                id: buttonid,
                name: inputvaluename,
                value: inputvaluevalue,
                file: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });

    });

    $('#tdrow').on("click", ".titleeditsubmit", function(e) {

        e.preventDefault();

        var thetitle = $("#TitleEdit_form").val();
        let img = $("#imagedropdown option:selected").text();

        console.log("new title: " + thetitle);
        console.log("img: " + img);

        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/titleeditsubmit',   //The server endpoint we are connecting to
            data: {
                title: thetitle,
                file: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });

    });

    $('#tdrow').on("click", ".descriptioneditsubmit", function(e) {

        e.preventDefault();

        var thedescription = $("#DescriptionEdit_form").val();
        let img = $("#imagedropdown option:selected").text();

        console.log("new desc: " + thedescription);
        console.log("img: " + img);

        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/descriptioneditsubmit',   //The server endpoint we are connecting to
            data: {
                description: thedescription,
                file: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });

    });

    $('#createSVG').on("click", ".createSVGsubmit", function(e) {

        e.preventDefault();
    
        console.log("CREATE AN SVG IMAGE");

        var filename = $("#createSVG_filename").val();

        if (filename === "")
            return;

        console.log(filename);

        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/newsvgsubmit',   //The server endpoint we are connecting to
            data: {
                file: filename
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                if (data.output == false) {
                    alert("Invalid Attribute");
                }
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });


    });
    
    $('#addrmodal').on("click", ".rectangleaddsubmit", function(e) {

        e.preventDefault();
    
        console.log("ADD A RECT: ");

        let img = $("#componentimagedropdown option:selected").text();

        if (img === "")
            return;


        var xval = parseFloat($(RectangleAddx_form).val());
        var yval = parseFloat($(RectangleAddy_form).val());
        var wval = parseFloat($(RectangleAddw_form).val());
        var hval = parseFloat($(RectangleAddh_form).val());
        var uval = $(RectangleAddu_form).val();


        console.log(img);
        console.log(xval);
        console.log(yval);
        console.log(wval);
        console.log(hval);
        console.log(uval);


        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/newrectsubmit',   //The server endpoint we are connecting to
            data: {
                x: xval,
                y: yval,
                w: wval,
                h: hval,
                u: uval,
                file: img
            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                if (data.output == false) {
                    alert("Invalid Attribute");
                }
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });


    });

    $('#addcmodal').on("click", ".circleaddsubmit", function(e) {

        e.preventDefault();
    
        console.log("ADD A CIRC: ");

        let img = $("#componentimagedropdown option:selected").text();

        if (img === "")
            return;


        var cxval = $(CircleAddcx_form).val();
        var cyval = $(CircleAddcy_form).val();
        var rval = $(CircleAddr_form).val();
        var uval = $(CircleAddu_form).val();


        console.log(img);
        console.log(cxval);
        console.log(cyval);
        console.log(rval);
        console.log(uval);


        $.ajax({

            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/newcircsubmit',   //The server endpoint we are connecting to
            data: {
                cx: cxval,
                cy: cyval,
                r: rval,
                u: uval,
                file: img

            },
            success: function (data) {
                //$('#blah').html("On page load, received string '"+data.foo+"' from server");
                console.log(data);
                if (data.output == false) {
                    alert("Invalid Attribute");
                }
                location.reload(true);
            },
            fail: function(error) {
                //$('#blah').html("On page load, received error from server");
                console.log(error); 
            }

        });


    });


});


function populateFileLog (filedata) {

    var tableRef = document.getElementById('FileLogPanel').getElementsByTagName('tbody')[0];
    //var newCell  = newRow.insertCell(0);
    console.log(filedata);

    if (filedata.length == 0) {

        var newRow = tableRef.insertRow();
        var msgcell = newRow.insertCell();
        msgcell.setAttribute("colspan", "7");
        var centertag = document.createElement("CENTER");
        var msg = document.createTextNode("No Files Uploaded");
        centertag.appendChild(msg);
        msgcell.appendChild(centertag);

    }

    for (var i = 0; i < filedata.length; i++) {


        var newRow   = tableRef.insertRow();

        var imgcell = newRow.insertCell(0);
        var img = document.createElement("IMG");
        var a = document.createElement("A");
        a.setAttribute('href', filedata[i].filename);
        a.setAttribute('download', filedata[i].filename);
        img.setAttribute('src', filedata[i].filename);
        img.setAttribute('height', '100px');
        a.appendChild(img);
        imgcell.appendChild(a);

        var titlecell = newRow.insertCell(1);
        var titlea = document.createElement("A");
        titlea.setAttribute('href', filedata[i].filename);
        titlea.setAttribute('download', filedata[i].filename);
        var title = document.createTextNode(filedata[i].filename)
        titlea.appendChild(title);
        titlecell.appendChild(titlea);

        var sizecell = newRow.insertCell(2);
        var size = document.createTextNode(Math.round(filedata[i].size/1000));
        sizecell.appendChild(size);

        var numrectcell = newRow.insertCell(3);
        var size = document.createTextNode(filedata[i].numRect);
        numrectcell.appendChild(size);

        var numcirccell = newRow.insertCell(4);
        var size = document.createTextNode(filedata[i].numCirc);
        numcirccell.appendChild(size);

        var numpathcell = newRow.insertCell(5);
        var size = document.createTextNode(filedata[i].numPaths);
        numpathcell.appendChild(size);

        var numgroupcell = newRow.insertCell(6);
        var size = document.createTextNode(filedata[i].numGroups);
        numgroupcell.appendChild(size);

    }
    //var newText  = document.createTextNode(filenames[0]);
    
};

function populateDropDown (files) {

    var dropDownRef = document.getElementById('imagedropdown');

    files.forEach(function (file) {
        
        var option = document.createElement("OPTION");
        option.setAttribute('value', file);
        var text = document.createTextNode(file);
    
        option.appendChild(text);
        dropDownRef.appendChild(option);

    });

};
function populateComponentDropDown (files) {

    var dropDownRef = document.getElementById('componentimagedropdown');

    files.forEach(function (file) {
        
        var option = document.createElement("OPTION");
        option.setAttribute('value', file);
        var text = document.createTextNode(file);
    
        option.appendChild(text);
        dropDownRef.appendChild(option);

    });

};
function populateScaleDropDown (files) {

    var dropDownRef = document.getElementById('scaleimagedropdown');

    files.forEach(function (file) {
        
        var option = document.createElement("OPTION");
        option.setAttribute('value', file);
        var text = document.createTextNode(file);
    
        option.appendChild(text);
        dropDownRef.appendChild(option);

    });

};

function updateViewPanel (data) {

    var imgRef = document.getElementById('SVGViewPanelImage');
    imgRef.setAttribute("src", data.filename);

    console.log("title: ");
    console.log(data.title);

    var titleRef = document.getElementById("titlecell");
    if (titleRef.childNodes[0])
        titleRef.removeChild(titleRef.childNodes[0]);
    var title;
    if (data.title == "")
        title = document.createTextNode("No Title");
    else
        title = document.createTextNode(data.title);

    titleRef.appendChild(title);

    var descriptionRef = document.getElementById("descriptioncell");
    if (descriptionRef.childNodes[0])
        descriptionRef.removeChild(descriptionRef.childNodes[0]);
    var description;
    if (data.description == "")
        description = document.createTextNode("No Description");
    else
        description = document.createTextNode(data.description);

    descriptionRef.appendChild(description);
    descriptionRef.setAttribute("colspan", "1");

    console.log('rectlist:');
    console.log(data.rects);

    var list = document.getElementById("componentList");

    var child = list.lastElementChild;  
    while (child) { 
        list.removeChild(child); 
        child = list.lastElementChild; 
    }

    updateViewPanelRects(data);
    updateViewPanelCircs(data);
    updateViewPanelPaths(data);
    updateViewPanelGroups(data);


}

function updateViewPanelRects (data) {

    var tableRef = document.getElementById("componentList");

    for (var i = 0; i < data.rects.length; i++) {

        var newRow = tableRef.insertRow();
        tableRef.appendChild(newRow);

        var namecell = newRow.insertCell(0);
        var name = document.createTextNode("Rectangle " + (i+1));

        namecell.appendChild(name);
        newRow.appendChild(namecell);

        var units = data.rects[i].units

        var summarycell = newRow.insertCell(1);
        var summary = document.createTextNode("Upper Left Corner, x: " + data.rects[i].x + units + ", y: " + data.rects[i].y + units +
                                             ", width: " + data.rects[i].w + units + ", height: " + data.rects[i].h + units);
        console.log("summary");
        console.log(data.rects[i]);

        summarycell.appendChild(summary);
        newRow.appendChild(summarycell);

        console.log("DATA.RECTS[i]");
        console.log(data.rects[i]);


        var othercell = newRow.insertCell(2);
        var button = document.createElement("BUTTON");
        button.setAttribute("type", "button");
        button.setAttribute("data-toggle", "modal");
        button.setAttribute("data-target", "#Rectangle "+ (i+1));
        var other = document.createTextNode(data.rects[i].numAttr);

        button.appendChild(other);
        othercell.appendChild(button);

        createModal(data.rectAttrs[i], "Rectangle", othercell, i)

        newRow.appendChild(othercell);

        
    }

}



function updateViewPanelCircs (data) {

    var tableRef = document.getElementById("componentList");

    for (var i = 0; i < data.circs.length; i++) {

        var newRow = tableRef.insertRow();
        tableRef.appendChild(newRow);

        var namecell = newRow.insertCell(0);
        var name = document.createTextNode("Circle " + (i+1));

        namecell.appendChild(name);
        newRow.appendChild(namecell);

        var units = data.circs[i].units

        var summarycell = newRow.insertCell(1);
        var summary = document.createTextNode("Upper Left Corner, x: " + data.circs[i].cx + units + ", y: " + data.circs[i].cy + units +
                                             ", radius: " + data.circs[i].r + units);
        console.log("summary");
        console.log(data.circs[i]);

        summarycell.appendChild(summary);
        newRow.appendChild(summarycell);

        var othercell = newRow.insertCell(2);
        var button = document.createElement("BUTTON");
        button.setAttribute("type", "button");
        button.setAttribute("data-toggle", "modal");
        button.setAttribute("data-target", "#Circle "+ (i+1));
        var other = document.createTextNode(data.circs[i].numAttr);

        button.appendChild(other);
        othercell.appendChild(button);

        createModal(data.circAttrs[i], "Circle", othercell, i)

        newRow.appendChild(othercell);

    }

}

function updateViewPanelPaths (data) {

    var tableRef = document.getElementById("componentList");

    console.log("PATHS");
    console.log(data.paths);

    for (var i = 0; i < data.paths.length; i++) {

        var newRow = tableRef.insertRow();
        tableRef.appendChild(newRow);

        var namecell = newRow.insertCell(0);
        var name = document.createTextNode("Path " + (i+1));

        namecell.appendChild(name);
        newRow.appendChild(namecell);

        var summarycell = newRow.insertCell(1);
        var summary = document.createTextNode("data: " + data.paths[i].d);


        summarycell.appendChild(summary);
        newRow.appendChild(summarycell);

        var othercell = newRow.insertCell(2);
        var button = document.createElement("BUTTON");
        button.setAttribute("type", "button");
        button.setAttribute("data-toggle", "modal");
        button.setAttribute("data-target", "#Path "+ (i+1));
        var other = document.createTextNode(data.paths[i].numAttr);

        button.appendChild(other);
        othercell.appendChild(button);

        createModal(data.pathAttrs[i], "Path", othercell, i)

        newRow.appendChild(othercell);

    }

}

function updateViewPanelGroups (data) {

    var tableRef = document.getElementById("componentList");

    console.log("groups");
    console.log(data.groups);

    for (var i = 0; i < data.groups.length; i++) {

        var newRow = tableRef.insertRow();
        tableRef.appendChild(newRow);

        var namecell = newRow.insertCell(0);
        var name = document.createTextNode("Group " + (i+1));

        namecell.appendChild(name);
        newRow.appendChild(namecell);

        var summarycell = newRow.insertCell(1);
        var summary = document.createTextNode("children: " + data.groups[i].children);
        summarycell.setAttribute("colspan", "1");

        summarycell.appendChild(summary);
        newRow.appendChild(summarycell);


        var othercell = newRow.insertCell(2);
        var button = document.createElement("BUTTON");
        button.setAttribute("type", "button");
        button.setAttribute("data-toggle", "modal");
        button.setAttribute("data-target", "#Group "+ (i+1));
        var other = document.createTextNode(data.groups[i].numAttr);

        button.appendChild(other);
        othercell.appendChild(button);

        createModal(data.groupAttrs[i], "Group", othercell, i)

        newRow.appendChild(othercell);

    }

}

function createModal (data, name, cell, number) {

    var rootmodal;
    var modal1;
    var modal2;
    var modal3_1;
    var modal3_2;
    var header;

    rootmodal = document.createElement("DIV");
    rootmodal.setAttribute("id", name + " " + (number+1));
    rootmodal.setAttribute("class", "modal fade");
    rootmodal.setAttribute("role", "dialog");

    modal1 = document.createElement("DIV");
    modal1.setAttribute("class", "modal-dialog");
    rootmodal.appendChild(modal1);
    
    modal2 = document.createElement("DIV");
    modal2.setAttribute("class", "modal-content");
    modal1.appendChild(modal2);

    modal3_1 = document.createElement("DIV");
    modal3_1.setAttribute("class", "modal-header");
    modal2.appendChild(modal3_1);
    modal3_2 = document.createElement("DIV");
    modal3_2.setAttribute("class", "modal-body");
    modal2.appendChild(modal3_2);

    header = document.createElement("H4");
    var text1 = document.createTextNode(name + " " + (number+1) + " Attributes:");
    header.appendChild(text1);
    modal3_1.appendChild(header);

    var button = document.createElement("BUTTON");
    button.setAttribute("type", "button");
    button.setAttribute("class", "close");
    button.setAttribute("data-dismiss", "modal");
    var text2 = document.createTextNode("X");
    button.appendChild(text2);
    modal3_1.appendChild(button);


    for (var j = 0; j < data.length; j++) {

        var tempp = document.createElement("P");
        var tempform = document.createElement("FORM");
        tempform.setAttribute("ref", name + "_" + number + "_attr_" + data[j].name);
        tempform.setAttribute("id", name + "_" + number + "_attr_" + data[j].name);
        var tempdiv1 = document.createElement("DIV");
        tempdiv1.setAttribute("class", "form-group");
        var attrlabel = document.createElement("LABEL");
        attrlabel.setAttribute("for", name + "_" + number + "_attr_" + data[j].name + "_form");
        var attrlabeltext = document.createTextNode(data[j].name + ": " + data[j].value);
        attrlabel.appendChild(attrlabeltext);
        var attrinput = document.createElement("INPUT");
        attrinput.setAttribute("type", "text");
        attrinput.setAttribute("class", "form-control");
        attrinput.setAttribute("id", name + "_" + number + "_attr_" + data[j].name + "_form");
        attrinput.setAttribute("value", "");
        attrinput.setAttribute("placeholder", "Edit");
        tempdiv1.appendChild(attrlabel);
        tempdiv1.appendChild(attrinput);
        tempform.appendChild(tempdiv1);
        tempp.appendChild(tempform);
        modal3_2.appendChild(tempp);

        var tempdiv2 = document.createElement("DIV");
        tempdiv2.setAttribute("class", "form-group");
        tempform.appendChild(tempdiv2);
        var tempbutton = document.createElement("INPUT");
        tempbutton.setAttribute("type", "submit");
        tempbutton.setAttribute("class", "attrsubmit");
        tempbutton.setAttribute("id", name + "_" + number + "_attr_" + data[j].name);

        tempdiv2.appendChild(tempbutton);

    }

    var newattrp = document.createElement("P");
    modal3_2.appendChild(newattrp);

    var newattrnameform = document.createElement("FORM");
    newattrnameform.setAttribute("ref", name + "_" + number + "_nameform");
    newattrnameform.setAttribute("id", name + "_" + number + "_nameform");
    newattrp.appendChild(newattrnameform);
    var divnameinput = document.createElement("DIV");
    divnameinput.setAttribute("class", "form-group");
    var nameinputlabel= document.createElement("LABEL");
    nameinputlabel.setAttribute("for", name + "_" + number + "_input_formname")
    var nameinputlabeltext = document.createTextNode("New Attribute Name:");
    var nameinput = document.createElement("INPUT");
    nameinput.setAttribute("type", "text");
    nameinput.setAttribute("class", "form-control");
    nameinput.setAttribute("id", name + "_" + number + "_input_formname");
    nameinput.setAttribute("placeholder", "Enter name");
    nameinputlabel.appendChild(nameinputlabeltext);
    divnameinput.appendChild(nameinputlabel);
    divnameinput.appendChild(nameinput);
    newattrnameform.appendChild(divnameinput);
    

    var divvalueinput = document.createElement("DIV");
    divvalueinput.setAttribute("class", "form-group");
    var valueinputlabel= document.createElement("LABEL");
    valueinputlabel.setAttribute("for", name + "_" + number + "_input_formvalue")
    var valueinputlabeltext = document.createTextNode("New Attribute Value:");
    var valueinput = document.createElement("INPUT");
    valueinput.setAttribute("type", "text");
    valueinput.setAttribute("class", "form-control");
    valueinput.setAttribute("id", name + "_" + number + "_input_formvalue");
    valueinput.setAttribute("placeholder", "Enter value");
    valueinputlabel.appendChild(valueinputlabeltext);
    divvalueinput.appendChild(valueinputlabel);
    divvalueinput.appendChild(valueinput);
    newattrnameform.appendChild(divvalueinput);
    
    var divvaluebutton = document.createElement("DIV");
    divvaluebutton.setAttribute("class", "form-group");
    var valuebutton = document.createElement("INPUT");
    valuebutton.setAttribute("type", "submit");
    valuebutton.setAttribute("class", "newattrsubmit");
    valuebutton.setAttribute("id", name + "_" + number + "_input");
    divvaluebutton.appendChild(valuebutton);
    newattrnameform.appendChild(divvaluebutton);


    cell.appendChild(rootmodal);

}

function updateScaleModal(data) {


    var rectform = document.getElementById("RectangleScale");
    var circform = document.getElementById("CircleScale");
    
    var child = rectform.lastElementChild;  

    while (child) { 
        rectform.removeChild(child); 
        child = rectform.lastElementChild; 
    }

    var child = circform.lastElementChild;  
    while (child) { 
        circform.removeChild(child); 
        child = circform.lastElementChild; 
    }
    
    

    
    if (data.rects.length === 0) {
        
        var norects = document.createTextNode("No Rectangles");
        var tempdiv = document.createElement("DIV");
        tempdiv.appendChild(norects);
        rectform.appendChild(tempdiv);
        
    }
    else {
        for (var i = 0; i < data.rects.length; i++) {
    
            var newRect = document.createElement("DIV");
            newRect.setAttribute("class", "form-group");
    
            var label = document.createElement("LABEL");
            label.setAttribute("for", "scaleRectangle_" + (i+1));
            var labeltext = document.createTextNode("Scale Rectangle " + (i+1));
            label.appendChild(labeltext);
            newRect.appendChild(label);
    
            var input = document.createElement("INPUT");
            input.setAttribute("type", "text");
            input.setAttribute("class", "form-control");
            input.setAttribute("id", "scaleRectangle_" + (i+1) + "_form");
            input.setAttribute("value", "");
            input.setAttribute("placeholder", "Scale Factor");
    
            newRect.appendChild(input);
            rectform.appendChild(newRect);
    
            var submit = document.createElement("DIV");
            submit.setAttribute("class", "form-group");
            var button = document.createElement("INPUT");
            button.setAttribute("type", "submit");
            button.setAttribute("class", "rectanglescalesubmit");
            button.setAttribute("id", "scaleRectangle_" + (i+1));
            submit.appendChild(button);
            rectform.appendChild(submit);
        }
    }


    
    if (data.circs.length === 0) {
        
        var nocircs = document.createTextNode("No Circles");
        var tempdiv = document.createElement("DIV");
        tempdiv.appendChild(nocircs);
        circform.appendChild(tempdiv);
        
    }
    else {
        
        for (var i = 0; i < data.circs.length; i++) {
    
            var newCirc = document.createElement("DIV");
            newCirc.setAttribute("class", "form-group");
    
            var label = document.createElement("LABEL");
            label.setAttribute("for", "scaleCircle_" + (i+1));
            var labeltext = document.createTextNode("Scale Circle " + (i+1) + "_form");
            label.appendChild(labeltext);
            newCirc.appendChild(label);
    
            var input = document.createElement("INPUT");
            input.setAttribute("type", "text");
            input.setAttribute("class", "form-control");
            input.setAttribute("id", "scaleCircle_" + (i+1) + "_form");
            input.setAttribute("value", "");
            input.setAttribute("placeholder", "Scale Factor");
    
            newCirc.appendChild(input);
            circform.appendChild(newCirc);
    
            var submit2 = document.createElement("DIV");
            submit2.setAttribute("class", "form-group");
            var button2 = document.createElement("INPUT");
            button2.setAttribute("type", "submit");
            button2.setAttribute("class", "circlescalesubmit");
            button2.setAttribute("id", "scaleCircle_" + (i+1));
            submit2.appendChild(button2);
            circform.appendChild(submit2);
    
        }
        

    }

    /*                      
    <div class="form-group">
        <label for="CircleAddu_form">Enter Units</label>
        <input type="text" class="form-control" id="CircleAddu_form" value="" placeholder="units">
    </div>
    <div class="form-group">
        <input type="submit" class="circleaddsubmit" id="CircleAdd">
    </div> 
    */

}