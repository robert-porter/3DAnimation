#include "Collision.h"
#include "mymath.h"
#include "Matrix4x4.h"
#include "glut.h"

bool RayIntersectSphere(const Vector3f &center, float radius, Vector3f origin, const Vector3f &direction)
{
	// a point, p, on a sphere satisfies Dot(p, p) = r*r
	// plugging in the equation of the ray in the sphere local coordinate space
	// Dot(o+t*d, o+t*d) = r*r
	// Dot(o, o) + 2 * Dot(o, t*d) + Dot(td, td) - r*r = 0;
	// Dot(o, o) - r * r + t * 2 * Dot(o, d) + t^2 * Dot(d, d) = 0
	// which we can solve by the quadratic formula

	origin -= center;
	float a = Dot(origin, origin) - radius * radius;
	float b = 2.0 * Dot(origin, direction);
	float c = Dot(direction, direction);

	float discriminant = b*b - 4 * a * c;
	if(discriminant < 0.0f)	// only imaginary solutions
		return false;
	float t0 = -b + sqrtf(discriminant) / (2.0 * a);
	float t1 = -b - sqrtf(discriminant) / (2.0 * a);

	if(t0 < 0.0 && t1 < 0.0)	// on the wrong side of the ray
		return false;

	return true;
}

bool RayIntersectTriangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Vector3f &origin, const Vector3f &direction, float &t)
{

	//the point of intersection is 
	// o + d*t = alpha*ab + beta*ac, t>0, alpha>0, beta>0, alpha+beta<1
	// Shoud I seperate this into 2 parts or just solve the 3x3 system all at once???

	// first solve for the time of intersection of the ray with the plane containing the triangle

	// equation of a plane Dot(x-p,n) = 0, where p is a point on the plane, since the triangle is on the plane
	// we can use one of it's vertices
	// n = the plane/triangle normal (ab)x(ac)
	// and we plug in our ray equation for x and solve for t

	// Dot(o + t*d - a, n) = 0
	// Dot(o-a,n) + t*Dot(d,n) = 0
	// t = -Dot(o-a,n) / Dot(d,n)

	Vector3f ab = b-a;
	Vector3f ac = c-a;
	Vector3f n = Cross(ab, ac);
	float DdN = Dot(direction, n);

	const float EPSILON = 0.001f;
	if(fabs(DdN) < EPSILON)	// ray parallel to the plane
		return false;

	t = -Dot(origin - a, n) / DdN;

	if(t < 0.0f)	// ray points away from the plane
		return false;

	// next check to see if the point lies in the triangle
	// p = alpha*ab + beta*ac, 
	// where p is the point at which the ray intersects the plane,
	// relative to the triangle
	// this gives a system of 3 equation with 2 unknowns
	// in general
	// solving for alpha/beta
	// p.x = alpha * ab.x + beta*ac.x
	// [p.x - alpha * ab.x] / ac.x = beta
	// p.y = alpha * ab.y + ([p.x - alpha * ab.x] / ac.x) * ac.y
	// p.y * ac.x = alpha * ab.y * ac.x + p.x * ac.y - alpha * ab.x * ac.y
	// (p.y * ac.x - p.x * ac.y) / (ab.y * ac.x - ab.x * ac.y) = alpha
	// [p.x - beta*ac.x]/ab.x = alpha
	// p.y = beta * ac.y + ([p.x - beta*ac.x]/ab.x)*ab.y
	// p.y * ab.x = beta * ac.y * ab.x + p.x * ab.y - beta * ac.x * ab.y
	// (p.y * ab.x - p.x * ab.y) / (ac.y * ab.x - ac.x * ab.y) = beta

	// but if the triangle lies in one of the cardinal planes
	// we get a system like
	// p.x = alpha*ab.x + beta*ac.x
	// p.y = alpha*0 + beta*0
	// with infinite solutions

	Vector3f p = origin + t * direction - a;


	float alpha = -1, beta = -1;
	if(ab.x == 0 && ac.x == 0)
	{
		alpha = (p.y * ac.z - p.z * ac.y) / (ab.y * ac.z - ab.z * ac.y); 
		beta  = (p.y * ab.z - p.z * ab.y) / (ac.y * ab.z - ac.z * ab.y) ;
	}
	else if(ab.y == 0 && ac.y == 0)
	{
		alpha = (p.z * ac.x - p.x * ac.z) / (ab.z * ac.x - ab.x * ac.z); 
		beta  = (p.z * ab.x - p.x * ab.z) / (ac.z * ab.x - ac.x * ab.z);
	}
	else
	{
		alpha = (p.y * ac.x - p.x * ac.y) / (ab.y * ac.x - ab.x * ac.y); 
		beta  = (p.y * ab.x - p.x * ab.y) / (ac.y * ab.x - ac.x * ab.y);
	}

	if(alpha > 0.0f && beta > 0.0f && alpha + beta < 1.0f)	// point in triangle
		return true;

	else return false;
}


