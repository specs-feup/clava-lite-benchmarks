#pragma once

#include "MotionEstimation.h"

void FullSearch(Image *block, Image *img_ref, int yc, int xc, int SearchLimit, int BlockSize, double *dx, double *dy);