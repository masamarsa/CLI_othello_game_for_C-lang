#ifndef OTHELLO_BOARD_H
#define OTHELLO_BOARD_H

enum { BOARD_SIZE = 8, DIRECTION_COUNT = 8 };

typedef enum { EMPTY, BLACK, WHITE, STONE_KIND_COUNT } Stone;

typedef struct {
    Stone cells[BOARD_SIZE][BOARD_SIZE];
} Board;

typedef struct {
    int x;
    int y;
    int flips[DIRECTION_COUNT]; /* 盤面モジュールの方向順に反転数を保持する。 */
} Move;

typedef struct {
    Move items[BOARD_SIZE * BOARD_SIZE];
    int count;
} MoveList;

void boardInit(Board *board);

/* BLACK または WHITE の合法手を行優先で moves に格納する。 */
void boardFindMoves(const Board *board, Stone color, MoveList *moves);

/* 現在の盤面で color に対して生成した合法手を適用する。 */
void boardApplyMove(Board *board, const Move *move, Stone color);

/* counts は Stone の値で参照でき、EMPTY のマス数も含む。 */
void boardCountStones(const Board *board, int counts[STONE_KIND_COUNT]);

#endif
