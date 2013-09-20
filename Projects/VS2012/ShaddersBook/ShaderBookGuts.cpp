

#include "ShaderBook.h"

using std::cout;
using std::string;
using glm::vec4;
using glm::mat4;

void initVertexShaderAttri(GLuint vaoHandle)
{
	
	//population VBA
	float positionData[] = {
		-0.8f, -.8f, 0.0f,
		0.8f, -.8f, 0.0f,
		0.0f, -.8f, 0.0f,
	};
	float colorData[] = {
		1.0f, 0.0f, 0.0f,
		0.8f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	};

	//create the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	//populate position buffers
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9* sizeof(float), positionData, GL_STATIC_DRAW);

	//populate color buffers
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9* sizeof(float), colorData, GL_STATIC_DRAW);

	//create and setup the vertex array
	glGenVertexArrays( 1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//Enable the vertex attribute arrays
	glEnableVertexAttribArray(0);	//Vertex position
	glEnableVertexAttribArray(1);	//Vertex color

	//Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(1 , 3 , GL_FLOAT , GL_FALSE , 0 , 
		(GLubyte *)NULL ); //not treated like normal pointer, use if passed array has elements before first elements

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
	////render function************************************

	//glBindVertexArray(vaoHandle);
	//glDrawArrays(GL_TRIANGLES , 0 , 3 );
}

void LinkShaders(GLuint programHandle, GLuint vertShader, GLuint fragShader )
{


	//if failed, tell user
	if( 0 == programHandle )
	{
		cout<< "Error creating program object. \n";
	}


	//create program attributes ; must be done before linking program
		//bind index 0 to the shader input varible vertexposition
	glBindAttribLocation(programHandle, 0, "VertexPostion");
	//bind index 1 to the shader input variable vertexColor
	glBindAttribLocation(programHandle, 1, "vertexColor");

	//attach shaders to object
	glAttachShader( programHandle, vertShader );
	glAttachShader( programHandle, fragShader );

	//initialize data - my function
	//initVertexShaderAttri(); - moved to init after linking

	//Link the program
	glLinkProgram( programHandle );

	//verify link status
	GLint status;
	glGetProgramiv( programHandle, GL_LINK_STATUS, &status );
	if( GL_FALSE == status)
	{
		cout << "Failed to link shader shader program\n" ;

		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);

		if(logLen >0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			cout << "Program log: \n"<<log;
			free(log);
		}
	}
	else //if linking success, install program to OpenGl pipeline
	{
		glUseProgram( programHandle );
	}
}

void CreateShaderObject(GLuint shaderObject, char* direct)
{
	
	if (0 == shaderObject )
	{
		cout << "Error creating "<<direct<<" shader.\n";
		exit(1);
	}

	//copy source code
	
	std::ifstream shader_file(direct, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
	const GLchar *shaderCode = str.c_str();
	const GLchar *codeArray[] = {shaderCode};
	glShaderSource( shaderObject, 1, codeArray, NULL );

	//compile shader
	glCompileShader( shaderObject);

	//verify compilation
	GLint result;
	glGetShaderiv( shaderObject, GL_COMPILE_STATUS, &result );
	if( GL_FALSE == result)
	{
		//if shader compilation failed
			//tell user and written failure to log
		cout<< direct <<" shader compilation failed! \n";
		GLint logLen;
		glGetShaderiv( shaderObject, GL_INFO_LOG_LENGTH, &logLen );

		if(logLen >0)
		{
			char *log = (char *)malloc(logLen);

			GLsizei written;
			glGetShaderInfoLog(shaderObject, logLen, &written, log);

			cout<< "Shader log: \n " <<log;
			free(log);
		}
	}
}

int init(GLuint vaoHandle)
{

	//Initialise GLFW
	if(!glfwInit() )
	{
		cout<<"Failed to initialize glfw\n";
	}

	// Select OpenGL 3.2 with a forward compatible core profile.
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    if( !glfwOpenWindow( 512, 384, 0,0,0,0, 32,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return -1;
    }
	
   //initalise AFTER gl context
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -3;
	}
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	//create shader object param = type of shader
	GLuint vertShader = glCreateShader( GL_VERTEX_SHADER);
	CreateShaderObject(vertShader, "basic.vert");
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	CreateShaderObject(fragShader, "FragShader.Frag" );

		//create program handle, link shaders, an attach to opengl
	GLuint programHandle = glCreateProgram();
	LinkShaders(programHandle, vertShader, fragShader);
	initVertexShaderAttri( vaoHandle);
	
	return 0;
}