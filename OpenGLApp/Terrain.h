#pragma once

#include <windows.h>
#include "GL/gl.h"
#include "Vector3.h"

class Terrain
{
public:
	Terrain(void);
	~Terrain(void);

	void Init();

	void Draw();

	float GetHeight(float x, float z);
	Vector3f GetNormal(float x, float z);

private:
	int numCellsX;
	int numCellsZ;

	float cellSize;

	float *heights;

	GLuint textureID;
};
