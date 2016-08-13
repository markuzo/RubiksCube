/* Title:		GLSLShaderGen.h																							         */
/* Author:	Markus Schlafli																							         */
/* Date:		31-Jul-2013																								           */
/*********************************************************************************/

#ifndef GLSLSHADERGEN_H
#define GLSLSHADERGEN_H

#include <string>
#include <tuple>
#include <vector>

/*
====================================================================================
	GLSLShaderGen static class

		Generates GLSL shader programs as text files.
====================================================================================
*/
class GLSLShaderGen {
	
public:
	// Constructor
	GLSLShaderGen() {}

	// Static predefined shader. Shouldn't exist because you
	// can't know what uniforms or input parameters will be given 
	// to the shader program
	static const std::string BasicVertexShader();
	static const std::string BasicFragmentShader();
	
	const std::string GetShaderProgram() const; 

private:

protected:

};

#endif // GLSLSHADERGEN_H