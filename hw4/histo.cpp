#include "hw4.h"

//a set of letters with its name and appearing time
struct letter{
	char alpha;
	int count;
	letter(char alpha1, int count1){
		alpha = alpha1;
		count = count1;
	}
	bool operator< (const letter other) const{
		if(count == other.count){
			return alpha < other.alpha;
		}else{
			return count > other.count;
		}
	}

};
void histo(int period, int which, FILE *fp){
	set<letter> letter_set;
	int size = 0;
	char buf1[5] = {'a'};
	int count[26] = {0};
	while(!feof(fp)){
		fread(buf1, sizeof(char), period, fp);
		if(!feof(fp)){
			if(isalpha(buf1[which - 1])){
				size++;
				count[buf1[which - 1] - 'a']++;
			}
			
		}
	}
	printf("L=%d\n", size);
	for(int i = 0; i < 26; i++){
		letter l1 = letter( i + 'a', count[i]);
		letter_set.insert(l1);
	}
	set<letter> :: iterator it = letter_set.begin();
	for(; it != letter_set.end(); it++){
		printf("%c: %d (%.2f%%)\n", it->alpha, it->count, (it->count) * 100.0 / size);
	}
	return;

}

