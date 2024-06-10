/* オセロ・ゲーム */
/* このプログラムはターミナルの背景色が "黒色" であることを前提に作成している */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* 実行確認用 */
#define EXIT 8
#define DEMO 9
int play;

/* ボード設定 */
#define MATRIX 8      /* ボードのマスの数。必ず偶数にすること（最初の4つの石の配置の便宜のため）*/

/* 1人プレイ */
#define PLAYER 0      /* 1人プレイ時，プレイヤーのマーク */
#define COMPUTER 1    /* 1人プレイ時，コンピュータのマーク */

/* 2人プレイ */
#define PLAYER1 2    /* 2人プレイ時，1人目 */
#define PLAYER2 3    /* 2人プレイ時，2人目 */

/* ボードのマーク定義 */
#define ENPTY 0       /* 空白のマス */
#define BLACK 1       /* 石の黒面"○" */
#define WHITE 2       /* 石の白面"●" */
#define PUTABLE 3     /* 「石を置けます」サイン */ 

/* チェック */
#define CHECK 5       /* プレイヤー及びコンピュータが石を置けるかどうか */

/* グローバル変数 */
/* ボード情報 */
int board [MATRIX][MATRIX];    /* ボードの状況 */
int put_x;                     /* 石を置いたマスの x 座標 */
int put_y;                     /* 石を置いたマスの y 座標 */

/* ターンなど */
int mode;        /* ゲームモード */
int player;      /* プレイヤーの石の色 */
int computer;    /* コンピュータの石の色 */
int player1;     /* プレイヤー 1 の石の色 */
int player2;     /* プレイヤー 2 の石の色 */
int turn1;       /* 1人プレイ時，石を置く人 ( プレイヤー：1，コンピュータ：2 ) */
int turn2;       /* 2人プレイ時，石を置く人 ( プレイヤー 1：1，プレイヤー 2：2 ) */

/* 名前 */
char name [4][20] = { "プレイヤー", "コンピュータ" };    /* 1人プレイの時のプレイヤーとコンピュータの名前 */

/* 先手と後手 */
char first [20];    /* 先手 */
char second [20];   /* 後手 */


/* ボードを初期化する関数 */
void initialize ( void ) {
	int y, x;

	/* ボードの全てのマスを空白に初期化 */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			board [y][x] = ENPTY;
		}
	}

	/* ボードの真ん中の 4 つのマスに石を置く */
	board [MATRIX / 2 - 1][MATRIX / 2 - 1] = WHITE;
	board [MATRIX / 2 - 1][MATRIX / 2] = BLACK;
	board [MATRIX / 2][MATRIX / 2 - 1] = BLACK;
	board [MATRIX / 2][MATRIX / 2] = WHITE;
}


