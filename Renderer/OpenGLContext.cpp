/*********************************************************************************/
/* Title:		OpenGLContext																								         */
/* Author:	Markus Schlafli																							         */
/* Date:		31-Jul-2013																								           */
/*********************************************************************************/

#include "stdafx.h"
#include "OpenGLContext.h"

#include <iostream>

// File-only functions
namespace {

}

OpenGLContext::OpenGLContext( HWND hwnd ) 
	: Context(hwnd), _projection(), _model(), _view(), _vao(), _vbo(), _camera(), _shader(nullptr), _cubeModel(nullptr) 
{
	createContext(); 
}

OpenGLContext::~OpenGLContext() {
	wglMakeCurrent( _hdc, 0 );
	wglDeleteContext( _hglrc );
	ReleaseDC( _hwnd, _hdc );

	if (_cubeModel != nullptr)
		delete _cubeModel;
	if (_shader != nullptr)
		delete _shader;
}

void
OpenGLContext::createContext() {
	_hdc = GetDC( _hwnd );

	bool passed = setupPFD();
	if ( !passed )
		return;

	_hglrc = wglCreateContext( _hdc );
	wglMakeCurrent( _hdc, _hglrc );

	if ( glewInit() != GLEW_OK )
		return;

	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	if ( wglewIsSupported( "WGL_ARB_create_context" ) != 1 )
		return;

	wglDeleteContext( _hglrc );

	_hglrc = wglCreateContextAttribsARB( _hdc, nullptr, attributes );
	bool makecurrentpassed = wglMakeCurrent( _hdc, _hglrc );
	
	_initialized = true;
}

bool
OpenGLContext::setupPFD() {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  
		1,                              
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,               
		PFD_TYPE_RGBA,                  
		32,                             
		0, 0, 0, 0, 0, 0,               
		0,                              
		0,                              
		0,                              
		0, 0, 0, 0,                     
		32,                             
		0,                              
		0,                              
		PFD_MAIN_PLANE,                 
		0,                              
		0, 0, 0,                        
	};

	int pixelFormat = ChoosePixelFormat( _hdc, &pfd );
	if ( pixelFormat == 0 )
		return false;

	bool passed = SetPixelFormat( _hdc, pixelFormat, &pfd );
	if ( !passed )
		return false;

	return true;
}

void
OpenGLContext::resizeWindow( int width, int height) {
	_windowHeight = height;
	_windowWidth = width;
}

void
OpenGLContext::render() {
	glViewport( 0, 0, (GLsizei)_windowWidth, (GLsizei)_windowHeight);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	_model = _cubeModel->modelMatrix;

	_shader->Bind();
	
	// bind uniforms
	int projectionLocation	= glGetUniformLocation( _shader->ShaderId(), "projectionMatrix" );
	int viewLocation		= glGetUniformLocation( _shader->ShaderId(), "viewMatrix" );
	int modelLocation		= glGetUniformLocation( _shader->ShaderId(), "modelMatrix" );

	glUniformMatrix4fv( projectionLocation, 1, GL_FALSE, &_projection[0][0] );
	glUniformMatrix4fv( viewLocation,		1, GL_FALSE, &_view[0][0]		);
	glUniformMatrix4fv( modelLocation,		1, GL_FALSE, &_model[0][0] 		);

	glBindVertexArray( _vao[0] );
	glDrawArrays( GL_TRIANGLES, 0, _cubeModel->getTriangleCount() );
	glBindVertexArray( 0 );

	_shader->Unbind();

	SwapBuffers( _hdc );
}

void
OpenGLContext::loadShaders() {
	if (_shader != nullptr)
		delete _shader;

	_shader = new GLSLShader();
	_shader->CreateShaderFromFile(GLSLShaderType::VERTEX,	"flat.vert");
	_shader->CreateShaderFromFile(GLSLShaderType::FRAGMENT, "flat.frag");
	_shader->InitializeProgram();
}

