#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "console.h"

static const Move *chooseMove(const Board *board, const MoveList *moves,
                              const Player *player, bool *demo)
{
    consoleShowTurn(board, moves, player);
    if (!player->is_computer && !*demo) {
        const Move *selected;
        switch (consoleReadMove(moves, &selected)) {
        case MOVE_INPUT_SELECTED:
            return selected;
        case MOVE_INPUT_EXIT:
            return NULL;
        case MOVE_INPUT_DEMO:
            /* デモに切り替えても盤面と手番は同じなので、候補を再利用できる。 */
            *demo = true;
            consoleShowTurn(board, moves, player);
            break;
        }
    }

    /* CPU とデモは合法手の一覧から選ぶ。 */
    const Move *move = &moves->items[rand() % moves->count];
    consoleShowAutomaticMove(player, move);
    return move;
}

static int playGame(const Player players[2], int current)
{
    Board board;
    boardInit(&board);

    bool demo = false;
    MoveList moves;
    for (;;) {
        boardFindMoves(&board, players[current].color, &moves);
        if (moves.count == 0) {
            /* 盤面を変えずに相手の合法手を調べ、両者に手がなければ終局。 */
            int passed = current;
            current = 1 - current;
            boardFindMoves(&board, players[current].color, &moves);
            if (moves.count == 0) {
                break;
            }
            consoleShowPass(&board, &players[passed]);
        }

        const Move *move = chooseMove(&board, &moves, &players[current], &demo);
        if (!move) {
            return EXIT_FAILURE;
        }
        boardApplyMove(&board, move, players[current].color);
        current = 1 - current;
    }

    consoleShowResult(&board, players);
    return EXIT_SUCCESS;
}

int main(void)
{
    srand((unsigned int)time(NULL));

    Player players[2];
    int first_player;
    if (!consoleSetup(players, &first_player)) {
        return EXIT_SUCCESS;
    }
    return playGame(players, first_player);
}
