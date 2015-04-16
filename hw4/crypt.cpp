#include "hw4.h"
#include <vector>
using namespace std;
void crypt(FILE *keyfile, FILE *fp){
	vector<string> keys;
	char line[29]={'a'};
	char buf1[3] = {'a'};
	int index = 0, key_num = 0, i_num = 0;
	string origin = "abcdefghijklmnopqrstuvwxyz";
	while(!feof(keyfile)){
		if(fgets(line, 29, keyfile) != NULL){ 
			string str(line);
			keys.push_back(str);
		}
	}
	while(!feof(fp)){
		fread(buf1, sizeof(char), 1, fp);
		if(!feof(fp)){
			if(isalpha(buf1[0])){
				index = findChar((char *)origin.c_str(), buf1[0]);
				i_num = key_num % (keys.size());
				printf("%c", keys[i_num][index]);
			}else{
				printf("%c", buf1[0]);
			}
		}
		key_num++;
	}
	fclose(keyfile);
	fclose(fp);
}
