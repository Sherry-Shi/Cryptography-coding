//#include "stdafx.h"
#include "decode_base64.h"
#include <string>

static const std::string base64_code = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


//check if the character is in base64 character set
bool is_base64(char c)
{
	return ( (c == '+') || (c == '/') || isalnum(c));
}
void Decode_base64(FILE *fp)
{
		unsigned char buf[1];
		unsigned char buf_3[3];
		unsigned char buf_4[4];	
		int i=0;
		int j=0;
		int bytes=0;
		
		while(!feof(fp))
		{
			if((bytes=fread(buf, 1, 1, fp))>0)   // read a byte to buf
			{
				if(buf[0]!='=' && is_base64(buf[0])) // if this character is not '=' and is in base64 character set, then store it to buf_4
				{
					buf_4[i++]=buf[0];
				}
				if(!is_base64(buf[0]) && buf[0]!='\n' && buf[0]!='=')
				{
					fputs("Input contains illegal characters\n", stderr);
					exit (1);
				}
				if(i==4)// already get 4 characters in buf_4
				{
					for(i=0;i<4;i++)
					{
						buf_4[i] = base64_code.find(buf_4[i]);  //get the ASCII character according to base64 table
					}
					//get the 3 bytes ASCII from 4 base64 ASCII
					buf_3[0]= (buf_4[0]<<2) + ((buf_4[1] & 0x30)>>4);
					buf_3[1]= ((buf_4[1] & 0xf)<<4) + ((buf_4[2] & 0x3c)>>2);
					buf_3[2]= ((buf_4[2] & 0x3)<<6) + buf_4[3];
					for( i=0;i<3;i++) printf("%c",buf_3[i]);
					i=0;
				}
			}
		}
			if(i)
			{
				for(j=i;j<4;j++)
				{
					buf_4[j] = 0;   //at the end of the file, it's not enough to get 4 base64 characters, put 0 to make it 4 characters
				}
				for(j=0;j<4;j++)
				{
					buf_4[j] = base64_code.find(buf_4[j]);  
				}
				buf_3[0]= (buf_4[0]<<2) + ((buf_4[1] & 0x30)>>4);
				buf_3[1]= ((buf_4[1] & 0xf)<<4) + ((buf_4[2] & 0x3c)>>2);
				buf_3[2]= ((buf_4[2] & 0x3)<<6) + buf_4[3];
				for( j=0;j<i-1;j++) printf("%c",buf_3[j]);
			}
		


}