/* プレイ人数と順番を決める関数 */
void decidePlay ( void ) {
	int holeStone;                        /* 伏せた石の色 ( 黒は 0，白は 1 ) */
	int select;                           /* 伏せ石の黒面は上か下か ( 上：0，下：1 ) */
	int selecter;                         /* 2人プレイの時，伏せ石の黒面の向きを答える人（ 1：プレイヤー 1，2：プレイヤー 2 ） */
	int s;                                /* forループに使用 */
	char side [2][3] = { "上", "下" };    /* 伏せ石の黒面の向き */
	int n = 0;

	while (1) {
		/* プレイ・モード選択 */
		char m = 0;       /* mode */
		/*
		数字以外の文字，または文字が渡された時の挙動を導入したい
		⇒ ASCIIコードで条件分岐する
		*/
		printf("\n何人で遊びますか？\n人数を入力( 1人でプレイ：1, 2人でプレイ：2 ) > ");
		do {
			scanf("%c", &m);
		} while (m == '\n');
		/* ASCIIコードで条件判定 */
		if (m == '1' || m == '2') {
			mode = m - '0';
			break;
		}
		/* 入力エラー処理 */
		if (m >= '3' && m <= '9') {            /* 数字 */
			printf("このゲームは2人まで遊べます。\n");
			continue;
		} else if (m <= 12 || ( m >= 14 && m <= '/' ) || m >= ':') { /* 数字以外 */
			printf("数字を入力してください。\n");
			continue;
		} else if (m == '0') {                /* ゲームを終了 */
			printf("ゲームを終了しますか？（yes/no）> ");
			scanf("%c", &m);
			while (m == '\n') {
				scanf("%c", &m);
				n++;
				if (n > 10) break;
			}
			if (m == 'y') {
				return;
			} else {
				continue;
			}
		}
	}

	/* 伏せた石の黒面の向きを決める。向き：上は 0，下は 1 */
	holeStone = rand () % 2;

	/* 2人プレイモードのとき，プレイヤーの名前を入力させる */
	if ( mode == 2 ) {
		printf ( "\nプレイヤーの名前を入力してください。（日本語，ローマ字のどちらでも可）\n\n" );
		for ( s = 0; s < 2; s++ ) {
			printf ( "プレイヤー %d > ", s + 1 ); scanf ( "%s", name [s + 2] );
		}
		printf("-----------------------------------------------------\n");
	}

	printf("\n伏せ石で先手（黒 \"●\"）と後手（白 \"○\"）を決めます。\n");

	/* 2人プレイモードのとき，回答者をランダムに決める */
	if ( mode == 2 ) {
		selecter = rand () % 2 + 2;
		printf ( "回答者は %s です。\n", name [selecter] );
	}

	/* 伏せ石の黒面の向きを聞く */
	do {
		printf ( "伏せ石の黒面は上ですか？ 下ですか？\n黒面は...(上：0，下：1) > " ); scanf ( "%d", &select );
	} while ( select < 0 || select > 1 ); /* select に 0, 1 のどちらかを入力しないとループを抜けられないようにする */
	
	/* 先手を決める */
	switch ( mode ) {
		case 1: /* 1人プレイモード */
			if ( holeStone == select ) { /* プレイヤーが黒面の向きを言い当てた場合，先手はプレイヤー */
				strcpy ( first, name [PLAYER] );
				strcpy ( second, name [COMPUTER] );
				player = BLACK;
				computer = WHITE;
				turn1 = player;
			} else {                     /* プレイヤーが黒面の向きをはずした場合，先手はコンピュータ */
				strcpy ( first, name [COMPUTER] );
				strcpy ( second, name [PLAYER] );
				computer = BLACK;
				player = WHITE;
				turn1 = computer;
			}
			break;
		case 2: /* 2人プレイモード */
			if ( ( selecter == PLAYER1 && holeStone == select ) ||  /* 条件 1 : 回答者がプレイヤー 1 かつ プレイヤー 1 が黒面の向きを言い当てた */
				 ( selecter == PLAYER2 && holeStone != select ) ) { /* 条件 2 : 回答者がプレイヤー 2 かつ プレイヤー 2 が黒面の向きをはずした */
                                                                    /* 以上の条件のいずれかを満たせば，先手はプレイヤー 1 */
				strcpy ( first, name [PLAYER1] );
				strcpy ( second, name [PLAYER2] );
				player1 = BLACK;
				player2 = WHITE;
				turn2 = player1;
			} else if ( ( selecter == PLAYER1 && holeStone != select ) ||  /* 条件 1 : 回答者がプレイヤー 1 かつ プレイヤー 1 が黒面の向きをはずした */
						( selecter == PLAYER2 && holeStone == select ) ) { /* 条件 2 : 回答者がプレイヤー 2 かつ プレイヤー 2 が黒面の向きを言い当てた */
                                                                           /* 以上の条件のいずれかを満たせば，先手はプレイヤー 2 */
				strcpy ( first, name [PLAYER2] );
				strcpy ( second, name [PLAYER1] );
				player2 = BLACK;
				player1 = WHITE;
				turn2 = player2;
			}
			break;
	}
	printf ( "伏せ石の黒面は %s なので、先手（黒）は %s 、後手（白）は %s です。\n", side [holeStone], first, second );
}


