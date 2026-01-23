#include "vect2.hpp"

vect2::vect2() : x_(0), y_(0)
{}

vect2::vect2(int x, int y) : x_(x), y_(y)
{}

vect2::vect2(const vect2 &other) : x_(other.x_), y_(other.y_)
{}

vect2 &vect2::operator=(vect2 other)
{
	if (this != &other)
	{
		x_ = other.x_;
		y_ = other.y_;
	}
	return (*this);
}

int &vect2::operator[](int index)
{
	if (index == 0)
		return (x_);
	return (y_);
}

int vect2::operator[](int index) const
{
	if (index == 0)
		return (x_);
	return (y_);
}

vect2 &vect2::operator+=(vect2 other)
{
	x_ += other.x_;
	y_ += other.y_;
	return (*this);
}

vect2 &vect2::operator-=(vect2 other)
{
	x_ -= other.x_;
	y_ -= other.y_;
	return (*this);
}

vect2 &vect2::operator*=(int scalar)
{
	x_ *= scalar;
	y_ *= scalar;
	return (*this);
}

vect2 &vect2::operator++()
{
	++x_;
	++y_;
	return (*this);
}

vect2 vect2::operator++(int)
{
	++(*this);
	return (*this);
}

vect2 &vect2::operator--()
{
	--x_;
	--y_;
	return (*this);
}

vect2 vect2::operator--(int)
{
	--(*this);
	return (*this);
}

vect2 operator+(vect2 lhs,  vect2 rhs)
{
	lhs += rhs;
	return (lhs);
}

vect2 operator-(vect2 lhs, vect2 rhs)
{
	lhs -= rhs;
	return (lhs);
}

vect2 operator-(vect2 value)
{
	return (vect2(-value.x_, -value.y_));
}

vect2 operator*(vect2 lhs, int scalar)
{
	lhs *= scalar;
	return (lhs);
}

vect2 operator*(int scalar, vect2 rhs)
{
	rhs *= scalar;
	return (rhs);
}

bool operator==( vect2 lhs,  vect2 rhs)
{
	return (lhs).x_ == rhs.x_ && lhs.y_ == rhs.y_;
}

bool operator!=(vect2 lhs, vect2 rhs)
{
	return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, vect2 value)
{
	os << "{" << value.x_ << ", " << value.y_ << "}";
	return (os);
}
