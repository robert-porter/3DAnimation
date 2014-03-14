#include "Terrain.h"
#include "mymath.h"
#include "Texture.h"

Terrain::Terrain(void)
{
	numCellsX = 20;
	numCellsZ = 20;
	cellSize = 1.0;

	heights = new float[(numCellsX + 1) * (numCellsZ + 1)];

	for(int x = 0; x < numCellsX + 1; x++)
		for(int z = 0; z < numCellsZ + 1; z++)
		{
			float fx = PI - 2 * PI * x / float(numCellsX);
			float fz = PI - 2 * PI * z / float(numCellsZ);
			heights[x + z * (numCellsX + 1)] = sin(0.7*fx*fz);
		}
}

Terrain::~Terrain(void)
{
	if(heights)
		delete[] heights;
}

void Terrain::Init()
{
	textureID = LoadBitmap("particle.bmp");
}

float Terrain::GetHeight(float x, float z)
{
	int ix = x / cellSize;
	int iz = z / cellSize;

	if(ix < 0 || ix >= numCellsX || iz < 0 || iz > numCellsZ)
		return 0.0f;

	float fx = x - ix * cellSize;
	float fz = z - iz * cellSize;
	fx /= cellSize; 
	fz /= cellSize;
	//ab
	//cd
	float a = heights[ix + iz * (numCellsX + 1)]; 
	float b = heights [(ix + 1) + iz * (numCellsX + 1)];
	float c = heights[ix + (iz + 1) * (numCellsX + 1)];
	float d = heights[(ix + 1) + (iz + 1) * (numCellsX + 1)];


	if(fx > 1.0f - fz)
	{
		return b + (1.0 - fx)*(a-b) + fz*(d-b); 
	}
	else
	{		
		return c + fx*(d-c) + (1.0 - fz)*(a-c); 
	}

}

Vector3f Terrain::GetNormal(float x, float z)
{
	// The point lies in cell (x/cellSize, z/cellSize),
	// but we still need to find out what triangle it lies in.
	// Our triangles split the quad on the line x = 1 - z
	// so the upper triangle is x > 1 - z and the lower x < 1 - z
	//[x,z]   [x+1,z]
	//[x,z+1] [x+1,z+1]
/*
	int ix = x/cellSize;
	int iz = z/cellSize;

	if(ix < 0 || ix >= numCellsX || iz < 0 || iz > numCellsZ)
		return Vector3f(0,1,0);

	float fx = x - ix * cellSize;
	float fz = z - iz * cellSize;
	fx /= cellSize;
	fz /= cellSize;

	Vector3f a, b, c, ac, ab, n;


	if(fx > 1.0f - fz)
	{
		a = Vector3f(ix*cellSize, heights[ix + iz * (numCellsX + 1)], iz*cellSize), 
		b = Vector3f((ix + 1)*cellSize, heights[(ix + 1) + iz * (numCellsX + 1)], iz * cellSize),
		c = Vector3f((ix + 1) * cellSize, heights[(ix + 1) + (iz + 1) * (numCellsX + 1)], (iz + 1) * cellSize);

	}
	else
	{		
		a = Vector3f(ix*cellSize, heights[ix + iz * (numCellsX + 1)], iz*cellSize);
		b = Vector3f((ix+1) * cellSize, heights[(ix + 1) + (iz + 1) * (numCellsX + 1)], (iz + 1) * cellSize);
		c = Vector3f(ix * cellSize, heights[ix + (iz + 1) * (numCellsX + 1)], (iz + 1) * cellSize);
	}
	ac = c-a;
	ab = b-a;
	n = ac.Cross(ab);
	n.Normalize();

	return n;
*/
	float h0 = GetHeight(x+cellSize, z);
	float h1 = GetHeight(x-cellSize, z);
	float h2 = GetHeight(x, z+cellSize);
	float h3 = GetHeight(x, z-cellSize);

	Vector3f normal(h1-h0, h3-h2, 1.0f);
	normal.Normalize();
	return normal;
}


