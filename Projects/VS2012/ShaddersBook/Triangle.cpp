
#include "ShaderBook.h"

using std::cout;
using std::string;
using glm::vec4;
using glm::mat4;

GLuint vaoHandle;

//////OpenGL Globals
mat4 g_ProjectionMatrix;
mat4 g_ViewMatrix;
mat4 g_ModelMatrix;
mat4 g_CameraMatrix;

void Update()
{

}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
	int returnValue = init(vaoHandle);
	if(returnValue <0) return returnValue;

	while (glfwGetWindowParam(GLFW_OPENED) && !glfwGetKey(GLFW_KEY_ESC) )
	{
		Update();
		Draw();
	}
}