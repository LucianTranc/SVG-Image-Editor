# SVG-Image-Editor

A web app that allows users to edit, scale and add or delete image elements from .svg images. The app works by calling a C library through Node.js' ffi-napi add on. The C library uses libxml2 to parse the images into xml trees which are converted to my own data structure that separates the images into groups, paths, rectangles and circles. One the image is parsed and converted into workable data, the user can then perform mutations on the elements that were parsed. Once the user makes the changes the library can then revert the data structure back into an xml tree which then results in the final .svg image as output

# What I Learned

* Full stack development experience
* How to integrate different software components into a working system
* REST API experience (Node.js/AJAX)

I am hosting this application at:
https://svg-editor.herokuapp.com/
It may take a little bit to load.

# To run

This repository will not run. This is public for people to look at the parser code if they wish

I will still include this in case I choose to update it in the future
works with Node v15.2.1 and npm v7.0.10

(on Windows/Linux)
* cd ./parser && make parser
* npm run dev

(on Mac)
* cd ./parser && make parserdylib
* npm run dev
app will run on localhost/3000

You may need to change your library path so the linker can find libxml2.
You can use the command LD_LIBRARY_PATH='your systems lib folder'.
