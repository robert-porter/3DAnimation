#pragma once

template<class Real>
struct Vector2 
{
	float x, y;

	operator const float *() { return &x; }
};

typedef Vector2<float> Vector2f;