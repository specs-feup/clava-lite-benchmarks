#include <cmath>
#include <iostream>
#include "check_result.h"

// the "official" threshold - if the absolute value of either
// flow component is greater, it's considered unknown
#define UNKNOWN_FLOW_THRESH 1e9

// value to use to represent unknown flow
#define UNKNOWN_FLOW 1e10

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool unknown_flow(float u, float v)
{
    return (fabs(u) > UNKNOWN_FLOW_THRESH) || (fabs(v) > UNKNOWN_FLOW_THRESH) || std::isnan(u) || std::isnan(v);
}

void check_results(velocity_t output[MAX_HEIGHT][MAX_WIDTH], float refFlow[MAX_HEIGHT][MAX_WIDTH * 2], std::string outFile)
{
    float outFlow[MAX_HEIGHT][MAX_WIDTH * 2] = {0};

    for (int i = 0; i < MAX_HEIGHT; i++)
    {
        for (int j = 0; j < MAX_WIDTH * 2; j += 2)
        {
            outFlow[i][j] = output[i][j].x;
            outFlow[i][j + 1] = output[i][j].y;
        }
    }

    double accum_error = 0;
    int num_pix = 0;
    for (int i = 0; i < MAX_HEIGHT; i++)
    {
        for (int j = 0; j < MAX_WIDTH * 2; j += 2)
        {
            double out_x = outFlow[i][j];
            double out_y = outFlow[i][j + 1];

            if (unknown_flow(out_x, out_y))
                continue;

            double out_deg = atan2(-out_y, -out_x) * 180.0 / M_PI;
            double ref_x = refFlow[i][j];
            double ref_y = refFlow[i][j + 1];
            double ref_deg = atan2(-ref_y, -ref_x) * 180.0 / M_PI;

            // Normalize error to [-180, 180]
            double error = out_deg - ref_deg;
            while (error < -180)
                error += 360;
            while (error > 180)
                error -= 360;

            accum_error += fabs(error);
            num_pix++;
        }
    }

    double avg_error = accum_error / num_pix;
    std::cout << "Average error: " << avg_error << " degrees" << std::endl;
}
