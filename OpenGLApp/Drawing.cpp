#include <cmath>
#include <windows.h>
#include <gl\gl.h>
#include "glut.h"
#include "mymath.h"


void DrawEllipse(float a, float b, int n)
{

	glBegin(GL_TRIANGLE_FAN);

	glNormal3f(0, 1, 0);
	for(int t = 0; t < n - 1; t++)
	{
		float theta = 2 * PI * float(t) / float(n);
		glVertex3f(cosf(theta) * a, 0, sinf(theta)* b);
	}

	glEnd();
}

void DrawCone(float h, float r)
{
	// Draw a right circular cone with the axis, y=0
	const int n = 16;
	const float delta = 1.0 / float(n);

	glBegin(GL_TRIANGLE_FAN);
	
	glVertex3f(0,0,0);
	
	for(int i = 0; i < n + 1; i++)
	{
		float theta = float(i)*delta*2.0*PI;
		glVertex3f(r*cosf(theta) , r*sinf(theta), h);

	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	
	for(int i = 0; i < n; i++)
	{
		float theta = float(i)*delta*2.0*PI;
		glVertex3f(r*cosf(theta) , r*sinf(theta), h);

	}
	glEnd();
}


void DrawPyramid(float length, float width, float height)
{
	float halfLength = length * 0.5f;
	float halfWidth = width * 0.5f;

	glBegin(GL_TRIANGLES);
	glColor3f(1,0,0);
	glVertex3f(-halfLength, -halfWidth, 0);
	glVertex3f(0, 0, height);
	glVertex3f(-halfLength, halfWidth, 0);
	glColor3f(0,1,0);
	glVertex3f(-halfLength, halfWidth, 0);
	glVertex3f(0, 0, height);
	glVertex3f(halfLength, halfWidth, 0);
	glColor3f(0,0,1);
	glVertex3f(halfLength, halfWidth, 0);
	glVertex3f(0, 0, height);
	glVertex3f(halfLength, -halfWidth, 0);
	glColor3f(1,1,0);
	glVertex3f(halfLength, -halfWidth, 0);
	glVertex3f(0, 0, height);
	glVertex3f(-halfLength, -halfWidth, 0);
	glEnd();
}

void DrawCylinder()
{
	// with the axis y=0
	const int n = 20;
	float h = 1.0f;
	float r = 0.5f;

	glBegin(GL_TRIANGLE_STRIP);
	
	for(int i = 0; i < n + 1; i++)
	{
		float theta1 = float(i) * 2.0 * PI / float(n);
		float theta2 = float(i+1) * 2.0 * PI / float(n);

		glVertex3f(r*cosf(theta1) , r*sinf(theta1), 0);
		glVertex3f(r*cosf(theta2), r * sinf(theta2), h);
	}

	//doesn't have ends

	glEnd();
}


void DrawBox(float hx, float hy, float hz)
{

	glBegin(GL_QUADS);
		
		glNormal3f(0, 0, -1);
		glVertex3f(-hx, hy, hz);
		glVertex3f(-hx, -hy, hz);
		glVertex3f(hx, -hy, hz);
		glVertex3f(hx, hy, hz);

		glNormal3f(0, 0, 1);
		glVertex3f(-hx, -hy, -hz);
		glVertex3f(hx, -hy, -hz);
		glVertex3f(hx, hy, -hz);
		glVertex3f(-hx, hy, -hz);

		glNormal3f(-1, 0, 0);
		glVertex3f(hx, -hy, hz);
		glVertex3f(hx, -hy, -hz);
		glVertex3f(hx, hy, -hz);
		glVertex3f(hx, hy, hz);

		glNormal3f(1, 0, 0);
		glVertex3f(-hx, -hy, hz);
		glVertex3f(-hx, -hy, -hz);
		glVertex3f(-hx, hy, -hz);
		glVertex3f(-hx, hy, hz);

		glNormal3f(0, -1, 0);
		glVertex3f(-hx, hy, hz);
		glVertex3f(-hx, hy, -hz);
		glVertex3f(hx, hy, -hz);
		glVertex3f(hx, hy, hz);

		glNormal3f(0, 1, 0);
		glVertex3f(-hx, -hy, hz);
		glVertex3f(-hx, -hy, -hz);
		glVertex3f(hx, -hy, -hz);
		glVertex3f(hx, -hy, hz);
	glEnd();
}

void DrawEllipsoid(float a, float b, float c, int numLatDivisions, int numLongDivisions) 
{
	// polar equation of a line
	// x = r*cos(theta)sin(phi)
	// y = r*sin(theta)sin(phi)
	// z = r*cos(phi)

	float dtheta = 2.0 * PI / (float)numLongDivisions;
	float dphi = PI / (float)numLatDivisions;

	for(float theta = 0; theta < 2.0 * PI; theta += dtheta) 
	{
		float theta0 = theta - dtheta;
		float theta1 = theta;

		glBegin(GL_QUAD_STRIP);
		for(float phi = 0; phi < PI + dphi; phi += dphi) 
		{
			float sintheta0 = sin(theta0);
			float sintheta1 = sin(theta1);
			float costheta0 = cos(theta0);
			float costheta1 = cos(theta1);
			float sinphi = sin(phi);
			float cosphi = cos(phi);
		
			float x0 = a*costheta0*sinphi;
			float y0 = b*sintheta0*sinphi;
			float x1 = a*costheta1*sinphi;
			float y1 = b*sintheta1*sinphi;
			float z  = c*cosphi;
			float r0 = sqrtf(x0 * x0 + y0 * y0 + z * z);
			float r1 = sqrtf(x1 * x1 + y1 * y1 + z * z);

			glNormal3f(-x0/r0, -y0/r0, -z/r0);
			glVertex3f(x0, y0, z);
			glNormal3f(-x1/r1, -y1/r1, -z/r1);
			glVertex3f(x1, y1, z);
       }
       glEnd();
	}
}

void DrawSphere(float r, int numLatDivisions, int numLongDivisions)
{
	DrawEllipsoid(r, r, r, numLatDivisions, numLongDivisions);
}