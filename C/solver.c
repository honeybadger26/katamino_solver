#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "block.h"
#include "puzzle.h"

/*
 * stores the placement information for each block
 */
struct PlacementInfo {
    int x;
    int y;
    int permutation;
};

/*
 * prints the game board
 */
void print_board(int board[HEIGHT][WIDTH]) {
    for (int i=0; i < HEIGHT; i++) {
        for (int j=0; j < WIDTH; j++) 
            printf("%d ", board[i][j]);
        printf("\n");
    }
    printf("\n");
}

/*
 * finds if block can be placed at x, y position on board
 * if block cannot be placed returns 0
 */
int place_block(int board[HEIGHT][WIDTH], int x, int y, struct Block block) {
    int height = block.height;
    int width = block.width;

    // check if can place block
    for (int i=0; i < height; i++) {
        for (int j=0; j < width; j++) {
            if (x+i >= HEIGHT || y+j >= WIDTH) return 0;
            if (board[x+i][y+j] != 0 && block.array[i*width + j] != 0) return 0;
        }
    }

    // place block
    for (int i=0; i < height; i++) {
        for (int j=0; j < width; j++) {
            if (board[x+i][y+j] == 0)
                board[x+i][y+j] = block.array[i*width + j];
        }
    }

    return 1;
}

/*
 * block = uninitialised block reference
 * block_num = an index of  a block from BLOCKS that will be used
 *      to base this newly creted block off of
 * permutation_num = the requested permutation. possible values:
 *      0-3 = 0, 90, 180, 270 degree rotations
 *      4-7 = same rotations as 0-3, but reflected horizontally
 */
void set_permutation(struct Block* block, int block_num, int permutation_num) {
    int height;
    int width;

    // change height and width if rotating 90 or 270 degrees
    if (permutation_num % 2 == 0) {
        height = BLOCKS[block_num].height;
        width = BLOCKS[block_num].width;
    } else {
        height = BLOCKS[block_num].width;
        width = BLOCKS[block_num].height;
    }

    block->height = height;
    block->width = width;

    int block_array[height][width];

    // rotate block
    for (int i=0; i < height; i++) {
        for (int j=0; j < width; j++) {
            int index;
            switch (permutation_num % 4) {
                case 0:
                    index = i*width + j;
                    break;
                case 1:
                    index = height*(width - j - 1) + i;
                    break;
                case 2:
                    index = width*(height - i) - j - 1;
                    break;
                case 3:
                    index = height*(j + 1) - i - 1;
                    break;
                default:
                    break;
            }
            block_array[i][j] = BLOCKS[block_num].array[index];
        }
    }

    // reflect if necessary
    if (permutation_num >= 4) {
        int reflected_block[height][width];

        for (int i=0; i < height; i++) {
            for (int j=0; j < width; j++)
                reflected_block[i][j] = block_array[i][width-j-1];
        }

        block->array = *reflected_block;
        return;
    }

    block->array = *block_array;
    return;
}

int solve() {
    // 0 = running, 1 = success, -1 = failed
    int status = 0;

    // initialise boards
    int boards[NUM_BLOCKS][HEIGHT][WIDTH];
    memset(boards, 0, sizeof(int) * NUM_BLOCKS * HEIGHT * WIDTH);

    // create array of placement info
    struct PlacementInfo bl_info[NUM_BLOCKS];
    for (int i=0; i < NUM_BLOCKS; i++) {
        bl_info[i].x = 0;
        bl_info[i].y = 0;
        bl_info[i].permutation = -1;
    }

    int block_num = 0;

    while (status == 0) {
        // copy previous board
        for (int i=0; i < HEIGHT; i++) {
            for (int j=0; j < WIDTH; j++) {
                if (block_num == 0)
                    boards[block_num][i][j] = 0;
                else
                    boards[block_num][i][j] = boards[block_num-1][i][j];
            }
        }

        int x = bl_info[block_num].x,
            y = bl_info[block_num].y,
            permutation = bl_info[block_num].permutation;

        // check whether we need to step back to previous block
        int go_back = 0;

        // go to next permutation
        if (++permutation >= 8) {
            permutation = 0;

            // go to next y value
            if (++y >= WIDTH) {
                y = 0;

                // go to next x value
                if (++x >= HEIGHT) {
                    // cannot place block so step back
                    bl_info[block_num].x = 0;
                    bl_info[block_num].y = 0;
                    bl_info[block_num].permutation = -1;
                    go_back = 1;

                    // solution not found
                    if (--block_num < 0) 
                        status = -1;
                }
            }
        }

        if (go_back == 1)
            continue;

        // save block placement
        bl_info[block_num].x = x;
        bl_info[block_num].y = y;
        bl_info[block_num].permutation = permutation;

        struct Block block;
        set_permutation(&block, block_num, permutation);

        // try to place block
        if (place_block(boards[block_num], x, y, block) == 1) {
            print_board(boards[block_num]);

            // solution found
            if (++block_num >= NUM_BLOCKS) 
                status = 1;
        }
    }

    // solution not found
    if (status == -1) printf("no solution found");

    return 0;
}

int main() {
    struct timeval start, end;

    gettimeofday(&start, NULL);
    solve();
    gettimeofday(&end, NULL);

    double time_taken = end.tv_sec + end.tv_usec / 1e6 -
        start.tv_sec - start.tv_usec / 1e6;

    printf("execution time: %f seconds\n", time_taken);
    return 0;
}
