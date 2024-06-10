/* �I�Z���E�Q�[�� */
/* ���̃v���O�����̓^�[�~�i���̔w�i�F�� "���F" �ł��邱�Ƃ�O��ɍ쐬���Ă��� */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* ���s�m�F�p */
#define EXIT 8
#define DEMO 9
int play;

/* �{�[�h�ݒ� */
#define MATRIX 8      /* �{�[�h�̃}�X�̐��B�K�������ɂ��邱�Ɓi�ŏ���4�̐΂̔z�u�̕֋X�̂��߁j*/

/* 1�l�v���C */
#define PLAYER 0      /* 1�l�v���C���C�v���C���[�̃}�[�N */
#define COMPUTER 1    /* 1�l�v���C���C�R���s���[�^�̃}�[�N */

/* 2�l�v���C */
#define PLAYER1 2    /* 2�l�v���C���C1�l�� */
#define PLAYER2 3    /* 2�l�v���C���C2�l�� */

/* �{�[�h�̃}�[�N��` */
#define ENPTY 0       /* �󔒂̃}�X */
#define BLACK 1       /* �΂̍���"��" */
#define WHITE 2       /* �΂̔���"��" */
#define PUTABLE 3     /* �u�΂�u���܂��v�T�C�� */ 

/* �`�F�b�N */
#define CHECK 5       /* �v���C���[�y�уR���s���[�^���΂�u���邩�ǂ��� */

/* �O���[�o���ϐ� */
/* �{�[�h��� */
int board [MATRIX][MATRIX];    /* �{�[�h�̏� */
int put_x;                     /* �΂�u�����}�X�� x ���W */
int put_y;                     /* �΂�u�����}�X�� y ���W */

/* �^�[���Ȃ� */
int mode;        /* �Q�[�����[�h */
int player;      /* �v���C���[�̐΂̐F */
int computer;    /* �R���s���[�^�̐΂̐F */
int player1;     /* �v���C���[ 1 �̐΂̐F */
int player2;     /* �v���C���[ 2 �̐΂̐F */
int turn1;       /* 1�l�v���C���C�΂�u���l ( �v���C���[�F1�C�R���s���[�^�F2 ) */
int turn2;       /* 2�l�v���C���C�΂�u���l ( �v���C���[ 1�F1�C�v���C���[ 2�F2 ) */

/* ���O */
char name [4][20] = { "�v���C���[", "�R���s���[�^" };    /* 1�l�v���C�̎��̃v���C���[�ƃR���s���[�^�̖��O */

/* ���ƌ�� */
char first [20];    /* ��� */
char second [20];   /* ��� */


/* �{�[�h������������֐� */
void initialize ( void ) {
	int y, x;

	/* �{�[�h�̑S�Ẵ}�X���󔒂ɏ����� */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			board [y][x] = ENPTY;
		}
	}

	/* �{�[�h�̐^�񒆂� 4 �̃}�X�ɐ΂�u�� */
	board [MATRIX / 2 - 1][MATRIX / 2 - 1] = WHITE;
	board [MATRIX / 2 - 1][MATRIX / 2] = BLACK;
	board [MATRIX / 2][MATRIX / 2 - 1] = BLACK;
	board [MATRIX / 2][MATRIX / 2] = WHITE;
}


