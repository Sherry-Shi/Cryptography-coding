#include "hexdump.h"

void hexdump(FILE *fp)
    {
        unsigned char buf[1];
		int i=0;
		int j=0;
		int change=0;
		bool f1 = true;
		int bytes=0;
		int value=0;
		int offset=0;
		bool flag=false;
		while(!feof(fp))  //while the file doesn't reach the end
		{
			unsigned char buffer[17];
			//buffer[0]='\0';
			
			for(i=0;i < 16; i++)  // get 16 bytes then print them out in the format of hex
			{
				
				if((bytes=fread(buf, 1, 1, fp))>0)   
				{
					if(change==1 || f1==true)
					{
						printf("%06x",offset); //  print out the offset
						printf(":");
						printf(" ");
						change=0;
						f1=false;
					}
					printf("%02x", buf[0]);
					buffer[i]=buf[0];
					if(i==7) printf("  ");
					else printf(" ");
				}
				if(bytes==0)  // if reaches the end of the file, print "--" until get 16 bytes
				{
					printf("--");
					flag=true;
					buffer[i]='\0';
					if(i==7) printf("  ");
					else printf(" ");
				}
			}
			printf(" ");
			for(j=0;j<16;j++) //print out the ASCII according to the hex number.
			{
				if(flag==true&&buffer[j]=='\0')   printf(" ");
				else{
					value = (int)buffer[j]; //get the value of hex
					if(value>=0x20 && value<=0x7e) printf("%c",buffer[j]); //if the value is larger than 0x20 and smaller than 0x7e, then print the character
					if(value<=0x1f || value == 0x7f) printf(".");// if the value is smaller than 0x1f and equal to 0x7f, then print "."
					if(value>=0x80) printf("~"); // if the value is larger than 0x80, then print '~'
				}
			}
			printf("\n");
			offset+=16;// compute the offset
			change=1;
		}	    
    }



