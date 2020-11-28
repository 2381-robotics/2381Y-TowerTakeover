#ifndef __AUTO_DRIVE__H__
#define __AUTO_DRIVE__H__
#include "auto_task.hpp"
#include <complex>
#include <array>

extern AutoTask AutoPath(std::complex<double> EndPoint, double angle, double speed, double tolerance);
extern AutoTask AutoPath(std::complex<double> EndPoint, double angle, double speed, std::array<double,2> tolerance = {1,1});
extern AutoTask AutoPath(std::complex<double> EndPoint, double angle, std::array<double,2> speed = {100, 100}, double tolerance = 1);
extern AutoTask AutoPath(std::complex<double> EndPoint, double angle, std::array<double,2> speed = {100, 100}, std::array<double,2> tolerance = {1,1});
extern AutoTask SingleRun(std::function<void(void)> run);

extern AutoTask AutoCurve(std::complex<double> Waypoint, double angle, std::complex<double> EndPoint, double endAngle, double speed, double curvature = 1);


#endif  //!__AUTO_DRIVE__H__