/* ボードを表示する関数 */
void writeBoard ( void ) {
	int s, t;                      /* forループで使用 */
	char mark [4][4] = { " ",      /* 空白 */
						 "○",      /* 黒面 */
						 "●",      /* 白面 */
						 "." };    /* 配置可能 */

	printf ( "\n    " );
	for ( s = 0; s < MATRIX; s++ ) {
		printf ( " %c", 97 + s ); /* ボードの列番号 */
	}
	printf ( "\n" );

	for ( s = 0; s < MATRIX; s++ ) {
		printf ( "   %d ", s + 1 ); /* ボードの行番号 */
		for ( t = 0; t < MATRIX; t++ ) {
			printf ( "%s ", mark [board [s][t]] );
		}
		printf ( "\n" );
	}
}


/* プレイヤーが選択したマスにマークする関数（1人プレイ，2人プレイ共用） */
int writePlayerMark ( int player ) {
	/*
	引数
		player : 石を置くプレイヤーとその人の石の色

	戻り値 rtn
		0 : 関数の処理がすべて終了
		1 : ゲームを中断
	*/


	int x;                       /* プレイヤーが選択したマスの x 座標 */
	int y;                       /* プレイヤーが選択したマスの y 座標 */
	char get [5];                /* 入力値の入れ子 */
	char finish [5] = "exit";    /* ゲームを中断するときに使う */
	char test [5] = "test";      /* 実行確認 */

	while ( 1 ) {
		printf ( "\nどのマスに置きますか？ （例：b2）> " ); scanf ( "%s", get );

		if ( strcmp ( get, finish ) == 0 ) {      /* 【ゲームを中断する】（実行検証時使用） */
			return EXIT;
		} else if ( strcmp ( get, test ) == 0 ) { /* 実行確認に移る */
			return DEMO;
		}

		/* get の入力値を分解して，それぞれの座標に当てはめる */
		y = get [1] - '1';
		x = get [0] - 'a';

		/* プレイヤーが選択したマスの座標が "配置可能" マスであるかどうかを判定する */
		if ( x >= 0 && x < MATRIX && y >= 0 && y < MATRIX && board [y][x] == PUTABLE ) {
			/* プレイヤーの色の石を置く */
			board [y][x] = player;

			/* グローバル変数に座標を記録 */
			put_x = x;
			put_y = y;

			break;
		} else { /* "配置可能" マスでない場合 */
			printf ( "                                そこには置けません\n" );
		}
	}

	/* PUTABLE マークに設定されているマスを ENPTY マークに初期化する */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == PUTABLE ) {
				board [y][x] = ENPTY;
			}
		}
	}

	return 0;
}


/* 実行確認用（マス自動選択） */
void writePlayerMarkDEMO ( int player ) {
	int x;    /* プレイヤーが選択したマスの x 座標 */
	int y;    /* プレイヤーが選択したマスの y 座標 */

	while ( 1 ) {
		y = rand () % MATRIX;
		x = rand () % MATRIX;
		if ( board [y][x] == PUTABLE ) {
			board [y][x] = player;
			put_x = x;
			put_y = y;
			if ( mode == 1 ) {
				printf ( "%sは %c%d に置きました。\n", name [PLAYER], 'a' + x, y + 1 );
			} else {
				if ( player == player1 ) {
					printf ( "%sは %c%d に置きました。\n", name [PLAYER1], 'a' + x, y + 1 );
				} else if ( player == player2 ) {
					printf ( "%sは %c%d に置きました。\n", name [PLAYER2], 'a' + x, y + 1 );
				}
			}
			break;
		}
	}

	/* PUTABLE マークに設定されているマスを ENPTY マークに初期化する */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == PUTABLE ) {
				board [y][x] = ENPTY;
			}
		}
	}
}


