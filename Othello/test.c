#include <stdio.h>

int main(void) {
	int mode;
	while (1) {
		/* �v���C�E���[�h�I�� */
		char m = 0;       /* mode */
		/*
		�����ȊO�̕����C�܂��͕������n���ꂽ���̋����𓱓�������
		�� ASCII�R�[�h�ŏ������򂷂�
		*/
		printf("\n���l�ŗV�т܂����H\n�l�������( 1�l�Ńv���C�F1, 2�l�Ńv���C�F2 ) > ");
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
		if (m >= '3' && m <= '9') {                               /* 3 �ȏ�̐��������͂��ꂽ�ꍇ */
			printf("���̃Q�[����2�l�܂ŗV�ׂ܂��B\n");
			continue;
		} else if (m <= 12 || (m >= 14 && m <= '/') || m >= ':') {
			printf("��������͂��Ă��������B\n");
			continue;
		} else if (m == '0') {                                     /* ���̐������͂��ꂽ�ꍇ */
			printf("�Q�[�����I�����܂����H�iyes/no�j> ");
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