#include "hw5.h"
#include <vector>
#include <string>
using namespace std;

int IP[64];
int E[48];
int P[32];
int S[8][64];
int V[16];
int PC1[56];
int PC2[48];

vector<int> split(char * input){
	vector<int> rst;
	string s(input);
	if(s.size() == 0){
		return rst;
	}
	size_t pos1,pos2;
	size_t npos = -1;
	pos1 = 0;
	pos2 = 0;
	for(;;){
		string tmp = "";
		int data = 0;
		pos2 = s.find(",", pos1);
		if(pos2 != npos){
			tmp += s.substr(pos1, pos2-pos1);
			sscanf(tmp.c_str(), "%d", &data);
			rst.push_back(data);
			pos1 = pos2 + 1;
		}else if(pos1 <= s.size()){
			tmp += s.substr(pos1);
			sscanf(tmp.c_str(), "%d", &data);
			rst.push_back(data);
			break;
		}else{
			break;
		}
		
	}
	return rst;
}

bool checkIP(int Table[], char line[], int len, string name){
	string line_str(line); 
	string table_name = "";
	table_name += name;
	table_name += "=";
	string line_name = line_str.substr(0, table_name.size());
	if(strcmp(table_name.c_str(),line_name.c_str()) != 0){
		printf("Error: Malformed table file!\n");
		return false;
	}

	//char *datas = NULL;
	int data = 0;
	map<int, int> buf;
	line = line + line_name.size();
	vector<int> datas = split(line);
	if(datas.size() > (unsigned)len){
		printf("Error: %d values read in %s, while 64 was expected!\n", (int)datas.size(), name.c_str());
		return false;
	}
	if(datas.size() < (unsigned)len){ 
		printf("Error:  %d values read in %s, while 64 was expected!\n", (int)datas.size(), name.c_str());
		return false;
	}
	for(int i = 0; i < (int)datas.size(); i++){
		data = datas[i];
		if(data <= len && data > 0){
			if(buf.count(data-1) == 0){
				buf[data-1] = i;
			}else{
				printf("Error:  Number %d should not occur more than once! in %s Table\n", data, name.c_str());
				return false;
			}	
		}else{
			printf("Error: Number %d is not allowed in %s Table\n", data, name.c_str());
			return false;
		}
	}
	for(int i = 0; i < len; i++){
		Table[buf[i]] = i+1;
	}
	return true;
}

bool checkE(int Table[], char line[]){
	string line_str(line); 
	string table_name = "";
	table_name += "E=";
	string line_name = line_str.substr(0, table_name.size());
	if(strcmp(table_name.c_str(),line_name.c_str()) != 0){
		printf("Error: Malformed table file!\n");
		return false;
	}
	int data = 0;
	map<int, int> buf;
	line = line+ line_name.size();
	vector<int> datas = split(line);
	if(datas.size() > 48){
		printf("Error: Too many values read in IP!\n");
		return false;
	}
	if(datas.size() < 48){ 
		printf("Error: Too few values read in IP!\n");
		return false;
	}
	for(int i = 0; i < (int)datas.size(); i++){
		data = datas[i];
		if(data <= 32 && data > 0){
			buf[data] = buf[data] + 1;
			E[i] = data;
		}else{
			printf("Error: Number %d is not allowed in E Table\n", data);
			return false;
		}
	}
	for(int i = 0; i < 32; i++){
		if(buf[i+1] > 2){
			printf("Error: Number %d in E occur more than twice\n", i + 1);
			return false;
		}else if(buf.count(i+1) == 0){
			printf("Error: Number %d in E doesn't occur\n", i + 1);
			return false;
		} 
	}
	return true;
}

bool checkSi(int Si[], char line[], int which){
	string line_str(line); 
	string table_name = "";
	table_name += "S";
	table_name += which + '1';
	table_name += "=";
	string line_name = line_str.substr(0, table_name.size());
	if(strcmp(table_name.c_str(),line_name.c_str()) != 0){
		printf("Error: Malformed table file!\n");
		return false;
	}
	int data = 0;
	map<int, int> buf;
	line = line+ line_name.size();
	vector<int> datas = split(line);
	if(datas.size() > 64){
		printf("Error: %d values read in %s, while 64 was expected!\n", (int)datas.size(), table_name.c_str());
		return false;
	}
	if(datas.size() < 64){ 
		printf("Error: %d values read in %s, while 64 was expected!\n", (int)datas.size(), table_name.c_str());
		return false;
	}
	for(int i = 0; i < (int)datas.size(); i++){
		data = datas[i];
		if(data < 0 || data > 15){
			printf("Error: Value %d is not allowed in %s Table\n", data, table_name.c_str());
			return false;
		}else{
			buf[data] = buf[data] + 1;
			Si[i] = data;
		}
	}
	for(int i = 0; i < 16; i++){
		if( buf[i] != 4){
			printf("Error: Number %d in %s should occur 4 times\n", i + 1, table_name.c_str());
			return false;
		}
	}
	return true;
}


