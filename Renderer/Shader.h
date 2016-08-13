/*********************************************************************************/
/* Title:		Shader.h																										         */
/* Author:	Markus Schlafli																							         */
/* Date:		31-Jul-2013																								           */
/*********************************************************************************/

#ifndef SHADER_H
#define SHADER_H

/*
====================================================================================
	Shader abstract class
	
		Base class for any type of shader. 
====================================================================================
*/
class Shader {

public:
	bool usingExternalFiles() const { return _usingExternalFiles; }
	bool isValid() const { return _isValid; }

private:

protected:
	Shader() : _usingExternalFiles(false), _isValid(false) {}
	virtual ~Shader() {}

	bool _usingExternalFiles;
	bool _isValid;

};

#endif // SHADER_H