void
OpenGLContext::setupScene() {
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	loadShaders();

	float ratio = static_cast<float>( _windowWidth ) / static_cast<float>( _windowHeight );
	_projection = glm::perspective( 60.0f, ratio, 0.1f, 100.0f );
	
	//_view = glm::lookAt( _camera.pos, _camera.dir, glm::vec3( 0.0f, 1.0f, 0.0f ) );
	_view = glm::translate(_view, glm::vec3(5.0f, 0.0f, 0.0f));

	_cubeModel = new CubeModel();
	createCube();
}

void
OpenGLContext::createCube() {
	
	std::vector<float> vertices = _cubeModel->getPositions();
	std::vector<float> colors = _cubeModel->getColors();

	const int size = vertices.size();

	// vertex arrays contains 1 buffer object.
	// this buffer object contains the vertices and the colors
	glGenVertexArrays( 1, _vao );
	glBindVertexArray( _vao[0] );

	glGenBuffers( 2, _vbo );

	glBindBuffer( GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData( GL_ARRAY_BUFFER, size * sizeof( float ), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData( GL_ARRAY_BUFFER, size * sizeof( float ), &colors[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer( (GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	glEnableVertexAttribArray( 1 );

	glBindVertexArray( 0 );

}

void OpenGLContext::updateCube()
{
	std::vector<float> vertices = _cubeModel->getPositions();
	std::vector<float> colors = _cubeModel->getColors();

	const int size = vertices.size();

	// vertex arrays contains 1 buffer object.
	// this buffer object contains the vertices and the colors
	glBindVertexArray(_vao[0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), &colors[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

void
OpenGLContext::rotateView( int dx, int dy ) {
	glm::mat4 modelI = glm::inverse(_model);
	_view = modelI * _view;
	
	_view = glm::rotate(_view, (float) -dx, glm::vec3(0.0f, 1.0f, 0.0f));
	_view = glm::rotate(_view, (float) -dy, glm::vec3(1.0f, 0.0f, 0.0f));

	_view = _model * _view;
}

void
OpenGLContext::rotateView(glm::mat4 mat) {
	_view = mat * _view;
}

void 
OpenGLContext::rotateCube(char key) 
{
	switch (key)
	{
	case 'p':
		rotateView(10, 0);
		break;
	case 'o':
		rotateView(-10, 0);
		break;
	case 'x':
		_cubeModel->rotate(ROTATE::X);
		break;
	case 'X':
		_cubeModel->rotate(ROTATE::Xp);
		break;
	case 'z':
		_cubeModel->rotate(ROTATE::Z);
		break;
	case 'Z':
		_cubeModel->rotate(ROTATE::Zp);
		break;
	case 'y':
		_cubeModel->rotate(ROTATE::Y);
		break;
	case 'Y':
		_cubeModel->rotate(ROTATE::Yp);
		break;
	case 'l':
		_cubeModel->rotate(ROTATE::L);
		break;
	case 'L':
		_cubeModel->rotate(ROTATE::Lp);
		break;
	case 'r':
		_cubeModel->rotate(ROTATE::R);
		break;
	case 'R':
		_cubeModel->rotate(ROTATE::Rp);
		break;
	case 'u':
		_cubeModel->rotate(ROTATE::U);
		break;
	case 'U':
		_cubeModel->rotate(ROTATE::Up);
		break;
	case 'd':
		_cubeModel->rotate(ROTATE::D);
		break;
	case 'D':
		_cubeModel->rotate(ROTATE::Dp);
		break;
	case 'f':
		_cubeModel->rotate(ROTATE::F);
		break;
	case 'F':
		_cubeModel->rotate(ROTATE::Fp);
		break;
	case 'b':
		_cubeModel->rotate(ROTATE::B);
		break;
	case 'B':
		_cubeModel->rotate(ROTATE::Bp);
		break;
	case 'm':
		_cubeModel->rotate(ROTATE::M);
		break;
	case 'M':
		_cubeModel->rotate(ROTATE::Mp);
		break;
	case 'e':
		_cubeModel->rotate(ROTATE::E);
		break;
	case 'E':
		_cubeModel->rotate(ROTATE::Ep);
		break;
	case 's':
		_cubeModel->rotate(ROTATE::S);
		break;
	case 'S':
		_cubeModel->rotate(ROTATE::Sp);
		break;
	}

	updateCube();
}