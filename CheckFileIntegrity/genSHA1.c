#include "utils.h"

#define BUFFER_SIZE 128
static unsigned char buffer[BUFFER_SIZE];

//Compile with: gcc genMD5.c -lm -lssl -lcrypto -o genMD5.lxf
//openssl dgst * | grep -ohs " \b[0-9a-z].*" Per prendere gli hash e metterli in un file

unsigned char result[MD5_DIGEST_LENGTH];

// Print the MD5 sum as hex-digits.
void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i<MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

int main(int argc, char *argv[]){
	//const char* filename = "listsum";
	char filename[50];//Se questo non ha una lunghezza fissa la variabile risulta vuota certe volte quando viene passato.	
	unsigned bytes = 0;
	char* crc;
	int deep=4;
	if(argc>1){
		printf("\nPath: %s\n",argv[1]);
		if(argc>2){
			stringToInt(argv[2],&deep);
		}
		FILE* pf;
		pf=fopen("listsum","wb");
		browseAndGenSHA1(argv[1],deep,pf);
		fclose(pf);
		crc=generaSHA1("listsum");
		printf("\nSHA1 listsum: %s\n",crc);
		printf("\nTake it with care! Preserve it\n");
		//crc=generaSHA256("listsum");
		//printf("\nSHA256 listsum: %s\n",crc);
	}else{
		printf("\nSpecify a directory!");
		printf("\nExample: ./genSHA1.lxf /path/to/directory/to/protect/ DEEP[5] (defualt deep is 4)\n");
	}
	return 0;
}