bool RayIntersectMd2Model(const MorphAnimation *pModel, int frame, const Vector3f &modelPosition, const Matrix4x4f &modelRotation,
						  Vector3f origin, Vector3f direction, float &tMin, float &tMax)
{
	Matrix4x4f invRotation = modelRotation.Transpose();
	origin -= modelPosition;
	origin = invRotation.TransformPoint(origin);
	direction = invRotation.TransformVector(direction);


	const float INFINITY = 100000;
	tMin = INFINITY;
	tMax = -INFINITY;
	bool foundHit = false;
	Vector3f *vertices = pModel->frames[frame].vertices;
	int *indices = pModel->vertexIndices;

	for(int i = 0; i < pModel->numTris; i++)
	{
		float t;
		Vector3f a = vertices[indices[i*3+0]];
		Vector3f b = vertices[indices[i*3+1]];
		Vector3f c = vertices[indices[i*3+2]];
		if(RayIntersectTriangle(a, b, c, origin, direction, t))
		{
			foundHit = true;

			if(t < tMin)
				tMin = t;
			if(t > tMax)
				tMax = t;
		}
	}

	return foundHit;
}

bool RayIntersectMd2AnimatedModel(const Md2AnimatedModel *pModel, const Vector3f &modelPosition, const Matrix4x4f &modelRotation,
						  Vector3f origin, Vector3f direction, float &tMin, float &tMax)
{

	
	int frame0 = pModel->animState.curr_frame;
	int frame1 = pModel->animState.next_frame;

	float r0 = pModel->GetModel()->frames[frame0].radius;
	float r1 = pModel->GetModel()->frames[frame1].radius;
	float r = max(r0, r1);

	if(!RayIntersectSphere(modelPosition, r, origin, direction))
		return false;

	Matrix4x4f invRotation = modelRotation.Transpose();
	origin -= modelPosition;
	origin = invRotation.TransformPoint(origin);
	direction = invRotation.TransformVector(direction);

	const float INFINITY = 100000;
	tMin = INFINITY;
	tMax = -INFINITY;
	bool foundHit = false;

	float t = pModel->animState.interpol;

	Vector3f *vertices0 = pModel->GetModel()->frames[frame0].vertices;
	Vector3f *vertices1 = pModel->GetModel()->frames[frame1].vertices;

	int *indices = pModel->GetModel()->vertexIndices;

	for(int i = 0; i < pModel->GetModel()->numTris; i++)
	{
		float time;
		Vector3f a = vertices0[indices[i*3+0]] + t * (vertices1[indices[i*3+0]] - vertices0[indices[i*3+0]]);
		Vector3f b = vertices0[indices[i*3+1]] + t * (vertices1[indices[i*3+1]] - vertices0[indices[i*3+1]]);
		Vector3f c = vertices0[indices[i*3+2]] + t * (vertices1[indices[i*3+2]] - vertices0[indices[i*3+2]]);
		if(RayIntersectTriangle(a, b, c, origin, direction, time))
		{
			foundHit = true;

			if(time < tMin)
				tMin = time;
			if(time > tMax)
				tMax = time;
		}
	
	}


	return foundHit;
}

