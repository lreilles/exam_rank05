#pragma once

#include <iostream>

class vect2 {
	public:
		vect2();
		vect2(int x, int y);
		vect2(const vect2 &other);
		vect2 &operator=(vect2 other);
	
		int &operator[](int index);
		int operator[](int index) const;
	
		vect2 &operator+=(vect2 other);
		vect2 &operator-=(vect2 other);
		vect2 &operator*=(int scalar);
	
		vect2 &operator++();   // prefix
		vect2 operator++(int); // postfix
		vect2 &operator--();   // prefix
		vect2 operator--(int); // postfix
	
		friend vect2 operator+(vect2 lhs, vect2 rhs);
		friend vect2 operator-(vect2 lhs, vect2 rhs);
		friend vect2 operator-(vect2 value);
	
		friend vect2 operator*(vect2 lhs, int scalar);
		friend vect2 operator*(int scalar, vect2 rhs);
	
		friend bool operator==(vect2 lhs,  vect2 rhs);
		friend bool operator!=(vect2 lhs, vect2 rhs);
	
		friend std::ostream &operator<<(std::ostream &os, vect2 value);
	
	private:
		int x_;
		int y_;
};