/* �v���C�l���Ə��Ԃ����߂�֐� */
void decidePlay ( void ) {
	int holeStone;                        /* �������΂̐F ( ���� 0�C���� 1 ) */
	int select;                           /* �����΂̍��ʂ͏ォ���� ( ��F0�C���F1 ) */
	int selecter;                         /* 2�l�v���C�̎��C�����΂̍��ʂ̌����𓚂���l�i 1�F�v���C���[ 1�C2�F�v���C���[ 2 �j */
	int s;                                /* for���[�v�Ɏg�p */
	char side [2][3] = { "��", "��" };    /* �����΂̍��ʂ̌��� */
	int n = 0;

	while (1) {
		/* �v���C�E���[�h�I�� */
		char m = 0;       /* mode */
		/*
		�����ȊO�̕����C�܂��͕������n���ꂽ���̋����𓱓�������
		�� ASCII�R�[�h�ŏ������򂷂�
		*/
		printf("\n���l�ŗV�т܂����H\n�l�������( 1�l�Ńv���C�F1, 2�l�Ńv���C�F2 ) > ");
		do {
			scanf("%c", &m);
		} while (m == '\n');
		/* ASCII�R�[�h�ŏ������� */
		if (m == '1' || m == '2') {
			mode = m - '0';
			break;
		}
		/* ���̓G���[���� */
		if (m >= '3' && m <= '9') {            /* ���� */
			printf("���̃Q�[����2�l�܂ŗV�ׂ܂��B\n");
			continue;
		} else if (m <= 12 || ( m >= 14 && m <= '/' ) || m >= ':') { /* �����ȊO */
			printf("��������͂��Ă��������B\n");
			continue;
		} else if (m == '0') {                /* �Q�[�����I�� */
			printf("�Q�[�����I�����܂����H�iyes/no�j> ");
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

	/* �������΂̍��ʂ̌��������߂�B�����F��� 0�C���� 1 */
	holeStone = rand () % 2;

	/* 2�l�v���C���[�h�̂Ƃ��C�v���C���[�̖��O����͂����� */
	if ( mode == 2 ) {
		printf ( "\n�v���C���[�̖��O����͂��Ă��������B�i���{��C���[�}���̂ǂ���ł��j\n\n" );
		for ( s = 0; s < 2; s++ ) {
			printf ( "�v���C���[ %d > ", s + 1 ); scanf ( "%s", name [s + 2] );
		}
		printf("-----------------------------------------------------\n");
	}

	printf("\n�����΂Ő��i�� \"��\"�j�ƌ��i�� \"��\"�j�����߂܂��B\n");

	/* 2�l�v���C���[�h�̂Ƃ��C�񓚎҂������_���Ɍ��߂� */
	if ( mode == 2 ) {
		selecter = rand () % 2 + 2;
		printf ( "�񓚎҂� %s �ł��B\n", name [selecter] );
	}

	/* �����΂̍��ʂ̌����𕷂� */
	do {
		printf ( "�����΂̍��ʂ͏�ł����H ���ł����H\n���ʂ�...(��F0�C���F1) > " ); scanf ( "%d", &select );
	} while ( select < 0 || select > 1 ); /* select �� 0, 1 �̂ǂ��炩����͂��Ȃ��ƃ��[�v�𔲂����Ȃ��悤�ɂ��� */
	
	/* �������߂� */
	switch ( mode ) {
		case 1: /* 1�l�v���C���[�h */
			if ( holeStone == select ) { /* �v���C���[�����ʂ̌������������Ă��ꍇ�C���̓v���C���[ */
				strcpy ( first, name [PLAYER] );
				strcpy ( second, name [COMPUTER] );
				player = BLACK;
				computer = WHITE;
				turn1 = player;
			} else {                     /* �v���C���[�����ʂ̌������͂������ꍇ�C���̓R���s���[�^ */
				strcpy ( first, name [COMPUTER] );
				strcpy ( second, name [PLAYER] );
				computer = BLACK;
				player = WHITE;
				turn1 = computer;
			}
			break;
		case 2: /* 2�l�v���C���[�h */
			if ( ( selecter == PLAYER1 && holeStone == select ) ||  /* ���� 1 : �񓚎҂��v���C���[ 1 ���� �v���C���[ 1 �����ʂ̌������������Ă� */
				 ( selecter == PLAYER2 && holeStone != select ) ) { /* ���� 2 : �񓚎҂��v���C���[ 2 ���� �v���C���[ 2 �����ʂ̌������͂����� */
                                                                    /* �ȏ�̏����̂����ꂩ�𖞂����΁C���̓v���C���[ 1 */
				strcpy ( first, name [PLAYER1] );
				strcpy ( second, name [PLAYER2] );
				player1 = BLACK;
				player2 = WHITE;
				turn2 = player1;
			} else if ( ( selecter == PLAYER1 && holeStone != select ) ||  /* ���� 1 : �񓚎҂��v���C���[ 1 ���� �v���C���[ 1 �����ʂ̌������͂����� */
						( selecter == PLAYER2 && holeStone == select ) ) { /* ���� 2 : �񓚎҂��v���C���[ 2 ���� �v���C���[ 2 �����ʂ̌������������Ă� */
                                                                           /* �ȏ�̏����̂����ꂩ�𖞂����΁C���̓v���C���[ 2 */
				strcpy ( first, name [PLAYER2] );
				strcpy ( second, name [PLAYER1] );
				player2 = BLACK;
				player1 = WHITE;
				turn2 = player2;
			}
			break;
	}
	printf ( "�����΂̍��ʂ� %s �Ȃ̂ŁA���i���j�� %s �A���i���j�� %s �ł��B\n", side [holeStone], first, second );
}


/* �{�[�h��\������֐� */
void writeBoard ( void ) {
	int s, t;                      /* for���[�v�Ŏg�p */
	char mark [4][4] = { " ",      /* �� */
						 "��",      /* ���� */
						 "��",      /* ���� */
						 "." };    /* �z�u�\ */

	printf ( "\n    " );
	for ( s = 0; s < MATRIX; s++ ) {
		printf ( " %c", 97 + s ); /* �{�[�h�̗�ԍ� */
	}
	printf ( "\n" );

	for ( s = 0; s < MATRIX; s++ ) {
		printf ( "   %d ", s + 1 ); /* �{�[�h�̍s�ԍ� */
		for ( t = 0; t < MATRIX; t++ ) {
			printf ( "%s ", mark [board [s][t]] );
		}
		printf ( "\n" );
	}
}


/* �v���C���[���I�������}�X�Ƀ}�[�N����֐��i1�l�v���C�C2�l�v���C���p�j */
int writePlayerMark ( int player ) {
	/*
	����
		player : �΂�u���v���C���[�Ƃ��̐l�̐΂̐F

	�߂�l rtn
		0 : �֐��̏��������ׂďI��
		1 : �Q�[���𒆒f
	*/


	int x;                       /* �v���C���[���I�������}�X�� x ���W */
	int y;                       /* �v���C���[���I�������}�X�� y ���W */
	char get [5];                /* ���͒l�̓���q */
	char finish [5] = "exit";    /* �Q�[���𒆒f����Ƃ��Ɏg�� */
	char test [5] = "test";      /* ���s�m�F */

	while ( 1 ) {
		printf ( "\n�ǂ̃}�X�ɒu���܂����H �i��Fb2�j> " ); scanf ( "%s", get );

		if ( strcmp ( get, finish ) == 0 ) {      /* �y�Q�[���𒆒f����z�i���s���؎��g�p�j */
			return EXIT;
		} else if ( strcmp ( get, test ) == 0 ) { /* ���s�m�F�Ɉڂ� */
			return DEMO;
		}

		/* get �̓��͒l�𕪉����āC���ꂼ��̍��W�ɓ��Ă͂߂� */
		y = get [1] - '1';
		x = get [0] - 'a';

		/* �v���C���[���I�������}�X�̍��W�� "�z�u�\" �}�X�ł��邩�ǂ����𔻒肷�� */
		if ( x >= 0 && x < MATRIX && y >= 0 && y < MATRIX && board [y][x] == PUTABLE ) {
			/* �v���C���[�̐F�̐΂�u�� */
			board [y][x] = player;

			/* �O���[�o���ϐ��ɍ��W���L�^ */
			put_x = x;
			put_y = y;

			break;
		} else { /* "�z�u�\" �}�X�łȂ��ꍇ */
			printf ( "                                �����ɂ͒u���܂���\n" );
		}
	}

	/* PUTABLE �}�[�N�ɐݒ肳��Ă���}�X�� ENPTY �}�[�N�ɏ��������� */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == PUTABLE ) {
				board [y][x] = ENPTY;
			}
		}
	}

	return 0;
}


