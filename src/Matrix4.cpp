#include <cmath>
#include <memory.h>

#include "../alledge/Matrix4.h"
#include "../alledge/Vector3.h"

Matrix4::Matrix4()
{
	memset(m_matrix, 0, 64);
}

Matrix4::Matrix4(float m11, float m12, float m13, float m14,
				 float m21, float m22, float m23, float m24,
				 float m31, float m32, float m33, float m34,
				 float m41, float m42, float m43, float m44)
{
	m_matrix[0] = m11;
	m_matrix[1] = m12;
	m_matrix[2] = m13;
	m_matrix[3] = m14;
	m_matrix[4] = m21;
	m_matrix[5] = m22;
	m_matrix[6] = m23;
	m_matrix[7] = m24;
	m_matrix[8] = m31;
	m_matrix[9] = m32;
	m_matrix[10] = m33;
	m_matrix[11] = m34;
	m_matrix[12] = m41;
	m_matrix[13] = m42;
	m_matrix[14] = m43;
	m_matrix[15] = m44;
}

Matrix4::Matrix4(const Matrix4 &matrix4)
{
	m_matrix[0] = matrix4.m_matrix[0];
	m_matrix[1] = matrix4.m_matrix[1];
	m_matrix[2] = matrix4.m_matrix[2];
	m_matrix[3] = matrix4.m_matrix[3];
	m_matrix[4] = matrix4.m_matrix[4];
	m_matrix[5] = matrix4.m_matrix[5];
	m_matrix[6] = matrix4.m_matrix[6];
	m_matrix[7] = matrix4.m_matrix[7];
	m_matrix[8] = matrix4.m_matrix[8];
	m_matrix[9] = matrix4.m_matrix[9];
	m_matrix[10] = matrix4.m_matrix[10];
	m_matrix[11] = matrix4.m_matrix[11];
	m_matrix[12] = matrix4.m_matrix[12];
	m_matrix[13] = matrix4.m_matrix[13];
	m_matrix[14] = matrix4.m_matrix[14];
	m_matrix[15] = matrix4.m_matrix[15];
}

const float* Matrix4::Get() const
{
	return m_matrix;
}

void Matrix4::Set(float *matrix)
{
	memcpy(m_matrix, matrix, 64);
}

void Matrix4::Set(float m11, float m12, float m13, float m14,
			 float m21, float m22, float m23, float m24,
			 float m31, float m32, float m33, float m34,
			 float m41, float m42, float m43, float m44)
{
	m_matrix[0] = m11;
	m_matrix[1] = m12;
	m_matrix[2] = m13;
	m_matrix[3] = m14;
	m_matrix[4] = m21;
	m_matrix[5] = m22;
	m_matrix[6] = m23;
	m_matrix[7] = m24;
	m_matrix[8] = m31;
	m_matrix[9] = m32;
	m_matrix[10] = m33;
	m_matrix[11] = m34;
	m_matrix[12] = m41;
	m_matrix[13] = m42;
	m_matrix[14] = m43;
	m_matrix[15] = m44;
}

void Matrix4::Zero()
{
	memset(m_matrix, 0, 64);
}

void Matrix4::Identity()
{
	m_matrix[0] = 1;
	m_matrix[1] = 0;
	m_matrix[2] = 0;
	m_matrix[3] = 0;
	m_matrix[4] = 0;
	m_matrix[5] = 1;
	m_matrix[6] = 0;
	m_matrix[7] = 0;
	m_matrix[8] = 0;
	m_matrix[9] = 0;
	m_matrix[10] = 1;
	m_matrix[11] = 0;
	m_matrix[12] = 0;
	m_matrix[13] = 0;
	m_matrix[14] = 0;
	m_matrix[15] = 1;
}

void Matrix4::SetRotation(float *rotation)
{
	SetRotation(rotation[0], rotation[1], rotation[2]);
}

