#include "hw6.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "stdint.h"
#include "unistd.h"
#include <string>
using namespace std;



void keyexpand(unsigned char key[]) {
	unsigned char w[NB * (NR + 1)][4];
	unsigned char tmp[4];
	unsigned char Rcon[NB * (NR+1) / NK - 1][4];
	for (int i = 0; i < NK; i++) {
		for(int j = 0; j < NK; j++) {
			w[i][j] = key[i * 4 + j];
		}
	}
	//calculate Rcon
	for (int i = 0; i < 4; i++) {
		Rcon[0][i] = 0x00;
	}
	Rcon[0][0] = 0x01;
	for (int i = 1; i < 10; i++) {
		unsigned char a1 = Rcon[i-1][0];
		unsigned char b1 = 0x02;
		unsigned char d1 = 0;
		unsigned char v[8];
		v[0] = a1;
		for (int mm = 1; mm < 8; mm++) {
			a1 = xtime(a1);
			v[mm] = a1;
		}
		for (int mm = 0; mm < 8; mm++) {
			unsigned char tmp_b = b1 >> mm;
			int flag = 0x01;
			if (tmp_b & flag) {
				d1 = d1 ^ v[mm];
			}
		}

		Rcon[i][1] = 0x00;
		Rcon[i][2] = 0x00;
		Rcon[i][3] = 0x00;
		Rcon[i][0] = d1;
	}

	for (int i = NK; i < NB * (NR + 1); i++) {
		//copy to tmp[4]
		for (int j = 0; j < 4; j++) {
			tmp[j] = w[i-1][j];
			//fprintf(stderr, "tmp = %x ", tmp[j]);
		} 
		//fprintf(stderr, "\n" );
		if (i % NK == 0) {
			RotWord(tmp, 4);
			subWord(tmp, 4);
			for (int j = 0; j < 4; j++) {
				int idx = i/NK -1;
				unsigned char tmp_1 = tmp[j] ^ Rcon[idx][j];
				tmp[j] = tmp_1;
			}

		}else if (NK > 6 && (i % NK == 4)) {
			subWord(tmp, 4);
		}
		for (int j = 0; j < 4; j++) {
			w[i][j] = w[i-NK][j] ^ tmp[j];
		}
	}
	//printf
	for (int i = 0; i < NB * (NR + 1); i++) {
		if (i < 10)  fprintf(stdout, "w[ %d]: ", i);
		else  fprintf(stdout, "w[%d]: ", i);
		for (int j = 0; j < 4; j++) {
			fprintf(stdout, "%02x", w[i][j]);
		}
		printf("\n");
	}


}

void subWord(unsigned char tmp[], int len) {
	for (int i = 0; i < len; i++) {
		int index = tmp[i];
		tmp[i] = S[index];
	}
}

void RotWord(unsigned char a[], int len) {
	unsigned char tmp = a[0];
	for (int i = 1; i < len; i++) {
		a[i-1] = a[i];
	}
	a[len-1] = tmp;
}