/* コンピュータが選択したマスにマークする関数 */
void writeComputerMark ( void ) {
	int x;    /* コンピュータが選択したマスの x 座標 */
	int y;    /* コンピュータが選択したマスの y 座標 */

	while ( 1 ) {

		/* マスの座標をランダムに選択する */
		y = rand () % MATRIX;
		x = rand () % MATRIX;

		/* 選択した座標が "配置可能" マスであれば以下の処理をする */
		if ( board [y][x] == PUTABLE ) {
			/* コンピュータの色の石を置く */
			board [y][x] = computer;

			/* グローバル変数に座標を記録 */
			put_x = x;
			put_y = y;
			printf ( "コンピュータは %c%d に置きました。\n", 'a' + x, y + 1 );
			break;
		}
	}

	/* PUTABLE マークに設定されているマスを ENPTY マークに初期化する */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == PUTABLE ) {
				board [y][x] = ENPTY;
			}
		}
	}

}


/* 石をひっくり返す関数 */
void turnOverStone ( int human ) {
	/*
	引数
		human : 置いた人の石の色

	戻り値
		なし
	*/
	
	int y, x, s, t, r;                           /* forループに使用 */
	int turnOver_x [MATRIX * MATRIX] = { 0 };    /* ひっくり返す石の x 座標 */
	int turnOver_y [MATRIX * MATRIX] = { 0 };    /* ひっくり返す石の x 座標 */
	int count;

	for ( t = 0; t < 8; t++ ) {
		count = 0;
		for ( s = 1; s < MATRIX; s++ ) {
			switch ( t ) {
				case 0: /* 左上 */
					y = put_y - s;
					x = put_x - s;
					break;
				case 1: /* 上 */
					y = put_y - s;
					x = put_x;
					break;
				case 2: /* 右上 */
					y = put_y - s;
					x = put_x + s;
					break;
				case 3: /* 左 */
					y = put_y;
					x = put_x - s;
					break;
				case 4: /* 右 */
					y = put_y;
					x = put_x + s;
					break;
				case 5: /* 左下 */
					y = put_y + s;
					x = put_x - s;
					break;
				case 6: /* 下 */
					y = put_y + s;
					x = put_x;
					break;
				case 7: /* 右下 */
					y = put_y + s;
					x = put_x + s;
					break;
			}
			if ( y >= 0 && y < MATRIX && x >= 0 && x < MATRIX ) {
				if ( board [y][x] == PUTABLE ) {
					break;
				} else if ( board [y][x] == human ) {
					if ( count > 0 ) {
						for ( r = 0; r < count; r++ ) {
							board [turnOver_y [r]][turnOver_x [r]] = human;
						}
						break;
					} else {
						break;
					}
				} else if ( board [y][x] == ENPTY ) {
					break;
				} else {
					turnOver_y [count] = y;
					turnOver_x [count] = x;
					count += 1;
				}
			}
		}
	}
}


