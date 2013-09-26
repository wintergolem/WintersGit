#include "Quaternion.h"
#include <math.h>
#include "MathHelper.h"


Quaternion::Quaternion()
{
	w=1;
	x=0;
	y=0;
	z=0;
}
Quaternion::Quaternion(float a_w, float a_x, float a_y, float a_z)
{
	w = a_w;
	x = a_x;
	y = a_y;
	z = a_z;
}

float Quaternion::DotProduct(const Quaternion &q0)
{
	return ((q0.w * w)+ (q0.x * x) + (q0.y * y) + (q0.z * z));
}

float Quaternion::CalMag()
{
	return sqrt( (w*w) + (x*x) + (y*y) + (z*z) );
}

/////operator functions///////////////////////

Quaternion Quaternion::operator* (const Quaternion &q0)
{
	return Quaternion(  ( w * q0.w - x * q0.x - y * q0.y - z * q0.z ),
						( w * q0.w + x * q0.x + y * q0.y + z * q0.z ),
						( w * q0.w - x * q0.x - y * q0.y - z * q0.z ),
						( w * q0.w + x * q0.x + y * q0.y + z * q0.z ) );
}

Quaternion Quaternion::operator *=(const Quaternion &q0)
{
	w = w * q0.w - x * q0.x - y * q0.y - z * q0.z;
	x = w * q0.w + x * q0.x + y * q0.y + z * q0.z;
	y = w * q0.w - x * q0.x - y * q0.y - z * q0.z;
	z = w * q0.w + x * q0.x + y * q0.y + z * q0.z;
	return *this;
}

glm::vec4 Quaternion::operator *(const glm::vec4 &a_v)
{
	//new q = to this ; w= 0
	Quaternion q1 (0, a_v.x, a_v.y, a_v.z);
	Quaternion q2 = *this * q1;
	Quaternion qInverse = (*this).Conjugate();
	Quaternion q3 = q2 * qInverse;
	return glm::vec4(q3.x, q3.y, q3.z, 1);
}

Quaternion Quaternion::operator * (const float &a_f)
{
	return Quaternion( w + a_f,
					   x + a_f,
					  y + a_f,
					  z + a_f );
}

Quaternion Quaternion::operator *=(const float &a_f)
{
	w *= a_f;
	x *= a_f;
	y *= a_f;
	z *= a_f;
	return *this;
}

//vec4 Quaternion::operator*(const glm::vec4 &a_v)
//{
//	return vec4 = 
//}



/////////////////////////////////
Quaternion Quaternion::CalNorm()
{
	float mag = CalMag();
	Quaternion q2;
	q2.w = this->x / mag;
	q2.x = this->x / mag;
	q2.y = this->x / mag;
	q2.z = this->x / mag;

	return q2;
}

Quaternion Quaternion::CrossProduct()
{
	return Quaternion(0,0,0,1);
}

Quaternion Quaternion::Conjugate()
{
	return Quaternion(w*-1, x*-1, y*-1, z*-1);
}

void Quaternion::CreateRotation(float a_a, glm::vec4 a_v)
{
	w = glm::cos(a_a / 2);
	x = a_v.x * sin( a_a /2);
	y = a_v.y * sin( a_a /2);
	z = a_v.z * sin( a_a /2);
}

glm::vec4 Quaternion::RotationVector(const glm::vec4 &a_v)
{
	Quaternion q1 (0, a_v.x, a_v.y, a_v.z);
	Quaternion q2 = *this * q1;
	Quaternion qInverse = (*this).Conjugate();
	Quaternion q3 = q2 * qInverse;
	return glm::vec4(q3.x, q3.y, q3.z, 1);
}

glm::mat4 Quaternion::ToMakeMatrix()
{
	float aa[16] = {1-2*(y*y)-2*(z*z) , 2*x*y-2*w*z , 2*x*z+2*w*y , 0 , 2*x*y+2*w*z , 1-2*(x*x)-2*(z*z) , 2*y*z+2*w*x , 0 , 2*x*z-2*w*y , 2*y*z-2*w*x , 1-2*(x*x)-2*(y*y),
		0, 0 , 0 , 0 ,1};
	glm::mat4 matrix = glm::mat4(1-2*(y*y)-2*(z*z) , 2*x*y-2*w*z , 2*x*z+2*w*y , 0 , 2*x*y+2*w*z , 1-2*(x*x)-2*(z*z) , 2*y*z+2*w*x , 0 , 2*x*z-2*w*y , 2*y*z-2*w*x , 1-2*(x*x)-2*(y*y),
		0, 0 , 0 , 0 ,1);//= glm::make_mat4(aa);
	//memcpy( glm::value_ptr( matrix ), aa, sizeof(aa) );
	return matrix;


}

Quaternion Quaternion::FromMatrix (const mat &a_matrix)
{
	Quaternion quat;

	quat.w = sqrt(1.f + a_matrix._11 + a_matrix._22 + a_matrix._33) / 2.f;
	quat.x = sqrt(a_matrix._32 - a_matrix._23) / (4.f * w);
	quat.y = sqrt(a_matrix._13 - a_matrix._31) / (4.f * w);
	quat.z = sqrt(a_matrix._21 - a_matrix._12) / (4.f * w);

	return quat;
}