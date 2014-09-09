Demo: "CharSim"
===============

Version 001
-----------

### INTRODUCTION

[final\_anim.mkv] Demo: The "Green Dragon" character running on the
CharSim 3D character simulation framework.

For an introduction to the demo's origin, see the following blog entry:
[link to blog page].

What follows is a technical description of *CharSim*'s framework after
completion of version 001 as well as a description of the example
character created for the demo.

Links to my *working* journal are provided for more detailed information
and links to the web and book resources which helped me considerably. I
emphasise "working" because any code or method I have outlined in a blog
entry may be changed or improved in the final code, so always look at
the source code for the working (in the different sense) implementation.

With that said...

#### Where can I get it?

All source code for the framework and Green Dragon demo is provided
under the MIT License and is distributed on Github [link].

What're not provided are the scene resources for the Green Dragon
character: the 3D meshes and their textures. I don't wish to distribute
these on Github for a number of reasons, but they can be downloaded from
my website [link] along with a binary if you want to run the demo
yourself.

### FRAMEWORK

[general\_architecture.png] Charsim Architecture (Generalised Layout)

The framework is designed to be a very basic 3D game engine in order to
simulate a 3D environment for the purpose of animating and rendering 3D
models and environments. The bias of my demo is to direct and simulate a
character's neck and face, so the framework provides support for Inverse
Kinematics and vertex morphing. The resource management system takes a
3D mesh which has been rigged and given UV mapping and bone animation
and creates an internal data type which can be animated and rendered by
the framework.

For these capabilities to be possible, the framework therefore supports
a scene graph [xref], a renderer [xref], timing [xref] and animation
support [xref] and resource management [xref]. Input [xref] is also
necessary for interactive environments and characters. These are
discussed in more detail below.

For flexibility in creating custom 3D environments and characters, the
framework/GameWorld.h class is intended to be inherited by a custom game
world in an object-oriented manner where the developer can create their
own scene graph elements and add them to the scene graph. This is
obviously very crude, hence the 'very basic' part. 'To make CharSim
resemble something more akin to a game engine, a scripting language
could be implemented in order to abstract away from the tedious scene
graph maintainence and allow scenes to be loaded as data separate from
the compiled code. A step further would be to create a 3D editing
environment which generates ids for elements and allows easy model
creation and placement.

Information on implementing inverse kinematics can be found here:
Information on vertex morphing for facial expressions can be found here:

The source code is well-documented - every class header contains a
description of its purpose.

#### SCENEGRAPH

In version 001, "ModelNodes" inherit from framework/SceneNode.h [link]
and "ControllerNodes" inherit from framework/SceneGraph.h [link].
SceneNode and SceneGraph inherit from the abstract framework/GraphNode.h
[link]. In a future version there will be "CameraNodes" and "LampNodes"
which inherit from SceneNode and can be controlled by ControllerNodes
just like ModelNodes.

ModelNodes and ControllerNodes are added to the scene graph.
ControllerNodes inherit from framework/SceneGraph.h [link] and can
contain other ControllerNodes and ModelNodes. This allows collections of
scene elements to be controllable as a group by a single ControllerNode,
and the effects of multiple ControllerNodes can be combined down the
hierarchy. This allows for useful control, for example: a character
could enter the driving seat of a ship and become a child element of the
ship's control node, so that all the movements of the ship translate to
the occupying character. Collision detection can still be performed on
the ship *and* the occupant if the game designer desires the occupant to
have more granular interaction with the environment.

When CameraNodes and LampNodes are added they can too be made child
elements of ControllerNodes which would allow cameras and lights to be
attached to moving scene elements for mobile POVs and lighting. In
version 001, however, the camera is hardcoded into Renderer and lights
are hardcoded into the shader program. These will be made SceneGraph
nodes. The CameraNode will define the Renderer's camera settings, and
LampNodes will add themselves to the Renderer's list of lights.

#### RENDERER

The scene graph and rendering system use an object-oriented design. The
world must declare a new renderer object based on a selection of
customised renderers which inherit from framework/Renderer.h [link] (in
version 001 there is only the base class Renderer, and it is designed to
suit my Green Dragon demo rather than be an abstract class).

The renderer object is composited with every ModelNode, which update the
renderer's World matrix and provide the OpenGL vertex buffer object and
shader program.

For skinned models, the renderer must accept an array of bone
transformation matrixes which are sent to the shader. For vertex
morphing models, it must accept an array of shape indices and their
associated weightings (see MATERIALS & SHADING).

