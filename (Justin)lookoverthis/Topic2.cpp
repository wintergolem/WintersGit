//////////////////////////////////////////////////////////////////////////
// Author:	Justin Loudermilk		
// Date:	December 6 2012
// Brief:	This class is here to put some of the Topic2 labs together  
//			I took the layout from Visualiser because it was good
//////////////////////////////////////////////////////////////////////////
#include "Topic2.h"
#include "Utilities.h"



#pragma warning( disable : 4996 )



//////////////////////////////////////////////////////////////////////////
void AIE::Build3DMesh( unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO)
{
	AIE::vec4 Mesh[DEPTH][DEPTH];
unsigned int uiIndices[DEPTH* DEPTH * 2];

	//This is for tracking the indices
	int i = -1;
	float fX = 0;
	float fZ = 0;
	// Create our mesh
	for (int x = 0; x < DEPTH; x++)
	{
		for (int z = 0; z < DEPTH; z++)
		{	
			fX = (float)(DEPTH / 2) - x;
			fZ = (float)(DEPTH / 2) - z;
			Mesh[x][z] = AIE::vec4(fX,0.0f,fZ,1.0f); 	
		}
	}



	for(int x = 0; x < DEPTH; x++)
	{
		//draw from the front in x is even
		if(x % 2 == 0)
		{
			for(int z = 0; z <DEPTH; z++)
			{
				uiIndices[++i] = DEPTH * x +z ;
				uiIndices[++i] = DEPTH * (x + 1) +z;
			}
		}
		//draw from the back in x is odd;
		else
		{
			for(int z = DEPTH-1; z >= 0; z--)
			{
				uiIndices[++i] = DEPTH * (x + 1) +z;
				uiIndices[++i] = DEPTH * x +z ;

			}
		}
	}
	// create and bind buffers to a vertex array object

	GLuint vertexSize = 4 * sizeof(GLfloat);
	glGenBuffers(1, &a_ruiVBO);
	glGenBuffers(1, &a_ruiIBO);
	glGenVertexArrays(1, &a_ruiVAO);

	glBindVertexArray(a_ruiVAO);
	glBindBuffer(GL_ARRAY_BUFFER, a_ruiVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_ruiIBO);


	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh), Mesh, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uiIndices), uiIndices, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0); //position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vertexSize, (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ((char*)0) + 16);
	
	// unbind vertex array
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

}