void Matrix4::SetRotation(float x, float y, float z)
{
	float cosX = cosf(x);
	float cosY = cosf(y);
	float cosZ = cosf(z);
	float sinX = sinf(x);
	float sinY = sinf(y);
	float sinZ = sinf(z);

	m_matrix[0] = cosY * cosZ;
	m_matrix[1] = cosY * sinZ;
	m_matrix[2] = -sinY;

	m_matrix[4] = sinX * sinY * cosZ - cosX * sinZ;
	m_matrix[5] = sinX * sinY * sinZ + cosX * cosZ;
	m_matrix[6] = sinX * cosY;

	m_matrix[8] = cosX * sinY * cosZ + sinX * sinZ;
	m_matrix[9] = cosX * sinY * sinZ - sinX * cosZ;
	m_matrix[10] = cosX * cosY;

	m_matrix[15] = 1.0f;
}

void Matrix4::SetInverseRotation(float *rotation)
{
	SetInverseRotation(rotation[0], rotation[1], rotation[2]);
}

void Matrix4::SetInverseRotation(float x, float y, float z)
{
	float cosX = cosf(x);
	float cosY = cosf(y);
	float cosZ = cosf(z);
	float sinX = sinf(x);
	float sinY = sinf(y);
	float sinZ = sinf(z);

	m_matrix[0] = cosY * cosZ;
	m_matrix[4] = cosY * sinZ;
	m_matrix[8] = -sinY;

	m_matrix[1] = sinX * sinY * cosZ - cosX * sinZ;
	m_matrix[5] = sinX * sinY * sinZ + cosX * cosZ;
	m_matrix[9] = sinX * cosY;

	m_matrix[2] = cosX * sinY * cosZ + sinX * sinZ;
	m_matrix[6] = cosX * sinY * sinZ - sinX * cosZ;
	m_matrix[10] = cosX * cosY;

	m_matrix[15] = 1.0f;
}

void Matrix4::SetTranslation(float *translation)
{
	SetTranslation(translation[0], translation[1], translation[2]);
}

void Matrix4::SetTranslation(float x, float y, float z)
{
	m_matrix[3] = x;
	m_matrix[7] = y;
	m_matrix[11] = z;
}

void Matrix4::SetInverseTranslation(float *translation)
{
	SetInverseTranslation(translation[0], translation[1], translation[2]);
}

void Matrix4::SetInverseTranslation(float x, float y, float z)
{
	m_matrix[3] = -x;
	m_matrix[7] = -y;
	m_matrix[11] = -z;
}

float Matrix4::Determinant()
{
	float subMatrix0[9];
		subMatrix0[0] = m_matrix[5]; subMatrix0[1] = m_matrix[6]; subMatrix0[2] = m_matrix[7];
		subMatrix0[3] = m_matrix[9]; subMatrix0[4] = m_matrix[10]; subMatrix0[5] = m_matrix[11];
		subMatrix0[6] = m_matrix[13]; subMatrix0[7] = m_matrix[14]; subMatrix0[8] = m_matrix[15];
	float subMatrix1[9];
		subMatrix1[0] = m_matrix[4]; subMatrix1[1] = m_matrix[6]; subMatrix1[2] = m_matrix[7];
		subMatrix1[3] = m_matrix[8]; subMatrix1[4] = m_matrix[10]; subMatrix1[5] = m_matrix[11];
		subMatrix1[6] = m_matrix[12]; subMatrix1[7] = m_matrix[14]; subMatrix1[8] = m_matrix[15];
	float subMatrix2[9];
		subMatrix2[0] = m_matrix[4]; subMatrix2[1] = m_matrix[5]; subMatrix2[2] = m_matrix[7];
		subMatrix2[3] = m_matrix[8]; subMatrix2[4] = m_matrix[9]; subMatrix2[5] = m_matrix[11];
		subMatrix2[6] = m_matrix[12]; subMatrix2[7] = m_matrix[13]; subMatrix2[8] = m_matrix[15];
	float subMatrix3[9];
		subMatrix3[0] = m_matrix[4]; subMatrix3[1] = m_matrix[5]; subMatrix3[2] = m_matrix[6];
		subMatrix3[3] = m_matrix[8]; subMatrix3[4] = m_matrix[9]; subMatrix3[5] = m_matrix[10];
		subMatrix3[6] = m_matrix[12]; subMatrix3[7] = m_matrix[13]; subMatrix3[8] = m_matrix[14];

	float subDeterminant0 =		subMatrix0[0] * (subMatrix0[4] * subMatrix0[8] - subMatrix0[7] * subMatrix0[5]) -
								subMatrix0[1] * (subMatrix0[3] * subMatrix0[8] - subMatrix0[6] * subMatrix0[5]) +
								subMatrix0[2] * (subMatrix0[3] * subMatrix0[7] - subMatrix0[6] * subMatrix0[4]);
	float subDeterminant1 =		subMatrix1[0] * (subMatrix1[4] * subMatrix1[8] - subMatrix1[7] * subMatrix1[5]) -
								subMatrix1[1] * (subMatrix1[3] * subMatrix1[8] - subMatrix1[6] * subMatrix1[5]) +
								subMatrix1[2] * (subMatrix1[3] * subMatrix1[7] - subMatrix1[6] * subMatrix1[4]);
	float subDeterminant2 =		subMatrix2[0] * (subMatrix2[4] * subMatrix2[8] - subMatrix2[7] * subMatrix2[5]) -
								subMatrix2[1] * (subMatrix2[3] * subMatrix2[8] - subMatrix2[6] * subMatrix2[5]) +
								subMatrix2[2] * (subMatrix2[3] * subMatrix2[7] - subMatrix2[6] * subMatrix2[4]);
	float subDeterminant3 =		subMatrix3[0] * (subMatrix3[4] * subMatrix3[8] - subMatrix3[7] * subMatrix3[5]) -
								subMatrix3[1] * (subMatrix3[3] * subMatrix3[8] - subMatrix3[6] * subMatrix3[5]) +
								subMatrix3[2] * (subMatrix3[3] * subMatrix3[7] - subMatrix3[6] * subMatrix3[4]);
	
	float determinant = (m_matrix[0] * subDeterminant0) - (m_matrix[1] * subDeterminant1) + 
						(m_matrix[2] * subDeterminant2) - (m_matrix[3] * subDeterminant3);

	return determinant;
}

