//////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glfw.h>

#include "Utilities.h"
#include "Visualiser.h"
//#include "./Topic2.h"
#include "Topic2.h"

using namespace AIE;

////////////////////////////////////ADD FROM BOOK//////////////////////////////////
//glPatchParameteri( GL_PATCH_VERTICES, 4);
//glDrawArrays(GL_PATCHES, 0 , 4 );

//////////////////////////////////////////////////////////////////////////
void Init();
void Shutdown();
void Update();
void Draw();
void BuildTriangle(unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO);
//GLuint = unsigned int
GLuint g_ShaderID = 0;
GLuint g_TextureID = 0;

GLuint g_VAO = 0;	//vertex array object
GLuint g_VBO = 0;	//vertex buffer object
GLuint g_IBO = 0; //index buffer object

mat4 g_ProjectionMatrix;
mat4 g_ViewMatrix;
mat4 g_ModelMatrix;

mat4 g_CameraMatrix;

float timeHolder = 0.0f;
float g_fPhase = 0.0f;
float Inner = 2;
float Outer = 2;

//////////////////////////////////////////////////////////////////////////
int main()
{
	// start GLFW and open a window
	glfwInit();

	// get the desktop mode
	GLFWvidmode oMode;
	glfwGetDesktopMode(&oMode);

	// Select OpenGL 3.2 with a forward compatible core profile.
	/*glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 4 );
	glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 3 );
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);*/

	// open a window
	glfwOpenWindow(	1280, 720,	// resolution
					oMode.RedBits,oMode.GreenBits,oMode.BlueBits,0,	// bits per colour channel (RGBA)
					24,			// depth bits
					8,			// stencil bits
					GLFW_WINDOW);
	glfwSetWindowTitle("AIE OpenGL Window");

	// start GLEW
	if (glewInit() != GLEW_OK)
	{
		// OpenGL didn't start-up! shutdown GLFW and return an error code
		glfwTerminate();
		return -1;
	}

	// initialise our own stuff
	Init();

	do 
	{
		// update and draw our stuff to the back-buffer
		Update();		
		Draw();

		// bring the back-buffer to the monitor
		glfwSwapBuffers();

	} while ( glfwGetKey( GLFW_KEY_ESC ) == GLFW_RELEASE &&
			  glfwGetWindowParam( GLFW_OPENED ) != 0 );
	
	// clean up our stuff
	Shutdown();

	// shutdown GLFW
	glfwTerminate();

	// return success
	return 0;
}

