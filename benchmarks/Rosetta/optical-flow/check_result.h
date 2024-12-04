/*===============================================================*/
/*                                                               */
/*                        check_result.h                         */
/*                                                               */
/*      Compare result and expected label to get error rate      */
/*                                                               */
/*===============================================================*/

#pragma once

#include <string>
#include "typedefs.h"

void check_results(velocity_t output[MAX_HEIGHT][MAX_WIDTH], float refFlow[MAX_HEIGHT][MAX_WIDTH * 2], std::string outFile);
