#include "SphereTree.h"
#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include "MD2.h"

SphereTree::SphereTree(Vector3f *verts, int *triIndices, int numTriangles) : vertices(verts)
{
	CreateSphereTree(triIndices, numTriangles, root);
}

SphereTree::SphereTree(const MorphAnimation *pAnimatedMesh)
{
	numVertices = pAnimatedMesh->numVertices;
	numFrames   = pAnimatedMesh->numFrames;
//	numTris     = pAnimatedMesh->numTris;
	frames      = new Frame[numFrames];
	
	for(int i = 0; i < numFrames; i++)
	{
		for(int j = 0; j < numVertices; j++)
			frames[i].vertices[j] = pAnimatedMesh->frames[i].vertices[j];
	}

	Vector3f *vertexAverage = new Vector3f[numVertices];
	for(int i = 0; i < pAnimatedMesh->numFrames; i++)
	{
		for(int j = 0; j < pAnimatedMesh->numVertices; j++)
			vertexAverage[i] = pAnimatedMesh->frames[i].vertices[j];
	}
//	CreateSphereTree(triIndices, numTriangles, root);
}

SphereTree::~SphereTree(void)
{
}

void SphereTree::SplitTriangles(int *indicesIn, int numTrisIn, const Plane3f &plane, 
								int *&indicesOut0, int &numTrisOut0, int *&indicesOut1, int &numTrisOut1)
{

	numTrisOut0 = 0;
	numTrisOut1 = 0;

	for(int i = 0; i < numTrisIn; i++)
	{
		Vector3f a, b, c;
		a = vertices[indicesIn[i*3+0]];
		b = vertices[indicesIn[i*3+1]];
		c = vertices[indicesIn[i*3+2]];

		int side = WhichSideIsTriangle(plane, a, b, c);

		if(side == 0)	// straddles the plane
			side = plane.WhichSide((a + b + c) / 3.0);

		if(side == -1)
		{
			numTrisOut0++;
			indicesOut0[numTrisOut0*3 - 3] = indicesIn[i*3+0];
			indicesOut0[numTrisOut0*3 - 2] = indicesIn[i*3+1];
			indicesOut0[numTrisOut0*3 - 1] = indicesIn[i*3+2];
		}
		else if(side == 1)
		{
			numTrisOut1++;
			indicesOut1[numTrisOut1*3 - 3] = indicesIn[i*3+0];
			indicesOut1[numTrisOut1*3 - 2] = indicesIn[i*3+1];
			indicesOut1[numTrisOut1*3 - 1] = indicesIn[i*3+2];
		}
	}
}


void SphereTree::FindContainingBox(int *indices, int numTris, AABB &aabb)
{
	aabb.boxMin = aabb.boxMax = vertices[indices[0]];

	for(int i = 1; i < numTris * 3; i++)
	{
		aabb.AddPoint(vertices[indices[i]]); 
	}
}

void SphereTree::CreateSphereTree(int *indices, int numTris, Node *&root, bool deleteIndices, int depth)
{
	AABB aabb;
	Plane3f splittingPlane;
	int *indicesOut0 = 0;
	int *indicesOut1 = 0;
	int numTrisOut0 = 0;
	int numTrisOut1 = 0;

	FindContainingBox(indices, numTris, aabb);

	root = new Node;
	root->center = 0.5f * (aabb.boxMax + aabb.boxMin);
	root->radius = (aabb.boxMax - root->center).Length();

	const int MIN_TRIANGLES = 1;
	const int MAX_DEPTH = 16;
	if(numTris <= MIN_TRIANGLES || depth >= MAX_DEPTH)// leaf node
	{ 
		std::cout<<"depth: "<<depth<<", numTris: "<<numTris<<std::endl;
		root->numTris = numTris;
		root->indices = new int[numTris * 3];
		for(int i = 0; i < numTris * 3; i++)
			root->indices[i] = indices[i];
		root->children[0] = root->children[1] = 0;
		return;
	}

	float spanX = aabb.boxMax.x - aabb.boxMin.x;
	float spanY = aabb.boxMax.y - aabb.boxMin.y;
	float spanZ = aabb.boxMax.z - aabb.boxMin.z;

	indicesOut0 = new int[numTris * 3];
	indicesOut1 = new int[numTris * 3];

	if(spanX > spanY && spanX > spanZ)
	{
		splittingPlane.normal = Vector3f(1, 0, 0);
		splittingPlane.constant = -Dot(splittingPlane.normal, root->center);
		SplitTriangles(indices, numTris, splittingPlane, indicesOut0, numTrisOut0, indicesOut1, numTrisOut1);
	}
	if(spanY > spanX && spanY > spanZ || numTrisOut0 == 0 || numTrisOut1 == 0)
	{
		splittingPlane.normal = Vector3f(0, 1, 0);
		splittingPlane.constant = -Dot(splittingPlane.normal, root->center);
		SplitTriangles(indices, numTris, splittingPlane, indicesOut0, numTrisOut0, indicesOut1, numTrisOut1);
	}
	if(numTrisOut0 == 0 || numTrisOut1 == 0)
	{
		splittingPlane.normal = Vector3f(0, 0, 1);
		splittingPlane.constant = -Dot(splittingPlane.normal, root->center);
		SplitTriangles(indices, numTris, splittingPlane, indicesOut0, numTrisOut0, indicesOut1, numTrisOut1);
	}
	if(numTrisOut0 == 0 || numTrisOut1 == 0)
	{
		numTrisOut0 = numTris/2;
		numTrisOut1 = numTris - numTris/2;
		for(int i = 0; i < numTrisOut0*3; i++)
			indicesOut0[i] = indices[i];
		for(int j = 0; j < numTrisOut1*3; j++)
			indicesOut1[j] = indices[j+numTrisOut0*3];
	}


	if(deleteIndices)
		delete[] indices;

	if(numTrisOut0 > 0)
		CreateSphereTree(indicesOut0, numTrisOut0, root->children[0], true, depth+1);
	if(numTrisOut1 > 0)
		CreateSphereTree(indicesOut1, numTrisOut1, root->children[1], true, depth+1);
}

	int i = 0;

