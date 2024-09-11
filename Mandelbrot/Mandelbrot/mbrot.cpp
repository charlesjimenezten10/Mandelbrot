#include "mbrot.h"
#include <iostream>


MandelbrotPointInfo compute_mandelbrot(d_complex c, int max_iters, bool collect_points)
{
	MandelbrotPointInfo mandelbrotPointInfo;
	//mandelbrotPointInfo.initial_point = c;
	//mandelbrotPointInfo.max_iters = max_iters;

	d_complex z = c;

	for (int i = 0; i < max_iters; i++) {
		z = (z * z) + c;
		if (collect_points) {
			mandelbrotPointInfo.points_in_path.push_back(z);
		}
		if (std::abs(z) > 2.0) {
			mandelbrotPointInfo.escaped = true;
			break;
		}
	}
	return mandelbrotPointInfo;
}