void Terrain::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBegin(GL_TRIANGLES);
	for(int x = 0; x < numCellsX; x++)
		for(int z = 0; z < numCellsZ; z++)
		{
			Vector3f a(x * cellSize, heights[x + z * (numCellsX + 1)], z * cellSize), 
				     b((x + 1) * cellSize, heights[(x + 1) + z * (numCellsX + 1)], z * cellSize),
					 c((x + 1) * cellSize, heights[(x + 1) + (z + 1) * (numCellsX + 1)], (z + 1) * cellSize);
			Vector3f ac = c-a;
			Vector3f ab = b-a;
			Vector3f n = ac.Cross(ab);
			n.Normalize();

			glNormal3f(n.x, n.y, n.z);
			glTexCoord2f((float)x/(float)numCellsX, (float)z/(float)numCellsZ);
			glVertex3f(a.x, a.y, a.z);
			glTexCoord2f((float)(x + 1)/(float)numCellsX, (float)z/(float)numCellsZ);
			glVertex3f(b.x, b.y, b.z);
			glTexCoord2f((float)(x + 1)/(float)numCellsX, (float)(z + 1)/(float)numCellsZ);
			glVertex3f(c.x, c.y, c.z);

			a = Vector3f(x * cellSize, heights[x + z * (numCellsX + 1)], z * cellSize);
			b = Vector3f((x + 1) * cellSize, heights[(x + 1) + (z + 1) * (numCellsX + 1)], (z + 1) * cellSize);
			c = Vector3f(x * cellSize, heights[x + (z + 1) * (numCellsX + 1)], (z + 1) * cellSize);
			ac = c-a;
			ab = b-a;
			n = ac.Cross(ab);
			n.Normalize();

			glNormal3f(n.x, n.y, n.z);
			glTexCoord2f((float)(x)/(float)numCellsX, (float)z/(float)numCellsZ);
			glVertex3f(a.x, a.y, a.z);
			glTexCoord2f((float)(x + 1)/(float)numCellsX, (float)(z + 1)/(float)numCellsZ);
			glVertex3f(b.x, b.y, b.z);
			glTexCoord2f((float)(x)/(float)numCellsX, (float)(z + 1)/(float)numCellsZ);
			glVertex3f(c.x, c.y, c.z);

		}
	glEnd();

	glDisable(GL_TEXTURE_2D);

/*
	// draws the normals
	glBegin(GL_LINES);
	for(int x = 0; x < numCellsX; x++)
		for(int z = 0; z < numCellsZ; z++)
		{
			Vector3f a(x * cellSize, heights[x + z * (numCellsX + 1)], z * cellSize), 
				     b((x + 1) * cellSize, heights[(x + 1) + z * (numCellsX + 1)], z * cellSize),
					 c((x + 1) * cellSize, heights[(x + 1) + (z + 1) * (numCellsX + 1)], (z + 1) * cellSize);
			Vector3f ac = c-a;
			Vector3f ab = b-a;
			Vector3f n = ac.Cross(ab);
			n.Normalize();

			Vector3f baryCenter = (a + b + c) / 3.0;
			n = GetNormal(baryCenter.x, baryCenter.z);
			Vector3f end = baryCenter + n * 0.3;
			glVertex3fv(baryCenter);
			glVertex3fv(end);

			a = Vector3f(x * cellSize, heights[x + z * (numCellsX + 1)], z * cellSize);
			b = Vector3f((x + 1) * cellSize, heights[(x + 1) + (z + 1) * (numCellsX + 1)], (z + 1) * cellSize);
			c = Vector3f(x * cellSize, heights[x + (z + 1) * (numCellsX + 1)], (z + 1) * cellSize);
			ac = c-a;
			ab = b-a;
			n = ac.Cross(ab);
			n.Normalize();

			baryCenter = (a + b + c) / 3.0;
			n = GetNormal(baryCenter.x, baryCenter.z);
			end = baryCenter + n * 0.3;
			glVertex3fv(baryCenter);
			glVertex3fv(end);

		}
	glEnd();
	*/
}