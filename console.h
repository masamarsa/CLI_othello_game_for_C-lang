#ifndef OTHELLO_CONSOLE_H
#define OTHELLO_CONSOLE_H

#include <stdbool.h>

#include "board.h"

enum { NAME_CAPACITY = 80 };

typedef struct {
    char name[NAME_CAPACITY];
    bool is_computer;
    Stone color;
} Player;

typedef enum {
    MOVE_INPUT_SELECTED,
    MOVE_INPUT_EXIT,
    MOVE_INPUT_DEMO
} MoveInputResult;

/* プレイヤーと先手の添字を設定する。終了または EOF なら false を返す。 */
bool consoleSetup(Player players[2], int *first_player);
void consoleShowTurn(const Board *board, const MoveList *moves, const Player *player);

/* MOVE_INPUT_SELECTED の場合、selected は moves 内の合法手を指す。 */
MoveInputResult consoleReadMove(const MoveList *moves, const Move **selected);

void consoleShowAutomaticMove(const Player *player, const Move *move);
void consoleShowPass(const Board *board, const Player *player);
void consoleShowResult(const Board *board, const Player players[2]);

#endif
