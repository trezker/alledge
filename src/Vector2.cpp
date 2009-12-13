#include "../alledge/Vector2.h"
#include <cmath>

Vector2::Vector2()
:x(0), y(0)
{
}

Vector2::Vector2(float ix, float iy)
:x(ix), y(iy)
{}

void Vector2::Set(float ix, float iy)
{
	x=ix;
	y=iy;
}

const Vector2& Vector2::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

bool Vector2::operator!=(const Vector2& v) const
{
	return !operator==(v);
}

bool Vector2::operator==(const Vector2& v) const
{
	if(x==v.x && y==v.y)
	{
		return true;
	}
	return false;
}

float Vector2::X() const
{
	return x;
}

float Vector2::Y() const
{
	return y;
}

float Vector2::Length() const
{
	return sqrt(Length_squared());
}

float Vector2::Length_squared() const
{
	return x*x+y*y;
}

void Vector2::Normalize()
{
	float length=Length();
	if(length!=0)
	{
		x/=length;
		y/=length;
	}
}

Vector2 Vector2::Normalized() const
{
	float length=Length();
	if(length!=0)
	{
		return Vector2(x/length, y/length);
	}
	return *this;
}

float Vector2::Dotproduct(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

const Vector2& Vector2::operator+=(const Vector2& v)
{
	x+=v.x;
	y+=v.y;
	return *this;
}

const Vector2& Vector2::operator-=(const Vector2& v)
{
	x-=v.x;
	y-=v.y;
	return *this;
}

const Vector2& Vector2::operator*=(float v)
{
	x*=v;
	y*=v;
	return *this;
}

const Vector2& Vector2::operator/=(float v)
{
	x/=v;
	y/=v;
	return *this;
}

const Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(x-v.x, y-v.y);
}

const Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(x+v.x, y+v.y);
}

void Vector2::Zero()
{
	x=0;
	y=0;
}

Vector2 Vector2::operator/(float s) const
{
	Vector2 temp;
	temp.Set(x/s, y/s);
	return temp;
}

Vector2 Vector2::operator*(float s) const
{
	Vector2 temp;
	temp.Set(x*s, y*s);
	return temp;
}

float Vector2::Angle() const
{
	return atan2(y, x);
}
