#ifndef _QUATERNIONS_H_
#define _QUATERNIONS_H_

#include "glm.hpp"

class Quaternion
{
public:
	float x; float y; float z; float w;

	Quaternion();
	Quaternion(float w, float x, float y, float z);


	Quaternion operator *(const Quaternion &q0);
	Quaternion operator *=(const Quaternion &q0);
	glm::vec4 operator *(const glm::vec4 &q0);
	Quaternion operator *=(const float &a_f);
	Quaternion operator *(const float &a_f);


	float DotProduct(const Quaternion &q0);
	Quaternion CalNorm();
	float CalMag();
	Quaternion CrossProduct();
	Quaternion Conjugate();
	Quaternion FromMatrix (const glm::mat4 &a_matrix);
	void CreateRotation(float a_a, glm::vec4 a_v); //float angle, vec4 
	glm::vec4 RotationVector(const glm::vec4 &a_v);
	glm::mat4 ToMakeMatrix();
	
};

#endif