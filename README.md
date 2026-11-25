# CLI_othello_game_for_C-lang

A command-line Othello game in C for one or two players.

## Build and run on Unix

Use a compiler that supports `-std=gnu23`, `make`, and a UTF-8 terminal for the Japanese prompts and board symbols.

```sh
make
./othello
```

Choose `1` to play against the computer or `2` to play with a second person.
Enter moves as coordinates such as `d3`. The board marks legal moves with `.`.
Enter `exit` to stop a game or `test` to let the computer play the remaining turns.

Run `make clean` to remove the executable and object files.

## Source layout

- `othello.c` handles the game loop, passes, and automatic move selection.
- `board.c` and `board.h` define the board, legal moves, flips, and scoring.
- `console.c` and `console.h` handle setup, input validation, and display.
