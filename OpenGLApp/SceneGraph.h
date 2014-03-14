#include "Matrix4x4.h"
#include <vector>

class Node	// represents a coordinate frame
{
public:
	virtual void Update()
	{
		if(parent)
			world = local * parent->world;
		else
			world = local;

		for(int i = 0; i < children.size(); i++)
			children[i]->Update();
	}
protected:
	Spacial *parent;
	std::vector<Spacial *> children;
	Matrix4x4f local, world;
};


class Geometry : public Spacial
{
public:

private:
	Vector3f *vertices;
	Vector3f *normals;

	int *indices;
};