void Matrix4::Transpose()
{
	float tempMatrix[16];// = m_matrix;
	tempMatrix[0] = m_matrix[0];
	tempMatrix[1] = m_matrix[1];
	tempMatrix[2] = m_matrix[2];
	tempMatrix[3] = m_matrix[3];
	tempMatrix[4] = m_matrix[4];
	tempMatrix[5] = m_matrix[5];
	tempMatrix[6] = m_matrix[6];
	tempMatrix[7] = m_matrix[7];
	tempMatrix[8] = m_matrix[8];
	tempMatrix[9] = m_matrix[9];
	tempMatrix[10] = m_matrix[10];
	tempMatrix[11] = m_matrix[11];
	tempMatrix[12] = m_matrix[12];
	tempMatrix[13] = m_matrix[13];
	tempMatrix[14] = m_matrix[14];
	tempMatrix[15] = m_matrix[15];

	m_matrix[1] = tempMatrix[4];
	m_matrix[2] = tempMatrix[8];
	m_matrix[3] = tempMatrix[12];
	m_matrix[4] = tempMatrix[1];
	m_matrix[6] = tempMatrix[9];
	m_matrix[7] = tempMatrix[13];
	m_matrix[8] = tempMatrix[2];
	m_matrix[9] = tempMatrix[6];
	m_matrix[11] = tempMatrix[14];
	m_matrix[12] = tempMatrix[3];
	m_matrix[13] = tempMatrix[7];
	m_matrix[14] = tempMatrix[11];
}

void Matrix4::Invert()
{
	Matrix4 identity;
	identity.Identity();

	unsigned int row, column, rowMax;
	float tmp;
	float *matrixA = m_matrix;
	float *matrixB = identity.m_matrix;

	for(column = 0; column < 4; column++)
	{
		rowMax = column;
		for(row = column + 1; row < 4; row++)
		{
			if(fabs(matrixA[4 * row + column]) > fabs(matrixA[4 * rowMax + column]))
			{
				rowMax = row;
			}
		}
		if(matrixA[4 * column + rowMax] == 0.0f)
		{
			Identity();
			return;
		}
		for(int c = 0; c < 4; c++)
		{
			tmp = matrixA[4 * column + c];
			matrixA[4 * column + c] = matrixA[4 * rowMax + c];
			matrixA[4 * rowMax + c] = tmp;

			tmp = matrixB[4 * column + c];
			matrixB[4 * column + c] = matrixB[4 * rowMax + c];
			matrixB[4 * rowMax + c] = tmp;
		}
		tmp = matrixA[4 * column + column];
		for(int c = 0; c < 4; c++)
		{
			matrixA[4 * column + c] /= tmp;
			matrixB[4 * column + c] /= tmp;
		}
		for(row = 0; row < 4; row++)
		{
			if(row != column)
			{
				tmp = matrixA[4 * row + column];
				for(int c = 0; c < 4; c++)
				{
					matrixA[4 * row + c] -= matrixA[4 * column + c] * tmp;
					matrixB[4 * row + c] -= matrixB[4 * column + c] * tmp;
				}
			}
		}

	}

	Set(matrixB);
}

