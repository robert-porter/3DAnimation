#pragma once

#include <cstdlib>
#include "Vector3.h"


template<class Real>
class Matrix4x4
{
public:
	Matrix4x4() { }
	//NOTE: OpenGL matrices are stored in column order, 
	//		these arguments are passed in a row major format
	//      for better formating.  (Should I change this??)
	Matrix4x4(Real m00, Real m01, Real m02, Real m03,
			  Real m10, Real m11, Real m12, Real m13,
			  Real m20, Real m21, Real m22, Real m23,
			  Real m30, Real m31, Real m32, Real m33);
	Matrix4x4(Real *f);

	operator const Real * () const;
	operator Real * ();

	Matrix4x4<Real>& operator - () const;

	Matrix4x4<Real> operator + (const Matrix4x4<Real> &rhs) const;
	Matrix4x4<Real> operator - (const Matrix4x4<Real> &rhs) const;
	Matrix4x4<Real> operator * (const Matrix4x4<Real> &rhs) const;
	Matrix4x4<Real> operator * (Real rhs) const;
	Matrix4x4<Real> operator / (Real denom) const;

	Matrix4x4<Real> &operator += (const Matrix4x4<Real> &rhs);
	Matrix4x4<Real> &operator -= (const Matrix4x4<Real> &rhs);
	Matrix4x4<Real> &operator *= (const Matrix4x4<Real> &rhs);
	Matrix4x4<Real> &operator *= (Real rhs);
	Matrix4x4<Real> &operator /= (Real denom);

	Matrix4x4<Real> Transpose() const { 
		return Matrix4x4<Real>(
			m[0],  m[1],  m[2],  m[3],
			m[4],  m[5],  m[6],  m[7],
			m[8],  m[9],  m[10], m[11],
			m[12], m[13], m[14], m[15]);
	}

	Matrix4x4 TransposeTimes(const Matrix4x4<Real> &) const;
	Matrix4x4 TimesTranspose(const Matrix4x4<Real> &) const;

	Real	  Determinant() const;
	Matrix4x4 Adjoint() const;
	Matrix4x4 Inverse() const;
	Real      Trace() const;

	Vector3<Real> TransformPoint(const Vector3<Real> &p) const; //this*<v,1>^T
	Vector3<Real> TransformVector(const Vector3<Real> &v) const; //this*<v,0>^T
	
	static Matrix4x4<Real> Identity();
	static Matrix4x4<Real> Translation(const Vector3<Real> &v);
	static Matrix4x4<Real> RotationX(Real theta);
	static Matrix4x4<Real> RotationY(Real theta);
	static Matrix4x4<Real> RotationZ(Real theta);
	static Matrix4x4<Real> RotationAxis(const Vector3<Real> &axis, Real angle);
	static Matrix4x4<Real> Scale(const Vector3<Real> &s);

	Real m[16];
};

template<class Real>
Matrix4x4<Real> operator * (Real lhs, const Matrix4x4<Real> &rhs);

#include "Matrix4x4.inl"

typedef Matrix4x4<float> Matrix4x4f;
typedef Matrix4x4<double> Matrix4x4d;

