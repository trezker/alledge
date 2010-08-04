#include <cmath>
#include "../alledge/Vector3.h"
#include "../alledge/Matrix4.h"

const Vector3 Vector3::UNIT_X = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_Y = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UNIT_Z = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::NEGATIVE_UNIT_X = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::NEGATIVE_UNIT_Y = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::NEGATIVE_UNIT_Z = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);

Vector3::Vector3()
	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
{
}

Vector3::Vector3(float xValue, float yValue, float zValue)
	:x(xValue)
	,y(yValue)
	,z(zValue)
{
}

Vector3::Vector3(const Vector3 &vector3)
	:x(vector3.x)
	,y(vector3.y)
	,z(vector3.z)
{
}

Vector3::Vector3(const float *vector3)
	:x(vector3[0])
	,y(vector3[1])
	,z(vector3[2])
{
}

Vector3
Vector3::GetNormalized() const
{
	Vector3 normalizedVector;
	float len = Length();
	len = (len != 0 ? len : 1.0f);
	float lengthMul = 1.0f / len;
	normalizedVector.x = x * lengthMul;
	normalizedVector.y = y * lengthMul;
	normalizedVector.z = z * lengthMul;

	return normalizedVector;
}

Vector3
Vector3::CrossProduct(const Vector3& vector3) const
{
	return Vector3(	(y * vector3.z - z * vector3.y),
					(z * vector3.x - x * vector3.z),
					(x * vector3.y - y * vector3.x));
}

float
Vector3::Length() const
{
	return sqrt( x * x + y * y + z * z );
}

float
Vector3::SquaredLength() const
{
	return (x * x + y * y + z * z);
}

float
Vector3::DotProduct(const Vector3& vector3) const
{
	return ((vector3.x * x) + (vector3.y * y) + (vector3.z * z));
}

float
Vector3::GetAngleDegree(const Vector3& vector3) const
{
	return acosf(((vector3.x * x) + (vector3.y * y) + (vector3.z * z)) / (vector3.Length() * Length())) * 57.29577951f;
}

float
Vector3::GetAngleRadians(const Vector3& vector3) const
{
	return acosf(((vector3.x * x) + (vector3.y * y) + (vector3.z * z)) / (vector3.Length() * Length()));
}

void
Vector3::Normalize()
{
	float len = Length();
	len = (len != 0.0f ? len : 1.0f);
	float lengthMul = 1.0f / len;
	x *= lengthMul;
	y *= lengthMul;
	z *= lengthMul;
}

void
Vector3::Normalize( float distance )
{
	float factor = 1.0f / distance;
	x *= factor;
	y *= factor;
	z *= factor;
}

void
Vector3::Zero()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

void
Vector3::Negate()
{
	x = -x;
	y = -y;
	z = -z;
}

/*		void
Vector3::TransformByMatrix3(const Matrix3& matrix3)
{
	const float *m = matrix3.Get();
	Vector3 vector3(
		(x * m[0] + y * m[3] + z * m[6]),
		(x * m[1] + y * m[4] + z * m[7]),
		(x * m[2] + y * m[5] + z * m[8]));
	*this = vector3;
}

void
Vector3::TransformByMatrix3(const Matrix4& matrix4)
{
	const float *m = matrix4.Get();
	Vector3 vector3(
		(x * m[0] + y * m[4] + z * m[8]),
		(x * m[1] + y * m[5] + z * m[9]),
		(x * m[2] + y * m[6] + z * m[10]));
	*this = vector3;
}
*/
void
Vector3::TransformByMatrix4(const Matrix4& matrix4)
{
	const float *m = matrix4.Get();
	Vector3 vector3(
		(x * m[0] + y * m[4] + z * m[8] + m[12]),
		(x * m[1] + y * m[5] + z * m[9] + m[13]),
		(x * m[2] + y * m[6] + z * m[10] + m[14]));
	*this = vector3;
}
/*
void
Vector3::TransformByQuaternion(const Quaternion& quaternion)
{
	Quaternion quaternion1(x, y, z, 0);
	Quaternion quaternion2 = quaternion * quaternion1 * (~quaternion);
	*this = quaternion2.GetVector();
}
*/
void
Vector3::Set(float xValue, float yValue, float zValue)
{
	x = xValue;
	y = yValue;
	z = zValue;
}

void
Vector3::Set(const Vector3& vector3)
{
	x = vector3.x;
	y = vector3.y;
	z = vector3.z;
}

void
Vector3::operator += (const Vector3 &vector3)
{
	x += vector3.x;
	y += vector3.y;
	z += vector3.z;
}

void
Vector3::operator -= (const Vector3 &vector3)
{
	x -= vector3.x;
	y -= vector3.y;
	z -= vector3.z;
}

void
Vector3::operator *= (const float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void
Vector3::operator /= (const float scalar)
{
	if (scalar != 0)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}
}

Vector3
Vector3::operator + (const Vector3 &vector3) const
{
	Vector3 vector3Temp;
	vector3Temp.x = x + vector3.x;
	vector3Temp.y = y + vector3.y;
	vector3Temp.z = z + vector3.z;

	return vector3Temp;
}

Vector3
Vector3::operator - (const Vector3 &vector3) const
{
	Vector3 vector3Temp;
	vector3Temp.x = x - vector3.x;
	vector3Temp.y = y - vector3.y;
	vector3Temp.z = z - vector3.z;

	return vector3Temp;
}

Vector3
Vector3::operator * (const float scalar) const
{
	Vector3 vector3Temp;
	vector3Temp.x = x * scalar;
	vector3Temp.y = y * scalar;
	vector3Temp.z = z * scalar;

	return vector3Temp;
}

Vector3
Vector3::operator / (const float scalar) const
{
	Vector3 vector3Temp;
	if (scalar != 0)
	{
		vector3Temp.x = x / scalar;
		vector3Temp.y = y / scalar;
		vector3Temp.z = z / scalar;
	}

	return vector3Temp;
}

void
Vector3::operator = (const Vector3 &vector3)
{
	x = vector3.x;
	y = vector3.y;
	z = vector3.z;
}

bool
Vector3::operator == (const Vector3 &vector3)
{
	if (vector3.x == x && vector3.y == y && vector3.z == z)
		return true;
	return false;
}

bool
Vector3::operator != (const Vector3 &vector3)
{
	return (!(*this == vector3));
}

Vector3
Vector3::operator - ()
{
	return Vector3(-x, -y, -z);
}

bool
Vector3::IsZero() const
{
	if (x == 0.0f && y == 0.0f && z == 0.0f)
		return true;
	
	return false;
}
