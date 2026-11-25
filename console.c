#include "console.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 石の記号は背景が黒いターミナルを想定している。 */
static const char *const stone_marks[] = {[EMPTY] = " ", [BLACK] = "○", [WHITE] = "●"};

static void writeBoard(const Board *board, const MoveList *moves)
{
    int next_move = 0;

    printf("\n    ");
    for (int x = 0; x < BOARD_SIZE; x++) {
        printf(" %c", 'a' + x);
    }
    putchar('\n');

    /* 合法手の「.」は表示時だけ重ね、盤面には石と空きマスを保持する。 */
    for (int y = 0; y < BOARD_SIZE; y++) {
        printf("   %d ", y + 1);
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (moves && next_move < moves->count &&
                moves->items[next_move].x == x && moves->items[next_move].y == y) {
                printf(". ");
                next_move++;
            } else {
                printf("%s ", stone_marks[board->cells[y][x]]);
            }
        }
        putchar('\n');
    }
}

typedef enum { INPUT_OK, INPUT_INVALID, INPUT_EOF } InputResult;

static InputResult readToken(const char *prompt, char *token, size_t capacity)
{
    fputs(prompt, stdout);
    fflush(stdout);

    int ch;
    do {
        ch = getchar();
    } while (isspace(ch));

    if (ch == EOF) {
        return INPUT_EOF;
    }

    /* 長すぎる入力も末尾まで読み、残りが次の入力に混ざるのを防ぐ。 */
    size_t length = 0;
    bool valid = true;
    do {
        if (length < capacity - 1 && ch != '\0') {
            token[length++] = (char)ch;
        } else {
            valid = false;
        }
        ch = getchar();
    } while (ch != EOF && !isspace(ch));
    token[length] = '\0';
    return valid ? INPUT_OK : INPUT_INVALID;
}

static int chooseMode(void)
{
    char input[4];

    for (;;) {
        InputResult result = readToken(
            "\n何人で遊びますか？\n人数を入力( 1人でプレイ：1, 2人でプレイ：2 ) > ",
            input, sizeof input);
        if (result == INPUT_EOF) {
            return 0;
        }
        if (result != INPUT_OK || strlen(input) != 1 ||
            input[0] < '0' || input[0] > '9') {
            puts("数字を入力してください。");
            continue;
        }

        int mode = input[0] - '0';
        if (mode == 1 || mode == 2) {
            return mode;
        }
        if (mode > 2) {
            puts("このゲームは2人まで遊べます。");
            continue;
        }

        result = readToken("ゲームを終了しますか？（yes/no）> ", input, sizeof input);
        if (result == INPUT_EOF ||
            (result == INPUT_OK && (strcmp(input, "y") == 0 || strcmp(input, "yes") == 0))) {
            return 0;
        }
    }
}

static bool readPlayerNames(Player players[2])
{
    puts("\nプレイヤーの名前を入力してください。（日本語，ローマ字のどちらでも可）\n");
    for (int i = 0; i < 2; i++) {
        char prompt[64];
        snprintf(prompt, sizeof prompt, "プレイヤー %d > ", i + 1);
        InputResult result;
        do {
            result = readToken(prompt, players[i].name, sizeof players[i].name);
            if (result == INPUT_EOF) {
                return false;
            }
            if (result == INPUT_INVALID) {
                puts("名前が長すぎるか、無効な文字が含まれています。");
            }
        } while (result != INPUT_OK);
    }
    puts("-----------------------------------------------------");
    return true;
}

static int readTossGuess(void)
{
    char guess[2];
    for (;;) {
        InputResult result = readToken(
            "伏せ石の黒面は上ですか？ 下ですか？\n黒面は...(上：0，下：1) > ",
            guess, sizeof guess);
        if (result == INPUT_EOF) {
            return -1;
        }
        if (result == INPUT_OK && (guess[0] == '0' || guess[0] == '1')) {
            return guess[0] - '0';
        }
    }
}