/* ボードの状況を確認する */
int checkOnBoard ( int mode, int human ) {
	/*
	引数
		mode  : ゲームモード
		human : 石を置く人

	判断要素 check_1, check_2, check_3
		0：置ける
		1：置けない

	戻り値 rtn
		0 : 石を置ける
		1 : 石を置けない
	*/

	int rtn;
	int check_1 = 0;         /* チェック項目 1 ：ボードのマスがすべて埋まっているかどうか */
	int check_2 = 0;         /* チェック項目 2 ：ボード上の石がすべて黒，または白になっているかどうか */
	int check_3 = 0;         /* チェック項目 3 ：ボードのマスに石を置けるかどうか */
	int x, y, w, h, s, t;    /* forループに使用 */
	int count;               /* チェック項目3で使用 */
	int stone = 0;           /* ボード上にある石の数 */
	int stone_B = 0;         /* ボード上にある黒面の石の数 */
	int stone_W = 0;         /* ボード上にある白面の石の数 */
	int set = 0;             /* チェック項目 3 で使用。PUTABLE の数を数える */
	int turnOverColor = 0;    /* ひっくり返す石の色 */


	/* チェック項目 1：ボードのマスがすべて埋まっているかどうか ------------------------- */

	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == BLACK || board [y][x] == WHITE ) {
			/* ボード上の石（色の区別なし）の総数を数える */
				stone++;
			}
		}
	}

	/* ボードのマスの総数とボード上の石の総数が一致した場合，「置けない」を返す */
	if ( stone == MATRIX * MATRIX ) {
		check_1 = 1;
	}

	/* チェック項目 2：ボード上の石がすべて黒，または白になっているかどうか --------------- */

	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == BLACK ) {
			/* ボード上にある黒面の石の数を数える */
				stone_B++;
			} else {
			/* ボード上にある白面の石の数を数える */
				stone_W++;
			}
		}
	}

	/* ボード上に黒面，または白面の石が無ければ，「置けない」を返す */
	if ( stone_B == 0 || stone_W == 0 ) {
		check_2 = 1;
	}

	/* チェック項目 3：ボードのマスに石を置けるかどうか --------------------------------- */

	/* PUTABLE マークに設定されているマスを ENPTY マークに初期化する */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == PUTABLE ) {
				board [y][x] = ENPTY;
			}
		}
	}

	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == human ) {
				for ( t = 0; t < 8; t++ ) {
					count = 0;
					for ( s = 1; s < MATRIX; s++ ) {
						switch ( t ) {
							case 0: /* 左上を調べる */
								h = y - s;
								w = x - s;
								break;
							case 1: /* 上を調べる */
								h = y - s;
								w = x;
								break;
							case 2: /* 右上を調べる */
								h = y - s;
								w = x + s;
								break;
							case 3: /* 左を調べる */
								h = y;
								w = x - s;
								break;
							case 4: /* 右を調べる */
								h = y;
								w = x + s;
								break;
							case 5: /* 左下を調べる */
								h = y + s;
								w = x - s;
								break;
							case 6: /* 下を調べる */
								h = y + s;
								w = x;
								break;
							case 7: /* 右下を調べる */
								h = y + s;
								w = x + s;
								break;
						}
						if ( h >= 0 && h < MATRIX && w >= 0 && w < MATRIX ) {
							if ( board [h][w] == PUTABLE ) {        /* PUTABLE マークのマスだったら，その方向は調べない */
								break;
							} else if ( board [h][w] == human) {    /* 置く人の石の色と同じ色だったら，その方向は調べない */
								break;
							} else if ( board [h][w] == ENPTY ) {   /* 調べている方向に自分の石の色がある */
								if ( count > 0 ) {                  /* 調べている方向に相手の石の色が 1 個以上ある */
									if ( mode < CHECK ) {           /* CHECK モードでなければ，空白のマスを PUTABLE マークに設定する */
										board [h][w] = PUTABLE;
									}
									set += 1;
								}
								break;
							} else {                                /* 調べている方向に相手の石があればその個数を数える */
								count += 1;
							}
						}
					}
				}
			}
		}
	}

	if ( set > 0 ) {    /* チェック項目 3 内で少なくとも一つ PUTABLE マークに設定できれば 「置ける」 */
		check_3 = 0;
	} else {            /* チェック項目 3 内で一つも PUTABLE マークに設定できなければ 「置けない」 */
		check_3 = 1;
	}
	
	/* 戻り値の判別 */
	if ( check_1 == 0 && check_2 == 0 && check_3 == 0 ) { /* チェック項目のすべてで「置ける」と判定されたら，「置ける」 */
		rtn = 0;
	} else {                                              /* チェック項目のいずれか一つでも「置けない」と判定されたら，「置けない」 */
		rtn = 1;
	}

	return rtn;
}


