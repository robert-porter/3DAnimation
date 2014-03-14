#pragma once

#include "Vector3.h"

// defined as Dot(X,normal) = constant, where X is any point on the plane

template<class Real>
class Plane3
{
public:
	Vector3<Real> normal;
	Real constant;

	Plane3() { }
	Plane3(const Vector3<Real> &n, Real d);
	Plane3(const Vector3<Real> &n, const Vector3<Real> &p);
	Plane3(const Vector3<Real> &p0, const Vector3<Real> &p1, const Vector3<Real> p2);

	int WhichSide(const Vector3<Real> &p) const; 
};

#include "Plane3.inl"

typedef Plane3<float> Plane3f;
typedef Plane3<double> Plane3d;