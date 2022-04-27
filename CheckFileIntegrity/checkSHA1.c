#include "utils.h"

//#define BUFFER_SIZE 128
//tatic unsigned char buffer[BUFFER_SIZE];

//Compile with: gcc checkSYS.c -lm -lssl -lcrypto -o checkSYS.lxf
//openssl dgst * | grep -ohs " \b[0-9a-z].*" Per prendere gli hash e metterli in un file
//Prima va lanciato genSHA1.c compilandolo opportunamente per le cose di cui si vuole generare gli hash quindi ad esempio 
//Per un server rimuovere il commento per il percorso /opt/lammp il file di cui parlo è genSHA1.c
//Una volta che il file listsum viene riempito dei valori hash prendere l'hash di listsum e usarlo per lanciare checkSYS.lxf

int main(int argc, char *argv[]){
    const char* filename = "listsum";
    unsigned bytes = 0;
    char* sha=generaSHA1("listsum");
    char* crc;
	int deep=4;
    FILE* pf;
    FILE* cf;
    if(argc<3){
	printf("\nAs argument insert the SHA1 obtained from genSHA1.lxf and the path to verify protection\n");
	printf("\nYou can pass the deep of analysis after all the stuff: ./checkSHA1.lxf /path/to/verify/ integritySHAString deep[int default 4]\n");
	return 0;	
    }
	if(argc>3){
		stringToInt(argv[3],&deep);
	}
    if(strcmp(sha,argv[2])==0){
	printf("\nReady to check the system!\n\nChecking...\n");
	pf=fopen("checklist","wb");
	cf=fopen("listsum","rb");
	browseAndCheckSHA1(argv[1],deep,pf,cf);
	fclose(pf);
        fclose(cf);
	crc=generaSHA1("checklist");
	if(strcmp(sha,crc)!=0){
		printf("\nCorruption detected, restoring all files!\n");
		//Better if can restore only interested files
	}else{
		printf("\nEverything is OK!\n");	
	}
    }else{
	printf("\nWarning: listsum has been modified!\n");
    }
}