bool consoleSetup(Player players[2], int *first_player)
{
    int mode = chooseMode();
    if (mode == 0) {
        return false;
    }

    players[0] = (Player){.name = "プレイヤー"};
    players[1] = (Player){.name = "コンピュータ", .is_computer = mode == 1};

    int hidden_side = rand() % 2;
    if (mode == 2 && !readPlayerNames(players)) {
        return false;
    }

    printf("\n伏せ石で先手（黒 \"%s\"）と後手（白 \"%s\"）を決めます。\n",
           stone_marks[BLACK], stone_marks[WHITE]);
    int guesser = 0;
    if (mode == 2) {
        guesser = rand() % 2;
        printf("回答者は %s です。\n", players[guesser].name);
    }

    int guess = readTossGuess();
    if (guess < 0) {
        return false;
    }

    /* 伏せ石を言い当てた側を黒（先手）にする。 */
    int first = guess == hidden_side ? guesser : 1 - guesser;
    players[first].color = BLACK;
    players[1 - first].color = WHITE;
    printf("伏せ石の黒面は %s なので、先手（黒）は %s 、後手（白）は %s です。\n",
           hidden_side == 0 ? "上" : "下", players[first].name, players[1 - first].name);
    *first_player = first;
    return true;
}

void consoleShowTurn(const Board *board, const MoveList *moves, const Player *player)
{
    writeBoard(board, player->is_computer ? NULL : moves);
    printf("\n%sのターン\n", player->name);
}

static const Move *findMoveAt(const MoveList *moves, int x, int y)
{
    for (int i = 0; i < moves->count; i++) {
        if (moves->items[i].x == x && moves->items[i].y == y) {
            return &moves->items[i];
        }
    }
    return NULL;
}

MoveInputResult consoleReadMove(const MoveList *moves, const Move **selected)
{
    for (;;) {
        char input[5];
        InputResult result = readToken("\nどのマスに置きますか？ （例：b2）> ",
                                       input, sizeof input);
        if (result == INPUT_EOF || (result == INPUT_OK && strcmp(input, "exit") == 0)) {
            printf("ゲームを中断します。");
            return MOVE_INPUT_EXIT;
        }
        if (result == INPUT_OK && strcmp(input, "test") == 0) {
            return MOVE_INPUT_DEMO;
        }
        if (result == INPUT_OK && strlen(input) == 2) {
            const Move *move = findMoveAt(moves, input[0] - 'a', input[1] - '1');
            if (move) {
                *selected = move;
                return MOVE_INPUT_SELECTED;
            }
        }
        puts("                                そこには置けません");
    }
}

void consoleShowAutomaticMove(const Player *player, const Move *move)
{
    printf("%sは %c%d に置きました。\n", player->name, 'a' + move->x, move->y + 1);
}

void consoleShowPass(const Board *board, const Player *player)
{
    if (player->is_computer) {
        writeBoard(board, NULL);
    }
    printf("\n%sはパス\n", player->name);
}

void consoleShowResult(const Board *board, const Player players[2])
{
    static const char *const color_names[] = {[BLACK] = "黒", [WHITE] = "白"};
    int counts[STONE_KIND_COUNT];
    boardCountStones(board, counts);

    writeBoard(board, NULL);
    puts("\n終了！\n\n------------- 結果発表 -------------");
    int player_one_score = counts[players[0].color];
    int player_two_score = counts[players[1].color];
    printf("%s (%s) : %d個  %s (%s) : %d個\n",
           players[0].name, color_names[players[0].color], player_one_score,
           players[1].name, color_names[players[1].color], player_two_score);

    if (player_one_score == player_two_score) {
        printf("\n引き分け");
    } else {
        const Player *winner = &players[player_one_score > player_two_score ? 0 : 1];
        printf("\n%sの勝ち%s", winner->name, winner->is_computer ? "..." : "！");
    }
}
