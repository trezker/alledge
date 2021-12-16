#ifndef __MATH__VECTOR3_H
#define __MATH__VECTOR3_H

class Matrix4;

class Vector3
{
public:
	//! \brief Constructor. Sets x=0 and y=0 and z=0.
	Vector3();

	//! \brief Constructor.
	//! \param xValue Sets x to xValue.
	//! \param yValue Sets y to yValue.
	//! \param zValue Sets z to zValue.
	Vector3(float xValue, float yValue, float zValue);

	//! \brief Constructor.
	//! \param vector3 Sets this vector to vector3.
	Vector3(const Vector3 &vector3);

	//! \brief Constructor.
	//! \param vector3 Sets this vector to vector3.
	Vector3(const float *vector3);

	//! \brief Normalized form of the vector.
	//! \return Returns a normalized form of the vector.
	Vector3		GetNormalized() const;

	//! \brief Crossproduct between the two vectors.
	//! \param vector3 Used to calculate the cross product with this vector.
	//! \return Returns the crossproduct between the two vectors
	Vector3		CrossProduct(const Vector3& vector3) const;

	//! \brief Length of the vector.
	//! \return Returns a float value with the length of the vector.
	float		Length() const;

	//!	\brief The squared length of the vector
	//!	\return Returns a float value with the squared length
	float		SquaredLength() const;

	//!	\brief Returns true if the vector is zero.
	//!	\return Returns true if the vector is zero, otherwise false.
	bool		IsZero() const;

	float		DotProduct2(float x, float y) const;

	//! \brief Dotproduct between the two vectors.
	//! \param vector3 Used to calculate the dot product with this vector.
	//! \return Returns the dot product between the two vectors
	float		DotProduct(const Vector3& vector3) const;

	//! \brief Angle between vectors.
	//! \param vector3 Used to calculate the angle with this vector.
	//! \return Returns a angle in degrees between the two vectors.
	float		GetAngleDegree(const Vector3& vector3) const;

	//! \brief Angle between vectors.
	//! \param vector3 Used to calculate the angle with this vector.
	//! \return Returns a angle in radians between the two vectors.
	float		GetAngleRadians(const Vector3& vector3) const;
	
	//! \brief Normalizes the vector.
	void		Normalize();

	//! \brief Normalizes the vector.
	//!	\param distance the distance vector
	void		Normalize( float distance );

	//! \brief Set the vector to zero.
	void		Zero();

	//! \brief Invert the vector.
	void		Negate();

	//! \brief Transforms the vector with a matrix3.
	//! \param matrix3 Is the matrix to transform this vector by
//	void		TransformByMatrix3(const Matrix3& matrix3);

	//! \brief Transforms the vector with a matrix3, using a matrix4.
	//! \param matrix4 Is the matrix to transform this vector by
//	void		TransformByMatrix3(const Matrix4& matrix4);

	//! \brief Transforms the vector with a matrix4.
	//! \param matrix4 Is the matrix to transform this vector by
	void		TransformByMatrix4(const Matrix4& matrix4);

	//! \brief Transforms the vector with a quaternion.
	//! \param quaternion Is the quaternion to transform this vector with
//	void		TransformByQuaternion(const Quaternion& quaternion);

	//! \brief Sets the x, y and z values of the vector
	//! \param xValue Sets the x-value of the vector to xValue
	//! \param yValue Sets the y-value of the vector to yValue
	//! \param zValue Sets the z-value of the vector to zValue
	void		Set(float xValue, float yValue, float zValue);

	//! \brief Sets the Vector.
	//! \param vector3 Sets this vector to vector3
	void		Set(const Vector3& vector3);

	Vector3		operator + (const Vector3 &vector3) const;
	Vector3		operator - (const Vector3 &vector3) const;
	Vector3		operator * (const float scalar) const;
	Vector3		operator / (const float scalar) const;

	void		operator += (const Vector3 &vector3);
	void		operator -= (const Vector3 &vector3);
	void		operator *= (const float scalar);
	void		operator /= (const float scalar);

	Vector3		operator = (const Vector3 &vector3);
	bool		operator == (const Vector3 &vector3);
	bool		operator != (const Vector3 &vector3);

	Vector3		operator - ();

	static const Vector3 ZERO; 
	static const Vector3 UNIT_X; 
	static const Vector3 UNIT_Y;
	static const Vector3 UNIT_Z;
	static const Vector3 NEGATIVE_UNIT_X;
	static const Vector3 NEGATIVE_UNIT_Y;
	static const Vector3 NEGATIVE_UNIT_Z;

	float	x;
	float	y;
	float	z;
	/*
	union
	{
		struct
		{
			float	x;
			float	y;
			float	z;
		};

		float vector[3];
	};*/
};

class Vector3_comparator {
    public:
        bool operator()(const Vector3& lhs, const Vector3& rhs) const {
            return lhs.x < rhs.x
                || ( lhs.x == rhs.x && ( lhs.y < rhs.y
                || ( lhs.y == rhs.y && lhs.z < rhs.z)));
        }
};

#endif
