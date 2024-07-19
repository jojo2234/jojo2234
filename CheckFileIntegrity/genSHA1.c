#include "utils.h"
#include <libgen.h>
#define BUFFER_SIZE 128
static unsigned char buffer[BUFFER_SIZE];

//Compile with: gcc genSHA1.c -lm -lssl -lcrypto -o genSHA1.lxf
//openssl dgst * | grep -ohs " \b[0-9a-z].*" Per prendere gli hash e metterli in un file

unsigned char result[MD5_DIGEST_LENGTH];

// Print the MD5 sum as hex-digits.
void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i<MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

int is_a_file(const char *pth){
	struct stat path_stat;
	stat(pth,&path_stat);
	return S_ISREG(path_stat.st_mode);
}

void recursiveGenSHA1(char *name, int level, FILE* pf){
	DIR *dir;
	struct dirent *entry;
	char* linea;
	char path[1024];
	if(is_a_file(name)){
		linea="";
		linea=generaSHA1(name);
		fputs(name,pf);
		fputs(",",pf);
		fputs(linea,pf);
		fputs("\n",pf);
	}	
	if(!(dir = opendir(name))){
		return;
	}
	if(!(entry = readdir(dir))){
		return;
	}
	do{
		path[0]='\0';
		int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
		path[len] = 0;
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){continue;}
		linea="";
		linea=generaSHA1(path);
		fputs(path,pf);
		fputs(",",pf);
		fputs(linea,pf);
		fputs("\n",pf);
		recursiveGenSHA1(path, level + 1,pf);
	}while((entry = readdir(dir)));
	closedir(dir);
}

int main(int argc, char *argv[]){
	//const char* filename = "listsum";
	//char filename[80];//Se questo non ha una lunghezza fissa la variabile risulta vuota certe volte quando viene passato.	
	//unsigned bytes = 0;
	int i=0;
	char c;
	int sc;
	FILE* pf;	
	FILE* csv;
	char* crc;
	char* dir=(char*)malloc(380*sizeof(char));
	char* chl=(char*)malloc(380*sizeof(char));
	char* listsum=(char*)malloc(380*sizeof(char));	
	memset(dir,0,sizeof(char));
	memset(chl,0,sizeof(char));
	memset(listsum,0,sizeof(char));
	if(argc<=2){
		printf("\r\nUSAGE:\n");
		printf("\n First argument is the path where do you want to save the file called listsum");
		printf("\n You do not need to create any file, you just be sure to have writing permission, for example /home/user/listsum \r\n");
		printf("\n The path where listsum will be written cannot be one of that you want to check! \r\n");
		printf("\n You do not need to create a file you just need a path with writing permission on it \r\n");
		printf("\n Second argument must be the path to a file containing a list of paths of directory to scan, each line with a carriage return or new line at the end is a path \r\n");
		printf("\n It can be /home/user/directoryToCheck.csv this time the name can be what you want \r\n");
		printf("\n This program create a unique SHA1 from the list of SHA1 of the given paths to verify \r\n");
		printf("\n This unique SHA1 help the program checkSYS to rapid detect corruption, for this reason you have to keep it with you \n");
		printf("\n It's a good solution to disable automatic updates and not give pahts that are used to change content in it, like your home folder \n");
		printf("\n Use checkSYS.lxf to verify the integrity of the paths \r\n");
		printf("\r\n Eg. ./genSHA1.lxf /path/to/listsum /path/to/dirtoscan.csv \r\n");
		printf("\r\n");
	}else{
	pf=fopen(argv[1],"wb");
	if(pf==NULL){
		printf("Check to have the permission to the path where listsum must be saved.");	
	}else{
		for(int j=0;argv[1][j]!='\0';j++){
			listsum[j]=argv[1][j];
		}	
	}
	csv=fopen(argv[2],"r");
	if(csv==NULL){
		printf("Impossible read the file within the list of directories to check.");	
	}else{
		for(int j=0;argv[2][j]!='\0';j++){
			chl[j]=argv[2][j];
		}
	}
	c=fgetc(csv);
	while(!feof(csv) && c!=0xff){
		while(c!='\n' && c!='\r' && c!=EOF){
			dir[i]=(char)c;
			i+=1;
			dir[i]='\0';
			c=fgetc(csv);	
		}
		i=0;
		c=fgetc(csv);
		recursiveGenSHA1(dir,3,pf);	
	}
	fclose(csv);
    	fclose(pf);
	crc=generaSHA1(argv[1]);
	printf("\r\n");
	char* pthToF=(char*)malloc(380*sizeof(char));
	memset(pthToF,0,sizeof(char));
	pthToF=dirname(argv[1]);
	int len=strlen(pthToF);
	pthToF[len]='/';
	pthToF[len+1]='s';	
	pthToF[len+2]='h';
	pthToF[len+3]='a';
	pthToF[len+4]='L';
	pthToF[len+5]='i';
	pthToF[len+6]='s';
	pthToF[len+7]='t';
	pthToF[len+8]='S';
	pthToF[len+9]='u';
	pthToF[len+10]='m';	
	pthToF[len+11]='\0';
	pf=fopen(pthToF,"wb");
	if(pf==NULL){
		printf("\nWarning impossible save shaListSum check your permission on running directory.\n");	
	}
	printf("\nSHA1 of listsum: %s\n",crc);
	printf("\nTake it with care! Preserve it\n");
	fputs(" ./checkSYS.lxf ",pf);
	fputs(crc,pf);
	fputs(" ",pf);
	fputs(listsum,pf);
	fputs(" ",pf);
	pthToF[len]='/';
	pthToF[len+1]='c';
	pthToF[len+2]='h';
	pthToF[len+3]='e';
	pthToF[len+4]='c';
	pthToF[len+5]='k';
	pthToF[len+6]='l';
	pthToF[len+7]='i';
	pthToF[len+8]='s';
	pthToF[len+9]='t';
	pthToF[len+10]='\0';	
	fputs(pthToF,pf);
	fputs(" ",pf);
	fputs(chl,pf);
	fputs(" \r\n",pf);
	printf("\r\nThe file shaListSum has been saved where you saved listsum \n");
	printf("\r\n");
	fclose(pf);
}
}