/* ゲームの勝敗を決める関数 */
void judge ( void ) {
	int y, x;                                      /* for ループに使用 */
	int stone_P = 0;                               /* プレイヤーの石の数 */
	int stone_P1 = 0;                              /* プレイヤー 1 の石の数 */
	int stone_P2 = 0;                              /* プレイヤー 2 の石の数 */
	int stone_C = 0;                               /* コンピュータの石の数 */
	char stoneColor [2][3] = { "黒", "白" }; /* 石の色 */

	printf("\n終了！\n");

	/* 石の数を数える */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == player ) {            /* プレイヤー */
				stone_P++;
			} else if ( board [y][x] == player1 ) {    /* プレイヤー 1 */
				stone_P1++;
			} else if ( board [y][x] == player2 ) {    /* プレイヤー 2 */
				stone_P2++;
			} else {                                   /* コンピュータ */
				stone_C++;
			}
		}
	}

	printf("\n------------- 結果発表 -------------\n");
	if ( mode == 1 ) { /* 1人プレイ */
		printf ( "%s (%s) : %d個  %s (%s) : %d個\n", name [PLAYER], stoneColor [player - 1], stone_P, name [COMPUTER], stoneColor [computer - 1], stone_C);
	} else {           /* 2人プレイ */
		printf ( "%s (%s) : %d個  %s (%s) : %d個\n", name [PLAYER1], stoneColor [player1 - 1], stone_P1, name [PLAYER2], stoneColor [player2 - 1], stone_P2 );
	}

	/* 誰が勝ちか */
	if ( ( stone_P - stone_C ) > 0 ) {
		printf ( "\n%sの勝ち！", name [PLAYER] );
	} else if ( ( stone_P - stone_C ) < 0 ) {
		printf ( "\n%sの勝ち...", name [COMPUTER] );
	} else if ( ( stone_P1 - stone_P2 ) > 0 ) {
		printf ( "\n%sの勝ち！", name [PLAYER1] );
	} else if ( ( stone_P1 - stone_P2 ) < 0 ) {
		printf ( "\n%sの勝ち！", name [PLAYER2] ); 
	} else {
		printf ( "\n引き分け" );
	}
}