void Matrix4::AxisAndAngleToMatrix(Vector3 &vector, float angleRadians)
{
	float cosAngle = cosf(angleRadians);
	float sinAngle = sinf(angleRadians);
	float t = 1 - cosAngle;

	m_matrix[0] = t * vector.x * vector.x + cosAngle;
	m_matrix[1] = t * vector.x * vector.y - vector.z * sinAngle;
	m_matrix[2] = t * vector.x * vector.z + vector.y * sinAngle;
	m_matrix[3] = 0;
	
	m_matrix[4] = t * vector.x * vector.y + vector.z * sinAngle;
	m_matrix[5] = t * vector.y * vector.y + cosAngle;
	m_matrix[6] = t * vector.y * vector.z - vector.x * sinAngle;
	m_matrix[7] = 0;

	m_matrix[8] = t * vector.x * vector.z - vector.y * sinAngle;
	m_matrix[9] = t * vector.y * vector.z + vector.x * sinAngle;
	m_matrix[10] = t * vector.z * vector.z + cosAngle;
	m_matrix[11] = 0;

	m_matrix[12] = 0;
	m_matrix[13] = 0;
	m_matrix[14] = 0;
	m_matrix[15] = 1;
}
/*
void Matrix4::QuaternionToMatrix(Quaternion &quaternion)
{
	const float* q = quaternion.Get();
	float qx = q[0];
	float qy = q[1];
	float qz = q[2];
	float qw = q[3];
	Set((1 - 2 * qy * qy - 2 * qz * qz),
		(2 * qx * qy - 2 * qz * qw),
		(2 * qx * qz + 2 * qy * qw),
		0,

		(2 * qx * qy + 2 * qz * qw),
		(1 - 2 * qx * qx - 2 * qz * qz),
		(2 * qy * qz - 2 * qx * qw),
		0,

		(2 * qx * qz - 2 * qy * qw),
		(2 * qy * qz + 2 * qx * qw),
		(1 - 2 * qx * qx - 2 * qy * qy),
		0,
		
		0,
		0,
		0,
		1);
*/
	/*Set(pow(q[3], 2) + pow(q[0], 2) - pow(q[1], 2) - pow(q[2], 2),
		(2 * (q[0] * q[1] + q[3] * q[2])),
		(2 * (q[0] * q[2] - q[3] * q[1])), 0,
		(2 * (q[0] * q[1] - q[3] * q[2])),
		pow(q[3], 2) - pow(q[0], 2) + pow(q[1], 2) - pow(q[2], 2),
		(2 * (q[1] * q[2] + q[3] * q[0])), 0,
		(2 * (q[0] * q[2] + q[3] * q[1])),
		(2 * (q[1] * q[2] - q[3] * q[0])),
		pow(q[3], 2) + pow(q[0], 2) - pow(q[1], 2) + pow(q[2], 2), 0,
		0, 0, 0, 1);*/
//}

Matrix4 Matrix4::operator + (const Matrix4 &matrix)
{
	const float *m = m_matrix;
	const float *n = matrix.m_matrix;

	return Matrix4(
		m[0]  + n[0],  m[1]  + n[1],  m[2]  + n[2],  m[3]  + n[3],
		m[4]  + n[4],  m[5]  + n[5],  m[6]  + n[6],  m[7]  + n[7], 
		m[8]  + n[8],  m[9]  + n[9],  m[10] + n[10], m[11] + n[11], 
		m[12] + n[12], m[13] + n[13], m[14] + n[14], m[15] + n[15]);
}

