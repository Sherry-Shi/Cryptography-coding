#include "hw4.h"

bool allAlpha(string s1){
	int size = s1 .size();
	for (int m = 0; m < size; m++){
		if(!isalpha(s1[m])){
			return false;
		}
	}
	return true;
}
void change_format(char s[]){
	for(int i = strlen(s)-1; i >= 0; i--){
		if(s[i] != '0'){
			s[i+1] = '\0';
			break;
		}
	}
}
void solve(int max_t, FILE* fp){
	char *file = NULL;
	int size = 0;
	int len = 4;
	bool has_sub = false;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	file = (char *) malloc(sizeof(char) * size);
	fread(file, sizeof(char), size, fp);
	string file_str(file);
	printf("Kasiski Method\n");
	printf("==============\n");
	for (; len >= 0; len++){
		has_sub = false;
		for (int i = 0; i + len < size; ++i){
			/* code */
			string s1 = file_str.substr(i, len);
			if (allAlpha(s1) == true){
				for (int pos = i + 1; pos + len < size; pos++){
					string s2 = file_str.substr(pos, len);
					if(s1 == s2){
						has_sub = true;
						printf("len=%d, i=%d, j=%d, j-i=%d, %s\n", len, i, pos, pos-i, s2.c_str());	

					}
				}
			}
		}
		if(has_sub == false){
			printf("len=%d, no more matches\n", len);
			break;
		}

	}
	//compute appearing time of each letter 
	printf("\nAverage Index of Coincidence\n");
	printf("============================\n");
	int size_alpha = 0;
	char buf1[3] = {'a'};
	int count[26] = {0};
	fseek(fp, 0, SEEK_SET);
	while(!feof(fp)){
		fread(buf1, sizeof(char), 1, fp);
		if(!feof(fp)){
			if(isalpha(buf1[0])){
				size_alpha++;
				count[buf1[0] - 'a']++;
			}
			
		}
	}
	printf("L=%d\n", size_alpha);
	for (int k = 0; k < 26; k++){
		printf("f('%c')=%d\n", k + 'a', count[k]);
	}
	//compute IC
	int sum  = 0;
	for (int k = 0; k < 26; k++){
			sum += count[k] * (count[k] - 1);
	}

	double IC = sum * 1.0 / (size_alpha * (size_alpha - 1));
	char float_string[20] = {'0'};
	sprintf(float_string,"%.9f", IC);
	change_format(float_string);
	printf("IC=%s\n", float_string);

	//compute E(IC)
	double E_IC = 0;
	int n = 26;
	double kp = 0.0658;
	for (int i = 1; i <= max_t; ++i){
		/* code */
		E_IC = (size_alpha - i) * kp / ((size_alpha - 1) * i) + (i - 1) * size_alpha * 1.0 / (n * i * (size_alpha - 1));
		char float_string1[20] = {'0'};

		sprintf(float_string1, "%.9f", E_IC);
		change_format(float_string1);
		printf("t=%d, E(IC)=%s\n", i, float_string1);
	}

	printf("\nAuto-correlation Method\n");
	printf("=======================\n");
	int t = 1;
	int count_num = 0;
	for (; t <= max_t; ++t){
		count_num  = 0;
		for (int i = 0; i + t < size; ++i){
			char c1 = file_str[i];
			char c2 = file_str[i+t];
			if(isalpha(c1) && c1 == c2) {
				count_num++;
			}
		}
		printf("t=%d, count=%d\n", t, count_num);

	}
	printf("\n");
	return;
}

