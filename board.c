#include "board.h"

#include <stdbool.h>

void boardInit(Board *board)
{
    const int center = BOARD_SIZE / 2;

    /* 中央の4マスに斜めに同じ色を配置する。残りは EMPTY（0）。 */
    *board = (Board){0};
    board->cells[center - 1][center - 1] = WHITE;
    board->cells[center - 1][center] = BLACK;
    board->cells[center][center - 1] = BLACK;
    board->cells[center][center] = WHITE;
}

typedef struct {
    int dx;
    int dy;
} Direction;

static const Direction directions[DIRECTION_COUNT] = {
    {-1, -1}, {0, -1}, {1, -1},
    {-1,  0},          {1,  0},
    {-1,  1}, {0,  1}, {1,  1}
};

static bool isOnBoard(int x, int y)
{
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

static int countFlips(const Board *board, int x, int y, Direction direction,
                      Stone color, Stone opponent)
{
    int count = 0;
    x += direction.dx;
    y += direction.dy;

    /* 連続する相手の石のすぐ先に自分の石があれば、その列を返せる。 */
    while (isOnBoard(x, y) && board->cells[y][x] == opponent) {
        count++;
        x += direction.dx;
        y += direction.dy;
    }
    return count > 0 && isOnBoard(x, y) && board->cells[y][x] == color ? count : 0;
}

void boardFindMoves(const Board *board, Stone color, MoveList *moves)
{
    const Stone opponent = color == BLACK ? WHITE : BLACK;
    moves->count = 0;

    /* 候補を行優先で登録し、各候補に方向ごとの反転数を記録する。 */
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (board->cells[y][x] != EMPTY) {
                continue;
            }

            Move move;
            move.x = x;
            move.y = y;
            int total_flips = 0;
            for (int i = 0; i < DIRECTION_COUNT; i++) {
                move.flips[i] = countFlips(board, x, y, directions[i],
                                           color, opponent);
                total_flips += move.flips[i];
            }
            if (total_flips > 0) {
                moves->items[moves->count++] = move;
            }
        }
    }
}

void boardApplyMove(Board *board, const Move *move, Stone color)
{
    /* 候補を作ったときの反転数を使い、各方向の終点を探し直さない。 */
    board->cells[move->y][move->x] = color;
    for (int i = 0; i < DIRECTION_COUNT; i++) {
        int x = move->x;
        int y = move->y;
        for (int step = 0; step < move->flips[i]; step++) {
            x += directions[i].dx;
            y += directions[i].dy;
            board->cells[y][x] = color;
        }
    }
}

void boardCountStones(const Board *board, int counts[STONE_KIND_COUNT])
{
    for (int color = 0; color < STONE_KIND_COUNT; color++) {
        counts[color] = 0;
    }

    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            counts[board->cells[y][x]]++;
        }
    }
}
