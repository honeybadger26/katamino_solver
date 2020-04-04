#ifndef PUZZLE_H
#define PUZZLE_H

#include "block.h"

// board dimensions
#define HEIGHT      5
#define WIDTH       6   // variable

#define NUM_BLOCKS  6

// blocks to be placed
const struct Block BLOCKS[NUM_BLOCKS] = { 
    { 
        2, 4, 
        *(int[2][4]) { 
            { 1, 0, 0, 0 }, 
            { 1, 1, 1, 1 } 
        } 
    },
    {
        3, 3,
        *(int[3][3]) { 
            { 2, 0, 0 }, 
            { 2, 0, 0 }, 
            { 2, 2, 2 } 
        }
    },
    {
        2, 3,
        *(int[2][3]) { 
            { 0, 3, 3 }, 
            { 3, 3, 3 } 
        }
    }, 
    {
        3, 3,
        *(int[3][3]) { 
            { 4, 4, 0 }, 
            { 0, 4, 0 }, 
            { 0, 4, 4 } 
        },
    },
    {
        2, 4,
        *(int[2][4]) { 
            { 0, 5, 0, 0 }, 
            { 5, 5, 5, 5 }
        },
    },
    {
        3, 3,
        *(int[3][3]) {
            { 0, 6, 6 },
            { 6, 6, 0 },
            { 6, 0, 0 }
        }
    },
//    {
//        4, 2,
//        *(int[4][2]) {
//            { 7, 0 },
//            { 7, 0 },
//            { 7, 7 },
//            { 0, 7 }
//        }
//    },
//    {
//        3, 3,
//        *(int[3][3]) {
//            { 8, 8, 0 },
//            { 0, 8, 8 },
//            { 0, 8, 0 }
//        }
//    }
};

#endif