Matrix4 Matrix4::operator - (const Matrix4 &matrix)
{
	const float *m = m_matrix;
	const float *n = matrix.m_matrix;

	return Matrix4(
		m[0]  - n[0],  m[1]  - n[1],  m[2]  - n[2],  m[3]  - n[3],
		m[4]  - n[4],  m[5]  - n[5],  m[6]  - n[6],  m[7]  - n[7], 
		m[8]  - n[8],  m[9]  - n[9],  m[10] - n[10], m[11] - n[11], 
		m[12] - n[12], m[13] - n[13], m[14] - n[14], m[15] - n[15]);
}

Matrix4 Matrix4::operator * (const Matrix4 &matrix)
{
	const float *n = m_matrix;
	const float *m = matrix.m_matrix;

	return Matrix4(
		(m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3]),
		(m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3]),
		(m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3]),
		(m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3]),

		(m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7]),
		(m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7]),
		(m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7]),
		(m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7]),

		(m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11]),
		(m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11]),
		(m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11]),
		(m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11]),

		(m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15]),
		(m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15]),
		(m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15]),
		(m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15]));
}

const void Matrix4::operator += (const Matrix4& matrix)
{
	m_matrix[0] += matrix.m_matrix[0];
	m_matrix[1] += matrix.m_matrix[1];
	m_matrix[2] += matrix.m_matrix[2];
	m_matrix[3] += matrix.m_matrix[3];
	m_matrix[4] += matrix.m_matrix[4];
	m_matrix[5] += matrix.m_matrix[5];
	m_matrix[6] += matrix.m_matrix[6];
	m_matrix[7] += matrix.m_matrix[7];
	m_matrix[8] += matrix.m_matrix[8];
	m_matrix[9] += matrix.m_matrix[9];
	m_matrix[10] += matrix.m_matrix[10];
	m_matrix[11] += matrix.m_matrix[11];
	m_matrix[12] += matrix.m_matrix[12];
	m_matrix[13] += matrix.m_matrix[13];
	m_matrix[14] += matrix.m_matrix[14];
	m_matrix[15] += matrix.m_matrix[15];
}

const void Matrix4::operator -= (const Matrix4& matrix)
{
	m_matrix[0] -= matrix.m_matrix[0];
	m_matrix[1] -= matrix.m_matrix[1];
	m_matrix[2] -= matrix.m_matrix[2];
	m_matrix[3] -= matrix.m_matrix[3];
	m_matrix[4] -= matrix.m_matrix[4];
	m_matrix[5] -= matrix.m_matrix[5];
	m_matrix[6] -= matrix.m_matrix[6];
	m_matrix[7] -= matrix.m_matrix[7];
	m_matrix[8] -= matrix.m_matrix[8];
	m_matrix[9] -= matrix.m_matrix[9];
	m_matrix[10] -= matrix.m_matrix[10];
	m_matrix[11] -= matrix.m_matrix[11];
	m_matrix[12] -= matrix.m_matrix[12];
	m_matrix[13] -= matrix.m_matrix[13];
	m_matrix[14] -= matrix.m_matrix[14];
	m_matrix[15] -= matrix.m_matrix[15];
}

const void Matrix4::operator *= (const Matrix4& matrix)
{
	const float *n = m_matrix;
	const float *m = matrix.m_matrix;

	Matrix4 tempMatrix(
	(m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3]),
	(m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3]),
	(m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3]),
	(m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3]),

	(m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7]),
	(m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7]),
	(m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7]),
	(m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7]),

	(m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11]),
	(m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11]),
	(m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11]),
	(m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11]),

	(m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15]),
	(m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15]),
	(m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15]),
	(m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15]));

	this->Set(tempMatrix.m_matrix);
}

Matrix4 Matrix4::operator * (const float scalar)
{
	return Matrix4(
	m_matrix[0] *= scalar, m_matrix[1] *= scalar, m_matrix[2] *= scalar, m_matrix[3] *= scalar,
	m_matrix[4] *= scalar, m_matrix[5] *= scalar, m_matrix[6] *= scalar, m_matrix[7] *= scalar,
	m_matrix[8] *= scalar, m_matrix[9] *= scalar, m_matrix[10] *= scalar, m_matrix[11] *= scalar,
	m_matrix[12] *= scalar, m_matrix[13] *= scalar, m_matrix[14] *= scalar, m_matrix[15] *= scalar);
}

