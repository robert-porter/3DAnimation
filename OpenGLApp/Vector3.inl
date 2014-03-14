
template<class Real>
Vector3<Real>::Vector3() { }

template<class Real>
Vector3<Real>::Vector3(Real X, Real Y, Real Z) : x(X), y(Y), z(Z) {}

template<class Real>
Vector3<Real>::Vector3(const float *f) : x(f[0]), y(f[1]), z(f[2]) { }

template<class Real>
Real Vector3<Real>::Length() const
{ 
	return sqrt(x * x + y * y + z * z); 
}

template<class Real>
Real Vector3<Real>::SquaredLength() const 
{ 
	return x * x + y * y + z * z; 
}

template<class Real>
Real Vector3<Real>::Normalize()
{ 
	Real length = Length();
	x /= length; y /= length; z /= length;
	return length;
}

template<class Real>
Real Vector3<Real>::Dot( const Vector3 &v ) 
{ 
	return x * v.x + y * v.y + z * v.z;
}

template<class Real>
Vector3<Real> Vector3<Real>::Cross( Vector3 b ) 
{ 
	return Vector3( y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x ); 
}

template<class Real>
Vector3<Real>& Vector3<Real>::operator += ( const Vector3& v )
{
	x += v.x; y += v.y; z += v.z; return *this; 
}

template<class Real>
Vector3<Real>& Vector3<Real>::operator -= ( const Vector3& v ) 
{ 
	x -= v.x; y -= v.y; z -= v.z; return *this; 
}

template<class Real>
Vector3<Real>& Vector3<Real>::operator *= ( Real f ) 
{ 
	x *= f; y *= f; z *= f; return *this;
}

template<class Real>
Vector3<Real>& Vector3<Real>::operator /= ( Real f ) 
{ 
	x /= f; y /= f; z /= f; return *this;
}

template<class Real>
Vector3<Real> Vector3<Real>::operator - () const
{ 
	return Vector3(-x, -y, -z);
}

template<class Real>
Vector3<Real> Vector3<Real>::operator + (const Vector3& v2) const 
{ 
	return Vector3(x + v2.x, y + v2.y, z + v2.z); 
}

template<class Real>
Vector3<Real> Vector3<Real>::operator - (const Vector3& v2) const 
{ 
	return Vector3(x - v2.x, y - v2.y, z - v2.z); 
}

template<class Real>
Vector3<Real> Vector3<Real>::operator * (Real f) const 
{ 
	return Vector3( x * f, y * f, z * f ); 
}

template<class Real>
Vector3<Real> Vector3<Real>::operator / (Real f) const 
{ 
	return Vector3(x/f, y/f, z/f); 
}

template<class Real>
Vector3<Real> operator * (Real f, const Vector3<Real>& v) 
{ 
	return Vector3<Real>(v.x * f, v.y * f, v.z * f); 
}

template<class Real>
Real Dot(const Vector3<Real> &v1, const Vector3<Real> &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; 
}

template<class Real>
Vector3<Real> Cross(const Vector3<Real> &a, const Vector3<Real> &b)
{
	return Vector3<Real>( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x ); 
}