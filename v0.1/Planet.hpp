#ifndef PLANET_HPP
#define PLANET_HPP

#include "Point.hpp"

struct Color
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	Color()
	{
		r = 255;
		g = 255;
		b = 255;
	}
	Color(GLubyte r, GLubyte g, GLubyte b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
	void set(GLubyte r, GLubyte g, GLubyte b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
	void get(GLubyte& r, GLubyte& g, GLubyte& b)
	{
		r = this->r;
		g = this->g;
		b = this->b;
	}
};

class Planet
{
	double m,r;
public:
	Point p,v;
	Color c;
	Planet(double m, double r, const Point& p, const Point& v, const Color& c)
	{
		this->m = m;
		this->r = r;
		this->p = p;
		this->v = v;
		this->c = c;
	}
	Planet()
	{
		m = 0;
		r = 0;
	}
	void set(double m, double r, const Point& p, const Point& v, const Color& c)
	{
		this->m = m;
		this->r = r;
		this->p = p;
		this->v = v;
		this->c = c;
	}
	double getM()
	{
		return m;
	}
	double getR()
	{
		return r;
	}
};

#endif