#### TIMING

Interactive motion, predefined animation and simulated physics require
knowledge of the elapsed time of the game and the time taken for each
frame to render (AKA the time delta or frame interval).
framework/GameLoop [link] creates these values which are provided to
scene elements with an `ITimer` interface.

In version 001 the time delta is calculated using [Euler
integration](http://gafferongames.com/game-physics/integration-basics/)
which is accurate enough for my short demo character interaction, but if
I want to extend the demo with gameplay and physics simulation, I would
implement a better integrator.

#### CLIPRUNNER

Each animateable scene model provides its own animation clip functions
which can be called by hosting controller nodes and contains an instance
of the framework/ClipRunner.h class. When an animation clip is played,
it is added to the ClipRunner's array of clips. Every time ClipRunner is
called each frame, it progresses the model's animation data forward
depending on the elapsed game time and time delta.

In version 001 it supports animation clips in the form of skeletal
animation and vertx morph targets and provides basic looping ability. It
allows the speed of any animation type to be changed via a "warp"
factor.

A character can have an unlimited number of vertex morph targets, but in
version 001 only two may be blended by the shader in any one frame.
Future versions will allow up to five targets to be blended in each
frame.

For more information, see blog entries: here, here, here, etc.

#### RESOURCE MANAGER

In version 001, the transfer of data follows the strict path: **HDD
storage** -\> **Host computer memory** -\> **GPU device memory**. The
resource manager's job is to transfer data from the HDD to the host
memory and provide a central repository for scene resources. It ensures
that only one copy of a resource exists in memory and that all resource
pointers are removed when unused.

The resource manager should never transfer anything to the GPU. GPU
device memory in version 001 is managed within the scene nodes and
resources themselves. The nature of the resource manager means that so
long as a mesh is not loaded multiple times from the HDD under different
names, there will only be one array of those mesh vertices in the GPU
memory.

Scene resources in version 001 include Meshes [link] and Materials
[link]. Textures should be a resource, but in version 001 they are
loaded directly by the materials/DiffuseMaterial.h class.

#### UTILITIES & SOLVERS

Bézier-based Inverse Kinematics (IK) is what I chose to use for the
Green Dragon character's neck because it was more simple to control
artistically. More traditional IK (i.e. the skeleton takes the shortest
path possible towards a target) is implemented and can be found in
framework/InverseKinematics.cpp [link], but in version 001 a mechanism
for constraining joint angles has not yet been implemented, which is
necessary to create natural movement in humanoid and animal characters.

In version 001, the Bézier-based IK is contained in
characters/dragon/DragonBustNode.cpp but this will be moved to the
InverseKinematics class.

In version 001, the Bézier method uses framework/CollisionDetection.h
[link] to compute the intersection between a finite line and a sphere.
This utility class will be expanded to support other sorts of
intersection testing such as between Axis-Aligned Bounding Boxes (AABB)
if Bounding Volume Hierarchies (BVH) are implemented for general
collision detection in a game scenario.

Journal entries on inverse kinematics: Journal entries on Bézier curves:

#### MATERIALS & SHADING

Though the framework is mostly written in C++, a lot of heavy lifting is
done on the GPU device where the problem is well-suited for parallel
computing.

Skinning defines the position of each vertex in a mesh as being based on
the rotation of every bone and a given weighting. The shader already
multiplies each vertex with a WVP transformation matrix, so it makes
sense for it to handle bone transformation matrix multiplication as
well.

Vertex morphing defines the position of each vertex as being based on a
number of alternate mesh shapes and a given weighting. These weights and
alternate mesh vertices are given ther own vertex buffer objects (VBOs)
and are processed along with the regular vertex attributes.

Otherwise, in version 001, the shaders use Lambert+Phong shading as per
the standard lighting model. I would like to look at physically-based
illumination methods and shadowing methods (such as shadow buffers and
shadow volumes).

The shaders are written using GLSL [link]. In version 001, GLSL v120
[link] is used for compatibility with older graphics cards, though v330
[link] versions are also provided).

#### INPUT

Mouse and keyboard input is supplied by the *GLFW 3* [link] mouse and
keyboard callback functions. GLFW also provides functions to determine
specific key presses which allow detection of combined presses.

Controls in version 001:
:   -   Moving the mouse cursor around the screen causes the dragon to
        follow it. This is the base of the interactive demo in version
        001.
    -   `Up` and `Down` cursor keys spin the Green Dragon model. This is
        for testing purposes and isn't part of the interactive demo.

