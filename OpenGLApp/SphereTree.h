#pragma once

#include "Vector3.h"
#include "Matrix4x4.h"
#include "Plane3.h"
#include "Md2.h"
#include <vector>

struct AABB
{
	Vector3f boxMin, boxMax;

	AABB() { }
	void AddPoint(const Vector3f &p)
	{
		if(p.x < boxMin.x)
			boxMin.x = p.x;
		if(p.y < boxMin.y)
			boxMin.y = p.y;
		if(p.z < boxMin.z)
			boxMin.z = p.z;
		if(p.x > boxMax.x)
			boxMax.x = p.x;
		if(p.y > boxMax.y)
			boxMax.y = p.y;
		if(p.z > boxMax.z)
			boxMax.z = p.z;

	}
};


class SphereTree
{
public:
	SphereTree(Vector3f *vertices, int *triIndices, int numTriangles);
	SphereTree(const MorphAnimation *pAnimatedMesh);

	~SphereTree(void);

	void Draw() { Draw(root); }
	
private:
	struct Node
	{	
		Vector3f center;
		float radius;
		int *indices;
		int numTris;
		Node *children[2];

		Node() { children[0] = children[1] = 0; }

		bool IsLeaf() const { return children[0] == 0 && children[1] == 0; }
	};

	struct Frame
	{
		Vector3f *vertices;
		Node	 *root;
	};

	Vector3f	*vertices;
	int			numVertices;
	Node		*root;

	Frame       *frames;
	int			numFrames;

	void SplitTriangles(int *indicesIn, int numTrisIn, const Plane3f &plane, 
				   int *&indicesOut0, int &numTrisOut0, int *&indicesOut1, int &numTrisOut1);
	void FindContainingBox(int *indices, int numTris, AABB &aabb);

	void CreateSphereTree(int *indices, int numTris, Node *&root, bool deleteIndices = false, int depth = 0);

	void Draw(Node *node) const;
	/*

	bool SphereTreeIntersectSphereTree(const Node *nodeA, const Vector3f *verticesA, const Vector3f& positionA, const Matrix4x4f &rotationA, 
									   const Node *nodeB, const Vector3f *verticesB, const Vector3f& positionB, const Matrix4x4f &rotationB);

   //IntersectLeaves
	bool SphereTree::IntersectTriangles(const Node *nodeA, const Vector3f& positionA, const Matrix4x4f &rotationA, 
									const Node *nodeB, const Vector3f& positionB, const Matrix4x4f &rotationB);
*/
	int WhichSideIsTriangle(const Plane3f &plane, const Vector3f &a, const Vector3f &b, const Vector3f &c); // -1 neg, 0 straddles, 1 pos
	
};