//////////////////////////////////////////////////////////////////////////
void Init()
{
	// create a visualiser
	//Visualiser::Create();

	// setup matrices
	g_CameraMatrix.SetFrame( vec4(0,10,-10,1), vec4(0,-1,1,0), vec4(0,1,0,0));
	g_ProjectionMatrix.Perspective(PI/6, 1200/720.0f, 0.1f, 100);
//	g_ProjectionMatrix.Orthographic(1280,720,0.1f,100);
	g_ViewMatrix = g_CameraMatrix.ToViewMatrix();
	g_ModelMatrix = mat4(1,0,0,0,
						0,1,0,0,
						0,0,1,0,
						0,0,0,1);
	// load shader
	const char* aszInputs[] = { "Position",	"UV" };
	const char* aszOutputs[] = { "outColour" };
	g_ShaderID = LoadShader( 2, aszInputs, 1, aszOutputs,
		"./shaders/vertex.glsl",
		"./shaders/pixel.glsl","./shaders/Geometry.glsl");

	// build 2-triangle plane
	float fPlaneSize = 2.0f;
	//Build3DPlane(fPlaneSize,g_VAO,g_VBO,g_IBO);
	//BuildTriangle(g_VAO,g_VBO,g_IBO);
	Build3DMesh(g_VAO,g_VBO,g_IBO);

	// load texture
	g_TextureID = LoadTexture("./images/crate_sideup.png", GL_BGRA);

	// set matrix uniforms within the shaders
	GLuint ProjectionID = glGetUniformLocation(g_ShaderID,"Projection");
	GLuint ViewID = glGetUniformLocation(g_ShaderID,"View");
	GLuint ModelID = glGetUniformLocation(g_ShaderID,"Model");
	GLuint PhaseID = glGetUniformLocation(g_ShaderID,"Phase");
	GLuint TimeID = glGetUniformLocation(g_ShaderID,"Time");
	GLuint OuterID = glGetUniformLocation(g_ShaderID, "Outer");
	GLuint InnerID = glGetUniformLocation(g_ShaderID, "Inner");

	glUniformMatrix4fv(ProjectionID, 1, false, g_ProjectionMatrix);
	glUniformMatrix4fv(ViewID, 1, false, g_ViewMatrix);
	glUniformMatrix4fv(ModelID, 1, false, g_ModelMatrix);

	// set the texture to use slot 0 in the shader
	GLuint texUniformID = glGetUniformLocation(g_ShaderID,"diffuseTexture");
	glUniform1i(texUniformID,0);
	
	// set clear colour
	glClearColor(0.25f,0.25f,0.25f,1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// start our timer
	AIE::ResetTimer();
}

//////////////////////////////////////////////////////////////////////////
void Update()
{
	float fDeltaTime = GetDeltaTime();

	// update movement of the camera's world transform
	FreeMovement(fDeltaTime,g_CameraMatrix,10);

	// example rotated world transform for the crate plane
	static float sfTimer = 0;
	sfTimer += fDeltaTime;
	timeHolder = glfwGetTime();

	g_fPhase += fDeltaTime;
	if(g_fPhase == 0){
		g_fPhase += fDeltaTime;
		//g_fPhase = -PI;
	}
	
}

//////////////////////////////////////////////////////////////////////////
void Draw()
{
	// clear the backbuffer to our clear colour and clear the depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set active shader
	glUseProgram(g_ShaderID);

	// convert camera's world matrix to a view matrix
	g_ViewMatrix = g_CameraMatrix.ToViewMatrix();

	// set current transforms in the shader
	GLuint ViewID = glGetUniformLocation(g_ShaderID,"View");
	glUniformMatrix4fv(ViewID, 1, false, g_ViewMatrix);

	GLuint ModelID = glGetUniformLocation(g_ShaderID,"Model");
	glUniformMatrix4fv(ModelID, 1, false, g_ModelMatrix);

	
	GLuint PhaseID = glGetUniformLocation(g_ShaderID,"Phase");
	glUniform1f(PhaseID,g_fPhase );

	GLuint TimeID = glGetUniformLocation(g_ShaderID,"Time");
	glUniform1f(TimeID, timeHolder );

	// set active texture, bind the crate quad's buffers and draw it
	//g_ModelMatrix.row3 = vec4(-4,0,0,1);
	//glUniformMatrix4fv(ModelID, 1, false, g_ModelMatrix);
	glBindVertexArray(g_VAO);
	glDrawElements(GL_PATCHES, 10, GL_UNSIGNED_INT, 0);
}

//////////////////////////////////////////////////////////////////////////
void Shutdown()
{
	// destroy buffers
	Visualiser::Destroy();
	glDeleteTextures(1,&g_TextureID);
	glDeleteVertexArrays(1, &g_VAO);
	glDeleteBuffers(1, &g_VBO);
	glDeleteBuffers(1, &g_IBO);
	glDeleteShader(g_ShaderID);
}

void BuildTriangle(unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO)
{
	float size = 5.0f;
	AIE::Vertex aoVertices[3];
	aoVertices[0].position = vec4 (0,size,0,1);
	aoVertices[1].position = vec4 (-1*size,0,0,1);
	aoVertices[2].position = vec4 (size,0,0,1);

	unsigned int auiIndex[3] = { 0,1,2};

	// create and bind buffers to a vertex array object
	glGenBuffers(1, &a_ruiVBO);
	glGenBuffers(1, &a_ruiIBO);
	glGenVertexArrays(1, &a_ruiVAO);

	glBindVertexArray(a_ruiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, a_ruiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_ruiIBO);

	glBufferData(GL_ARRAY_BUFFER, 4* sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), auiIndex, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//index size type normalized stride pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	// unbind vertex array
	glBindVertexArray(0);
}

void Build3DTri(unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO)
{
	float size = 5.0f;
	AIE::Vertex aoVertices[4];
	aoVertices[0].position = vec4 (0,size,0,1);
	aoVertices[1].position = vec4 (-1*size,0,0,1);
	aoVertices[2].position = vec4 (size,0,0,1);

	unsigned int auiIndex[3] = { 
		0,1,2

	
	};

	// create and bind buffers to a vertex array object
	glGenBuffers(1, &a_ruiVBO);
	glGenBuffers(1, &a_ruiIBO);
	glGenVertexArrays(1, &a_ruiVAO);

	glBindVertexArray(a_ruiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, a_ruiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_ruiIBO);

	glBufferData(GL_ARRAY_BUFFER, 4* sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), auiIndex, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//index size type normalized stride pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	// unbind vertex array
	glBindVertexArray(0);
}