#### TECHNOLOGY & APIS

-   OpenGL API [link]

I chose OpenGL as the graphics API specification for its multiplatform
nature. In version 001 CharSim is only compatible for Linux, but lack of
Windows compatibility is listed as a bug.

I am intrigued by the use of WebGL for mobile game development, which is
another reason I sought to familiarise myself with the GL way of doing
things. I have worked with Microsoft Direct3D 11 [link] and Direct3D 9
[link] in previous projects, and a Microsoft DirectX (i.e. Direct3D 12)
alternate backend for CharSim isn't out of the question.

-   GLFW [link]

GLFW [link] provides the OpenGL context with a window and input
handling. I chose it for its cross-platform compatibility, its flexible
input integration and its accessible documentation.

-   RapidXML [link]

A C++ XML parser I chose to parse the COLLADA [xref] files for rigged
models. I tried using ASSIMP [link] but it was unable to parse the files
exported by Blender 2,70a's COLLADA exporter.

-   JSON-C [link]

A C JSON parser I used to parse JSON model files with exported by the
Three.js exporter for Blender.

-   GLM [link]

Instead of creating my own linear algebra utility for graphics-related
mathematical computation on the CPU as I did in a previous framework
[link to portfolio page], I went with a third-party library, GLM [link].
I chose it because it aims to match the functionality of shader
languages.

-   Others

I had a lot of help from a number of web resources. Code borrowed from
these is identified and referenced in the source code and LICENSE
[link].

#### MODEL FORMATS

-   Three.js Model format 3 [link]

Due to my interest in WebGL, I decided to use a rigged model format
which used JSON, a data format popularly associated with Javascript
apps.

I didn't find the transformation data for the bones to be suitable, so I
switched to COLLADA part-way through development. Future versions will
likely use only COLLADA and perhaps implement an internal data storage
format for faster scene loading.

-   COLLADA [link]

A 3D model data storage format which supports rigging armatures and
armature animation.

#### PIPELINE TOOLS

-   Blender [link]

Blender is a fantastic free and open source 3D modelling and animation
suite. It also supports video editing and compositing. Needless to say
it's what I used to

-   GIMP [link]

An invaluable free and Photoshop-like image editor. Equally good for
quickly editing a texture, or for working on a large drawing or
painting.

Its brush tools are quite limited, so for better painting support look
at Krita.

-   Krita [link]

More painting-oriented than GIMP. Has great textured brushes which I
used to create the skin on the Green Dragon.

-   The Compressonator [link]

A tool for turning texture bitmaps into mipmap'd DDS textures.

#### COMPATIBILTY & SUPPORT

I developed the framework on a Linux system so it should compile as long
as you have the following requirements:

-   GLFW 3.0.4 (static library, but you can modify the Makefile for a
    dynamic library)
-   GLM 3.5
-   JSON-C library \>= 0.11
-   OpenGL implementation

The "Green Dragon" character resources (meshes and textures) are not
provided in the GitHub repo, but can be downloaded from the project
homepage [link]. Binaries containing the demo are available there also.

I can't get JSON-C to work in Windows so I am switching to RapidJSON. I
may even switch to an entirely COLLADA-based model format, removing the
need for JSON parsing.

### CHARACTERS

#### GREEN DRAGON

The demo portrays a green dragon from the shoulders-up. He/she has a
rigged neck and interacts with the mouse cursor when the player moves it
inside the window to demonstrate real-time interaction with the armature
system and its IK solver.

At all times, though especially noticeable when the player moves the
cursor outside of the window, the dragon displays a gentle swaying
motion to make the character feel more natural. This is a demonstration
of armature animation: the animation was created in Blender and exported
in the chosen model format and is played by the animation subsystem
(ClipRunner).

In future versions I want to explore procedural animation, which is so
far uncovered by the demo. The idle swaying motion could be generated
using procedural noise or a Markov chain, to give examples.

The character has two vertex morph targets in the form of facial
expressions: "Blink" and "Snap". In version 001 these are simply placed
on random timers (I suppose that counts as really basic procedural
animation), but they and more expressions could be made to be trigged by
interactive gameplay.

I am considering interactive gameplay elements to explore effects such
as said procedural animation and as particle effects: flocks of enemies
(perhaps fireflies) could swarm around the dragon's environment and the
player clicks to cause the dragon to breath fire on or snap at an enemy
in front of his/her snout.

Development on the green dragon design modelling and animation can be
found here:
