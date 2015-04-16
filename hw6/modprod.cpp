#include "hw6.h"
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "stdint.h"
#include "unistd.h"
#include <string>
using namespace std;

void modProduct(unsigned char a[], unsigned char b[], unsigned char d[]);

void modprod(long unsigned poly1, long unsigned poly2) {
	unsigned char a[4];
	unsigned char b[4];
	unsigned char d[4];
	long unsigned tmp = poly1;
	for (int i = 0; i < 4; i++) {
		a[i] = tmp & 0xff;
		tmp = tmp >> 8;
	}
	tmp = poly2;
	for (int i = 0; i < 4; i++) {
		b[i] = tmp & 0xff;
		tmp = tmp >> 8;
	}
	//compute d[4]
	modProduct(a, b, d);
	//print product

	 tmp = poly1;
	for (int i = 0; i < 4; i++) {
		a[i] = tmp & 0xff;
		tmp = tmp >> 8;
	}

	for (int i = 3; i >= 0; i--) {
		int tmp_a = a[i];
		printf("{%02x}", tmp_a);
	}

	printf(" CIRCLEX ");

	for (int i = 3; i >= 0; i--) {
		int tmp_b = b[i];
		printf("{%02x}", tmp_b);
	}
 
	printf(" = ");

	for (int i = 3; i >= 0; i--) {
		int tmp_d = d[i];
		printf("{%02x}", tmp_d);
	}

	printf("\n");	
}

unsigned char xtime(unsigned char c) {
	int mask = 1;
	mask = mask << 7;
	if ((c & mask) != mask) {
		return c << 1;
	}else {
		unsigned char c1 = c << 1;
		c1 = c1 ^ 0x1b;
		return c1;
	}
}

void modProduct(unsigned char a[], unsigned char b[], unsigned char d[]) {
	int count = 0;
	while (count < 4) {
		d[3 - count] = 0;
		for (int k = 0; k < 4; k++) {
			unsigned char a1 = a[3 - k];
			unsigned char b1 = b[k];
			unsigned char d1 = 0;
			unsigned char v[8];
			v[0] = a1;
			for (int i = 1; i < 8; i++) {
				a1 = xtime(a1);
				v[i] = a1;
			}
			for (int i = 0; i < 8; i++) {
				unsigned char tmp_b = b1 >> i;
				int flag = 0x01;
				if (tmp_b & flag) {
					d1 = d1 ^ v[i];
				}
			}
			d[3 - count] = d[3 - count] ^ d1;
		}
		//right rotate  array
		int last = a[3];
		for (int i = 3; i > 0; i--) {
			a[i] = a[i-1];
		}
		a[0] = last;
		count++;
	}
}

