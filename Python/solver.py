import time
import copy, puzzle

def get_reflection(block):
    # reflect a block vertically
    new_block = []

    for i in range(len(block)):
        new_block.append(block[-1 - i])

    return new_block


def find_rots(block):
    # returns the different rotations (0, 90, 180, 270) of a given block
    rots = [block]

    for i in range(3):
        old_rot = rots[-1]
        new_rot = []

        for j in range(len(old_rot[0])):
            new_rot.append([])
            for k in range(len(old_rot)):
                new_rot[-1].append(old_rot[-1 - k][j])

        rots.append(new_rot)

    return rots


def find_placeability(board, pos, block):
    # finds whether a block can be placed in given location
    placeable = True
    x, y = pos['x'], pos['y']

    for i in range(len(block)):
        for j in range(len(block[i])):
            if x + i >= len(board) or y + j >= len(board[x]):
                placeable = False
                break
            if board[x + i][y + j] != 0 and block[i][j] != 0:
                placeable = False

    return placeable


def place_block(board, pos, block):
    # place the block at the given location
    x, y = pos['x'], pos['y']

    for i in range(len(block)):
        for j in range(len(block[i])):
            if board[x + i][y + j] == 0:
                board[x + i][y + j] = block[i][j]


def solve():
    # initialize
    blocks = puzzle.blocks
    brd_length = puzzle.board_dimensions[0]
    brd_height = puzzle.board_dimensions[1]
    solved = False
    history = [[[0 for i in range(brd_length)] for j in range(brd_height)]]     # history of the board
    bl_info = [{'pos': {'x': 0, 'y': 0}, 'perm': 0}]                            # holds placement info of block
    block_num = 0                                                               # the current block to place

    while not solved:
        cur_pos = bl_info[block_num]['pos']
        cur_perm = bl_info[block_num]['perm']
        perms = find_rots(blocks[block_num].copy()) + find_rots(get_reflection(blocks[block_num].copy()))
        board = copy.deepcopy(history[-1])

        placement = None
        for i in range(cur_pos['x'], brd_length):
            for j in range(cur_pos['y'], brd_height):
                for k in range(cur_perm, len(perms)):
                    pos = {'x': i, 'y': j}
                    if find_placeability(board, pos, perms[k]):
                        placement = {'pos': pos, 'perm': k}
                        break
                if placement:
                    break
                cur_perm = 0
            if placement:
                break
            cur_pos['y'] = 0

        if placement:
            place_block(board, placement['pos'], perms[placement['perm']])
            history.append(board)
            bl_info.pop()
            bl_info.append(placement)
            bl_info.append({'pos': {'x': 0, 'y': 0}, 'perm': 0})
            block_num += 1
            for r in history[-1]:
                print(r)
            print()
        else:
            block_num -= 1
            history.pop()
            bl_info.pop()

            if len(bl_info) == 0:
                print('no solution found')
                break

            bl_info[-1]['perm'] += 1

        if block_num == len(blocks):
            solved = True

def main():
    start = time.time()
    solve()
    end = time.time()
    print("exection time: %s seconds\n" % (end-start))
    

if __name__ == "__main__":
        # execute only if run as a script
        main()
