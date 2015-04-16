#include "hw6.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "stdint.h"
#include "unistd.h"
#include <map>
#include <string>
using namespace std;

void expandKey (unsigned char key[], unsigned char w[][4]) {
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
}

void printState(int round_AES, unsigned char array[], const char name[]) {
	if (round_AES == 10) {
		printf("round[%d]", round_AES);
	}else {
		printf("round[ %d]", round_AES);
	}
	printf(".%s", name);
	for( int i = 0; i < 9 - strlen(name); i++) {
		printf(" ");
	}
	for (int i = 0; i < 16; i++) {
		printf("%02x", array[i]);
	}
	printf("\n");
}
void AddRoundKey(unsigned char state[][4], unsigned char w[][4], int round_AES) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[j][i] = state[j][i] ^ w[4 * round_AES + i][j];
		}
	}
}
void SubBytes(unsigned char state[][4], int round_AES) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			unsigned char tmp = state[i][j];
			state[i][j] = S[tmp];
		}
	}
}
void ShiftRows(unsigned char state[][4], int round_AES) {
	for ( int i = 1; i < 4; i++) {
		for (int j = 0; j < i; j++) {
			RotWord(state[i], 4);
		}
	}
}

void MixColumns(unsigned char state[][4], int round_AES, unsigned char table[]) {
	unsigned char tmp[4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[j] = state[j][i];
		}
		unsigned char d[4];
		modProduct(tmp, table, d);
		for (int j = 0; j < 4; j++) {
			state[j][i] = d[j];
		}
	}
}

void encrypt (unsigned char in[], unsigned char out[], unsigned char w[][4]) {
	unsigned char state[4][NB];
	unsigned char key[16];
	//copy in to state
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[j][i] = in[i*4 + j];
		}
	}
	string name = "input";
	printState(0, in, name.c_str());
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++) {
			key[i * 4 + j] = w[i][j];
		}
	}
	name = "k_sch";
	printState(0, key, name.c_str());
	AddRoundKey(state, w, 0);
	unsigned char tmp[16];
	int round_AES = 0;
	for (round_AES = 1; round_AES < NR; round_AES++) {
		
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i * 4+j] = state[j][i];
			}
		}
		name = "start";
		printState(round_AES, tmp, name.c_str());
		SubBytes(state, round_AES);
		//tmp = {'0'};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i * 4+j] = state[j][i];
			}
		}
		name = "s_box";
		printState(round_AES, tmp, name.c_str());
		ShiftRows(state, round_AES);
		//tmp = {0};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i * 4+j] = state[j][i];
			}
		}
		name = "s_row";
		printState(round_AES, tmp, name.c_str());

		MixColumns(state, round_AES, P);
		//tmp = {'0'};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i * 4+j] = state[j][i];
			}
		}
		name = "m_col";
		printState(round_AES, tmp, name.c_str());
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++) {
				key[i * 4 + j] = w[4 * round_AES + i][j];
			}
		}
		name = "k_sch";
		printState(round_AES, key,  name.c_str());
		AddRoundKey(state, w, round_AES);
	}
	//tmp = {'0'};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i * 4+j] = state[j][i];
		}
	}
	name = "start";
	printState(round_AES, tmp, name.c_str());
	SubBytes(state, round_AES);
	//tmp = {'0'};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i * 4+j] = state[j][i];
		}
	}
	name = "s_box";
	printState(round_AES, tmp, name.c_str());
	ShiftRows(state, round_AES);
	//tmp = {'0'};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i * 4+j] = state[j][i];
		}
	}
	name = "s_row";
	printState(round_AES, tmp, name.c_str());
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++) {
			key[i * 4 + j] = w[4 * round_AES + i][j];
		}
	}
	name = "k_sch";
	printState(round_AES, key,name.c_str());
	AddRoundKey(state, w, round_AES);
	//tmp = {'0'};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i * 4+j] = state[j][i];
		}
	}
	name = "output";
	printState(round_AES, tmp, name.c_str());

}

void iShiftRows(unsigned char state[][4], int round_AES) {
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < i; j++) {
			int last = state[i][3];
			for (int k = 3; k > 0; k--) {
				state[i][k] = state[i][k-1];
			}
			state[i][0] = last;
		}
	}
}

void iSubBytes(unsigned char state[][4], int round_AES, unsigned char IS[]){
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			unsigned char tmp = state[i][j];
			state[i][j] = IS[tmp];
		}
	}
}
void iMixColumns(unsigned char state[][4], int round_AES) {
	MixColumns(state, round_AES, INVP);
}
void decrypt (unsigned char in[], unsigned char out[], unsigned char w[][4]) {
	unsigned char state[4][NB];
	unsigned char key[16];
	unsigned char INS[256];
	//inverse S
	for (int i = 0; i < 256; i++) {
		INS[S[i]] = i;
	}

	//copy in to state
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			state[j][i] = in[i*4 + j];
		}
	}
	printState(0, in, "iinput");
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++) {
			key[i * 4 + j] = w[4 * 10 + i][j];
		}
	}
	printState(0, key,"ik_sch");
	AddRoundKey(state, w, NR);
	unsigned char tmp[16];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i * 4+j] = state[j][i];
		}
	}
	printState(1, tmp, "istart");
	int round_AES = 0;
	for (round_AES = NR-1; round_AES > 0; round_AES--) {
		//unsigned char tmp[16];
		iShiftRows(state, NR-round_AES);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i * 4+j] = state[j][i];
			}
		}
		printState(NR-round_AES, tmp, "is_row");
		iSubBytes(state, NR - round_AES, INS);
	//	tmp = {0};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i * 4+j] = state[j][i];
			}
		}
		printState(NR - round_AES, tmp, "is_box");
		for (int i = 0; i < 4; i++){
			for (int j = 0; j < 4; j++) {
				key[i * 4 + j] = w[4 * round_AES + i][j];
			}
		}
		printState(NR-round_AES, key, "ik_sch");
		AddRoundKey(state, w, round_AES);
	//	tmp = {0};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i * 4+j] = state[j][i];
			}
		}
		printState(NR-round_AES, tmp, "ik_add");
		iMixColumns(state, NR-round_AES+1);
	//	tmp = {0};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i * 4+j] = state[j][i];
			}
		}
		printState(NR-round_AES+1, tmp, "istart");
		
	}
	iShiftRows(state, NR - round_AES);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i * 4+j] = state[j][i];
		}
	}
	printState(NR-round_AES, tmp, "is_row");
	iSubBytes(state, NR-round_AES, INS);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i * 4+j] = state[j][i];
		}
	}
	printState(NR - round_AES, tmp, "is_box");
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++) {
			key[i * 4 + j] = w[4 * round_AES + i][j];
		}
	}
	printState(NR-round_AES,  key,"ik_sch");
	AddRoundKey(state, w, round_AES);
	//tmp = {0};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp[i * 4+j] = state[j][i];
		}
	}
	printState(NR, tmp, "ioutput");
}
void crypt(unsigned char key[], FILE* ifp, int mode){
	unsigned char in[16];
	unsigned char out[16];
	unsigned char w[NB * (NR + 1)][4];
	expandKey(key, w);
	int bytes = fread(in, sizeof(unsigned char), 16, ifp);
	if (bytes >= 16) {
		if (mode == 0) {
			encrypt(in, out, w);
		}else {
			decrypt(in, out, w);
		}
	}else {
		fprintf(stderr, "Error: the size of the file is incorrect.\n");
		return;
	}
	return;
}


