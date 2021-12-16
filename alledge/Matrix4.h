#ifndef __MATH__MATRIX4_H
#define __MATH__MATRIX4_H

//#include "Quaternion.h"

class Vector3;
//class Quaternion;

class Matrix4
{
public:
	//! \brief Constructor
	Matrix4();

	Matrix4(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44);

	Matrix4(float *m);

	//! \brief Constructor
	//! \param matrix4 Sets this matrix to matrix4
	Matrix4(const Matrix4 &matrix4);

	//! \brief Set the matrix to zero
	void	Zero();

	//! \brief Set the matrix to Identity
	void	Identity();

	//! \brief Rotates the matrix
	//! \param rotation Is the factor to rotate by
	void	SetRotation(float *rotation);

	//! \brief Rotates the matrix
	//! \param x is the rotation along the x axis
	//! \param y is the rotation along the y axis
	//! \param z is the rotation along the z axis
	void	SetRotation(float x, float y, float z);

	//! \brief Inverse rotates the matrix
	//! \param rotation Is the factor to inverse rotate by
	void	SetInverseRotation(float *rotation);

	//! \brief Inverse rotates the matrix
	//! \param x is the rotation along the x axis
	//! \param y is the rotation along the y axis
	//! \param z is the rotation along the z axis
	void	SetInverseRotation(float x, float y, float z);

	//! \brief Translates the matrix
	//! \param translation
	void	SetTranslation(float *translation);

	//! \brief Translates the matrix
	//! \param x is the x-factor of the translation
	//! \param y is the y-factor of the translation
	//! \param z is the z-factor of the translation
	void	SetTranslation(float x, float y, float z);

	//! \brief Inverse Translates the matrix
	//! \param translation
	void	SetInverseTranslation(float *translation);

	//! \brief Inverse Translates the matrix
	//! \param x is the x-factor of the translation
	//! \param y is the y-factor of the translation
	//! \param z is the z-factor of the translation
	void	SetInverseTranslation(float x, float y, float z);

	//! \brief Retrieves the Determinant of the matrix
	//! \return Returns the Determinant of the matrix.
	float	Determinant();

	//! \brief Transposes the matrix
	void	Transpose();

	//! \brief Inverts the matrix
	void	Invert();

	//! \brief Changes the matrix by axis and angles
	//! \param vector Is the axises to change the matrix by.
	//! \param angleRadians Is the angle (in radians) to change the matrix by.
	void	AxisAndAngleToMatrix(Vector3& vector, float angleRadians);

	//! \brief Converts a quaternion to a matrix
	//! \param quaternion is the quaternion to be changed into a matrix.
//	void	QuaternionToMatrix(Quaternion& quaternion);

	//! \brief Retrieves the matrix
	//! \return Returns the Matrix in a float-array
	const float	*Get() const;

	//! \brief Sets the matrix
	//! \param m11 sets element 1-1 to m11
	//! \param m12 sets element 1-2 to m12
	//! \param m13 sets element 1-3 to m13
	//! \param m14 sets element 1-4 to m14
	//! \param m21 sets element 2-1 to m21
	//! \param m22 sets element 2-2 to m22
	//! \param m23 sets element 2-3 to m23
	//! \param m24 sets element 2-4 to m24
	//! \param m31 sets element 3-1 to m31
	//! \param m32 sets element 3-2 to m32
	//! \param m33 sets element 3-3 to m33
	//! \param m34 sets element 3-4 to m34
	//! \param m41 sets element 4-1 to m41
	//! \param m42 sets element 4-2 to m42
	//! \param m43 sets element 4-3 to m43
	//! \param m44 sets element 4-4 to m44
	void	Set(float m11, float m12, float m13, float m14,
				float m21, float m22, float m23, float m24,
				float m31, float m32, float m33, float m34,
				float m41, float m42, float m43, float m44);

	//! \brief Sets the matrix
	//! \param matrix sets the values of this Matrix to the ones in matrix
	void	Set(float *matrix);

	Matrix4	operator + (const Matrix4& matrix);
	Matrix4	operator - (const Matrix4& matrix);
	Matrix4	operator * (const Matrix4& matrix);

	const void	operator += (const Matrix4& matrix);
	const void	operator -= (const Matrix4& matrix);
	const void	operator *= (const Matrix4& matrix);

	Matrix4		operator * (const float scalar);
	Matrix4		operator / (const float scalar);
	const void	operator *= (const float scalar);
	const void	operator /= (const float scalar);

	void	operator = (const Matrix4& matrix);
	bool	operator == (const Matrix4& matrix);
	bool	operator != (const Matrix4& matrix);

private:
	float	m_matrix[16];
};

#endif
