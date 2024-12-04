/*===============================================================*/
/*                                                               */
/*                        typedefs.h                             */
/*                                                               */
/*        Defines types and constants for host function          */
/*                                                               */
/*===============================================================*/

#pragma once

const int MAX_HEIGHT = 436;
const int MAX_WIDTH = 1024;

// basic typedefs

typedef float pixel_t;
typedef float outer_pixel_t;
typedef float vel_pixel_t;

typedef struct
{
    pixel_t x;
    pixel_t y;
    pixel_t z;
} gradient_t;

typedef struct
{
    outer_pixel_t val[6];
} outer_t;

typedef struct
{
    outer_pixel_t val[6];
} tensor_t;

typedef struct
{
    vel_pixel_t x;
    vel_pixel_t y;
} velocity_t;
