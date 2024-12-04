/*===============================================================*/
/*                                                               */
/*                    optical_flow_host.cpp                      */
/*                                                               */
/*      Main host function for the Optical Flow application.     */
/*                                                               */
/*===============================================================*/

// standard C/C++ headers
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <sys/time.h>

// other headers
#include "utils.h"
#include "typedefs.h"
#include "check_result.h"

#include "optical_flow_sw.h"

int main(int argc, char **argv)
{
    printf("Optical Flow Application\n");

    // parse command line arguments
    std::string dataPath("");
    std::string outFile("");

    parse_sdsoc_command_line_args(argc, argv, dataPath, outFile);

    // create actual file names according to the datapath
    std::string frame_files[5];
    std::string reference_file;
    frame_files[0] = dataPath + "/frame1.dat";
    frame_files[1] = dataPath + "/frame2.dat";
    frame_files[2] = dataPath + "/frame3.dat";
    frame_files[3] = dataPath + "/frame4.dat";
    frame_files[4] = dataPath + "/frame5.dat";
    reference_file = dataPath + "/ref.flo.dat";

    static pixel_t frames[5][MAX_HEIGHT][MAX_WIDTH];
    static velocity_t outputs[MAX_HEIGHT][MAX_WIDTH];
    static float refFlow[MAX_HEIGHT][MAX_WIDTH * 2] = {0};

    // read in images
    printf("Reading input files ... \n");

    for (int k = 0; k < 5; k++)
    {
        FILE *file = fopen(frame_files[k].c_str(), "r");

        int row = 0;
        int col = 0;

        while (row < MAX_HEIGHT)
        {
            fscanf(file, "%f,", &frames[k][row][col]);

            col++;
            if (col >= MAX_WIDTH)
            {
                row++;
                col = 0;
            }
        }
    }

    // read in reference flow file
    printf("Reading reference output flow... \n");

    FILE *file = fopen(reference_file.c_str(), "r");

    int row = 0;
    int col = 0;

    while (row < MAX_HEIGHT)
    {
        fscanf(file, "%f,", &refFlow[row][col]);

        col++;
        if (col >= MAX_WIDTH * 2)
        {
            row++;
            col = 0;
        }
    }

    // timers
    struct timeval start,
        end;

    gettimeofday(&start, NULL);

    // run
    optical_flow_sw(frames[0], frames[1], frames[2], frames[3], frames[4], outputs);
    gettimeofday(&end, NULL);

    // check results
    // printf("Checking results:\n");

    check_results(outputs, refFlow, outFile);

    // print time
    long long elapsed = (end.tv_sec - start.tv_sec) * 1000000LL + end.tv_usec - start.tv_usec;
    printf("elapsed time: %lld us\n", elapsed);

    return EXIT_SUCCESS;
}
