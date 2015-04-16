#include "stdio.h"
#include <sys/types.h>
#include <openssl/md5.h>  /* please read this */
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "stdlib.h"
using namespace std;

void keygen(char *pphrase, int t);
void crypt(FILE *keyfile, FILE *fp);
void invkey(FILE *keyfile);
void histo(int period, int which, FILE *fp);
void solve(int max_t, FILE* fp);
int findChar(char * input, char c);



