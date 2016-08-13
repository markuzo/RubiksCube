/*********************************************************************************/
/* Title:		GLSLShader.h																								         */
/* Author:	Markus Schlafli																							         */
/* Date:		31-Jul-2013																								           */
/*********************************************************************************/

#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include "Shader.h"
#include <GL/glew.h>
#include <GL/GL.h>

#include <map>
#include <string>

/*
====================================================================================
	GLSLShaderType Enum

		Holds all different types of GLSL shader types.
====================================================================================
*/
enum GLSLShaderType {
	VERTEX = GL_VERTEX_SHADER,
	TESS_CTRL = GL_TESS_CONTROL_SHADER,
	TESS_EVAL = GL_TESS_EVALUATION_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};

/*
====================================================================================
	GLSLShader class
		
		Contains all the logic for creating shader programs in OpenGL using the GLSL 
		language.
====================================================================================
*/
class GLSLShader : public Shader {

public:
	GLSLShader();
	~GLSLShader();

	void CreateShaderFromFile( GLSLShaderType shaderType, const char * filePath );
	
	// Basic shader program that will use the vertex and fragment shaders only.
	// It requires MVP matrices and position and color vectors to be passed in. 
	// The shader program will just output the positions and their colors.
	void CreateStandardShaders();

	// Create the entire shader program using the combination of the 
	// previously given shader programs.
	bool InitializeProgram();

	void Bind();
	void Unbind();

	unsigned int ShaderId() const { return _shaderId; }

private:

	unsigned int _shaderId;

	// ids of different shader types in GLSL
	unsigned int _vertexShader;
	unsigned int _tessCtrlShader;
	unsigned int _tessEvalShader;
	unsigned int _geometryShader;
	unsigned int _fragmentShader;

	std::map<int,std::string> _attribLocations;

protected:

	virtual bool IsValidShader( GLuint shader );
	virtual bool IsValidProgram( GLuint program );

};

#endif // GLSLSHADER_H