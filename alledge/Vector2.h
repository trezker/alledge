#ifndef Vector2_h
#define Vector2_h

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);
	const Vector2& operator=(const Vector2& v);
	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;
	void Set(float x, float y);
	float X() const;
	float Y() const;

	void Zero();
	float Length() const;
	float Length_squared() const;
	//Normalize does nothing if Length is 0.
	void Normalize();
	//Normalize returns itself if Length is 0.
	Vector2 Normalized() const;
	float Dotproduct(const Vector2& v) const;
	const Vector2& operator+=(const Vector2& v);
	const Vector2& operator-=(const Vector2& v);
	const Vector2& operator*=(float v);
	const Vector2& operator/=(float v);
	const Vector2 operator+(const Vector2& v) const;
	const Vector2 operator-(const Vector2& v) const;
	const Vector2 operator-() const;
	Vector2 operator/(float s) const;
	Vector2 operator*(float s) const;
	
	float Angle() const;
public:
	float x;
	float y;
};

#endif
