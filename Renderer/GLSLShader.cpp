/*********************************************************************************/
/* Title:		GLSLShader.cpp																							         */
/* Author:	Markus Schlafli																							         */
/* Date:		31-Jul-2013																								           */
/*********************************************************************************/

#include "stdafx.h"
#include "GLSLShader.h"
#include "GLSLShaderGen.h"
#include "Utilities.h"
#include <iostream>
#include <climits>

GLSLShader::GLSLShader() 
	: Shader(),
		_attribLocations(),
		_vertexShader( UINT_MAX ),
		_tessCtrlShader( UINT_MAX ),
		_tessEvalShader( UINT_MAX ),
		_geometryShader( UINT_MAX ),
		_fragmentShader( UINT_MAX )
{
}

GLSLShader::~GLSLShader() {
	if ( _vertexShader != UINT_MAX ) {
		glDetachShader( _shaderId, _vertexShader );
		glDeleteShader( _vertexShader );
	}

	if ( _tessCtrlShader != UINT_MAX ) {
		glDetachShader( _shaderId, _tessCtrlShader );
		glDeleteShader( _tessCtrlShader );
	}

	if ( _tessEvalShader != UINT_MAX ) {
		glDetachShader( _shaderId, _tessEvalShader );
		glDeleteShader( _tessEvalShader );
	}

	if ( _geometryShader != UINT_MAX ) {
		glDetachShader( _shaderId, _geometryShader );
		glDeleteShader( _geometryShader );
	}

	if ( _vertexShader != UINT_MAX ) {
		glDetachShader( _shaderId, _fragmentShader );
		glDeleteShader( _fragmentShader );
	}

	glDeleteProgram( _shaderId );
}

void 
GLSLShader::CreateShaderFromFile( GLSLShaderType shaderType, const char * filePath ) {
	// Not implemented yet
	// Using standard files instead
	std::string dataString = Utilities::convertFileToString(filePath);
	const char * data = dataString.c_str();
	
	unsigned int * shader;

	switch (shaderType) {
	case GLSLShaderType::VERTEX:
		shader = &_vertexShader;
		break;
	case GLSLShaderType::FRAGMENT:
		shader = &_fragmentShader;
		break;
	case GLSLShaderType::GEOMETRY:
		shader = &_geometryShader;
		break;
	case GLSLShaderType::TESS_CTRL:
		shader = &_tessCtrlShader;
		break;
	case GLSLShaderType::TESS_EVAL:
		shader = &_tessEvalShader;
		break;
	}

	*shader = glCreateShader(shaderType);
	glShaderSource(*shader, 1, &data, 0);
	glCompileShader(*shader);

	bool valid = IsValidShader(*shader);
	if (!valid)
		return;

	// Set the locations for the attributes
	// TODO: these should be dynamically changeable depending on the shader code
	_attribLocations[0] = "position";
	_attribLocations[1] = "colorIn";
}

void
GLSLShader::CreateStandardShaders() {

	const std::string vertexShaderString = GLSLShaderGen::BasicVertexShader();
	const std::string fragmentShaderString = GLSLShaderGen::BasicFragmentShader();
	const char * vertexShaderText = vertexShaderString.c_str();
	const char * fragmentShaderText = fragmentShaderString.c_str();

	_vertexShader = glCreateShader( GL_VERTEX_SHADER );
	_fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( _vertexShader, 1, &vertexShaderText, 0 );
	glShaderSource( _fragmentShader, 1, &fragmentShaderText, 0 );

	glCompileShader( _vertexShader );
	glCompileShader( _fragmentShader );

	bool isValidVertex = IsValidShader( _vertexShader );
	bool isValidFrag = IsValidShader( _fragmentShader );

	if ( !isValidVertex || !isValidFrag ) {
		return;
	}

	// Set the locations for the attributes
	_attribLocations[0] = "position";
	_attribLocations[1] = "colorIn";

}

bool 
GLSLShader::InitializeProgram() {
	_shaderId = glCreateProgram();
	
	// Attach the shaders to the program
	if ( _vertexShader != UINT_MAX )
		glAttachShader( _shaderId, _vertexShader );
	if ( _tessCtrlShader != UINT_MAX )
		glAttachShader( _shaderId, _tessCtrlShader );
	if ( _tessEvalShader != UINT_MAX )
		glAttachShader( _shaderId, _tessEvalShader );
	if ( _geometryShader != UINT_MAX )
		glAttachShader( _shaderId, _geometryShader );
	if ( _fragmentShader != UINT_MAX )
		glAttachShader( _shaderId, _fragmentShader );
	
	// Bind the attributes to their correct location
	for ( auto iter = _attribLocations.begin(); iter != _attribLocations.end(); iter++ ) {
		int attribLocation = (*iter).first;
		std::string attibute = (*iter).second;

		glBindAttribLocation( _shaderId, attribLocation, attibute.c_str() );
	}

	// Link program
	glLinkProgram( _shaderId );

	// Check validity of program
	if ( IsValidProgram( _shaderId ) ) {
		_isValid = true;
	} else {
		throw std::exception("Shader program not valid.");
	}
}

bool 
GLSLShader::IsValidShader( GLuint shader ) {
	const unsigned int size = 512;
	char buffer[size];
	memset( buffer, 0, size );
	GLsizei length = 0;
	glGetProgramInfoLog( shader, size, &length, buffer );
	if ( length > 0 ) {
		std::cerr << "Shader " << shader << ". Error: " << buffer << std::endl;
		return false;
	}
	return true;
}

bool
GLSLShader::IsValidProgram( GLuint program ) {
	const unsigned int size = 512;
	char buffer[size];
	memset( buffer, 0, size );
	GLsizei length = 0;
	glGetProgramInfoLog( program, size, &length, buffer );
	if ( length > 0 ) {
		std::cerr << "Program " << program << ". Error: " << buffer << std::endl;
	}
	glValidateProgram( program );
	GLint status;
	glGetProgramiv( program, GL_VALIDATE_STATUS, &status );
	if ( status == GL_FALSE ) {
		std::cerr << "Error validating shader " << program << std::endl;
		return false;
	}
	return true;
}

void
GLSLShader::Bind() {
	glUseProgram( _shaderId );
}

void
GLSLShader::Unbind() {
	glUseProgram( 0 );
}

