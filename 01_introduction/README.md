# Introductory scene

The first introductory scene is available in this directory
- Each exercise is described as a unique program (with its own main) defined in the src/ directory. 
- A CMakeLists.txt is associated to each program for its compilation (+ a Makefile for Unix systems).
- You need to adapt the path of the [compilation instructions](https://graphicscomputing.fr/cgp/compilation/content/01_compilation/index.html) to the directory of the CMakeLists.txt for each exercise.

Once executed, you should observe a basic scene as seen below with rotating cylinder, cube, and curve (some details may vary).

[Example](01_introduction.gif)

- Note that the source code of the scene is fully described in the files src/main.cpp and src/scene.cpp (and src/scene.hpp). 
- main.cpp file contains common elements through all the exercises (general set up of the scene, animation loop, basic handling of the keyboard/mouse input). Usually you won't have to edit the main.cpp.
- scene.cpp and (scene.hpp) describe the specific content of the scene exercise. You may have to do your edit in this file.

## Information on the library (only general information, no exercise)

CGP library provides a set of functionalities to ease 3D graphics programming such as:
- Structure for 3D vectors and matrices (and 2D, 4D).
- Object structure ready to be displayed with OpenGL (Meshes, lines, etc) and their associated shaders.
- Easy and secured storage for buffers of elements.
- 3D scene manipulation: Mouse controlled camera.

Structures and functions of the library are accessible in the namespace cgp::
The library tries to be as lightweight as possible. It is written to ease 3D graphics programming, and to not hide underlying working state. Although this is not required, you should be able to understand every part of the code.
It keeps in particular the following principles
- Minimalistic code written to be simple to understand (not necessarily the most generic nor efficient).
- Objects are written to be lightweight and with minimal hidden states - You can directly interact with object attributes.
- The code and functionalities remain close to basic C++/OpenGL - You can use direct OpenGL calls with the library. You should also keep in mind the way that OpenGL works to avoid unexpected display.

Code editor
- The library contains multiples files. Make sure you use a sufficiently advanced (or well parameterized) IDE to have C++ code completion (in particular complete function names, display expected arguments and types, objects arguments, etc), efficiently switch between files and jump to the signature and code of any function and object.
- You may use for instance:
    - On Linux and MacOS: VS Code. You may open the scene project using the file vscode.code-workspace provided in each scene directory.
    - On Windows: Visual Studio. You can generate a visual studio project using the provided scripts/windows_compile_visual.bat.

### Use of the code library and program structure (the exercise starts here)

First Observe the file main.cpp. You should recognize the general organization of the program, in particular the setup stage, and the animation loop stage. Note where the two main methods scene.initialize() and scene.display_frame() are called, and their implementation in the file scene.cpp.
In the following you will only modify code in the file scene.cpp (and scene.hpp).

- Change the rotation (parameterized by and axis and an angle) from some of the object within the scene in the function display_scene.
    - The function rotation_transform::from_axis_angle(axis_of_rotation, angle_of_rotation); generate a structure cgp::rotation_transform from a given 3D axis (cgp::vec3) and scalar angle (float) in radians.
    - The structure cgp::vec3 implements a model of 3D vector with (x,y,z) coordinates.
        You can apply most of the basic operations between vec3 and mat3 using mathematical operators (+,-,*,/).
    - The structure rotation_transform allowing to generate and manipulate a rotation. The rotation is stored internaly as a quaternion, and can be manipulated similarily to a matrix when applied on a vector.
    The rotation is stored in the variable "model" of a mesh_drawable structure. model stores an affine transformation which is used as uniform variable when the shape is displayed.

Note that there is two type of "mesh" structures in use

- `cgp::mesh` storing buffer on data (per vertex: position, normal, uv, color, and triangle connectivity) on CPU
    This structure allows to conveniently access to all the data defining a mesh from the C++ code. However these data are not on the GPU, so a mesh cannot be directly displayed.

- `cgp::mesh_drawable` storing VBOs associated to these buffer once sent on the GPU memory (in the sub-structure mesh_drawable_gpu_data) as well as its VAO. The structure also stores uniform parameters that are sent to the shader at every draw call. A default shader and texture id are also be stored with the structure.
    This structure only stores the index corresponding to elements on GPU. You cannot modify individual per-vertex elements easily from this structure.

- Remark: a mesh_drawable can be automatically generated/initialized from a mesh structure in calling the construction mesh_drawable.initialize_data_on_gpu(meshName). However, you cannot create a mesh from a mesh_drawable.

## Adding a sphere
In this first part we display a new sphere to the scene.

- Add the class variable to scene_structure class (in the file scene.hpp)
```
cgp::mesh_drawable sphere; 
```
In this example, cgp:: is optional as the header file already indicated "using cgp::mesh_drawable;" in its beginning.

- Initialize this variable in the file scene.cpp in the initialize() function as a sphere of radius 0.5
```
mesh sphere_mesh = mesh_primitive_sphere(0.5f);
sphere.initialize_data_on_gpu(sphere_mesh);
```

    (or variant: simply in one line without explicitely storing the mesh variable)

```
sphere.initialize_data_on_gpu(mesh_primitive_sphere(0.5f));
```
- Display this sphere in the display_frame() function with the following code
```
draw(sphere, environment);
```
- Compile, run, and observe the sphere in the 3D scene.

    - Note: You can set geometric transformation using the uniform properties of the mesh_drawable structure (reminder: uniform are parameters passed to the shaders).
        sphere.model.translation/rotation/scaling = ... (see example on the other displayed shapes)
    - Note also that unless specified explicitely, the default shader associated to the mesh_drawable structure is used.
    The three trasformations: translation, rotation, and scaling are simply variables that you can write on. By default, their values are respectively, (0,0,0), the identity, and 1. The content of the variables are used (as uniform values in the shader) when the draw call is used.

- Add the following line before calling draw on the sphere and observe that the color (as well as any other uniform parameter) can be changed through time in modifying its value at every frame.
```
sphere.material.color = vec3(1+std::cos(time), 1+std::sin(time), 2.0)/2.0f;
```

## Checkbox interface
We will now add a GUI checkbox (button that can be checked) to activate/deactivate the wireframe display of the sphere.

- Add the following variable in the definition of the structure struct gui_parameters (in the file scene.hpp) to store a boolean state indicating when the wireframe should be displayed or not
```
bool display_wireframe = false;
```
- In the function display_gui() (in scene.cpp) add a Checkbox (handled by ImGui library) and link it (through its adress) to the variable display_wireframe in adding this line of code
```
ImGui::Checkbox("Wireframe", &gui.display_wireframe);
```
In the running code, the checkbox should appear. Every time you select/unselect it, the value of the variable display_wireframe change from true to false but doesn't change yet anything in the 3D display.

The last step consists in adapting the behavior of the display.

- Add the following code in the display_frame function
```
  if (gui.display_wireframe)
    draw_wireframe(sphere, environment, { 1,1,0 });
```
Check that you can now interactively display the wireframe representation of the sphere.

## Deforming the vertices of a surface

The following example show a case where mesh position are modified in the C++ code and need to be updated at each frame.

- Add the following elements as variables of the object scene_structure
```
  mesh shape;
  numarray<vec3> initial_position;
  mesh_drawable shape_visual;
```
`shape` is use to store (on CPU memory) the current state of the deformed mesh
`initial_position` is use to store (on CPU memory) the initial position of each vertex of the shape
`shape_visual` is use to display the deformed shape.

- Initialise these variables to a uniformly sampled grid shape in the initialize function with the following code
```
int N = 100;
shape = mesh_primitive_grid({ 0,0,0 }, { 1,0,0 }, { 1,1,0 }, { 0,1,0 }, N, N);
initial_position = shape.position;
shape_visual.initialize_data_on_gpu(shape);
shape_visual.material.color = { 0.6f, 0.6f, 0.9f };
```
- Call the drawing of this surface at the end of the display_frame() function
```
draw(shape_visual, environment);
if (gui.display_wireframe)
    draw_wireframe(shape_visual, environment, { 0,0,0 });
```
- Add a new method to the scene class computing the deformation of the surface in
    - Adding the following content in the file scene.cpp
    - Adding the signature of this new method (void evolve_shape()) in the file scene.hpp
    - Call this method in the display() function.
```
void scene_structure::evolve_shape(float time)
{
    size_t const N = initial_position.size();
    for(size_t k=0; k<N; ++k)
    {
        vec3 const& p0 = initial_position[k];
        vec3& p        = shape.position[k];
        p.z = p0.z + 0.1f * std::cos(10 * p.x + 4 * time);
    }
}
```
While the structure mesh is updated by the function, its visual representation (and the associated VBO) are not therefore no change is visible when the code is run. An explicit update of the data stored on the GPU is necessary.
- To this end add the following code after your call to evolve_shape
```
shape_visual.vbo_position.update(shape.position);
```
This function send again to the GPU the position from the buffer. Note that the update doesn't reallocate any VBO (it is therefore faster that generating a new object) and assume that the size of the buffer remains constant.

Observe that the surface is now deformed, but its color remains uniform despite the undulation. Indeed, the shader still use the initial normals of the planar grid, and doesn't take into account the change of geometry in the shading.
- Normals of the surface can be recomputed and updated to the GPU using the following code
```
// Recompute normals on the CPU (given the position and the connectivity currently in the mesh structure)
shape.normal_update();
// Send updated normals on the GPU
shape_visual.vbo_normal.update(shape.normal);
```

Observe that the surface is now correctly updated through its deformation.
- Change in the evolve_shape function the deformation to the following one using Perlin noise, make sure you understand the result.

```
float const dz = 0.3f * noise_perlin({ p0.x + 0.2f * time, p0.y, 0 }, 2) + 0.015f * noise_perlin({ 4 * p0.x, 4 * p0.y, time }, 2);
p = p0 + vec3(0, 0, dz);
```

## Texture
Textures images can also be associated to surface
Note that the per-vertex uv coordinates need to be defined correctly to get the mapping of the image on the surface.

- Update the initialization of your shape variable with the following two lines code and observe that your surface should now be textured
```
// Reset the color of the shape to white (only the texture image will be seen)
shape_visual.material.color = {1,1,1};

// Load the image and associate the texture id to the structure
shape_visual.texture.load_and_initialize_texture_2d_on_gpu(project::path+"assets/squirrel.jpg");
```
- Note: The variable project::path is automatically filled in the beginning of the program and tries to adapt the relative path to find the assets directory. If you omit it, you need to run your executable program from the root directory (where you can find the assets/ and shaders/ directory).
