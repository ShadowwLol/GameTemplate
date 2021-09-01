#ifndef MISC_H
#define MISC_H

#ifdef MISC_RANDOM
#include <stdlib.h>
#include <time.h>
#define randomize() srand(time(NULL))
#define rand_range(min, max) ((rand() % (max - min + 1)) + min)
#endif

#define clamp(val, min, max){if (val < min){val = min;}else if (val > max){val = max;}}

#endif