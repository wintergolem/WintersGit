#ifndef  SHADER_BOOK_H
#define SHADER_BOOK_H

	#include "stdafx.h"
	#include <glew.h>
	#include <gl/gl.h>
	#include <gl/glu.h>
	#include "glfw.h"
	#include <string>
	#include <iostream>
	#include <fstream>
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtx/transform2.hpp>

	void initVertexShaderAttri(GLuint vaoHandle);
	void LinkShaders(GLuint programHandle, GLuint vertShader, GLuint fragShader );
	void CreateShaderObject(GLuint vertShader, char* direct);
	int init(GLuint vaoHandle);

#endif