#include <stdio.h>

int main(void) {
	int mode;
	while (1) {
		/* プレイ・モード選択 */
		char m = 0;       /* mode */
		/*
		数字以外の文字，または文字が渡された時の挙動を導入したい
		⇒ ASCIIコードで条件分岐する
		*/
		printf("\n何人で遊びますか？\n人数を入力( 1人でプレイ：1, 2人でプレイ：2 ) > ");
		scanf("%c", &m);
		int n = 0;
		while (m == '\n') {
			scanf("%c", &m);
			n++;
			if (n > 10) break;
		}
		if (m == '1' || m == '2') {
			mode = m - '0';
			break;
		} else 
		if (m >= '3' && m <= '9') {                               /* 3 以上の整数が入力された場合 */
			printf("このゲームは2人まで遊べます。\n");
			continue;
		} else if (m <= 12 || (m >= 14 && m <= '/') || m >= ':') {
			printf("数字を入力してください。\n");
			continue;
		} else if (m == '0') {                                     /* 負の数が入力された場合 */
			printf("ゲームを終了しますか？（yes/no）> ");
			scanf("%c", &m);
			while (m == '\n') {
				scanf("%c", &m);
				n++;
				if (n > 10) break;
			}
			if (m == 'y') {
				return 1;
			} else {
				continue;
			}
		}
	}
	return 0;
}