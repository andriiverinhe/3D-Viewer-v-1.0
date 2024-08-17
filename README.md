# s21_3DViewer

This program for viewing 3D wireframe models (3D Viewer) in the C programming language. The models themselves must be loaded from .obj files and be viewable on screen with the ability to rotate, scale and translate. A wireframe model is a model of an object in 3D graphics, which is a set of vertices and edges that defines the shape of the displayed polyhedral object in three-dimensional space.

![alt text](misc/images/image.png)

![Video title](misc/video/video.webm)

The program provides the ability to:

    - Load a wireframe model from an obj file (vertices and surfaces list support only);
    - Translate the model by a given distance in relation to the X, Y, Z axes;
    - Rotate the model by a given angle relative to its X, Y, Z axes;
    - Scale the model by a given value;

The program allows customizing the type of projection (parallel and central);

The program allows setting up the type (solid, dashed), color and thickness of the edges, display method (none, circle, square), color and size of the vertices;

The program allows choosing the background color;

Settings can be saved between program restarts.

The program allows saving the captured (rendered) images as bmp and jpeg files;

The program allows recording small screencasts by a special button - the current custom affine transformation of the loaded object into gif-animation.

The graphical user interface contains:

    - A button to select the model file and a field to output its name;
    - A visualisation area for the wireframe model;
    - Buttons and input fields for translating the model;
    - Buttons and input fields for rotating the model;
    - Buttons and input fields for scaling the model;
    - Information about the uploaded model - file name, number of vertices and edges;

Use standard set of Makefile targets: all, install, uninstall, clean, dvi, dist, tests, gcov. 

Don't forget to specify your own qmake and installation path in Makefile.