bool checkV(int V[], char line[]){
	string line_str(line); 
	string table_name = "";
	table_name += "V=";
	string line_name = line_str.substr(0, table_name.size());
	if(strcmp(table_name.c_str(),line_name.c_str()) != 0){
		printf("Error: Malformed table file!\n");
		return false;
	}

	int data = 0;
	map<int, int> buf;
	line = line+ line_name.size();
	vector<int> datas = split(line);
	if(datas.size() > 16){
		printf("Error: Too many values read in IP!\n");
		return false;
	}
	if(datas.size() < 16){ 
		printf("Error: Too few values read in IP!\n");
		return false;
	}
	for(int i = 0; i < (int)datas.size(); i++){
		data = datas[i];
		if(data < 1 || data > 2){
			printf("Error: Value %d is not allowed in V Table\n", data);
			return false;
		}else{
			buf[data] = buf[data] + 1;
			V[i] = data;
		}
	}
	if(buf[1] != 4){
		printf("Error: The value 1 in V doesn't occur 4 times\n");
		return false;
	}
	if(buf[2] != 12){
		printf("Error: The value 2 in V doesn't occur 12 times\n");
		return false;
	}
	
	return true;
}

bool checkPC(int Table[], char line[], int len, string name, int max){
	string line_str(line); 
	string table_name = "";
	table_name += name;
	table_name += "=";
	string line_name = line_str.substr(0, table_name.size());
	if(strcmp(table_name.c_str(),line_name.c_str()) != 0){
		printf("Error: Malformed table file!\n");
		return false;
	}
	int data = 0;
	map<int, int> buf;
	line = line+ line_name.size();
	vector<int> datas = split(line);
	if(datas.size() > (unsigned)len){
		printf("Error: %d values read in %s while 32 was expected!\n", (int)datas.size(), table_name.c_str());
		return false;
	}
	if(datas.size() < (unsigned)len){ 
		printf("Error:  %d values read in %s while 32 was expected!\n",(int)datas.size(), table_name.c_str());
		return false;
	}
	for(int i = 0; i < (int)datas.size(); i++){
		data = datas[i];
		if(data > max) {
			printf("Error: Value %d is not allowed in %s Table\n", data, name.c_str());
			return false;
		}else if( data <= 0){
			printf("Error: Value %d is not allowed in %s Table\n", data, name.c_str());
			return false;
		}else{
			if(buf.count(data-1) == 0){
				buf[data-1] = i;
			}else{
				printf("Error:  Value %d should not occur more than once! in %s Table\n", data, name.c_str());
				return false;
			}
		}
	}
	for(int i = 0; i < max; i++){
		if(buf.count(i) != 0){
			Table[buf[i]] = i+1;	
		}
	}
	for(int i = 0; i < max; i++){
		if(buf.count(i) == 0){
			if(strcmp(name.c_str(), "PC1") ==0){
				if((i + 1) % 8){
					printf("Error: Value %d doesn't occur in PC1 \n", i+1);
					return false;
				}
			}
		}
	}
	return true;
}
bool tablecheck(FILE *fp){
	char line[256] = {'a'};

	fgets(line, 256, fp);
	bool rst = checkIP(IP, line, 64, "IP");
	if(rst == false) return false;
	fgets(line, 256, fp);
	rst = checkE(E, line);
	if(rst == false) return false;
	fgets(line, 256, fp);
	rst = checkIP(P, line, 32, "P");
	if(rst  == false) return false;
	for(int i = 0; i < 8; i++){
		fgets(line, 256, fp);
		rst = checkSi(S[i], line, i);
		if(rst == false) return false;
	}
	fgets(line, 256, fp);
	rst = checkV(V, line);
	if(rst == false) return false;
	fgets(line, 256, fp);
	rst = checkPC(PC1, line, 56, "PC1", 64);
	if(rst == false) return false;
	fgets(line, 256, fp);
	rst = checkPC(PC2, line, 48, "PC2", 56);
	if(rst == false) return false;

	return true;
}


