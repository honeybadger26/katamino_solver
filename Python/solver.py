import time
import copy, puzzle

from puzzle import blocks as BLOCKS
from puzzle import HEIGHT
from puzzle import WIDTH


# prints the board
def print_board(board):
    for i in range(HEIGHT):
        for j in range(WIDTH):
            print("%d " % board[i][j], end="")
        print("")
    print("")


# place the block at the given location
def place_block(board, x, y, block):
    # checks whether block can be placed
    for i in range(len(block)):
        for j in range(len(block[i])):
            if x + i >= HEIGHT or y + j >= WIDTH:
                return False
            if board[x+i][y+j] != 0 and block[i][j] != 0:
                return False

    # place block
    for i in range(len(block)):
        for j in range(len(block[i])):
            if board[x+i][y+j] == 0:
                board[x+i][y+j] = block[i][j]

    return True


def set_permutation(block_num, permutation_num):
    # returns the different rotations (0, 90, 180, 270) of a given block
    new_block = []

    # 90 and 270 degrees require change of width and height
    if permutation_num % 2 == 0:
        height = len(BLOCKS[block_num])
        width = len(BLOCKS[block_num][0])
    else:
        height = len(BLOCKS[block_num][0])
        width = len(BLOCKS[block_num])

    rotation = permutation_num % 4

    # rotate block
    for i in range(height):
        new_block.append([])
        for j in range(width):
            if rotation == 0:
                new_block[i].append(BLOCKS[block_num][i][j])
            elif rotation == 1:
                new_block[i].append(BLOCKS[block_num][width-j-1][i])
            elif rotation == 2:
                new_block[i].append(BLOCKS[block_num][height-i-1][height-j-1])
            else:
                new_block[i].append(BLOCKS[block_num][j][height-i-1])

    # reflect block
    if permutation_num >= 4:
        reflected_block = []
        for i in range(height):
            reflected_block.append([])
            for j in range(width):
                reflected_block[i].append(new_block[i][-j-1])
        return reflected_block

    return new_block


def solve():
    # holds whether running, failed or solved
    status = 0

    boards = [[[ 0 for _ in range(WIDTH) ] 
        for __ in range(HEIGHT) ] 
    for ___ in range(len(BLOCKS)) ]

    bl_info = [ dict(x=0, y=0, permutation=0) for _ in range(len(BLOCKS)) ]
    block_num = 0

    while status == 0:
        for i in range(HEIGHT):
            for j in range(WIDTH):
                if block_num == 0:
                    boards[block_num][i][j] = 0
                else:
                    boards[block_num][i][j] = boards[block_num-1][i][j]

        x = bl_info[block_num]['x']
        y = bl_info[block_num]['y']
        permutation = bl_info[block_num]['permutation']

        go_back = False

        permutation += 1
        if permutation >= 8:
            permutation = 0

            y += 1
            if y > WIDTH:
                y = 0

                x += 1
                if x > HEIGHT:
                    bl_info[block_num]['x'] = 0
                    bl_info[block_num]['y'] = 0
                    bl_info[block_num]['permutation'] = 0
                    go_back = True

                    block_num -= 1
                    if block_num < 0:
                        status = -1

        if go_back:
            continue

        bl_info[block_num]['x'] = x
        bl_info[block_num]['y'] = y
        bl_info[block_num]['permutation'] = permutation

        block = set_permutation(block_num, permutation)

        if place_block(boards[block_num], x, y, block):
            print_board(boards[block_num])

            block_num += 1
            if block_num >= len(BLOCKS):
                status = 1

    if status == -1:
        print("no solution found")


def main():
    start = time.time()
    solve()
    end = time.time()
    print("exection time: %s seconds\n" % (end-start))


if __name__ == "__main__":
    # execute only if run as a script
        main()
