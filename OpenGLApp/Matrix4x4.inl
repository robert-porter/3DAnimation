
template<class Real>
Matrix4x4<Real>::Matrix4x4(
		Real m00, Real m01, Real m02, Real m03,
		Real m10, Real m11, Real m12, Real m13,
		Real m20, Real m21, Real m22, Real m23,
		Real m30, Real m31, Real m32, Real m33)
{
	m[0] = m00;
	m[1] = m10;
	m[2] = m20;
	m[3] = m30;
	m[4] = m01;
	m[5] = m11;
	m[6] = m21;
	m[7] = m31;
	m[8] = m02;
	m[9] = m12;
	m[10] = m22;
	m[11] = m32;
	m[12] = m03;
	m[13] = m13;
	m[14] = m23;
	m[15] = m33;
}

template<class Real>
Matrix4x4<Real>::Matrix4x4(Real *f) 
{ 
	memcpy(m, f, 16 * sizeof(Real)); 
}

template<class Real>
Matrix4x4<Real>::operator const Real*() const 
{ 
	return m; 
}

template<class Real>
Matrix4x4<Real>::operator Real*() 
{ 
	return m; 
}

template<class Real>
Matrix4x4<Real>& Matrix4x4<Real>::operator - () const
{
	return Matrix4x4<Real>(
					 -m[0], -m[4], -m[8],  -m[12],
					 -m[1], -m[5], -m[9],  -m[13],
					 -m[2], -m[6], -m[10], -m[14],
					 -m[3], -m[7], -m[11], -m[15]);
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::operator + (const Matrix4x4<Real> &rhs) const
{
	return Matrix4x4<Real>(
					 m[0] + rhs.m[0], m[4] + rhs.m[4], m[8] + rhs.m[8],  m[12] + rhs.m[12],
					 m[1] + rhs.m[1], m[5] + rhs.m[5], m[9] + rhs.m[9],  m[13] + rhs.m[13],
					 m[2] + rhs.m[2], m[6] + rhs.m[6], m[10] + rhs.m[10], m[14] + rhs.m[14],
					 m[3] + rhs.m[3], m[7] + rhs.m[7], m[11] + rhs.m[11], m[15] + rhs.m[15])
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::operator - (const Matrix4x4<Real> &rhs) const
{
	return Matrix4x4<Real>(
					 m[0] - rhs.m[0], m[4] - rhs.m[4], m[8] - rhs.m[8],  m[12] - rhs.m[12],
					 m[1] - rhs.m[1], m[5] - rhs.m[5], m[9] - rhs.m[9],  m[13] - rhs.m[13],
					 m[2] - rhs.m[2], m[6] - rhs.m[6], m[10] - rhs.m[10], m[14] - rhs.m[14],
					 m[3] - rhs.m[3], m[7] - rhs.m[7], m[11] - rhs.m[11], m[15] - rhs.m[15])
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::operator * (const Matrix4x4<Real> &rhs) const
{
	return Matrix4x4<Real>(
		m[0]*rhs.m[0]  + m[4]*rhs.m[1]  + m[8]*rhs.m[2]   + m[12]*rhs.m[3],
		m[0]*rhs.m[4]  + m[4]*rhs.m[5]  + m[8]*rhs.m[6]   + m[12]*rhs.m[7],
		m[0]*rhs.m[8]  + m[4]*rhs.m[9]  + m[8]*rhs.m[10]  + m[12]*rhs.m[11],
		m[0]*rhs.m[12] + m[4]*rhs.m[13] + m[8]*rhs.m[14]  + m[12]*rhs.m[15],
		m[1]*rhs.m[0]  + m[5]*rhs.m[1]  + m[9]*rhs.m[2]   + m[13]*rhs.m[3],
		m[1]*rhs.m[4]  + m[5]*rhs.m[5]  + m[9]*rhs.m[6]   + m[13]*rhs.m[7],
		m[1]*rhs.m[8]  + m[5]*rhs.m[9]  + m[9]*rhs.m[10]  + m[13]*rhs.m[11],
		m[1]*rhs.m[12] + m[5]*rhs.m[13] + m[9]*rhs.m[14]  + m[13]*rhs.m[15],
		m[2]*rhs.m[0]  + m[6]*rhs.m[1]  + m[10]*rhs.m[2]  + m[14]*rhs.m[3],
		m[2]*rhs.m[4]  + m[6]*rhs.m[5]  + m[10]*rhs.m[6]  + m[14]*rhs.m[7],
		m[2]*rhs.m[8]  + m[6]*rhs.m[9]  + m[10]*rhs.m[10] + m[14]*rhs.m[11],
		m[2]*rhs.m[12] + m[6]*rhs.m[13] + m[10]*rhs.m[14] + m[14]*rhs.m[15],
		m[3]*rhs.m[0]  + m[7]*rhs.m[1]  + m[11]*rhs.m[2]  + m[15]*rhs.m[3],
		m[3]*rhs.m[4]  + m[7]*rhs.m[5]  + m[11]*rhs.m[6]  + m[15]*rhs.m[7],
		m[3]*rhs.m[8]  + m[7]*rhs.m[9]  + m[11]*rhs.m[10] + m[15]*rhs.m[11],
		m[3]*rhs.m[12] + m[7]*rhs.m[13] + m[11]*rhs.m[14] + m[15]*rhs.m[15]);
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::operator * (Real rhs) const
{
	return Matrix4x4<Real>(
					 m[0] * rhs, m[4] * rhs, m[8] * rhs,  m[12] * rhs,
					 m[1] * rhs, m[5] * rhs, m[9] * rhs,  m[13] * rhs,
					 m[2] * rhs, m[6] * rhs, m[10] * rhs, m[14] * rhs,
					 m[3] * rhs, m[7] * rhs, m[11] * rhs, m[15] * rhs);
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::operator / (Real denom) const
{
	Real inv = Real(1.0) / denom;

	return Matrix4x4<Real>(
					 m[0] * inv, m[4] * inv, m[8] * inv,  m[12] * inv,
					 m[1] * inv, m[5] * inv, m[9] * inv,  m[13] * inv,
					 m[2] * inv, m[6] * inv, m[10] * inv, m[14] * inv,
					 m[3] * inv, m[7] * inv, m[11] * inv, m[15] * inv);
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator += (const Matrix4x4<Real> &rhs)
{

	m[0] += rhs.m[0];
	m[1] += rhs.m[1];
	m[2] += rhs.m[2];
	m[3] += rhs.m[3];
	m[4] += rhs.m[4];
	m[5] += rhs.m[5];
	m[6] += rhs.m[6];
	m[7] += rhs.m[7];
	m[8] += rhs.m[8];
	m[9] += rhs.m[9];
	m[10] += rhs.m[10];
	m[11] += rhs.m[11];
	m[12] += rhs.m[12];
	m[13] += rhs.m[13];
	m[14] += rhs.m[14];
	m[15] += rhs.m[15];
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator -= (const Matrix4x4<Real> &rhs)
{
	m[0] -= rhs.m[0];
	m[1] -= rhs.m[1];
	m[2] -= rhs.m[2];
	m[3] -= rhs.m[3];
	m[4] -= rhs.m[4];
	m[5] -= rhs.m[5];
	m[6] -= rhs.m[6];
	m[7] -= rhs.m[7];
	m[8] -= rhs.m[8];
	m[9] -= rhs.m[9];
	m[10] -= rhs.m[10];
	m[11] -= rhs.m[11];
	m[12] -= rhs.m[12];
	m[13] -= rhs.m[13];
	m[14] -= rhs.m[14];
	m[15] -= rhs.m[15];
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator *= (const Matrix4x4<Real> &rhs)
{
	// TODO:
	operator * (rhs);
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator *= (Real rhs)
{
	m[0] *= rhs;
	m[1] *= rhs;
	m[2] *= rhs;
	m[3] *= rhs;
	m[4] *= rhs;
	m[5] *= rhs;
	m[6] *= rhs;
	m[7] *= rhs;
	m[8] *= rhs;
	m[9] *= rhs;
	m[10] *= rhs;
	m[11] *= rhs;
	m[12] *= rhs;
	m[13] *= rhs;
	m[14] *= rhs;
	m[15] *= rhs;
	return *this;
}

template<class Real>
Matrix4x4<Real> &Matrix4x4<Real>::operator /= (Real denom)
{
	Real inv = Real(1.0) / denom;

	m[0] *= inv;
	m[1] *= inv;
	m[2] *= inv;
	m[3] *= inv;
	m[4] *= inv;
	m[5] *= inv;
	m[6] *= inv;
	m[7] *= inv;
	m[8] *= inv;
	m[9] *= inv;
	m[10] *= inv;
	m[11] *= inv;
	m[12] *= inv;
	m[13] *= inv;
	m[14] *= inv;
	m[15] *= inv;
	return *this;
}

template<class Real>
Matrix4x4<Real> operator * (Real lhs, const Matrix4x4<Real> rhs)
{
	return Matrix4x4<Real>(
					 lhs * rhs.m[0], lhs * rhs.m[4], lhs * rhs.m[8],  lhs * rhs.m[12],
					 lhs * rhs.m[1], lhs * rhs.m[5], lhs * rhs.m[9],  lhs * rhs.m[13],
					 lhs * rhs.m[2], lhs * rhs.m[6], lhs * rhs.m[10], lhs * rhs.m[14],
					 lhs * rhs.m[3], lhs * rhs.m[7], lhs * rhs.m[11], lhs * rhs.m[15]);
}

template<class Real>
Vector3<Real> Matrix4x4<Real>::TransformPoint(const Vector3<Real> &p) const
{

	return Vector3<Real>(m[0]*p.x + m[4]*p.y + m[8]*p.z + m[12],
						 m[1]*p.x + m[5]*p.y + m[9]*p.z + m[13],
						 m[2]*p.x + m[6]*p.y + m[10]*p.z + m[14]);

}

template<class Real>
Vector3<Real> Matrix4x4<Real>::TransformVector(const Vector3<Real> &v) const
{

	return Vector3<Real>(m[0]*v.x + m[4]*v.y + m[8]*v.z,
						 m[1]*v.x + m[5]*v.y + m[9]*v.z,
						 m[2]*v.x + m[6]*v.y + m[10]*v.z);

}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::Identity()
{
	return Matrix4x4<Real>(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);

}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::Translation(const Vector3<Real> &v)
{
	return Matrix4x4<Real>(
		1.0, 0.0, 0.0, v.x,
		0.0, 1.0, 0.0, v.y,
		0.0, 0.0, 1.0, v.z,
		0.0, 0.0, 0.0, 1.0);

}
template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::RotationX(Real theta)
{	
	Real c = cos(theta);
	Real s = sin(theta);

	return Matrix4x4<Real>(
		1.0, 0.0, 0.0, 0.0,
		0.0, c,   -s,  0.0,
		0.0, s,   c,   0.0,
		0.0, 0.0, 0.0, 1.0);

}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::RotationY(Real theta)
{
	Real c = cos(theta);
	Real s = sin(theta);

	return Matrix4x4<Real>(
		c,   0.0,   s, 0.0,
		0.0, 1.0, 0.0, 0.0,
		-s,  0.0, c,   0.0,
		0.0, 0.0, 0.0, 1.0);
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::RotationZ(Real theta)
{
	Real c = cos(theta);
	Real s = sin(theta);

	return Matrix4x4<Real>(
		c,   -s,  0.0, 0.0,
		s,   c,   0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::RotationAxis(const Vector3<Real> &axis, Real angle)
{
	Real rcos = cos(angle);
	Real rsin = sin(angle);

	Real m[] = {
			rcos + axis.x*axis.x*(1-rcos), 
			axis.z * rsin + axis.y*axis.x*(1-rcos),
			-axis.y * rsin + axis.z*axis.x*(1-rcos),
			0,
			-axis.z * rsin + axis.x*axis.y*(1-rcos),
			rcos + axis.y*axis.y*(1-rcos),
			axis.x * rsin + axis.z*axis.y*(1-rcos),
			0,
			axis.y * rsin + axis.x*axis.z*(1-rcos),
			-axis.x * rsin + axis.y*axis.z*(1-rcos),
			rcos + axis.z*axis.z*(1-rcos),
			0,
			0,
			0,
			0,
			1 };
	return Matrix4x4<Real>(m);
}

template<class Real>
Matrix4x4<Real> Matrix4x4<Real>::Scale(const Vector3<Real> &s)
{
	return Matrix4x4<Real>(s.x, 0,   0,   0,
						   0,   s.y, 0,   0,
						   0,   0,   s.z, 0, 
						   0,   0,   0,   1);
}
