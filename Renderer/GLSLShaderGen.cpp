/* Title:		GLSLShaderGen.cpp																						         */
/* Author:	Markus Schlafli																							         */
/* Date:		31-Jul-2013																								           */
/*********************************************************************************/

#include "stdafx.h"
#include "GLSLShaderGen.h"

const std::string
GLSLShaderGen::GetShaderProgram() const {
	return "";
} 

const std::string
GLSLShaderGen::BasicVertexShader() {
	// #version 430
	//
	// uniform mat4 projectionMatrix;
	// uniform mat4 viewMatrix;
	// uniform mat4 modelMatrix;
	//
	// in vec3 position;
	// in vec3 colorIn;
	//
	// out vec3 colorPass;
	// 
	// void main() {
	//   colorPass = colorIn;
	//   gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( position, 1.0 );
	// }
	std::string shader = 
		"#version 430\n"
		"\n"
		"uniform mat4 projectionMatrix;\n"
		"uniform mat4 viewMatrix;\n"
		"uniform mat4 modelMatrix;\n"
		"\n"
		"in vec3 position;\n"
		"in vec3 colorIn;\n"
		"\n"
		"out vec3 colorPass;\n"
		"\n"
		"void main() {\n"
		"  colorPass = colorIn;\n"
		"  gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( position, 1.0 );\n"
		"}";

	return shader.c_str();
}

const std::string
GLSLShaderGen::BasicFragmentShader() {
	// #version 430
	//
	// in vec3 colorPass;
	//
	// out vec4 colorOut;
	// 
	// void main() {
	//   colorOut = vec4(colorPass, 1.0);
	// }
	std::string shader = 
		"#version 430\n"
		"\n"
		"in vec3 colorPass;\n"
		"\n"
		"out vec4 colorOut;\n"
		"\n"
		"void main() {\n"
		"  colorOut = vec4(colorPass, 1.0);\n"
		"}";

	return shader.c_str();
}

