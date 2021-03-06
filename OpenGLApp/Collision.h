#pragma once

#include "Vector3.h"
#include "Md2.h"
#include "Matrix4x4.h"


bool SphereIntersectSphere(const Vector3f &centerA, float radiusA, const Vector3f &centerB, float radiusB);

bool RayIntersectSphere(const Vector3f &center, float radius, Vector3f origin, const Vector3f &direction);
bool RayIntersectTriangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Vector3f &origin, const Vector3f &direction, float &t);
bool RayIntersectMd2AnimatedModel(const Md2AnimatedModel *pModel, const Vector3f &modelPosition, 
								  const Matrix4x4f &modelRotation, Vector3f origin, Vector3f direction, float &tMin, float &tMax);
bool SegmentIntersectSphere(const Vector3f &center, float radius, const Vector3f &P0, const Vector3f &P1);
bool SegmentIntersectTriangle(const Vector3f &a, const Vector3f&b, const Vector3f& c, const Vector3f &P0, const Vector3f &P1, float &t);
bool SegmentIntersectMd2AnimatedModel(const Md2AnimatedModel *pModel, const Vector3f &modelPosition, const Matrix4x4f &modelRotation,
						  Vector3f P0, Vector3f P1, float &tMin, float &tMax);

bool Md2AnimatedModelIntersectMd2AnimatedModel(const Md2AnimatedModel *pModelA, const Vector3f &modelPositionA, const Matrix4x4f &modelRotationA,
											   const Md2AnimatedModel *pModelB, const Vector3f &modelPositionB, const Matrix4x4f &modelRotationB);

bool TriangleIntersectTriangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Vector3f &d, const Vector3f &e, const Vector3f &f);


