# Rubik's Cube

This side-project was an ambitious attempt at creating a OS-agnostic and graphics API-agnostic Rubik's Cube application.

Initial goals:
- OpenGL & Direct3D graphics APIs
- Linux & Windows without glfw/glut
- Interactive cube manipulation
- Automatic solvers

Achieved goals:
- OpenGL rendering (only phong shading, no skybox, no texturing)
- Windows OS only
- Interative cube manipulation

# Third Party
- GLM is required for the GL maths. 
- GLEW is required to avoid crazy function pointers.

The project requires that GLEW is in the Windows system folder and that GLM is in the default VC++ include directories in at the VS install location. Both of these things are terribly stupid; it was done as a shortcut to avoid having third party folder duplicates under every project, but now it's just a convenience so that this stuff doesn't have to be uploaded.