/* ���s�m�F�p�i�}�X�����I���j */
void writePlayerMarkDEMO ( int player ) {
	int x;    /* �v���C���[���I�������}�X�� x ���W */
	int y;    /* �v���C���[���I�������}�X�� y ���W */

	while ( 1 ) {
		y = rand () % MATRIX;
		x = rand () % MATRIX;
		if ( board [y][x] == PUTABLE ) {
			board [y][x] = player;
			put_x = x;
			put_y = y;
			if ( mode == 1 ) {
				printf ( "%s�� %c%d �ɒu���܂����B\n", name [PLAYER], 'a' + x, y + 1 );
			} else {
				if ( player == player1 ) {
					printf ( "%s�� %c%d �ɒu���܂����B\n", name [PLAYER1], 'a' + x, y + 1 );
				} else if ( player == player2 ) {
					printf ( "%s�� %c%d �ɒu���܂����B\n", name [PLAYER2], 'a' + x, y + 1 );
				}
			}
			break;
		}
	}

	/* PUTABLE �}�[�N�ɐݒ肳��Ă���}�X�� ENPTY �}�[�N�ɏ��������� */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == PUTABLE ) {
				board [y][x] = ENPTY;
			}
		}
	}
}


/* �R���s���[�^���I�������}�X�Ƀ}�[�N����֐� */
void writeComputerMark ( void ) {
	int x;    /* �R���s���[�^���I�������}�X�� x ���W */
	int y;    /* �R���s���[�^���I�������}�X�� y ���W */

	while ( 1 ) {

		/* �}�X�̍��W�������_���ɑI������ */
		y = rand () % MATRIX;
		x = rand () % MATRIX;

		/* �I���������W�� "�z�u�\" �}�X�ł���Έȉ��̏��������� */
		if ( board [y][x] == PUTABLE ) {
			/* �R���s���[�^�̐F�̐΂�u�� */
			board [y][x] = computer;

			/* �O���[�o���ϐ��ɍ��W���L�^ */
			put_x = x;
			put_y = y;
			printf ( "�R���s���[�^�� %c%d �ɒu���܂����B\n", 'a' + x, y + 1 );
			break;
		}
	}

	/* PUTABLE �}�[�N�ɐݒ肳��Ă���}�X�� ENPTY �}�[�N�ɏ��������� */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == PUTABLE ) {
				board [y][x] = ENPTY;
			}
		}
	}

}


