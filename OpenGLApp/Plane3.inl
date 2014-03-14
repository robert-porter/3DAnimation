


template<class Real> 
Plane3<Real>::Plane3(const Vector3<Real> &n, Real d) : noraml(n), constant(d)
{
}

template<class Real> 
Plane3<Real>::Plane3(const Vector3<Real> &n, const Vector3<Real> &point) : normal(n)
{
	normal = n;
	constant = Dot(normal, point);
}

template<class Real> 
Plane3<Real>::Plane3(const Vector3<Real> &p1, const Vector3<Real> &p2, const Vector3<Real> p3)
{
	Vector3<Real> e1 = p2 - p1;
	Vector3<Real> e2 = p3 - p2;
	
	normal = Cross(e1, e2);
	constant = Dot(normal, p1);
}

template<class Real> 
int Plane3<Real>::WhichSide(const Vector3<Real> &p) const
{
	return (int) Sign(Dot(p, normal) - constant); 
}