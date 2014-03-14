#pragma once

#include <cmath>

const double PI = 3.141592653589793238462;
const double DEG_TO_RAD = PI / 180.0;
const double RAD_TO_DEG = 180.0 / PI;

template<class Real>
Real Sign(Real r) { if(r > Real(0)) return Real(1); else if(r < Real(0)) return Real(-1); else return Real(0); }
