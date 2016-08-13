/*********************************************************************************/
/* Title:		OpenGLContext.h																							         */
/* Author:	Markus Schlafli																							         */
/* Date:		31-Jul-2013																								           */
/*********************************************************************************/

#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

// OpenGL
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>

// Math
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/projection.hpp>

#include "GLSLShader.h"
#include "Context.h"
#include "Camera.h"
#include "CubeModel.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

/*
====================================================================================
	OpenGLContext class 
	
		Class that contains the context if we are using the OpenGL graphics API.
====================================================================================
*/
class RENDERER OpenGLContext : public Context {

public:
	OpenGLContext( HWND hwnd );
	virtual ~OpenGLContext();

	virtual void resizeWindow( int width, int height);
	virtual void render();
	virtual void rotateView( int dx, int dy );
	virtual void rotateView(glm::mat4 mat);
	virtual void rotateCube(char key);
	virtual void loadShaders();

	virtual void setupScene();

private:
	HGLRC _hglrc;
	HDC _hdc;

	glm::mat4 _model;
	glm::mat4 _view;
	glm::mat4 _projection;

	CubeModel * _cubeModel;

	unsigned int _vao[1];
	unsigned int _vbo[2];
	
	GLSLShader * _shader;

	Camera _camera;

	void createContext();
	
	bool setupPFD();

	// Helper for scene setup.
	void createCube();
	void updateCube();
protected:


};

#endif // OPENGLCONTEXT_H