/* �΂��Ђ�����Ԃ��֐� */
void turnOverStone ( int human ) {
	/*
	����
		human : �u�����l�̐΂̐F

	�߂�l
		�Ȃ�
	*/
	
	int y, x, s, t, r;                           /* for���[�v�Ɏg�p */
	int turnOver_x [MATRIX * MATRIX] = { 0 };    /* �Ђ�����Ԃ��΂� x ���W */
	int turnOver_y [MATRIX * MATRIX] = { 0 };    /* �Ђ�����Ԃ��΂� x ���W */
	int count;

	for ( t = 0; t < 8; t++ ) {
		count = 0;
		for ( s = 1; s < MATRIX; s++ ) {
			switch ( t ) {
				case 0: /* ���� */
					y = put_y - s;
					x = put_x - s;
					break;
				case 1: /* �� */
					y = put_y - s;
					x = put_x;
					break;
				case 2: /* �E�� */
					y = put_y - s;
					x = put_x + s;
					break;
				case 3: /* �� */
					y = put_y;
					x = put_x - s;
					break;
				case 4: /* �E */
					y = put_y;
					x = put_x + s;
					break;
				case 5: /* ���� */
					y = put_y + s;
					x = put_x - s;
					break;
				case 6: /* �� */
					y = put_y + s;
					x = put_x;
					break;
				case 7: /* �E�� */
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


/* �{�[�h�̏󋵂��m�F���� */
int checkOnBoard ( int mode, int human ) {
	/*
	����
		mode  : �Q�[�����[�h
		human : �΂�u���l

	���f�v�f check_1, check_2, check_3
		0�F�u����
		1�F�u���Ȃ�

	�߂�l rtn
		0 : �΂�u����
		1 : �΂�u���Ȃ�
	*/

	int rtn;
	int check_1 = 0;         /* �`�F�b�N���� 1 �F�{�[�h�̃}�X�����ׂĖ��܂��Ă��邩�ǂ��� */
	int check_2 = 0;         /* �`�F�b�N���� 2 �F�{�[�h��̐΂����ׂč��C�܂��͔��ɂȂ��Ă��邩�ǂ��� */
	int check_3 = 0;         /* �`�F�b�N���� 3 �F�{�[�h�̃}�X�ɐ΂�u���邩�ǂ��� */
	int x, y, w, h, s, t;    /* for���[�v�Ɏg�p */
	int count;               /* �`�F�b�N����3�Ŏg�p */
	int stone = 0;           /* �{�[�h��ɂ���΂̐� */
	int stone_B = 0;         /* �{�[�h��ɂ��鍕�ʂ̐΂̐� */
	int stone_W = 0;         /* �{�[�h��ɂ��锒�ʂ̐΂̐� */
	int set = 0;             /* �`�F�b�N���� 3 �Ŏg�p�BPUTABLE �̐��𐔂��� */
	int turnOverColor = 0;    /* �Ђ�����Ԃ��΂̐F */


	/* �`�F�b�N���� 1�F�{�[�h�̃}�X�����ׂĖ��܂��Ă��邩�ǂ��� ------------------------- */

	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == BLACK || board [y][x] == WHITE ) {
			/* �{�[�h��̐΁i�F�̋�ʂȂ��j�̑����𐔂��� */
				stone++;
			}
		}
	}

	/* �{�[�h�̃}�X�̑����ƃ{�[�h��̐΂̑�������v�����ꍇ�C�u�u���Ȃ��v��Ԃ� */
	if ( stone == MATRIX * MATRIX ) {
		check_1 = 1;
	}

	/* �`�F�b�N���� 2�F�{�[�h��̐΂����ׂč��C�܂��͔��ɂȂ��Ă��邩�ǂ��� --------------- */

	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == BLACK ) {
			/* �{�[�h��ɂ��鍕�ʂ̐΂̐��𐔂��� */
				stone_B++;
			} else {
			/* �{�[�h��ɂ��锒�ʂ̐΂̐��𐔂��� */
				stone_W++;
			}
		}
	}

	/* �{�[�h��ɍ��ʁC�܂��͔��ʂ̐΂�������΁C�u�u���Ȃ��v��Ԃ� */
	if ( stone_B == 0 || stone_W == 0 ) {
		check_2 = 1;
	}

	/* �`�F�b�N���� 3�F�{�[�h�̃}�X�ɐ΂�u���邩�ǂ��� --------------------------------- */

	/* PUTABLE �}�[�N�ɐݒ肳��Ă���}�X�� ENPTY �}�[�N�ɏ��������� */
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
							case 0: /* ����𒲂ׂ� */
								h = y - s;
								w = x - s;
								break;
							case 1: /* ��𒲂ׂ� */
								h = y - s;
								w = x;
								break;
							case 2: /* �E��𒲂ׂ� */
								h = y - s;
								w = x + s;
								break;
							case 3: /* ���𒲂ׂ� */
								h = y;
								w = x - s;
								break;
							case 4: /* �E�𒲂ׂ� */
								h = y;
								w = x + s;
								break;
							case 5: /* �����𒲂ׂ� */
								h = y + s;
								w = x - s;
								break;
							case 6: /* ���𒲂ׂ� */
								h = y + s;
								w = x;
								break;
							case 7: /* �E���𒲂ׂ� */
								h = y + s;
								w = x + s;
								break;
						}
						if ( h >= 0 && h < MATRIX && w >= 0 && w < MATRIX ) {
							if ( board [h][w] == PUTABLE ) {        /* PUTABLE �}�[�N�̃}�X��������C���̕����͒��ׂȂ� */
								break;
							} else if ( board [h][w] == human) {    /* �u���l�̐΂̐F�Ɠ����F��������C���̕����͒��ׂȂ� */
								break;
							} else if ( board [h][w] == ENPTY ) {   /* ���ׂĂ�������Ɏ����̐΂̐F������ */
								if ( count > 0 ) {                  /* ���ׂĂ�������ɑ���̐΂̐F�� 1 �ȏ゠�� */
									if ( mode < CHECK ) {           /* CHECK ���[�h�łȂ���΁C�󔒂̃}�X�� PUTABLE �}�[�N�ɐݒ肷�� */
										board [h][w] = PUTABLE;
									}
									set += 1;
								}
								break;
							} else {                                /* ���ׂĂ�������ɑ���̐΂�����΂��̌��𐔂��� */
								count += 1;
							}
						}
					}
				}
			}
		}
	}

	if ( set > 0 ) {    /* �`�F�b�N���� 3 ���ŏ��Ȃ��Ƃ���� PUTABLE �}�[�N�ɐݒ�ł���� �u�u����v */
		check_3 = 0;
	} else {            /* �`�F�b�N���� 3 ���ň�� PUTABLE �}�[�N�ɐݒ�ł��Ȃ���� �u�u���Ȃ��v */
		check_3 = 1;
	}
	
	/* �߂�l�̔��� */
	if ( check_1 == 0 && check_2 == 0 && check_3 == 0 ) { /* �`�F�b�N���ڂ̂��ׂĂŁu�u����v�Ɣ��肳�ꂽ��C�u�u����v */
		rtn = 0;
	} else {                                              /* �`�F�b�N���ڂ̂����ꂩ��ł��u�u���Ȃ��v�Ɣ��肳�ꂽ��C�u�u���Ȃ��v */
		rtn = 1;
	}

	return rtn;
}


