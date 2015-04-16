
#include "encode_base64.h"
#include <string>

static const std::string base64_code = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  //
void Encode_base64(FILE *fp)
{
		unsigned char buf[1];
		unsigned char buf_3[3];  //store three bytes from the file
		unsigned char buf_4[4];	 //store four base64 bytes 
		int i=0;
		int bytes=0;
		int count=0;
		while(!feof(fp))
		{
			
			if((bytes=fread(buf, 1, 1, fp))>0) // read a byte from file
			{
				buf_3[i++]=buf[0];	//store bytes from file to buf_3	
			}
			if(i==3) //if three bytes have been stored, get the four base64 bytes by manipulating bits. 
			{
				buf_4[0] = (buf_3[0]&0xfc)>>2;
				buf_4[1] = ((buf_3[0]&0x03)<<4) + ((buf_3[1]&0xf0)>>4);
				buf_4[2] = ((buf_3[1]&0x0f)<<2) + ((buf_3[2]&0xc0)>>6);
				buf_4[3] = buf_3[2]&0x3f;
				for( i=0;i<4;i++) printf("%c",base64_code[buf_4[i]]); // print all these four base64 bytes
				count++;
				if(count==16) 
				{
					printf("\n");
					count=0;
				}
				i=0;
			
			}
		}
 
		if(i)
		{
			for(int j=i;j<3;j++)  //at the end of the file, if it's not enough to get three bytes, put '\0' to the rest bytes
			{
				buf_3[j] = '\0';
			}
			 //get the four base64 bytes by manipulating bits.
			buf_4[0] = (buf_3[0]&0xfc)>>2;
			buf_4[1] = ((buf_3[0]&0x03)<<4) + ((buf_3[1]&0xf0)>>4);
			buf_4[2] = ((buf_3[1]&0x0f)<<2) + ((buf_3[2]&0xc0)>>6);
			buf_4[3] = buf_3[2]&0x3f;
			for(int j=0;j< i+1;j++) printf("%c",base64_code[buf_4[j]]);
			while((i++<3))
			{
				printf("%c", '='); //for the padding bytes, print '='
			}
		
		}
		printf("\n");			

}

