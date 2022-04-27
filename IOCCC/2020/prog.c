#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define PATH_LEN 80
#define _ASM printf
//.prog filename
char* getchars(char* str);
int print(char* str);//Improved print function
int play(char *path);

int main(int argc, char *argv[]){
	char s[4];
	char c = ' ';
	int n,i;
	uint8_t b, byte = '1';
	unsigned short endFile = 0;
	char pathAudio[PATH_LEN];
	char pathFile[PATH_LEN];
	char chars[127];
	if(argc>1){
		return play(argv[1]);  
	}
	FILE *f,*fi,*dt;
	for(i=0;i<127;i++){
		chars[i]= 0x55b120 + i;
	}
	fi = fopen("prog.c", "rb");
	putchar('\n');
	fseek(fi,280*sizeof(char),0);
	while(c!=0x3A){
		fread(&c,sizeof(char),1,fi);
	}
	fseek(fi,sizeof(char),SEEK_CUR);
	i=1;
	do{
		fread(s,sizeof(char),i,fi);
		if(s[0]==0x27){i=2;}
		if(s[0]==0x2C){i=1;fseek(fi,-1*sizeof(char),SEEK_CUR);}
		if(s[0]!=0x27 && s[0]!=0x2C){
			n = atoi(s);
			putchar(chars[n]);
			fread(&c,sizeof(char),1,fi);
			fseek(fi,-1*sizeof(char),SEEK_CUR);
		}
	}while(feof(fi)==0 && c!='\n');
	fclose(fi);
	print("This program read and play a WAV file");
	putchar('\n');
	getchars("Audio file name%c");
	scanf("%s",pathAudio);
	getchars("\nFile name%c ");
	scanf("%s",pathFile);
	if(!(f = fopen(pathAudio, "rb+"))){
		print("Error opening the file!\n");
		return -1;    
	}
	if (fseek(f, 84 * sizeof(uint8_t), 0) != 0){ //76 if you wish destroy the chunk
		print("fseek error!");
		return -1;
	}else{
		if(!(dt = fopen(pathFile,"rb"))){
			print("Error opening the file!\n");
			return -1;
		}
		fseek(dt,0,0);
		do{
			fread(&byte,sizeof(uint8_t),1,dt);
			for(i=0;i<8;i++){
				fread(&b, sizeof(uint8_t),1,f);
				if((byte&0x7F) != byte){
					if((b&0xFE) == b){
						b+=0x01;
					}
				}else{
					if((b&0xFE) != b){
						b-=0x01;
					}
				}
				byte = (byte<<1);
				fseek(f,-1*sizeof(uint8_t),SEEK_CUR);
				fwrite(&b, sizeof(uint8_t), 1, f);
			}
			endFile++;
		}while(feof(dt)==0);
		//fseek(f,-sizeof(int),2); Not used anymore
		//fwrite(&(endFile-endFile),sizeof(unsigned short),2,f); //Close the file
		fseek(f, 77 * sizeof(uint8_t), 0);
		fwrite(&endFile,sizeof(unsigned short),1,f);
		fclose(dt);
		fclose(f);
		print("\nDONE!\n");
		return 0;
	}
}

char* getchars(char* str){
	char var = 0x3A;
	_ASM(str,var);
	return "0x3A";
}

int print(char str[]){
	if(str[0]!='T'){
		fputs(str,stdout);
	}
	return 0;
}

int play(char *path){
	FILE *f, *c;
	uint8_t byte,i,wr=0xFF;
	unsigned short we;
	if(!(f = fopen(path, "rb"))){
		printf("Error opening %s\n",path);
		return -1;
	}
	fseek(f, 77 * sizeof(uint8_t), 0);
	fread(&we,sizeof(unsigned short),1,f);
	c = fopen("resoult","wb");
	fseek(f, 84 * sizeof(uint8_t), 0);
	wr=(wr<<8);
	we--;
	do{
		for(i=0;i<8;i++){
			fread(&byte,sizeof(uint8_t),1,f);
			if((byte&0xFE) != byte){
				wr=(wr<<1);
				wr+=0x01;
			}else{
				wr=(wr<<1);
			}
		}
		fwrite(&wr, sizeof(uint8_t), 1, c);
		wr=0xFF;
		wr=(wr<<8);
		we--;
	}while(feof(f)==0 && we>0);
	fclose(f);
	fclose(c);
	return 0;
}

/**
The real badly written programs are those that seem to be well written:
'52727383,0'80827971826577,0'737883698284,0'7378,0'65,0'553354,0'7073766912,0'65787984726982,0'7073766914
*/