/* メインの動き */
int main ( void ) {
	int rtn; /* writePlayerMark 関数，checkTheBoard 関数の戻り値を受け取る */

	srand ( (unsigned int) time ( NULL ) );

	/* ボードの初期設定 */
	initialize ();

	/* プレイ人数と先手後手を決める */
	decidePlay (); 

	while ( 1 ) {
		if ( mode == 1 ) {
		/* 1人プレイ・モード */

			if ( turn1 == player ) {
			/* プレイヤーのターン */

				/* 置けるかどうかを調べる */
				rtn = checkOnBoard ( mode, player );

				if ( rtn == 1 ) { /* 「プレイヤーは石を置けない」と判定されたら，以降の処理をスキップしてループの先頭に戻る */
					printf ( "\n%sはパス\n", name [PLAYER] );
					turn1 = computer;
					continue;
				}

				/* checkOnBoard 関数の処理結果を含めてボードを表示する */
				writeBoard ();

				printf ( "\n%sのターン\n", name [PLAYER] );

				/* プレイヤーがマスを選んで石を置く */
				if ( play == DEMO ) {
					writePlayerMarkDEMO ( player );
				} else {
					rtn = writePlayerMark ( player );
				}

				if ( rtn == EXIT ) { /* ゲームを途中で中断できるようにする */
					printf ( "ゲームを中断します。" );
					return 1;
				} else if ( rtn == DEMO ) {
					play = DEMO;
					continue;
				}

			} else {
			/* コンピュータのターン */

				/* checkOnBoard 関数の処理の前にボードを表示する（コンピュータのターンでは "配置可能" のマークを表示する必要がないから） */
				writeBoard ();

				/* 置けるかどうかを調べる */
				rtn = checkOnBoard ( mode, computer );

				if ( rtn == 1 ) { /* 「コンピュータは石を置けない」と判定されたら，以降の処理をスキップしてループの先頭に戻る */
					printf ( "\n%sはパス\n", name [COMPUTER] );
					turn1 = player;
					continue;
				}

				printf ( "\n%sのターン\n", name [COMPUTER] ); /* "コンピュータのターン"と表示 */

				/* コンピュータがマスを選んで石を置く */
				writeComputerMark ();
			}

			/* 石をひっくり返す */
			turnOverStone ( turn1 );

			/* 両者が石を置けるかどうか判定する */
			rtn = checkOnBoard ( CHECK, player ) + checkOnBoard ( CHECK, computer );

			if ( rtn == 2 ) { /* 「両者共に置けない」と判定されたら，ボードを表示してループを抜ける */
				writeBoard ();
				break;
			}

			if ( turn1 == player ) { /* 先手がプレイヤーのとき，次はコンピュータのターン */
				turn1 = computer;
			} else {                 /* 先手がコンピュータのとき，次はプレイヤーのターン */
				turn1 = player;
			}

		} else {
		/* 2人プレイ・モード */

			if ( turn2 == player1 ) {
			/* プレイヤー 1 のターン */

				/* 置けるかどうかを調べる */
				rtn = checkOnBoard ( mode, player1 );
				//printf("rtn = %d", rtn);
				//return 2;
				if ( rtn == 1 ) { /* 「プレイヤー 1 は石が置けない」と判定されたら，以降の処理をスキップしてループの先頭に戻る */
					printf ( "\n%sはパス\n", name [PLAYER1] );
					turn2 = player2;
					continue;
				}

				/* checkOnBoard 関数の処理結果を含めてボードを表示する */
				writeBoard ();

				printf ( "\n%sのターン\n", name [PLAYER1] ); /* "＜プレイヤー 1 の名前＞ のターン"と表示 */

				/* マスを選んで石を置く */
				if ( play == DEMO ) {
					writePlayerMarkDEMO ( player1 );
				} else {
					rtn = writePlayerMark ( player1 );
				}

				if ( rtn == EXIT) { /* ゲームを途中で中断できるようにする */
					printf ( "ゲームを中断します。" );
					return 1;
				} else if ( rtn == DEMO ) {
					play = DEMO;
					continue;
				}

			} else {
			/* 先手がプレイヤー 2 のとき */

				/* 置けるかどうかを調べる */
				rtn = checkOnBoard ( mode, player2 );

				if ( rtn == 1 ) { /* 「プレイヤー 2 は石が置けない」と判定されたら，以降の処理をスキップしてループの先頭に戻る */
					printf ( "\n%sはパス\n", name [PLAYER2] );
					turn2 = player1;
					continue;
				}

				/* checkOnBoard 関数の処理結果を含めてボードを表示する */
				writeBoard ();

				printf ( "\n%sのターン\n", name [PLAYER2] ); /* "＜プレイヤー 2 の名前＞ のターン"と表示 */

				/* マスを選んで石を置く */
				if ( play == DEMO ) {
					writePlayerMarkDEMO ( player2 );
				} else {
					rtn = writePlayerMark ( player2 );
				}

				if ( rtn == EXIT ) { /* ゲームを途中で中断できるようにする */
					printf ( "ゲームを中断します。" );
					return 1;
				} else if ( rtn == DEMO ) {
					play = DEMO;
					continue;
				}
			}

			/* 石をひっくり返す */
			turnOverStone ( turn2 );

			/* 両者が石を置けるかどうか判定する */
			rtn = checkOnBoard ( CHECK, player1 ) + checkOnBoard ( CHECK, player2 );

			if ( rtn == 2 ) { /* 「両者共に置けない」と判定されたら，ボードを表示してループを抜ける */
				writeBoard ();
				break;
			}

			if ( turn2 == player1 ) { /* 先手がプレイヤー 1 のとき，次はプレイヤー 2 のターン */
				turn2 = player2;
			} else {                  /* 先手がプレイヤー 2 のとき，次はプレイヤー 1 のターン */
				turn2 = player1;
			}
		}
	}

	/* ゲームの勝敗を決める */
	judge ();

	return 0;
}