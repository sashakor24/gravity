#ifndef POINT_HPP
#define POINT_HPP

#include <cmath>

class Point
{
	double x;
	double y;
public:
	double len()
	{
		return sqrt(x * x + y * y);
	}
	Point getUnit()
	{
		Point p;
		if(len() != 0) p.set(x / len(), y / len());
		return p;
	}
	Point()
	{
		x = 0;
		y = 0;
	}
	Point(double x1, double y1)
	{
		x = x1;
		y = y1;
	}
	Point(const Point& other)
	{
		x = other.x;
		y = other.y;
	}
	void set(double x1, double y1)
	{
		x = x1;
		y = y1;
	}
	void setX(double x1)
	{
		x = x1;
	}
	void setY(double y1)
	{
		y = y1;
	}
	double getX()
	{
		return x;
	}
	double getY()
	{
		return y;
	}
	bool check(double x1, double y1)
	{
		if (x == x1 && y == y1) return true;
		return false;
	}
	bool operator==(const Point& other)
	{
		return check(other.x,other.y);
	}
	bool operator!=(const Point& other)
	{
		return !check(other.x,other.y);
	}
	void operator=(const Point& other)
	{
		x = other.x;
		y = other.y;
	}
	Point operator-() const
	{
		Point p(-x,-y);
		return p;
	}
	friend Point operator+(const Point&, const Point&);
	friend Point operator-(const Point&, const Point&);
	friend Point operator*(const Point&, double);
	friend Point operator*(double, const Point&);
	friend Point operator/(const Point&, double);
	void operator+=(const Point& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator*=(double n)
	{
		x *= n;
		y *= n;
	}
	void operator/=(double n)
	{
		operator*=(1 / n);
	}
	void operator-=(const Point& other)
	{
		operator+=(-other);
	}
};

Point operator+(const Point& p1, const Point& p2)
{
	Point p(p1);
	p += p2;
	return p;
}
Point operator-(const Point& p1, const Point& p2)
{
	return operator+(p1,-p2);
}
Point operator*(const Point& p, double n)
{
	Point p1(p);
	p1 *= n;
	return p1;
}
Point operator*(double n, const Point& p)
{
	return operator*(p,n);
}
Point operator/(const Point& p, double n)
{
	return operator*(p,1 / n);
} 

#endif