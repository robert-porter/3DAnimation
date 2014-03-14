#pragma once

//point, line, circle, ellipse, sphere, a-3D ellipsoid, triangle, square, hexagon, and a cube
#include "Matrix4x4.h"

void DrawEllipse(float a, float b, int n = 30);
void DrawCone(float h, float r);
void DrawPyramid(float length, float width, float height);
void DrawCylinder();
void DrawBox(float hx, float hy, float hz);
void DrawEllipsoid(float a, float b, float c, int numLatDivisions, int numLongDivisions);
void DrawSphere(float r, int numLatDivisions, int numLongDivisions);



// I draw the planar objects on the xz plane

class Drawable
{
public:
	Drawable() { worldMatrix = Matrix4x4f::Identity(); }
	Drawable(const Matrix4x4f &W) { worldMatrix = W; } 

	virtual void Draw() = 0;

	Matrix4x4f worldMatrix;
};

class Drawable_Point : public Drawable
{
public:
	void Draw()
	{
		glBegin(GL_POINTS);
		glVertex3f(0, 0, 0);
		glEnd();
	}
};

class Drawable_Line : public Drawable // a line segment
{
public:
	void Draw()
	{
		glBegin(GL_LINES);
		glVertex3f(-0.5f, 0, 0);
		glVertex3f(0.5f, 0, 0);
		glEnd();
	}
};



class Drawable_Circle : public Drawable
{
public:
	void Draw()
	{
		DrawEllipse(1.0f, 1.0f);
	}
};

class Drawable_Ellipse : public Drawable 
{
public:
	void Draw()
	{
		DrawEllipse(1.5, 1.0);
	}
};

class Drawable_Sphere : public Drawable
{
public:
	void Draw()
	{
		DrawSphere(1, 20, 20);
	}
};

class Drawable_Ellipsoid : public Drawable
{
public:
	void Draw()
	{
		DrawEllipsoid(1, 1.5, 0.7, 45, 45);
	}
};

class Drawable_Triangle : public Drawable
{
public:
	void Draw()
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(0, 1, 0);
		glVertex3f(-0.5f, 0, 0);
		glVertex3f(0.5f, 0, 0);
		glVertex3f(0.0f, 0, 1.0f);
		glEnd();
	}
};

class Drawable_Square : public Drawable
{
public:
	void Draw()
	{
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glVertex3f(0.5, 0.5, 0);
		glVertex3f(0.5, -0.5, 0);
		glVertex3f(-0.5, -0.5, 0);
		glVertex3f(-0.5, 0.5, 0);
		glEnd();
	}
};

class Drawable_Hexagon : public Drawable
{
public:
	void Draw()
	{
		DrawEllipse(1, 1, 6);
	}
};

class Drawable_Cube : public Drawable
{
public:
	void Draw()
	{
		DrawBox(0.5, 0.5, 0.5f);
	}
};