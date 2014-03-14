#pragma once

#include "mymath.h"

template<class Real>
class Vector3
{
public:
	Real x, y, z;

	Vector3();
	Vector3(Real, Real, Real);
	Vector3(const float *);
	
	Real Length() const;
	Real SquaredLength() const;
	Real Normalize();

	Real Dot(const Vector3 &);
	Vector3 Cross(Vector3);

	operator float*() { return (float *)&x; }
	operator const float * () const { return (const float *)&x; }

	Vector3& operator += (const Vector3&);
	Vector3& operator -= (const Vector3&);
	Vector3& operator *= (Real);
	Vector3& operator /= (Real);

	Vector3 operator - () const;

	Vector3 operator + (const Vector3&) const;
	Vector3 operator - (const Vector3&) const;
	Vector3 operator * (Real) const;
	Vector3 operator / (Real) const;
};

template<class Real>
Real Dot(const Vector3<Real> &, const Vector3<Real> &);

template<class Real>
Vector3<Real> Cross(const Vector3<Real> &, const Vector3<Real> &);

template<class Real>
Vector3<Real> operator * (Real, const Vector3<Real>&);

#include "Vector3.inl"

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;