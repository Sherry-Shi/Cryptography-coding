
#include "encrypt.h"

int getPixel(FILE *pFile, int x, int width);
int getw_h(FILE *pfile, int *width,int *height);
void get8key(unsigned char *key_buf, unsigned char *md5_buf, int len);

void encrypt(char *pphrase, char *outName, FILE *pfile)
{
	// unsigned char buf[1];
	 int width = 0;
	 int height = 0;
	// int count = 0;
	 if(!getw_h(pfile, &width, &height)){
		return;
	 }
	 //create pbm files
	 FILE *fp1;
	 FILE *fp2;
	 char s1[20];
	 sprintf(s1,"%d %d\n", 2 * width, 2 * height);
	 char *filename1 = NULL;
	 if((filename1 = (char *)malloc(strlen(outName) + 1 + 6)) == NULL){
		fprintf(stderr , "Error: malloc failed!\n");
		//return -1;
	 }
	 strcpy(filename1, outName);
	 strcat(filename1,".1.pbm");
	 char *filename2 = NULL;
	 if((filename2 = (char *)malloc(strlen(outName) + 1 + 6)) == NULL){
		fprintf(stderr , "Error: malloc failed!\n");
		//return -1;
	 }
	 strcpy(filename2, outName);
	 strcat(filename2,".2.pbm");
	 fp1 = fopen(filename1, "w");
	 fp2 = fopen(filename2, "w");
	 fwrite("P4\n", 1, 3, fp1);
	 fwrite("P4\n", 1, 3, fp2);
	 fwrite(s1, 1, strlen(s1), fp1);
	 fwrite(s1, 1, strlen(s1), fp2);
	// fclose(fp1);
	/// fclose(fp2);
	 ////////////////////////////////////////////////////
	 //generate key stream
	 unsigned char md5_buf[MD5_DIGEST_LENGTH];
	 unsigned char key_buf[8];
	 unsigned char key_buf1[8];
	 int len1 = strlen(pphrase)+2+MD5_DIGEST_LENGTH;
     char *s = (char *)malloc(len1+1);
	 unsigned char *pbm_buf1, * pbm_buf2, *pbm_buf21, *pbm_buf22;
	 unsigned char tmp_buf1 = 0, tmp_buf2 = 0, tmp_buf21 = 0, tmp_buf22 = 0;
     int i=0, k = 0, bit = 0, x = 0;
	 int length = width * height;
	 int n = length / (8*8); // the number of 8 bytes of output, number of loops
	 int q = length % (8*8); // the remaining bytes of output
	 i = (2 * width) % 8;
	 if(i == 0) i = width/4;
	 else i = (width/4)+1;
	 pbm_buf1 = (unsigned char *)malloc(i);
	 pbm_buf2 = (unsigned char *)malloc(i);
	 pbm_buf21 = (unsigned char *)malloc(i);
	 pbm_buf22 = (unsigned char *)malloc(i);
	 int lenbuff = i;
	 int j = 0;
	 int key = 0;
     MD5((unsigned char *)pphrase, strlen(pphrase), md5_buf);
     for (i = 0, j = 0; k < n; k++) {
		//ki = 0;
        sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5((unsigned char *)s, len1, md5_buf);
       // fwrite(md5_buf, 1, sizeof(md5_buf)>>1, stdout);
		get8key(key_buf, md5_buf, 8);  //get the 8 bytes key
		
		//for each 8 bytes key_buf, get each byte and then get each bit in each byte
		for(int ki = 0; ki < 8; ki++){
			for(int kj = 7; kj >= 0; kj--){
				key = (key_buf[ki] >> kj) & 0x01;
				bit = getPixel(pfile, x, width);
			
				if(key == 1){
					tmp_buf1 = tmp_buf1 | ((0x40) >> j * 2);
					tmp_buf2 = tmp_buf2 | ((0x80) >> j * 2);
					if(bit == 1){
						tmp_buf21 = tmp_buf21 | ((0x80) >> j * 2);
						tmp_buf22 = tmp_buf22 | ((0x40) >> j * 2);
					}
					else{
						tmp_buf21 = tmp_buf21 | ((0x40) >> j * 2);
						tmp_buf22 = tmp_buf22 | ((0x80) >> j * 2);
					}
				}
					else if(key == 0){
					tmp_buf1 = tmp_buf1 | ((0x80) >> j * 2);//first bit in share1
					tmp_buf2 = tmp_buf2 | ((0x40) >> j * 2);//second bit in share1
					if(bit == 0){
						tmp_buf21 = tmp_buf21 | ((0x80) >> j * 2);
						tmp_buf22 = tmp_buf22 | ((0x40) >> j * 2);
					}
					else{
						tmp_buf21 = tmp_buf21 | ((0x40) >> j * 2);
						tmp_buf22 = tmp_buf22 | ((0x80) >> j * 2);
					}
				}
				j++;
				x++;
				if(x == width){
					pbm_buf1[lenbuff - 1] = tmp_buf1;
					pbm_buf2[lenbuff - 1] = tmp_buf2;
					pbm_buf21[lenbuff - 1] = tmp_buf21;
					pbm_buf22[lenbuff - 1] = tmp_buf22;
					fwrite(pbm_buf1, 1, lenbuff, fp1);
					fwrite(pbm_buf2, 1, lenbuff, fp1);
					fwrite(pbm_buf21, 1, lenbuff, fp2);
					fwrite(pbm_buf22, 1, lenbuff, fp2);
					tmp_buf21 = tmp_buf22 = tmp_buf1 = tmp_buf2 = 0;
					x = 0;
					j = 0;
				}
				if(x % 4 == 0 && x != width){
					pbm_buf1[x / 4 - 1] = tmp_buf1;
					pbm_buf2[x / 4 - 1] = tmp_buf2;
					pbm_buf21[x / 4 - 1] = tmp_buf21;
					pbm_buf22[x / 4 - 1] = tmp_buf22;
					tmp_buf21 = tmp_buf22 = tmp_buf1 = tmp_buf2 = 0;
					j = 0; 
				}
				
			}
		}
		
		if (++i == 100) i = 0;
     }
	 if(q != 0){
		k = q / 8;
		int last = q % 8;
		if(last != 0){
			k++;
		}
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5((unsigned char *)s, len1, md5_buf);
		get8key(key_buf1,md5_buf,8);
		int offsetl = 0;
		//kj = 7;
		for(int ki = 0; ki < k; ki++){
			if(ki == (k - 1) &&(last != 0)){
				offsetl = 8 - last;
			}
			else{
				offsetl = 0;
			}
			
			for(int kj = 7; kj >= offsetl; kj--){
				key = (key_buf1[ki] >> kj) & 0x01;
				bit = getPixel(pfile, x, width);
				if(key == 1){
					tmp_buf1 = tmp_buf1 | ((0x40) >> j * 2);
					tmp_buf2 = tmp_buf2 | ((0x80) >> j * 2);
					if(bit == 1){
						tmp_buf21 = tmp_buf21 | ((0x80) >> j * 2);
						tmp_buf22 = tmp_buf22 | ((0x40) >> j * 2);
					}
					else{
						tmp_buf21 = tmp_buf21 | ((0x40) >> j * 2);
						tmp_buf22 = tmp_buf22 | ((0x80) >> j * 2);
					}
				}
					else if(key == 0){
					tmp_buf1 = tmp_buf1 | ((0x80) >> j * 2);//first bit in share1
					tmp_buf2 = tmp_buf2 | ((0x40) >> j * 2);//second bit in share1
					if(bit == 0){
						tmp_buf21 = tmp_buf21 | ((0x80) >> j * 2);
						tmp_buf22 = tmp_buf22 | ((0x40) >> j * 2);
					}
					else{
						tmp_buf21 = tmp_buf21 | ((0x40) >> j * 2);
						tmp_buf22 = tmp_buf22 | ((0x80) >> j * 2);
					}
				}
				j++;
				x++;
				if(x % 4 == 0 && x != width){
					pbm_buf1[x / 4 - 1] = tmp_buf1;
					pbm_buf2[x / 4 - 1] = tmp_buf2;
					pbm_buf21[x / 4 - 1] = tmp_buf21;
					pbm_buf22[x / 4 - 1] = tmp_buf22;
					tmp_buf21 = tmp_buf22 = tmp_buf1 = tmp_buf2 = 0;
					j = 0; 
				}
				if(x == width){
					pbm_buf1[lenbuff - 1] = tmp_buf1;
					pbm_buf2[lenbuff - 1] = tmp_buf2;
					pbm_buf21[lenbuff - 1] = tmp_buf21;
					pbm_buf22[lenbuff - 1] = tmp_buf22;
					fwrite(pbm_buf1, 1, lenbuff, fp1);
					fwrite(pbm_buf2, 1, lenbuff, fp1);
					fwrite(pbm_buf21, 1, lenbuff, fp2);
					fwrite(pbm_buf22, 1, lenbuff, fp2);
					tmp_buf21 = tmp_buf22 = tmp_buf1 = tmp_buf2 = 0;
					x = 0;
					j = 0;
				}
			}
		}
		
		if (++i == 100) i = 0;
       // fwrite(md5_buf, 1, q, stdout);
	 }
     free(s);	


}
void get8key(unsigned char *key_buf, unsigned char *md5_buf, int len){
	for(int i = 0; i < len; i++){
		key_buf[i] = md5_buf[i];
	}
}
int getPixel(FILE *pFile, int x, int width){
	static unsigned char buf[1];
	//get every 8 bits a time
	if(x % 8 == 0 && x != width){
		fread(buf, 1, 1, pFile);
	}
	else{
		if(x == width){
			fread(buf, 1, 1,pFile);
		}
	}
	return (buf[0] >> (7 - (x % 8))) & 0x01; // get bit one by one from 1 bytes
} 

int getw_h(FILE *pfile, int * width,int * height){
	char buf[25];
	fgets(buf, 25,pfile); //get one line from file
	if(strcmp(buf,"P4\n") != 0)
	{
		fprintf(stderr, "Error: the pbm file is incorrect, the header should be \"P4\", not %s\n", buf);
		return 0;
	}
	fgets(buf, 25, pfile);
	sscanf(buf, "%d%d", width, height);
	return 1;
}

