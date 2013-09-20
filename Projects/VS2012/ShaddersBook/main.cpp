
#include "ShaderBook.h"


using std::cout;
using std::string;
using glm::vec4;
using glm::mat4;

//////OpenGL Globals
mat4 g_ProjectionMatrix;
mat4 g_ViewMatrix;
mat4 g_ModelMatrix;
mat4 g_CameraMatrix;

const char* loadShaderAsString(const char* file, string st)
{
	//doesn't ever return anything useful   unknown reason   just copy code
    std::ifstream shader_file(file, std::ifstream::in);
    std::string str((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
	st = str;
    return st.c_str();
}






int init()
{
	 // Initialise GLFW
	 if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -2;
    }

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
	//create shader object para = type of shader
	GLuint vertShader = glCreateShader( GL_VERTEX_SHADER);
	CreateShaderObject(vertShader, "basic.vert");
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	CreateShaderObject(fragShader, "FragShader.Frag" );

		//creating shader object
	GLuint programHandle = glCreateProgram();
	LinkShaders(programHandle, vertShader, fragShader);
	return 0;
}



int notmain()
{
   int returnValue = init();;
   if(returnValue < 0 ) return returnValue;
	
	glfwSetWindowTitle( "Shader Book" );

	// Ensure we can capture the escape key being pressed below
    glfwEnable( GLFW_STICKY_KEYS );

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    do{
		// Draw nothing, see you in tutorial 2 !

        // Swap buffers
        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
           glfwGetWindowParam( GLFW_OPENED ) );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