bool SegmentIntersectTriangle(const Vector3f &a, const Vector3f&b, const Vector3f& c, const Vector3f &P0, const Vector3f &P1, float &t)
{

	// the point of intersection is 
	// P0 + t*(P1 - P0) = alpha*ab + beta*ac, 0<t<1, alpha>0, beta>0, alpha+beta<1

	// first solve for the time of intersection of the segment with the plane containing the triangle

	// equation of a plane Dot(x-p,n) = 0, where p is a point on the plane, since the triangle is on the plane
	// we can use one of it's vertices
	// n = the plane/triangle normal (ab)x(ac)
	// and we plug in our segment equation for x and solve for t

	// Dot(P0 + t*(P1 - P0) - a, n) = 0
	// Dot(P0-a,n) + t*Dot(P1 - P0,n) = 0
	// t = -Dot(P0-a,n) / Dot(P1 - P0,n)

	Vector3f ab = b-a;
	Vector3f ac = c-a;
	Vector3f n = Cross(ab, ac);
	float DdN = Dot(P1 - P0, n);

	const float EPSILON = 0.001f;
	if(fabs(DdN) < EPSILON)	// segment parallel to the plane
		return false;

	t = -Dot(P0 - a, n) / DdN;

	if(t < 0.0f || t > 1.0)	// ray points away from the plane
		return false;

	// next check to see if the point lies in the triangle
	// p = alpha*ab + beta*ac, 
	// where p is the point at which the ray intersects the plane,
	// relative to the triangle
	// this gives a system of 3 equation with 2 unknowns
	// in general
	// solving for alpha/beta
	// p.x = alpha * ab.x + beta*ac.x
	// [p.x - alpha * ab.x] / ac.x = beta
	// p.y = alpha * ab.y + ([p.x - alpha * ab.x] / ac.x) * ac.y
	// p.y * ac.x = alpha * ab.y * ac.x + p.x * ac.y - alpha * ab.x * ac.y
	// (p.y * ac.x - p.x * ac.y) / (ab.y * ac.x - ab.x * ac.y) = alpha
	// [p.x - beta*ac.x]/ab.x = alpha
	// p.y = beta * ac.y + ([p.x - beta*ac.x]/ab.x)*ab.y
	// p.y * ab.x = beta * ac.y * ab.x + p.x * ab.y - beta * ac.x * ab.y
	// (p.y * ab.x - p.x * ab.y) / (ac.y * ab.x - ac.x * ab.y) = beta

	// but if the triangle lies in one of the cardinal planes
	// we get a system like
	// p.x = alpha*ab.x + beta*ac.x
	// p.y = alpha*0 + beta*0
	// with infinite solutions

	Vector3f p = P0 + t * (P1 - P0) - a;


	float alpha = -1, beta = -1;
	if(ab.x == 0 && ac.x == 0)
	{
		alpha = (p.y * ac.z - p.z * ac.y) / (ab.y * ac.z - ab.z * ac.y); 
		beta  = (p.y * ab.z - p.z * ab.y) / (ac.y * ab.z - ac.z * ab.y) ;
	}
	else if(ab.y == 0 && ac.y == 0)
	{
		alpha = (p.z * ac.x - p.x * ac.z) / (ab.z * ac.x - ab.x * ac.z); 
		beta  = (p.z * ab.x - p.x * ab.z) / (ac.z * ab.x - ac.x * ab.z);
	}
	else
	{
		alpha = (p.y * ac.x - p.x * ac.y) / (ab.y * ac.x - ab.x * ac.y); 
		beta  = (p.y * ab.x - p.x * ab.y) / (ac.y * ab.x - ac.x * ab.y);
	}

	if(alpha > 0.0f && beta > 0.0f && alpha + beta < 1.0f)	// point in triangle
		return true;

	else return false;
}


bool SegmentIntersectMd2AnimatedModel(const Md2AnimatedModel *pModel, const Vector3f &modelPosition, const Matrix4x4f &modelRotation,
						  Vector3f P0, Vector3f P1, float &tMin, float &tMax)
{

	
	int frame0 = pModel->animState.curr_frame;
	int frame1 = pModel->animState.next_frame;

	float r0 = pModel->GetModel()->frames[frame0].radius;
	float r1 = pModel->GetModel()->frames[frame1].radius;
	float r = max(r0, r1);

	//if(!RayIntersectSphere(modelPosition, r, origin, direction))
	//	return false;

	Matrix4x4f invRotation = modelRotation.Transpose();
	P0 -= modelPosition;
	P1 -= modelPosition;
	P0 = invRotation.TransformPoint(P0);
	P1 = invRotation.TransformPoint(P1);

	const float INFINITY = 100000;
	tMin = INFINITY;
	tMax = -INFINITY;
	bool foundHit = false;

	float t = pModel->animState.interpol;

	Vector3f *vertices0 = pModel->GetModel()->frames[frame0].vertices;
	Vector3f *vertices1 = pModel->GetModel()->frames[frame1].vertices;

	int *indices = pModel->GetModel()->vertexIndices;

	for(int i = 0; i < pModel->GetModel()->numTris; i++)
	{
		float time;

		Vector3f a = vertices0[indices[i*3+0]] + t * (vertices1[indices[i*3+0]] - vertices0[indices[i*3+0]]);
		Vector3f b = vertices0[indices[i*3+1]] + t * (vertices1[indices[i*3+1]] - vertices0[indices[i*3+1]]);
		Vector3f c = vertices0[indices[i*3+2]] + t * (vertices1[indices[i*3+2]] - vertices0[indices[i*3+2]]);
		if(SegmentIntersectTriangle(a, b, c, P0, P1, time))
		{
			return true;
			foundHit = true;

			if(time < tMin)
				tMin = time;
			if(time > tMax)
				tMax = time;
		}
	
	}


	return foundHit;
}

