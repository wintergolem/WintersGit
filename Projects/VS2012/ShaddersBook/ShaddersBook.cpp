// ShaddersBook.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <glew.h>
#include <gl\gl.h>
#include <gl/glu.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


void initExample()
{
	//load glew and check that it works
	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		std::cout << "Error initializing GLEW: "<<glewGetErrorString(err);
	}
	//check for individual varibles like this
	if( ! GLEW_ARB_vertex_program )
	{
		std::cout << "ARB_Vertex_program is missing! \n";
	}
}

void GLMExample()
{
	//resprendes a position
	glm::vec4 position = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f);
	//setting the view
	glm::mat4 view		=	glm::lookAt( glm::vec3(0.0 , 0.0 , 5.0),
										glm::vec3(0.0 , 0.0 , 5.0),
										glm::vec3(0.0 , 0.0 , 5.0) );
	//matrix idenity
	glm::mat4 model = glm::mat4(1.0f);
	//matrix multiply
	model = glm::rotate( model, 90.0f, glm::vec3(0.0f, 1.0f,0.0) );
	glm::mat4 mv = view * model;
	glm::vec4 transformed = mv * position;

	//////////////////////glm as input to OpenGL//////////////////////////////////////
	//glm::mat4 proj = glm::perspective( viewAngle, aspect, nearDist, farDist );
	//glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0] );
}

void CheckGLVersion()
{
	//get gl version
	const GLubyte *renderer = glGetString( GL_RENDERER ); //driver info and video card info
	const GLubyte *vendor = glGetString( GL_VENDOR ); // driver info and company who implementated opengl
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glsVersion = glGetString( GL_SHADING_LANGUAGE_VERSION); //which version of glsl is available on this machine

	//glGetInterger is only available OpenGL 3.0 +
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	//print GL version
	printf("GL Vendor    : %s\n", vendor);
	printf("GL Renderer    : %s\n", renderer);
	printf("GL Version (string)    : %s\n", version);
	printf("GL Version (integer)    : %d.&d\n", major, minor);
	printf("GL Version:   : %s\n", glsVersion);	

	//list gl extensions
	GLint nExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
	for( int i =0; i< nExtensions; i++)
		printf("%s\n", glGetStringi( GL_EXTENSIONS, i ) );
}