Matrix4 Matrix4::operator / (const float scalar)
{
	return Matrix4(
	m_matrix[0] /= scalar, m_matrix[1] /= scalar, m_matrix[2] /= scalar, m_matrix[3] /= scalar,
	m_matrix[4] /= scalar, m_matrix[5] /= scalar, m_matrix[6] /= scalar, m_matrix[7] /= scalar,
	m_matrix[8] /= scalar, m_matrix[9] /= scalar, m_matrix[10] /= scalar, m_matrix[11] /= scalar,
	m_matrix[12] /= scalar, m_matrix[13] /= scalar, m_matrix[14] /= scalar, m_matrix[15] /= scalar);
}

const void Matrix4::operator *= (const float scalar)
{
	m_matrix[0] *= scalar;
	m_matrix[1] *= scalar;
	m_matrix[2] *= scalar;
	m_matrix[3] *= scalar;
	m_matrix[4] *= scalar;
	m_matrix[5] *= scalar;
	m_matrix[6] *= scalar;
	m_matrix[7] *= scalar;
	m_matrix[8] *= scalar;
	m_matrix[9] *= scalar;
	m_matrix[10] *= scalar;
	m_matrix[11] *= scalar;
	m_matrix[12] *= scalar;
	m_matrix[13] *= scalar;
	m_matrix[14] *= scalar;
	m_matrix[15] *= scalar;
}

const void Matrix4::operator /= (const float scalar)
{
	m_matrix[0] /= scalar;
	m_matrix[1] /= scalar;
	m_matrix[2] /= scalar;
	m_matrix[3] /= scalar;
	m_matrix[4] /= scalar;
	m_matrix[5] /= scalar;
	m_matrix[6] /= scalar;
	m_matrix[7] /= scalar;
	m_matrix[8] /= scalar;
	m_matrix[9] /= scalar;
	m_matrix[10] /= scalar;
	m_matrix[11] /= scalar;
	m_matrix[12] /= scalar;
	m_matrix[13] /= scalar;
	m_matrix[14] /= scalar;
	m_matrix[15] /= scalar;
}

void Matrix4::operator = (const Matrix4& matrix)
{
	memcpy(m_matrix, matrix.m_matrix, 64);
}

bool Matrix4::operator == (const Matrix4& matrix)
{
	if (m_matrix[0] == matrix.m_matrix[0] && m_matrix[1] == matrix.m_matrix[1] &&
		m_matrix[2] == matrix.m_matrix[2] && m_matrix[3] == matrix.m_matrix[3] &&
		m_matrix[4] == matrix.m_matrix[4] && m_matrix[5] == matrix.m_matrix[5] &&
		m_matrix[6] == matrix.m_matrix[6] && m_matrix[7] == matrix.m_matrix[7] &&
		m_matrix[8] == matrix.m_matrix[8] && m_matrix[9] == matrix.m_matrix[9] &&
		m_matrix[10] == matrix.m_matrix[10] && m_matrix[11] == matrix.m_matrix[11] &&
		m_matrix[12] == matrix.m_matrix[12] && m_matrix[13] == matrix.m_matrix[13] &&
		m_matrix[14] == matrix.m_matrix[14] && m_matrix[15] == matrix.m_matrix[15])
		return true;
	return false;
}

bool Matrix4::operator != (const Matrix4& matrix)
{
	if (m_matrix[0] == matrix.m_matrix[0] && m_matrix[1] == matrix.m_matrix[1] &&
		m_matrix[2] == matrix.m_matrix[2] && m_matrix[3] == matrix.m_matrix[3] &&
		m_matrix[4] == matrix.m_matrix[4] && m_matrix[5] == matrix.m_matrix[5] &&
		m_matrix[6] == matrix.m_matrix[6] && m_matrix[7] == matrix.m_matrix[7] &&
		m_matrix[8] == matrix.m_matrix[8] && m_matrix[9] == matrix.m_matrix[9] &&
		m_matrix[10] == matrix.m_matrix[10] && m_matrix[11] == matrix.m_matrix[11] &&
		m_matrix[12] == matrix.m_matrix[12] && m_matrix[13] == matrix.m_matrix[13] &&
		m_matrix[14] == matrix.m_matrix[14] && m_matrix[15] == matrix.m_matrix[15])
		return false;
	return true;
}