bool SegmentIntersectSphere(const Vector3f &center, float radius, const Vector3f &P0, const Vector3f &P1)
{
	return true;
}

bool SphereIntersectSphere(const Vector3f &centerA, float radiusA, const Vector3f &centerB, float radiusB)
{
	// the spheres intersect when |Cb-Ca| < ra + rb, or |Cb-Ca|^2 < (ra+rb)^2
	return (centerB - centerA).SquaredLength() < (radiusA + radiusB) * (radiusA + radiusB);
}

bool Md2AnimatedModelIntersectMd2AnimatedModel(const Md2AnimatedModel *pModelA, const Vector3f &modelPositionA, const Matrix4x4f &modelRotationA,
											   const Md2AnimatedModel *pModelB, const Vector3f &modelPositionB, const Matrix4x4f &modelRotationB)
{	
	int frameA0 = pModelA->animState.curr_frame;
	int frameA1 = pModelA->animState.next_frame;

	float rA0 = pModelA->GetModel()->frames[frameA0].radius;
	float rA1 = pModelA->GetModel()->frames[frameA1].radius;
	float rA = max(rA0, rA1);

	int frameB0 = pModelB->animState.curr_frame;
	int frameB1 = pModelB->animState.next_frame;

	float rB0 = pModelB->GetModel()->frames[frameA0].radius;
	float rB1 = pModelB->GetModel()->frames[frameA1].radius;
	float rB = max(rB0, rB1);

	if(!SphereIntersectSphere(modelPositionA, rA, modelPositionB, rB))
		return false;


	float tA = pModelA->animState.interpol;
	Vector3f *verticesA0 = pModelA->GetModel()->frames[frameA0].vertices;
	Vector3f *verticesA1 = pModelA->GetModel()->frames[frameA1].vertices;
	int *edgesA = pModelA->GetModel()->edges;

	bool foundHit = false;


	for(int i = 0; i < pModelA->GetModel()->numEdges; i++)
	{
		float tMin, tMax;
		Vector3f P0 = verticesA0[edgesA[i*2+0]] + tA * (verticesA1[edgesA[i*2+0]] - verticesA0[edgesA[i*2+0]]);
		Vector3f P1 = verticesA0[edgesA[i*2+1]] + tA * (verticesA1[edgesA[i*2+1]] - verticesA0[edgesA[i*2+1]]);

		P0 = modelRotationA.TransformPoint(P0) + modelPositionA;
		P1 = modelRotationA.TransformPoint(P1) + modelPositionA;

		if(SegmentIntersectMd2AnimatedModel(pModelB, modelPositionB, modelRotationB, P0, P1, tMin, tMax))
			return true;
	
	}

	return false;
}

bool TriangleIntersectTriangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Vector3f &d, const Vector3f &e, const Vector3f &f)
{
	float t;

	if(SegmentIntersectTriangle(a, b, c, d, e, t))
		return true;
	if(SegmentIntersectTriangle(a, b, c, d, f, t))
		return true;
	if(SegmentIntersectTriangle(a, b, c, e, f, t))
		return true;
	if(SegmentIntersectTriangle(d, e, f, a, b, t))
		return true;
	if(SegmentIntersectTriangle(d, e, f, a, c, t))
		return true;
	if(SegmentIntersectTriangle(d, e, f, b, c, t))
		return true;
	
	return false;
}