void SphereTree::Draw(Node *node) const
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	i++;

	glColor3ub(255 - i * 50, i * 50, i * 50);


	glPushMatrix();
	glTranslatef(node->center.x, node->center.y, node->center.z);
	//glutWireSphere(node->radius, 10, 10);
	glPopMatrix();

	if(node->children[0])
		Draw(node->children[0]);
	if(node->children[1])
		Draw(node->children[1]);

	if(node->IsLeaf())
	{
		glBegin(GL_TRIANGLES);
		for(int i = 0; i < node->numTris * 3; i++)
		{	
			glVertex3fv(vertices[node->indices[i]]);
		}
		glEnd();
	}
}
/*

bool SphereTree::IntersectTriangles(const Node *nodeA, const Vector3f& positionA, const Matrix4x4f &rotationA, 
									const Node *nodeB, const Vector3f& positionB, const Matrix4x4f &rotationB)
{
	Vector3f posArelB = positionB - positionA;

	for(int i = 0; i < nodeA->indices.size() / 3; i++)
		for(int j = 0; j < nodeB->indices.size() / 3; j++)
		{
			if(TriangleIntersectTriangle(
				vertices[nodeA->indices[i*3+0]] + posArelB, 
				verticesA[nodeA->indices[i*3+1]] + posArelB, 
				verticesA[nodeA->indices[i*3+2]] + posArelB,
				verticesB[nodeB->indices[j*3+0]], 
				verticesB[nodeB->indices[j*3+1]],
				verticesB[nodeB->indices[j*3+2]]))
			{
				return true;
			}
		}
	return false;
}

bool SphereTreeIntersectSphereTree(const Node *nodeA, const Vector3f& positionA, const Matrix4x4f &rotationA, 
								   const Node *nodeB, const Vector3f& positionB, const Matrix4x4f &rotationB)
{
	if(nodeA == 0 || nodeB == 0)
		return false;

	Vector3f posArelB = positionB - positionA;

	if(!SphereIntersectSphere(nodeA->center + posArelB, nodeA->radius, nodeB->center, nodeB->radius))
		return false;

	if(nodeA->IsLeaf() && nodeB->IsLeaf())
		return IntersectTriangles(nodeA, verticesA, positionA, rotationA, nodeB, verticesB, positionB, rotationB);

	else if(nodeB->IsLeaf())
	{
		if(SphereTreeIntersectSphereTree(nodeA->children[0], verticesA, positionA, rotationA, nodeB, verticesB, positionB, rotationB))
			return true;
		else if(SphereTreeIntersectSphereTree(nodeA->children[1], verticesA, positionA, rotationA, nodeB, verticesB, positionB, rotationB))
			return true;
		else
			return false;
	}
	else if(nodeA->IsLeaf())
	{
		if(SphereTreeIntersectSphereTree(nodeA, verticesA, positionA, rotationA, nodeB->children[0], verticesB, positionB, rotationB))
			return true;
		else if(SphereTreeIntersectSphereTree(nodeA, verticesA, positionA, rotationA, nodeB->children[1], verticesB, positionB, rotationB))
			return true;
		else
			return false;
	}
	else
	{

		if(SphereTreeIntersectSphereTree(nodeA->children[0], verticesA, positionA, rotationA, nodeB->children[0], verticesB, positionB, rotationB))
			return true;
		if(SphereTreeIntersectSphereTree(nodeA->children[0], verticesA, positionA, rotationA, nodeB->children[1], verticesB, positionB, rotationB))
			return true;
		else if(SphereTreeIntersectSphereTree(nodeA->children[1], verticesA, positionA, rotationA, nodeB->children[0], verticesB, positionB, rotationB))
			return true;
		else if(SphereTreeIntersectSphereTree(nodeA->children[1], verticesA, positionA, rotationA, nodeB->children[1], verticesB, positionB, rotationB))
			return true;
		else
			return false;
	}
}
*/

int SphereTree::WhichSideIsTriangle(const Plane3f &plane, const Vector3f &a, const Vector3f &b, const Vector3f &c) // -1 neg, 0 straddles, 1 pos
{
		int s0 = plane.WhichSide(a);
		int s1 = plane.WhichSide(b);
		int s2 = plane.WhichSide(c);
		if(s0 == -1 && s1 == -1 && s2 == -1)
			return -1;
		else if(s0 == 1 && s1 == 1 && s2 == 1)
			return 1;
		else
			return 0;
}
