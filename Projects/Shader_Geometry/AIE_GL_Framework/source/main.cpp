//////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glfw.h>

#include "Utilities.h"
#include "Visualiser.h"

using namespace AIE;

//////////////////////////////////////////////////////////////////////////
void Init();
void Shutdown();
void Update();
void Draw();
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

//////////////////////////////////////////////////////////////////////////
int main()
{
	// start GLFW and open a window
	glfwInit();

	// get the desktop mode
	GLFWvidmode oMode;
	glfwGetDesktopMode(&oMode);

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
	Visualiser::Create();

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
		"./shaders/pixel.glsl", "./shaders/geometry.glsl");

	// build 2-triangle plane
	float fPlaneSize = 2.0f;
	Build3DPlane(fPlaneSize,g_VAO,g_VBO,g_IBO);

	// load texture
	g_TextureID = LoadTexture("./images/crate_sideup.png", GL_BGRA);

	// set matrix uniforms within the shaders
	GLuint ProjectionID = glGetUniformLocation(g_ShaderID,"Projection");
	GLuint ViewID = glGetUniformLocation(g_ShaderID,"View");
	GLuint ModelID = glGetUniformLocation(g_ShaderID,"Model");
	GLuint TimeID = glGetUniformLocation(g_ShaderID, "Time");

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

	phase += fDeltaTime;
	if(phase >= PI)
		phase = -PI;

	//g_ModelMatrix.RotateY(sfTimer);

	// clear the visualiser so we can add shapes this frame
	Visualiser::Get()->Clear();
	
	//////////////////////////////////////////////////////////////////////////
	// example visualiser use
	// line shapes
	Visualiser::Get()->AddDisk(vec4(-4,-1,4,1),2,8,vec4(0,1,0,0));
	Visualiser::Get()->AddHermiteSpline(vec4(-2,0,-3,1),vec4(2,0,-3,1),vec4(0,5,0,0),vec4(2,0,0,0),10,vec4(1,1,0,1));

	// tri shapes
	Visualiser::Get()->AddArc(vec4(0,0.5f,0,1),0,2,PI*0.25f,4,vec4(1,0,0,1),&g_ModelMatrix);
	Visualiser::Get()->AddRing(vec4(4,0,4,1),2,2.5f,8,vec4(1,0,0,1),&g_ModelMatrix);
	Visualiser::Get()->AddArcRing(vec4(0,0,0,1),PI,2,2.5f,PI*0.25f,4,vec4(1,0,1,0.5f),&g_ModelMatrix);
	Visualiser::Get()->AddSphere(vec4(-4,0,0,1),8,8,2,vec4(1,0,0,0.5f),&g_ModelMatrix);
	Visualiser::Get()->AddCylinderFilled(vec4(0,0,0,1),1,4,8,vec4(0,0,1,0.5f),&g_ModelMatrix);
	Visualiser::Get()->AddAABBFilled(vec4(0,0,0,1),vec4(1,1,1,0),vec4(1,0,0,0.5f),&g_ModelMatrix);

	// transform lines
	Visualiser::Get()->AddTransform(g_ModelMatrix,2);

	g_ModelMatrix.row3 = vec4(4,0,0,1);
	Visualiser::Get()->AddTransform(g_ModelMatrix,2);
	g_ModelMatrix.row3 = vec4(0,0,0,1);

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

	GLuint TimeID = glGetUniformLocation(g_ShaderID, "Time");
	glUniform1f(TimeID, phase );

	// set active texture, bind the crate quad's buffers and draw it
	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, g_TextureID );
	glBindVertexArray(g_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// draw the quad again at a different position
	g_ModelMatrix.row3 = vec4(4,0,0,1);
	glUniformMatrix4fv(ModelID, 1, false, g_ModelMatrix);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	g_ModelMatrix.row3 = vec4(0,0,0,1);

	// draw the visualiser shapes
	Visualiser::Get()->Draw(&g_ViewMatrix,&g_ProjectionMatrix);
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