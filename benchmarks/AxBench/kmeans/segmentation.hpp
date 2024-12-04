/*
 * segmentation.h
 *
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#pragma once

#include "rgbimage.hpp"

typedef struct
{
    float r;
    float g;
    float b;
    int n;
} Centroid;

typedef struct
{
    int k;
    Centroid *centroids;
} Clusters;

int initClusters(Clusters *clusters, int k, float scale);
void segmentImage(RgbImage *image, Clusters *clusters, int n);
void freeClusters(Clusters *clusters);