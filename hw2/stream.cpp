#include "stream.h"

void stream(char *pphrase, int length)
{
	unsigned char md5_buf[MD5_DIGEST_LENGTH];
    int len1 = strlen(pphrase)+2+MD5_DIGEST_LENGTH;
    char *s = (char *)malloc(len1+1);
    int i=0;
	int k = 0;
	int n = length / 8; // the number of 8 bytes of output
	int q = length % 8; // the remaining bytes of output
    MD5((unsigned char *)pphrase, strlen(pphrase), md5_buf);
    for (; k < n; k++) {
        sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5((unsigned char *)s, len1, md5_buf);
        fwrite(md5_buf, 1, sizeof(md5_buf)>>1, stdout);
        if (++i == 100) i = 0;
    }
	if(q != 0){
		sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, pphrase);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5((unsigned char *)s, len1, md5_buf);
        fwrite(md5_buf, 1, q, stdout);
	}
    free(s);
}