/* �Q�[���̏��s�����߂�֐� */
void judge ( void ) {
	int y, x;                                      /* for ���[�v�Ɏg�p */
	int stone_P = 0;                               /* �v���C���[�̐΂̐� */
	int stone_P1 = 0;                              /* �v���C���[ 1 �̐΂̐� */
	int stone_P2 = 0;                              /* �v���C���[ 2 �̐΂̐� */
	int stone_C = 0;                               /* �R���s���[�^�̐΂̐� */
	char stoneColor [2][3] = { "��", "��" }; /* �΂̐F */

	printf("\n�I���I\n");

	/* �΂̐��𐔂��� */
	for ( y = 0; y < MATRIX; y++ ) {
		for ( x = 0; x < MATRIX; x++ ) {
			if ( board [y][x] == player ) {            /* �v���C���[ */
				stone_P++;
			} else if ( board [y][x] == player1 ) {    /* �v���C���[ 1 */
				stone_P1++;
			} else if ( board [y][x] == player2 ) {    /* �v���C���[ 2 */
				stone_P2++;
			} else {                                   /* �R���s���[�^ */
				stone_C++;
			}
		}
	}

	printf("\n------------- ���ʔ��\ -------------\n");
	if ( mode == 1 ) { /* 1�l�v���C */
		printf ( "%s (%s) : %d��  %s (%s) : %d��\n", name [PLAYER], stoneColor [player - 1], stone_P, name [COMPUTER], stoneColor [computer - 1], stone_C);
	} else {           /* 2�l�v���C */
		printf ( "%s (%s) : %d��  %s (%s) : %d��\n", name [PLAYER1], stoneColor [player1 - 1], stone_P1, name [PLAYER2], stoneColor [player2 - 1], stone_P2 );
	}

	/* �N�������� */
	if ( ( stone_P - stone_C ) > 0 ) {
		printf ( "\n%s�̏����I", name [PLAYER] );
	} else if ( ( stone_P - stone_C ) < 0 ) {
		printf ( "\n%s�̏���...", name [COMPUTER] );
	} else if ( ( stone_P1 - stone_P2 ) > 0 ) {
		printf ( "\n%s�̏����I", name [PLAYER1] );
	} else if ( ( stone_P1 - stone_P2 ) < 0 ) {
		printf ( "\n%s�̏����I", name [PLAYER2] ); 
	} else {
		printf ( "\n��������" );
	}
}


