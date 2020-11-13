#ifndef __AUTO_DRIVE__H__
#define __AUTO_DRIVE__H__
#include "auto_task.hpp"
#include <complex>

extern AutoTask AutoPath(std::complex<double> EndPoint, double angle, double speed);

#endif  //!__AUTO_DRIVE__H__