/* ���C���̓��� */
int main ( void ) {
	int rtn; /* writePlayerMark �֐��CcheckTheBoard �֐��̖߂�l���󂯎�� */

	srand ( (unsigned int) time ( NULL ) );

	/* �{�[�h�̏����ݒ� */
	initialize ();

	/* �v���C�l���Ɛ��������߂� */
	decidePlay (); 

	while ( 1 ) {
		if ( mode == 1 ) {
		/* 1�l�v���C�E���[�h */

			if ( turn1 == player ) {
			/* �v���C���[�̃^�[�� */

				/* �u���邩�ǂ����𒲂ׂ� */
				rtn = checkOnBoard ( mode, player );

				if ( rtn == 1 ) { /* �u�v���C���[�͐΂�u���Ȃ��v�Ɣ��肳�ꂽ��C�ȍ~�̏������X�L�b�v���ă��[�v�̐擪�ɖ߂� */
					printf ( "\n%s�̓p�X\n", name [PLAYER] );
					turn1 = computer;
					continue;
				}

				/* checkOnBoard �֐��̏������ʂ��܂߂ă{�[�h��\������ */
				writeBoard ();

				printf ( "\n%s�̃^�[��\n", name [PLAYER] );

				/* �v���C���[���}�X��I��Ő΂�u�� */
				if ( play == DEMO ) {
					writePlayerMarkDEMO ( player );
				} else {
					rtn = writePlayerMark ( player );
				}

				if ( rtn == EXIT ) { /* �Q�[����r���Œ��f�ł���悤�ɂ��� */
					printf ( "�Q�[���𒆒f���܂��B" );
					return 1;
				} else if ( rtn == DEMO ) {
					play = DEMO;
					continue;
				}

			} else {
			/* �R���s���[�^�̃^�[�� */

				/* checkOnBoard �֐��̏����̑O�Ƀ{�[�h��\������i�R���s���[�^�̃^�[���ł� "�z�u�\" �̃}�[�N��\������K�v���Ȃ�����j */
				writeBoard ();

				/* �u���邩�ǂ����𒲂ׂ� */
				rtn = checkOnBoard ( mode, computer );

				if ( rtn == 1 ) { /* �u�R���s���[�^�͐΂�u���Ȃ��v�Ɣ��肳�ꂽ��C�ȍ~�̏������X�L�b�v���ă��[�v�̐擪�ɖ߂� */
					printf ( "\n%s�̓p�X\n", name [COMPUTER] );
					turn1 = player;
					continue;
				}

				printf ( "\n%s�̃^�[��\n", name [COMPUTER] ); /* "�R���s���[�^�̃^�[��"�ƕ\�� */

				/* �R���s���[�^���}�X��I��Ő΂�u�� */
				writeComputerMark ();
			}

			/* �΂��Ђ�����Ԃ� */
			turnOverStone ( turn1 );

			/* ���҂��΂�u���邩�ǂ������肷�� */
			rtn = checkOnBoard ( CHECK, player ) + checkOnBoard ( CHECK, computer );

			if ( rtn == 2 ) { /* �u���ҋ��ɒu���Ȃ��v�Ɣ��肳�ꂽ��C�{�[�h��\�����ă��[�v�𔲂��� */
				writeBoard ();
				break;
			}

			if ( turn1 == player ) { /* ��肪�v���C���[�̂Ƃ��C���̓R���s���[�^�̃^�[�� */
				turn1 = computer;
			} else {                 /* ��肪�R���s���[�^�̂Ƃ��C���̓v���C���[�̃^�[�� */
				turn1 = player;
			}

		} else {
		/* 2�l�v���C�E���[�h */

			if ( turn2 == player1 ) {
			/* �v���C���[ 1 �̃^�[�� */

				/* �u���邩�ǂ����𒲂ׂ� */
				rtn = checkOnBoard ( mode, player1 );
				//printf("rtn = %d", rtn);
				//return 2;
				if ( rtn == 1 ) { /* �u�v���C���[ 1 �͐΂��u���Ȃ��v�Ɣ��肳�ꂽ��C�ȍ~�̏������X�L�b�v���ă��[�v�̐擪�ɖ߂� */
					printf ( "\n%s�̓p�X\n", name [PLAYER1] );
					turn2 = player2;
					continue;
				}

				/* checkOnBoard �֐��̏������ʂ��܂߂ă{�[�h��\������ */
				writeBoard ();

				printf ( "\n%s�̃^�[��\n", name [PLAYER1] ); /* "���v���C���[ 1 �̖��O�� �̃^�[��"�ƕ\�� */

				/* �}�X��I��Ő΂�u�� */
				if ( play == DEMO ) {
					writePlayerMarkDEMO ( player1 );
				} else {
					rtn = writePlayerMark ( player1 );
				}

				if ( rtn == EXIT) { /* �Q�[����r���Œ��f�ł���悤�ɂ��� */
					printf ( "�Q�[���𒆒f���܂��B" );
					return 1;
				} else if ( rtn == DEMO ) {
					play = DEMO;
					continue;
				}

			} else {
			/* ��肪�v���C���[ 2 �̂Ƃ� */

				/* �u���邩�ǂ����𒲂ׂ� */
				rtn = checkOnBoard ( mode, player2 );

				if ( rtn == 1 ) { /* �u�v���C���[ 2 �͐΂��u���Ȃ��v�Ɣ��肳�ꂽ��C�ȍ~�̏������X�L�b�v���ă��[�v�̐擪�ɖ߂� */
					printf ( "\n%s�̓p�X\n", name [PLAYER2] );
					turn2 = player1;
					continue;
				}

				/* checkOnBoard �֐��̏������ʂ��܂߂ă{�[�h��\������ */
				writeBoard ();

				printf ( "\n%s�̃^�[��\n", name [PLAYER2] ); /* "���v���C���[ 2 �̖��O�� �̃^�[��"�ƕ\�� */

				/* �}�X��I��Ő΂�u�� */
				if ( play == DEMO ) {
					writePlayerMarkDEMO ( player2 );
				} else {
					rtn = writePlayerMark ( player2 );
				}

				if ( rtn == EXIT ) { /* �Q�[����r���Œ��f�ł���悤�ɂ��� */
					printf ( "�Q�[���𒆒f���܂��B" );
					return 1;
				} else if ( rtn == DEMO ) {
					play = DEMO;
					continue;
				}
			}

			/* �΂��Ђ�����Ԃ� */
			turnOverStone ( turn2 );

			/* ���҂��΂�u���邩�ǂ������肷�� */
			rtn = checkOnBoard ( CHECK, player1 ) + checkOnBoard ( CHECK, player2 );

			if ( rtn == 2 ) { /* �u���ҋ��ɒu���Ȃ��v�Ɣ��肳�ꂽ��C�{�[�h��\�����ă��[�v�𔲂��� */
				writeBoard ();
				break;
			}

			if ( turn2 == player1 ) { /* ��肪�v���C���[ 1 �̂Ƃ��C���̓v���C���[ 2 �̃^�[�� */
				turn2 = player2;
			} else {                  /* ��肪�v���C���[ 2 �̂Ƃ��C���̓v���C���[ 1 �̃^�[�� */
				turn2 = player1;
			}
		}
	}

	/* �Q�[���̏��s�����߂� */
	judge ();

	return 0;
}