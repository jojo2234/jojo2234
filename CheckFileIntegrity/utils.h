#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
//#include <X11/Xlib.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/md5.h>
#ifdef _WIN32
    #include <windows.h>
#else
#include <sys/stat.h>
#include <sys/mman.h>
#endif

/* 
.lxf = Linux eXecutable File
.lsi = Linux Software Installer
.sh = Shell file
gcc nomeFile.c -S -o nomeFile.asm To obtain an assembly
gcc nomeFile.c -O0 -o nomeFile.lxf To obtain a Linux-eXecutable-File (executable) without any optimization
gcc nomeFile.c -o nomeFile.lxf You can obtain the executable with optimization offered by compiler if supported
gcc nomeFile.c -lm -o nomeFile.lxf The lm parameter is used for math.h library
Use A to move the cursor on N line up
Use B to move it down, C to forward it and D to roll it back.
\033[<N>D
\033[<N>A
where N is the number that I have choosen.
Escape sequence \033 and \x1B are the same 033 is octal, 27 is decimal.
On linux use xev from terminal to obtain characters informations.
GHex hex editor.
To compile with x11 add -L/usr/x11/lib -lx11 -lstdc++
note %p in printf allows to print memory address
//Menu use case: int scelta = printCascadeMenu("Start\0","Nuovo File,Leggi File,Modifica File,Cancella File,Esci,",5,36,44,00);
//When you use openssl library compile with: gcc main.c -lm -lssl -lcrypto -o main.lxf
*/
#define CURP "\033[%d;%df" //To set the cursor in a position f or H are used, instead m is for colors
#define COLR "\x1B[%dm"
#define SYSB "\x1B[%dm"
#define SYSF "\x1B[0;%dm"
#define BLK "\x1B[0m"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define MAX_CHAR 20
#define POSS_LENG 300
#define PILA_LENG 10
#define NEG_NUM -9999999
#define POS_NUM 9999999
#define SINGLE_WORD_LENGHT 100
#define NUM_TO_FLOAT 6
#define MATHOP_LENGHT 11

int toRet[5];
char ps1[55];
char nomeFunzione[MAX_CHAR];

typedef struct{
    enum{
        typUndefined,
        typInt,
        typUint,
        typString,
        typLong
        /*,typFloat, typDouble*/
    }types;

    void* elem;

    /*union{ int i; unsigned int u; char* s; long l;}elem;*/
}object;

typedef struct{
    char *possessore;
    char *posseduto;
}possedimenti;

typedef struct node{
    char * dato;
    struct node *next;
}nodo;

typedef struct{
    nodo first;
    nodo n;
    int size;
}pila;

typedef struct{
    nodo first;
    nodo temp;
    nodo prec;
    int size;
}lista;

typedef struct{
    char value;
    char *operazione; //Italian Language
    char *operation; //English language or abbreviation
    void (*express)();
}mathOp;

typedef struct{
    int intNum;
    float floatNum;
    bool is_float;
}number;

typedef struct act{
    char *daTesto;
    mathOp azione;
}action;

typedef struct con{
    char *daTesto;
    mathOp tipologia; //sequenziale, interno, sottraendo, aggiungendo, congiunzione diretta, articolo no utile - I don't know
}connettore;

typedef struct{
    char *word;
    bool isAction;
    bool isString;
    number numero;
    mathOp azione;
}wordsScopo;

nodo insertNodo(nodo nd,char *ob){
    nd.dato = ob;
    nd.next = NULL;
    return nd;
}
nodo nextNodo(nodo nd, char *ob, nodo n){
    nd.dato = ob;
    nd.next = &n;
    return nd;
}

pila createPila(char *obj, pila stack){
    stack.first.next = NULL;
    stack.n = insertNodo(stack.n,obj);
    stack.size += 1;
    return stack;
}

pila push(char *obj, pila stack){
    stack.first = stack.n;
    stack.n = insertNodo(stack.n,obj);
    //stack.n.next = &stack.first;
    stack.size += 1;
    return stack;
}

possedimenti dataOwn[POSS_LENG]; //Max array lenght is 300 possessions

int * generateIntNumbers(int *ofNum){
    int vetSize = (POS_NUM*2)+1;
    int i, value = NEG_NUM;
    for(i=0;i<vetSize;i++){
        ofNum[i] = ++value;
    }
    return ofNum;
}

char * alphabetMaiusc(char *alph){
    alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //{'A','B','C'};
    return alph;
}

char * alphabetMinusc(char *alph){ 
    alph = "abcdefghijklmnopqrstuvwxyz"; 
    return alph;
}

char * convertToMinusc(char *str){
    //Change a string to minusc
    int len = strlen(str);
    char *alphMin;
    char *alphMai;
    int i,j;
    alphMin = alphabetMinusc(alphMin);
    alphMai = alphabetMaiusc(alphMai);
    int lenAlf = strlen(alphMin);
    for(i=0;i<len;i++){
        for(j=0;j<lenAlf;j++){
            if(str[i] == alphMai[j]){
                str[i] = alphMin[j];
            }
        }
    }
    return str;
}

void gotoXY(int x, int y)
{
    //X and Y are inverted in linux
    //The x moves the cursor on the screen column topo and bottom.
    //The y moves cursor on left or righ.
    //For MS Windows is the opposite.
    printf(CURP, x, y);
}

void setColor(int v)
{
    printf(COLR, v);
}

void clear()
{
    printf("\033c");
    //Restore terminal default config
}

void bestSleep(int val)
{
    fflush(stdout);
    struct timespec ts;
    ts.tv_sec = val / 1000;
    ts.tv_nsec = (val % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void forceColor(int a, int b)
{
    /*This function is the alternative to sysColor it clear the screen using the color everywhere.*/
    fflush(stdout);
    clear();
    fflush(stdout);
    printf(SYSF, b);
    fflush(stdout);
    printf(SYSB, a);
    fflush(stdout);
}

void sysColor(int a, int b)
{
    /* The idea was that to change color with the standard method and change PS1 in a way that the color would change after calling clear, but not works. */
    fflush(stdout);
    printf(SYSF, b);
    fflush(stdout);
    printf(SYSB, a);
    fflush(stdout);
    snprintf(ps1, 55, "PS1=\'\\[\\e[%d;%dm\\][\\u@\\h \\W]\\$\\[\\e[0m\\]\'", a, b);
    fflush(stdout);
    system(ps1);
}

void hide_cursor()
{
    printf("\e[?25l");
}

void show_cursor()
{
    printf("\e[?25h");
}

/*
Both functions call getchar(), but one remove the return key pressed with termios is not needed.
*/
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

int fattoriale(int num)
{
    int i = (num - 1);
    while (i > 0)
    {
        num *= i;
        --i;
    }
    return num;
}

int fatRic(int num)
{
    if (num == 1)
    {
        return 1;
    }
    return fatRic(--num) * (num + 1);
}

int fatRic2(int num)
{
    if (num == 0)
    {
        return 1;
    }
    return num * fatRic(--num);
}

//int fatRic3(int num){(num == 0)?return 1:return (num*fatRic(--num));}

void dividiNumInVet(int val, int *toRet)
{
    int temp;
    if (val <= 9 && val > 0)
    {
        toRet[0] = val;
    }
    else
    {
        if (val < 100)
        {
            toRet[0] = (int)(val / 10);
            toRet[1] = val % 10;
        }
        else
        {
            if (val > 99999)
            {
                toRet[0] = (int)(val / 100000);
                temp = val % 100000;
                toRet[1] = (int)(temp / 10000);
                temp = temp % 10000;
                toRet[2] = (int)(temp / 1000);
                temp = temp % 1000;
                toRet[3] = (int)(temp / 100);
                temp = temp % 100;
                toRet[4] = (int)(temp / 10);
                toRet[5] = temp % 10;
            }
            else
            {
                if (val > 9999)
                {
                    toRet[0] = (int)(val / 10000);
                    temp = val % 10000;
                    toRet[1] = (int)(temp / 1000);
                    temp = temp % 1000;
                    toRet[2] = (int)(temp / 100);
                    temp = temp % 100;
                    toRet[3] = (int)(temp / 10);
                    toRet[4] = temp % 10;
                }
                else
                {
                    if (val > 999)
                    {
                        toRet[0] = (int)(val / 1000);
                        temp = val % 1000;
                        toRet[1] = (int)(temp / 100);
                        temp = temp % 100;
                        toRet[2] = (int)(temp / 10);
                        toRet[3] = temp % 10;
                    }
                    else
                    {
                        toRet[0] = (int)(val / 100);
                        temp = val % 100;
                        toRet[1] = (int)(temp / 10);
                        toRet[2] = temp % 10;
                    }
                }
            }
        }
    }
}

void dividiNumInVet2(int val, int *toRet)
{
    int temp, i;
    int lenVal = 0;
    int temVal = val;
    int divisore = 1;
    if (val <= 9 && val > 0)
    {
        toRet[0] = val;
    }
    else
    {
        while(temVal >= 1){
            lenVal++;
            temVal = (int)temVal/10;
            divisore *= 10;
        }
        for(i=0;i<lenVal;i++){
            toRet[i] = (int)(val/divisore);
            val = val%divisore;
            divisore = (int)divisore/10;
        }
    }
}

int* dividiNumInVet3(int val, int* v)
{
    int temp,i;
    int lenVal = 0;
    int temVal = val;
    int divisore = 1;
    if(val < 1){
        int vet[1];
        vet[0] = (int)val;
        v=vet;
    }else{
        while(temVal >= 1){
            lenVal++;
            temVal = (int)temVal/10;
            divisore *= 10;
        }
        int toRet[lenVal];
        for(i=0;i<lenVal;i++){
            toRet[i] = (int)(val/divisore);
            val = val%divisore;
            divisore = (int)divisore/10;
        }
        v=toRet;
    }
    return v;
}

void stringToInt(char *numStr, int *num){ //passare variabile num con & (&num)
    *num = atoi(numStr);
}

void stringToFloat(char *numStr, float *num){ //passare variabile num con & (&num)
    *num = atof(numStr);
}

void intToString(int num, char *toRet)
{
    int vetInt[7] = {0, 0, 0, 0, 0, 0, 0},i;
    dividiNumInVet(num, vetInt);
    for (i = 0; i < 7; i++)
    {
        toRet[i] = '0' + vetInt[i];
    }
}

void decToBin(int num)
{
    int v[20],j;
    int mx = 0, i = 19;
    while (mx < 20)
    {
        v[mx] = 0;
        mx++;
    }
    while (num >= 1)
    {
        v[i] = num % 2;
        num /= 2; //(int)ceil(num/2);
        i--;
    }
    for (j = 0; j < 20; j++)
    {
        printf("%d", v[j]);
    }
}

void binToDecSing(char byte)
{
    //char as single byte to be converted in dec (int value)
    //For examble & in ascii value
    printf("%d", (int)(byte));
}

int potenza(int a, int b){
    int i, ret=a;
    if(b==0){ret=1;}
    for(i=1;i<b;i++){
        ret = ret*a;    
    }
    return ret;
}

int binToDec(char *num)
{
    int j = 0, res = 0;
    int i = 0;
    while (num[i] != '\0'){i++;}
    res = 0;
    j=0;
    --i;
    for(i;i>0;i--){
        if (num[i] == '1'){
            res += pow(2,j);
        }
        ++j;
    }
    return res;
}

void binToHex(char byte)
{
    //char so a single byte
    char hex[8];
    sprintf(hex, "%02x", (unsigned int)byte & 0xff);
    printf("hex value of '%c' is: 0x%s", byte, hex);
}

void hexToBin(char *hexVal)
{
    //A hexadecimal string
    //Converted in binary value
    char quads[16][5] = {"0000\0", "0001\0", "0010\0", "0011\0", "0100\0", "0101\0", "0110\0", "0111\0", "1000\0", "1001\0", "1010\0", "1011\0", "1100\0", "1101\0", "1110\0", "1111\0"};
    char *reslt[20];
    int i = 0,j;
    while (hexVal[i] != '\0')
    {
        i++;
    }
    for (j = 0; j < i; j++)
    {
        if (hexVal[j] >= 48 && hexVal[j] <= 57)
        {
            reslt[j] = quads[hexVal[j] - 48];
        }
        if (hexVal[j] >= 65 && hexVal[j] <= 70)
        {
            reslt[j] = quads[hexVal[j] - 55];
        }
        if (hexVal[j] >= 97 && hexVal[j] <= 102)
        {
            reslt[j] = quads[hexVal[j] - 87];
        }
    }
    printf("\n");
    for (j = 0; j < i; j++)
    {
        printf("%s", reslt[j]);
    }
}

bool checkNum(char car, int *num){
    char * numeri = "0123456789";
    bool found = false;
    int i;
    for(i=0;i<10 && found==false;i++){
        if(car == numeri[i]){
            *num = i;
            found = true;
        }
    }
    if(found == false){
        *num = -1;
    }
    return found;
}

char *congiunzioni(char * vet){
    vet = "di a da in con su per tra fra e piu più giù giu";
    return vet;
}

void somma(double x, double y, double *res){
    *res = x+y;
}

void subt(double x, double y, double *res){
    *res = x-y;
}

void mult(double x, double y, double *res){
    *res = x*y;
}

void divi(double x, double y, double *res){
    if(y!=0){
        *res = x/y;
    }else{
        *res = 0;
    }
}

void pote(double x, double y, double *res){
    *res = pow(x,y);
}

void my_pow(double x, double y, double *res){
    double i;
    for(i=0;i<y;i++){
        *res *= x;
    }
}

void fact(int *num){
    *num = fatRic2(*num);
}

void my_sqrt(double *x){
    *x = sqrt(*x);
}

void my_log(double *x){
    *x = log(*x);
}

void my_cos(double *x){
    *x = cos(*x);
}

void my_sin(double *x){
    *x = sin(*x);
}

//Create an array of operands associated to its function
mathOp * mathOperators(mathOp *operands){
    //void (*fun_ptr)() = somma;
    operands[0].value = '+';
    operands[0].operazione = "somma";
    operands[0].operation = "sum";
    operands[0].express = &somma;
    operands[1].value = '-';
    operands[1].operazione = "sottrai";
    operands[1].operation = "subtract";
    operands[1].express = &subt;
    operands[2].value = '*';
    operands[2].operazione = "moltiplica";
    operands[2].operation = "multiply";
    operands[2].express = &mult;
    operands[3].value = '/';
    operands[3].operazione = "dividi";
    operands[3].operation = "divide";
    operands[3].express = &divi;
    operands[4].value = '^';
    operands[4].operazione = "eleva";
    operands[4].operation = "pow";
    operands[4].express = &pote;
    operands[5].value = '!';
    operands[5].operazione = "fattoriale";
    operands[5].operation = "factorial";
    operands[5].express = &fact;
    operands[6].value = 'r';
    operands[6].operazione = "radice quadrata";
    operands[6].operation = "sqrt";
    operands[6].express = &my_sqrt;
    operands[7].value = 'l';
    operands[7].operazione = "logaritmo";
    operands[7].operation = "log";
    operands[7].express = &my_log;
    operands[8].value = 'c';
    operands[8].operazione = "coseno";
    operands[8].operation = "cos";
    operands[8].express = &my_cos;
    operands[9].value = 's';
    operands[9].operazione = "seno";
    operands[9].operation = "sin";
    operands[9].express = &my_sin;
    operands[10].value = 'N';
    operands[10].operazione = "NULL";
    operands[10].operation = "null";
    operands[10].express = NULL;
    //Pow, logarithm, sqrt(), factorial, cos(), sin()
    return operands;
}

/**Logic functions*/
/*and
| x | y | and |
| 0 | 0 |  0  |
| 0 | 1 |  0  |
| 1 | 0 |  0  |
| 1 | 1 |  1  |
*/
bool and_f(bool x, bool y){
    return (x && y);
}
/*not and
| x | y | and |
| 0 | 0 |  1  |
| 0 | 1 |  1  |
| 1 | 0 |  1  |
| 1 | 1 |  0  |
*/
bool nand_f(bool x, bool y){
    return !(x && y);
}
/*or
| x | y |  or |
| 0 | 0 |  0  |
| 0 | 1 |  1  |
| 1 | 0 |  1  |
| 1 | 1 |  1  |
*/
bool or_f(bool x, bool y){
    return (x || y);
}

/*
particular or
| x | y | p_or|
| 0 | 0 |  0  |
| 0 | 1 |  0  |
| 1 | 0 |  1  |
| 1 | 1 |  1  |
*/

bool p_or_f(bool x, bool y){
    if((x==true && y==false) || (x==true && y==false)){
        return true;
    }else{
        return false;
    }
}

/*not or
| x | y | nor |
| 0 | 0 |  1  |
| 0 | 1 |  0  |
| 1 | 0 |  0  |
| 1 | 1 |  0  |
*/
bool nor_f(bool x, bool y){
    return !(x || y);
}

/*exclusive or
| x | y | xor |
| 0 | 0 |  0  |
| 0 | 1 |  1  |
| 1 | 0 |  1  |
| 1 | 1 |  0  |
*/
bool xor_f(bool x, bool y){
    bool z = nand_f(x,y);
    return nand_f(nand_f(x,z),nand_f(z,y));
}

/*particular xor
| x | y | p_xor |
| 0 | 0 |   0   |
| 0 | 1 |   0   |
| 1 | 0 |   1   |
| 1 | 1 |   0   |
*/
bool p_xor_f(bool x, bool y){
    if(x==true && y==false){
        return true;
    }else{
        return false;
    }
}


/*invert
| x | y | -> | x | y |
| 0 | 1 | -> | 1 | 0 |
| 1 | 0 | -> | 0 | 1 |
*/
void inv_f(bool *x, bool *y){
    bool t = *x;
    *x = *y;
    *y = t;
}
/*invert integer values*/
void inv_int_f(int *x, int *y){
    int t = *x;
    *x = *y;
    *y = t;
}
/*Declare a possesion, for example Luke own a pen (in natural language)*/
void declare_own(char *a, char *b){
    int i = 0;
    for(i=0;i<POSS_LENG;i++){
        if(dataOwn[i].possessore == NULL){
            dataOwn[i].possessore = a;
            dataOwn[i].posseduto = b;
            i = POSS_LENG;
        }
    }
}
/*Check if "luca" own a "pen"?*/
bool verify_own(char *a, char *b){
    int i = 0;
    for(i=0;i<POSS_LENG;i++){
        if(dataOwn[i].possessore == a && dataOwn[i].posseduto == b){
            return true;
        }
    }
    return false;
}
/*Check if the owner own something*/
bool verify_owner(char *a){
    int i = 0;
    for(i=0;i<POSS_LENG;i++){
        if(dataOwn[i].possessore == a){
            return true;
        }
    }
    return false;
}
/*Return in valued passed in b an array, every object that luca own
Return an int of returned objects
*/
int check_owner(char *a, char **b){
    int i = 0;
    int j = 0;
    for(i=0;i<POSS_LENG;i++){
        if(dataOwn[i].possessore == a){
            b[j] = dataOwn[i].posseduto;
            j++;
        }
    }
    return j;
}
/*Check if something is owned*/
bool verify_owned(char *b){
    int i = 0;
    for(i=0;i<POSS_LENG;i++){
        if(dataOwn[i].posseduto == b){
            return true;
        }
    }
    return false;
}
/*Who own what?It check it.*/
int check_owned(char **a, char *b){
    int i = 0;
    int j = 0;
    for(i=0;i<POSS_LENG;i++){
        if(dataOwn[i].posseduto == b){
            a[j] = dataOwn[i].possessore;
            j++;
        }
    }
    return j;
}

#ifdef _WIN32
void hide_cursor()
{
    CONSOLE_CURSOR_INFO cursor = {1, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void show_cursor()
{
    CONSOLE_CURSOR_INFO cursor = {1, TRUE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void gotoXY(int x, int y)
{
    COORD CursorPos = {x, y};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, CursorPos);
}

void setColor(char color)
{
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon, color);
}
#endif

void gString(char *str)
{
    int lun = strlen(str),i;
    for (i = 0; i < lun; i++)
    {
        nomeFunzione[i] = str[i];
    }
}

void speakMachineEffect(char *str)
{
    int j = 0;
    do
    {
        printf("%c", str[j]);
        bestSleep(60);
        fflush(stdout);
        ++j;
    } while (str[j] != '\0');
}

int menu(char *opts, int numOpts, int mainColor, int secoColor, int terzColor)
{ //opts max 13 caratteri
    int exit1 = 0, i = 0, j = 0, n, slp = 70;
    int select = 0, keyP = 0;
    char tpr[40];
    gotoXY(1, 0);
    fflush(stdout);
    fflush(stdin);
    setColor(35);
    printf("MENU ");
    setColor(mainColor); //setColor(25);
    gotoXY(2, 0);
    hide_cursor();
    while (i < numOpts)
    { //Takes opts strings and divide them so will be printable in the menu with the dorp-down graphic effect.
        if (i > 0)
        {
            j += 1;
        }
        n = 0;
        do
        {
            tpr[n] = opts[j];
            ++j;
            ++n;
        } while (opts[j] != ',');
        gotoXY((2 + i), 0);
        while ((13 - (n)) > 0)
        {
            tpr[n] = ' ';
            tpr[n + 1] = '\0';
            n++;
            fflush(stdout);
        }
        if (i == 0)
        {
            setColor(secoColor); //setColor(34);
        }
        else
        {
            setColor(terzColor); //setColor(35);
        }
        printf("|%s|", tpr);
        fflush(stdout);
        bestSleep(slp);
        slp -= 5;
        if (i > 5)
        {
            slp = 36;
        }
        fflush(stdout);
        ++i;
    }
    hide_cursor();
    bestSleep(50);
    while (exit1 == 0)
    {
        fflush(stdin);
        keyP = getch();
        fflush(stdout);
        fflush(stdin);
        i = 0, j = 0;
        if (keyP == 115)
        { //s key
            if (select == (numOpts - 1))
            {
                select = 0;
            }
            else
            {
                ++select;
            }
        }
        if (keyP == 119)
        { //w key
            if (select == 0)
            {
                select = (numOpts - 1);
            }
            else
            {
                --select;
            }
        }
        if (keyP == 10)
        { //10 equivale a invio [ENTER]
            exit1 = 1;
        }
        /*	if(keyP != 10 && keyP != 115 && keyP != 119){
		keyP = 0;
		select = 0;
	}*/
        fflush(stdout);
        fflush(stdin);
        while (i < numOpts)
        { //Same thing but without graphic effectsbecause it must change color depending on selection.
            if (i > 0)
            {
                j += 1;
            }
            n = 0;
            do
            {
                tpr[n] = opts[j];
                ++j;
                ++n;
            } while (opts[j] != ',');
            gotoXY((2 + i), 0);
            while ((13 - (n)) > 0)
            { //Add spaces to align menu.
                tpr[n] = ' ';
                tpr[n + 1] = '\0';
                n++;
                fflush(stdout);
            }
            if (select == i)
            {
                setColor(secoColor);
            }
            else
            {
                setColor(terzColor);
            }
            printf("|%s|", tpr);
            fflush(stdout);
            ++i;
        }
    }
    fflush(stdout);
    fflush(stdin);
    return (select + 1);
}

int printCascadeMenu(char *titleBarText, char *opts, int numOpts, int mainColor, int secoColor, int terzColor)
{
    /*
Uso: 	int scelta = 0;	
	scelta = printCascadeMenu("Start\0","Nuovo File,Leggi File,Modifica File,Cancella File,Esci,",5,36,44,00);

First options is 0 when selected
To pass options fill opts with a string structured in this way: "option1,option2,option3,..."
Use , to create a option item in menu
Remeber to insert , as last char,
Supported color are in this list:

42,34,35 title bar standard menu verde e marrone
25,34,35 title bar standard menu viola e nero scritte blu
36,44,00 title bar standard menu nero scritte bianche selezione azzura
33,56,00 title bar standard menu nero scritte bianche selezione scritta arancione NON SUPPORTATA
Other color give as resoult default coloration.
For example yellow give a menu yellow with orange text e red title bar text with background yellow
*/
    int j, i, screen = 68; //Screen size to adapt at screen
    int altez, keyP, stop = 0;
    char car;
    //getScreenSize(screen, alt);
    gString(titleBarText);
    hide_cursor();
    forceColor(36, 44);
    gotoXY(0, 0);
    setColor(36);
    printf("MENU ");
    setColor(01); //01
    printf("| %s", nomeFunzione);
    for (j = 0; car != '\0'; j++)
    {
        car = nomeFunzione[j];
    }
    screen -= j;
    setColor(33);
    for (i = 0; i < screen; i++)
    {
        fflush(stdout);
        printf("%c", 35); //176 print triangles on gnome-terminal and square on a real tty
    }
    setColor(00);
    fflush(stdin);
    fflush(stdout);
    bestSleep(500);
    int toReturn = 0;
    //This solution is not good because the menu must be opened when you want, a solution with FORK and PIPE it's better.
    //while(stop==0){ not used
    keyP = getch();
    if (keyP == 109)
    { //press m for open menu
        toReturn = menu(opts, numOpts, mainColor, secoColor, terzColor);
        stop = 1;
    }
    if (keyP == 88)
    { //End character. SHIFT+X
        stop = 1;
    }
    //}
    return toReturn;
}

int printSimpleMenu(char *opts, int numOpts, int firstColor, int secondColor)
{
    /*Uso: 	printSimpleMenu("mucca1,mucca2,mucca3,mucca4,",4,96,02); */
    //Options as string with , and number of options passed and first e secondary color.
    //0 if no options are been passed
    int scelta = 0, i = 0, j = 0, n;
    char tpr[40];
    forceColor(0, 0);
    gotoXY(3, 0);
    speakMachineEffect("Premi il numero corrispondente all'azione nel menu:");
    setColor(firstColor); //setColor(96);
    gotoXY(9, 30);
    while (i < numOpts)
    {
        if (i > 0)
        {
            j += 1;
        }
        n = 0;
        do
        {
            tpr[n] = opts[j];
            ++j;
            ++n;
        } while (opts[j] != ',');
        gotoXY((10 + i), 30);
        printf("%d)%s ", (i + 1), tpr);
        fflush(stdout);
        ++i;
    }
    gotoXY(16, 60);
    setColor(03);
    printf("====");
    gotoXY(17, 60);
    printf("   ))");
    gotoXY(18, 60);
    printf(" ===");
    gotoXY(19, 60);
    printf("   ))");
    gotoXY(20, 60);
    printf("====");
    gotoXY(17, 65);
    setColor(04);
    printf("||\\");
    gotoXY(18, 65);
    printf("|  )");
    gotoXY(19, 65);
    printf("|   )");
    gotoXY(20, 65);
    printf("|  )");
    gotoXY(21, 65);
    printf("||/");
    setColor(07);
    do
    {
        gotoXY(2, 74);
        setColor(secondColor); //setColor(02);
        scanf("%d", &scelta);
    } while (scelta < 1 || scelta > numOpts);
    return scelta;
}

void browseDir(char *name, int level)
{
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name)))
    {
        return;
    }
    if (!(entry = readdir(dir)))
    {
        return;
    }
    do
    {
        if (entry->d_type == DT_DIR)
        {
            char path[1024];
            int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
            path[len] = 0;
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            printf("%*s[%s]\n", level * 2, "", entry->d_name);
            browseDir(path, level + 1);
        }
        else
        {
            printf("%*s-%s\n", level * 2, "", entry->d_name);
        }
    } while (entry = readdir(dir));
    closedir(dir);
}

void fillStringVect(char *vect1, char *vect2)
{
    int i;
    for (i = 0; i < strlen(vect1); i++)
    {
        vect2[i] = vect1[i];
    }
}

void tree_cleaner()
{
    //This function clean the ouput of tree /dir/.. -aifdo /outdir/..
    //Remove not equals pathes.
    /*Clean function for command: tree /discLocation -aifdo ./Documents/outputAlbero.txt da:
	1.The tree must be cleaned by the number of directories at the end of the file.
	2.The must be cleaned by link signed with ->
	3.The tree must cleaned by pathes that are not in the other disks to pair.
First ./SHACreator.lxf and then when the first end it's possible launch ./SHAMerge.lxf
SHACreator and SHAMerge must be compiled for that specified disk.
*/
    FILE *rd1;
    FILE *rd2;
    FILE *wr;
    char path1[50];
    char path2[50];
    char path3[50];
    char res1[500];
    char res2[500];
    int lengRes = 0, i = 0;
    short notCopyMe = 0;
    printf("\nPassami il percorso del albero del primo disco: ");
    scanf("%s", path1);
    printf("\nPassami l'albero da confrontare: ");
    scanf("%s", path2);
    printf("\nDove metto il risultato? : ");
    scanf("%s", path3);
    rd1 = fopen(path1, "r");
    rd2 = fopen(path2, "r");
    wr = fopen(path3, "a");
    while (feof(rd1) == 0)
    {
        if (fgets(res1, 490, rd1) != NULL)
        {
            while (feof(rd2) == 0)
            {
                if (fgets(res2, 490, rd2) != NULL)
                {
                    if (strcmp(res1, res2) == 0)
                    { //They will never be the same because the mount position is different
                        //Check if it a link if not copy it in append
                        lengRes = strlen(res1);
                        for (i = 0; i < lengRes; i++)
                        {
                            if (res1[i] == '-' && res1[i + 1] == '>')
                            {
                                notCopyMe = 1;
                            }
                        }
                        if (notCopyMe == 0)
                        {
                            //Copy the path in the file append
                            fprintf(wr, "%s", res1);
                        }
                        //Otherwise it will ignore the copy and set the flag to false
                        notCopyMe = 0;
                    }
                }
            }
            //fseek to read again the second file from the beginning
            fseek(rd2, 0L, 0);
        }
    }
    fclose(rd1);
    fclose(rd2);
    fclose(wr);
    printf("\nAttenzione hai ottenuto solo l'albero pulito di: %s e non del albero usato per la comparazione.", path1);
}

void sha_merge(char *pathOutFile, char *pathOfTreeFile)
{
    char res[491];
    char ptCh[47];
    fillStringVect(pathOutFile, ptCh);
    FILE *pf;
    fflush(stdin);
    pf = fopen(pathOfTreeFile, "r");
    if (pf != NULL)
    {
        int i = 1;
        int aryNum[6] = {0, 0, 0, 0, 0, 0};
        while (feof(pf) == 0)
        {
            if (i == 1000 || i == 3000 || i == 4000 || i == 6000 || i == 8000 || i == 9000)
            {
                printf("\n\t\tRAFFREDDAMENTO DISCO...\n");
                bestSleep(60000);
            }
            if (fgets(res, 490, pf) != NULL)
            {
                if (i > 9)
                {
                    dividiNumInVet(i, aryNum);
                    ptCh[32] = '0' + aryNum[0];
                    ptCh[33] = '0' + aryNum[1];
                    ptCh[34] = '0' + aryNum[2];
                    ptCh[35] = '0' + aryNum[3];
                    ptCh[36] = '0' + aryNum[4];
                    ptCh[37] = '0' + aryNum[5];
                    //Who cares this character is inserted \0.
                }
                else
                {
                    ptCh[32] = '0' + i; //Now it is the name of the file.
                }
                //The same thing as above for ptCh.
                if (i <= 9)
                {
                    ptCh[33] = '\0';
                }
                if (i >= 10 && i <= 99)
                {
                    ptCh[34] = '\0';
                }
                if (i >= 100 && i <= 999)
                {
                    ptCh[35] = '\0';
                }
                if (i >= 1000 && i <= 9999)
                {
                    ptCh[36] = '\0';
                }
                if (i >= 10000 && i <= 99999)
                {
                    ptCh[37] = '\0';
                }
                if (i >= 100000)
                {
                    ptCh[38] = '\0';
                }
                printf("\nptCh: %s\n", ptCh);
                FILE *redF = fopen(ptCh, "r");
                if (redF != NULL)
                {
                    FILE *wrtF = fopen(pathOutFile, "a");
                    char tmSr[500];
                    while (feof(redF) == 0)
                    {
                        if (fgets(tmSr, 500, redF) != NULL)
                        {
                            fprintf(wrtF, "%s", tmSr); //I will copy the content of the file in the file *.sha
                        }
                    }
                    fclose(redF);
                    fclose(wrtF);
                    char fRem[120] = "rm ";
                    strcat(fRem, ptCh);
                    strcat(fRem, "\0");
                    system(fRem); //Delete the file
                }
                //Then must delete the opened file (opened in reading) that as finished to copy.
                i++;
            }
        } //End While
    }
    fclose(pf);
}

void sha_creator(char *pathOutFile, char *pathOfTreeFile)
{
    //If you didn't see disk activity for a long time press CTRL+C in this way the process will go on. The result will be in a single file.
    //The path (pathOutFile) cannot be relative and the output file must be absolute too.
    //The pathOfTreeFile it is the file where the output of the command tree is saved: /dir/da/cui/ottenere/albero -aifdo /dir/out/resoult.txt
    char res[491];
    char scPth[47];
    fillStringVect(pathOutFile, scPth);
    char conPh[500] = "openssl  dgst -md5 -hex /myPath/docs* -out ";
    FILE *pf; //File where the directory tree is written
    fflush(stdin);
    pf = fopen(pathOfTreeFile, "r");
    if (pf != NULL)
    {
        int i = 1;
        int aryNum[6] = {0, 0, 0, 0, 0, 0};
        while (feof(pf) == 0)
        { //Until the file with the directory tree it's not finished one more is processed. (ONE MORE...)
            /*This process create sha512 for each file in a directory in the worst way possible */
            if (i == 1000 || i == 3000 || i == 4000 || i == 6000 || i == 8000 || i == 9000)
            {
                printf("\n\t\tRAFFREDDAMENTO DISCO...\n");
                bestSleep(60000);
            }
            strcpy(conPh, "openssl  dgst -sha512 -hex ");
            if (fgets(res, 490, pf) != NULL)
            {
                strcat(conPh, res);
                conPh[strlen(conPh) - 1] = '/';
                strcat(conPh, "* > ");
                if (i > 9)
                {
                    //To name random file that contains sha512 for a directory
                    dividiNumInVet(i, aryNum); //DiviNumVet should return a memory address and this should be solved
                    scPth[32] = '0' + aryNum[0];
                    scPth[33] = '0' + aryNum[1];
                    scPth[34] = '0' + aryNum[2];
                    scPth[35] = '0' + aryNum[3];
                    scPth[36] = '0' + aryNum[4];
                    scPth[37] = '0' + aryNum[5];
                    //The ending character is added \0.
                }
                else
                {
                    scPth[32] = '0' + i; //Now i it's the name of the file
                }
                if (i <= 9)
                {
                    scPth[33] = '\0';
                }
                if (i >= 10 && i <= 99)
                {
                    scPth[34] = '\0';
                }
                if (i >= 100 && i <= 999)
                {
                    scPth[35] = '\0';
                }
                if (i >= 1000 && i <= 9999)
                {
                    scPth[36] = '\0';
                }
                if (i >= 10000 && i <= 99999)
                {
                    scPth[37] = '\0';
                }
                if (i >= 100000)
                {
                    scPth[38] = '\0';
                }
                strcat(conPh, scPth);
                system(conPh);
                i++;
            }
        } //End While
    }
    fclose(pf); //Close the tree file
    sha_merge(pathOutFile, pathOfTreeFile);
}

bool isANumber(char *str, number *toPass){
    int lun = strlen(str);
    int i, num=0, num2=0;
    bool end = false;
    bool isNumero = false;
    number numero;
    for(i=0;i<lun && end==false;i++){
        if(i>0){
            if(checkNum(str[i+1],&num) && str[i]=='.' && checkNum(str[i-1], &num2)){
                    numero.floatNum = atof(str);
                    numero.is_float = true;
                    *toPass = numero;
                    end=true;
                    isNumero = true;
            }
        }
    }
    if(!end){
        if(checkNum(str[0], &num) || checkNum(str[1],&num2)){
            numero.is_float = false;
            numero.intNum = atoi(str);
            *toPass = numero;
            isNumero = true;
        }else{
            isNumero = false;
        }
    }
    return isNumero;
}

number * isANumberWrong(char *str, number *numero){
    int lun = strlen(str);
    int x, y, i=0, tag=0;
    int num = 0, num2 = 0, t_mp=0;
    float numF = 0;
    bool frstTime = true;
    bool floatValue = false;
    for(i=0;i<lun;i++){
        fflush(stdin);
        checkNum(str[i], &num);
        if(i>0){
            checkNum(str[i-1], &num2);
        }
        //For example dhs23jsio
        //If the character at i it's a number and character at i-1 no than enter in the IF
        //If you can eneter in the IF check if i-1 is point or comma, and if i-2 is a number.
        //The idea behind is that if it is 23 I will analyze 2 and behind there is a 2 c'è and if there is a point or comma I will check if it is a numer like
        //9.23 means that I can enter in the if because the first condition it is 1 and the second it is one 1 (NOT WORKING YET!!!)
        //If it is "asd 23" and I analyze 2 behind there is space than the first gives me 1 instead the second gives me 0 so I can enter in it
        //If it is 234 and I'm on 3 I can enter in the if because the first give me 1 and the second zero
        //The condition enter in this way:
        //    |X|Y|XOR|
        //    and that p_xor enter in this way:
        //    |1|0|1
        if(p_or_f(p_xor_f((num != -1),(num2 != -1)), ((str[i-1]=='.' || str[i-1]==',') && checkNum(str[i-2],&t_mp)) )){ //trovato un numero, controlla anche se il carattere precedente era un numero così da capire se faceva parte del numero di prima o no
            if(frstTime){
                //To know where the number of the string start.
                x = i;
                y = i;
                frstTime = false;
            }
            int8_t u;
            int chk=0;
            for(u=1;u<NUM_TO_FLOAT;u++){ //
                if(str[i+u] == '.' || str[i+u] == ','){ //I don't remember maybe it works or maybe it see only 3.4 and not 43.6. That's because while I was programming this I got interrupted many times and I had to start programming it from scratch after weeks or months.
                //Check if the number found is float
                    if((i+(u+1))<=lun){ //I will check that the point is not a end of sentence.
                        chk = 0;
                        checkNum(str[i+(u+1)], &chk);
                        if(chk != -1){ //I will check that after the comma will exists a number - Comma or point in italy and in french I think it's comma but in USA it's a point
                            floatValue = true;
                        }
                    }
                }else{
                    if(!checkNum(str[i+u],&chk)){
                        break; //If it is a number go on with the cycle otherwise break it, because it's not a float.
                    }
                }
            }
        }else{
            if(frstTime == false){ //A number was found so it can go on.
                y=(i-1);
                int len, j, s;
                len = y+1;
                s = x;
                char tmpStr[len];
                for(j=0;j<len;j++){
                    tmpStr[j] = str[s];
                    s++;
                }
                //The only thing to is convert tmpStr in a float or int value.
                if(floatValue == true){
                    stringToFloat(tmpStr,&numF);
                    numero[tag].floatNum = numF;
                    numero[tag].is_float = true;
                    floatValue = false;
                }else{
                    stringToInt(tmpStr, &num);
                    numero[tag].intNum = num;
                    numero[tag].is_float = false;
                }
                frstTime = true;
                tag++;
            }
        }
    }
    return numero;
}

char* substring(char *str, int start, int stop){
    if(stop>start && stop<=strlen(str)){
        int len = (stop-start);
        char substr[(len+2)];
        int i = 0;
        /*
        for(i=0;i<(len-1);i++){
            substr[i] = str[i];
        }
        substr[len]='\0';*/
        for(i=start;i<=stop;i++){
            if(str[i]!= ' '){
                substr[(i-start)]=str[i];
            }else{
                substr[(i-start)]='\0';
            }
        }
        /*
        substr[len+1] = '\0';
        str[0] = '\0';
        for(i=0;i<len;i++){
            str[i] = substr[i];
        }
        str[len+1]='\0';*/
        return (str=substr);
    }else{
        printf("ERRORE :>ç_");
        return NULL;
    }
}

int spaceCount(char *str){
    int numSpace=0,i;
    int len=strlen(str);
    for(i=0;i<len;i++){
        if(str[i]==' '){
            numSpace++;
        }
    }
    return numSpace;
}

bool isAnAction(char *str, mathOp *actions, mathOp *toExecute){
    //Should return an array of actions
    int len = strlen(str);
    char *key; //Note *key="mnuc" and key[]="mnuc" are two different things. One is defined as an array (key[]) so it is sequential in memory the other is a pointer(*key). - Maybe the compiler compile it in different ways, I don't understan what I had written.
    int x=0,j,i;
    int spaces = spaceCount(str);
    bool inside = false;
    while(spaces > 0){
        for(j=x;j<len;j++){
            if(str[j] == ' '){
                inside = true;
                if(j<=(len-1)){
                    j++;
                }
                int c=0;
                for(i=x;i<(j-1);i++){
                    key[c] = str[i];
                    c++;
                }
                key[c] = '\0';
                //key=substring(str,x,j);
                x=j;
                break;
            }
        }
        for(i=0;i<MATHOP_LENGHT;i++){
            if((strcmp(key,actions[i].operazione) == 0 || strcmp(key,actions[i].operation) == 0)){
                //Found a math with sample string sum (or somma) and sum inside actions(azioni)
                *toExecute = actions[i];
                return true;
            }
        }
        spaces--;
    }
    if(str != "" && inside==false){
        key = str; //WARNING
        for(i=0;i<MATHOP_LENGHT;i++){
            if((strcmp(key,actions[i].operazione) == 0 || strcmp(key,actions[i].operation) == 0)){
                //Found a math with sample string sum (or somma) and sum inside actions(azioni)
                *toExecute = actions[i]; //An addressing error could appear, in case isAction is called. It's the last value of execute to determine everything. - I don't remember what I meant.
                return true;
            }
        }
    }
    return false;
}

bool isAConnector(char *str){
    //Should return an array of connettore
    char *cong;
    cong = congiunzioni(cong);
    int len = strlen(str),j,i,q,u;
    int conLen = strlen(cong);
    int spaces = spaceCount(str);
    int spaCon = spaceCount(cong);
    int x=0,y=0;
    char key[SINGLE_WORD_LENGHT];
    char keya[SINGLE_WORD_LENGHT];
    bool connector = false;
    bool inside = false;
    while(spaces > 0){
        for(j=x;j<len;j++){
            if(str[j] == ' '){
                inside = true;
                if(j<=(len-1)){
                    j++;
                }
                int c=0;
                for(i=x;i<(j-1);i++){
                    key[c] = str[i];
                    c++;
                }
                key[c] = '\0';
                //key = substring(str,x,j);
                x=j;
                break;
            }
        }
        char tmK[strlen(key)];
        //memset(tmK,0,sizeof(tmK));
        for(q=0;q<strlen(key);q++){
                tmK[q]=key[q];
                tmK[strlen(key)] = '\0';
        }
        y=0;
        spaCon = spaceCount(cong);
        while(spaCon > 0){
            for(u=y;u<conLen;u++){
                if(cong[u] == ' '){
                    if(u<=(conLen-1)){
                        u++;
                    }
                    int j=0;
                    for(i=y;i<(u-1);i++){
                        keya[j] = cong[i];
                        j++;
                    }
                    keya[j] = '\0';
                    //keya=substring(cong,y,u);
                    y=u;
                    break;
                }
            }
            if(strcmp(keya,tmK) == 0){
                connector = true;
                break;
            }
            spaCon--;
        }
        spaces--;
    }
    if(str!="" && inside==false){
        while(spaCon > 0){
            for(u=y;u<conLen;u++){
                if(cong[u] == ' '){
                    if(u<=(conLen-1)){
                        u++;
                    }
                    int j=0;
                    for(i=y;i<(u-1);i++){
                        keya[j] = cong[i];
                        j++;
                    }
                    keya[j] = '\0';
                    //keya=substring(cong,y,u);
                    y=u;
                    break;
                }
            }
            /*
            if(strlen(keya)==strlen(str)){
                for(i=0;i<strlen(keya);i++){
                    if(keya[i]==str[i]){
                        connector = true;
                    }else{
                        connector = false;
                    }
                }
            }*/
            if(strcmp(keya,str) == 0){
                return true;
                connector = true;
                break;
            }
            spaCon--;
        }
        //If I setup a printf here, arrayParole takes random characters.
    }
    return connector;
}

char **getStringsFromSeparator(char *str, char separator, char **arrayOfWords){
    int numSeparator = spaceCount(str);
    int conLen = strlen(str);
    int y=0, j=0,q;
    char word[SINGLE_WORD_LENGHT];
    int i=y,r;
    char arrayParole[numSeparator+1][SINGLE_WORD_LENGHT];
    int lenW = 0;
    for(i=y;i<conLen;i++){
        if(str[i] == separator){
            if(i<=(conLen-1)){
                i++;
            }
            int c=0;
            for(r=y;r<(i-1);r++){
                word[c] = str[r];
                c++;
            }
            word[c] = '\0';
            //word=substring(str,y,i);
            int wordLen = strlen(word);
            for(q=0;q<wordLen;q++){
                arrayParole[j][q]=word[q];
            }
            arrayParole[j][wordLen] = '\0';
            arrayOfWords[j] = arrayParole[j];
            y=i;
            j++;
        }
    }
    return arrayOfWords;
}

wordsScopo *takeScopo(char *scopo, wordsScopo *aryOfActions){
    scopo = convertToMinusc(scopo);
    int lenAW = spaceCount(scopo);
    int i;
    char *arrayDiParole[lenAW];
    mathOp vetActions[MATHOP_LENGHT];
    wordsScopo paroleAzioni[lenAW];
    memcpy(vetActions,mathOperators(vetActions),MATHOP_LENGHT);
    memcpy(arrayDiParole,getStringsFromSeparator(scopo, ' ', arrayDiParole),lenAW);
    for(i=0;i<lenAW;i++){ //For each word in the array
        paroleAzioni[i].word = arrayDiParole[i];
        if(isAConnector(arrayDiParole[i])==true){
            paroleAzioni[i].isAction = false;
            paroleAzioni[i].isString = true;
            paroleAzioni[i].numero.is_float = false;
            paroleAzioni[i].numero.intNum = 0;
        }else{
            if(isAnAction(arrayDiParole[i],vetActions, &paroleAzioni[i].azione)){
                //paroleAzioni[i].azione this should be empty if it's not an action
                paroleAzioni[i].isAction = true;
                paroleAzioni[i].isString = false;
                paroleAzioni[i].numero.is_float = false;
                paroleAzioni[i].numero.intNum = 0;
                //Action should be taken usign this & but...
            }else{
                if(isANumber(arrayDiParole[i],&paroleAzioni[i].numero)){
                    paroleAzioni[i].isAction = false;
                    paroleAzioni[i].isString = false;
                }
            }
        }
        aryOfActions[i] = paroleAzioni[i];
	//Action takes NULL or object mathOP to NULL if word is an int or a float or a string - This could be a bug
        //Boolean values are created depending on type.
    }
    //Try to compile like this gcc ai.c -lm -o0 ai.lxf
    //This code is related to a project not yet published.
    //For each element string in the output array that must be inserted in an array correlated to what you can do.
    //Only if you check every DB
    return aryOfActions;
}

//openssl dgst -md5 filename - hash for unreadable file are filled with 0
char* generaMD5(char* filename){
    static unsigned char buffer[128];
    int i=0;
    char conv[33];
    char *str = (char *)malloc(sizeof(char)*33);
    unsigned bytes = 0;
    FILE* datafile = fopen(filename , "rb");
    if(datafile!=NULL){
	    unsigned char digest[MD5_DIGEST_LENGTH];
	    MD5_CTX ctx;
	    MD5_Init(&ctx);
	    while((bytes = fread(buffer, 1, 128, datafile)))
	    {
		MD5_Update(&ctx, buffer, bytes);
	    }
	    
	    MD5_Final(digest, &ctx);
	    fclose(datafile);
	    for(i=0; i < 16; ++i){
		sprintf(&conv[i*2], "%02x", (unsigned int)digest[i]);
	    }
	    for(i=0;i<33;i++){
	    	str[i]=conv[i];
	    }
    }else{
       str="00000000000000000000000000000000";
    }
    return str;
}

//openssl dgst -sha1 filename - hash for unreadable file are filled with 0
char* generaSHA1(char* filename){
    static unsigned char buffer[128];
    int i=0;
    char conv[41];
    char *str = (char *)malloc(sizeof(char)*41);
    unsigned bytes = 0;
    FILE* datafile = fopen(filename , "rb");
    if(datafile!=NULL){
	    unsigned char digest[SHA_DIGEST_LENGTH];
	    SHA_CTX ctx;
	    SHA1_Init(&ctx);
	    while((bytes = fread(buffer, 1, 128, datafile)))
	    {
		SHA1_Update(&ctx, buffer, bytes);
	    }
	    
	    SHA1_Final(digest, &ctx);
	    fclose(datafile);
	    for(i=0; i < 20; ++i){
		sprintf(&conv[i*2], "%02x", (unsigned int)digest[i]);
	    }
	    for(i=0;i<41;i++){
	    	str[i]=conv[i];
	    }
    }else{
       str="0000000000000000000000000000000000000000";
    }
    return str;
}
//openssl dgst -sha256 filename - hash for unreadable file are filled with 0
char* generaSHA256(char* filename){
    static unsigned char buffer[256];
    int i=0;
    char conv[65];
    char *str = (char *)malloc(sizeof(char)*65);
    unsigned bytes = 0;
    FILE* datafile = fopen(filename , "rb");
    if(datafile!=NULL){
    	unsigned char digest[SHA256_DIGEST_LENGTH];
    	SHA256_CTX ctx;
    	SHA256_Init(&ctx);
   	while((bytes = fread(buffer, 1, 256, datafile)))
   	{
        	SHA256_Update(&ctx, buffer, bytes);
    	}
    	SHA256_Final(digest, &ctx);
    	fclose(datafile);
    	for(i=0; i < 32; ++i){
        	sprintf(&conv[i*2], "%02x", (unsigned int)digest[i]);
    	}
    	for(i=0;i<65;i++){
    		str[i]=conv[i];
    	}
    }else{
       str="0000000000000000000000000000000000000000000000000000000000000000";
    }
    return str;
}

//Generate mdf of each file in a directory and save them on the selected file with the path.
void browseAndGenMD5(char *name, int level, FILE* pf)
{
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name)))
    {
        return;
    }
    if (!(entry = readdir(dir)))
    {
        return;
    }
    char* linea;
    char path[1024];
    do
    {
	path[0]='\0';
	int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
	path[len] = 0;
	if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
	continue;}
	linea="";
	linea=generaMD5(path);
	fputs(path,pf);
	fputs(",",pf);
	fputs(linea,pf);
	fputs("\n",pf);
	browseAndGenMD5(path, level + 1,pf);
    } while ((entry = readdir(dir)));
    closedir(dir);
}

//Browse a directory and generate a sha1 for each file that can be read and save them in the selected file without the path.
void browseAndGenSHA1(char *name, int level, FILE* pf)
{
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name)))
    {
        return;
    }
    if (!(entry = readdir(dir)))
    {
        return;
    }
    char* linea;
    char path[1024];
    do
    {
	path[0]='\0';
	int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
	path[len] = 0;
	if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
	continue;}
	linea="";
	linea=generaSHA1(path);	
	fputs(linea,pf);
	fputs("\n",pf);
	browseAndGenSHA1(path, level + 1,pf);
    } while ((entry = readdir(dir)));
    closedir(dir);
}

//Browse and generate a sha256 for each file in the given directory and save them in the given file without the path.
void browseAndGenSHA256(char *name, int level, FILE* pf)
{
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name)))
    {
        return;
    }
    if (!(entry = readdir(dir)))
    {
        return;
    }
    char* linea;
    char path[1024];
    pf=fopen("listsum","ab");
    do
    {
	path[0]='\0';
	int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
	path[len] = 0;
	if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
	continue;}
	linea="";
	linea=generaSHA256(path);
	//fputs(path,pf);
	//fputs(",",pf);
	fputs(linea,pf);
	fputs("\n",pf);
	browseAndGenSHA256(path, level + 1,pf);
    } while ((entry = readdir(dir)));
    closedir(dir);
}

/**
Usage ex of browseAndCheckSHA1
pf=fopen("checklist","wb");
cf=fopen("listsum","rb");
browseAndCheckSHA1("/bin",1,pf,cf);
browseAndCheckSHA1("/boot",2,pf,cf);
browseAndCheckSHA1("/etc",2,pf,cf);
browseAndCheckSHA1("/usr/bin",1,pf,cf);
fclose(pf);
fclose(cf);
**/

void browseAndCheckSHA1(char *name, int level, FILE* pf, FILE* cf)
{
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name)))
    {
        return;
    }
    if (!(entry = readdir(dir)))
    {
        return;
    }
    char* linea;
    char orln[41];
    char path[1024];
    int i=0;
    do
    {
	path[0]='\0';
	int len = snprintf(path, sizeof(path) - 1, "%s/%s", name, entry->d_name);
	path[len] = 0;
	if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
	continue;}
	fflush(stdin);
	linea="";
	for(i=0;i<40;i++){
		orln[i]=0;	
	}
	orln[40]='\0';
	linea=generaSHA1(path);
	//fwrite(linea,sizeof(linea),1,pf);
	fputs(linea,pf);
	fputs("\n",pf);
	fflush(stdin);
	fread(orln,1,40,cf);
	fseek(cf,1,1);
	if(strcmp(orln,linea)!=0){
		printf("\nDifferent: %s\n",path);
		printf("\norln: %s\n",orln);
		printf("\nlinea: %s\n",linea);
	}
	browseAndCheckSHA1(path, level + 1,pf,cf);
    } while ((entry = readdir(dir)));
    closedir(dir);
}


//In order to call this function you need to open the file with fopen(rb)
//linelen is the number of byte to read
//You can read a test file line per line of chars setting jumpByte to 1 and position to 1
char* readALine(char* line, int linelen, int jumpByte, int position, FILE* pf){
	line = (char *)malloc(sizeof(char)*linelen);
	fread(line,1,linelen,pf);
	fseek(pf,jumpByte,position); //Jump n byte from current position
	return line;
}

/*
int getRootWindowSize(int *w, int *h){
	Display* pdsp = NULL;
	Window wid = 0;
	XWindowAttributes xwAttr;
	pdsp = XOpenDisplay(NULL);
	if(!pdsp){
		fprintf(stderr, "Failed to open default display.\n");
		return -1;
	}
	wid = DefaultRootWindow(pdsp);
	if(0>wid){
		fprintf(stderr, "Failed to obtain the root windows Id of the default screen of given display.\n");
		return -2;
	}
	Status ret = XGetWindowAttributes(pdsp, wid,&xwAttr);
	*w = xwAttr.width;
	*h = xwAttr.height;
	XCloseDisplay(pdsp);
	return 0;
}

int getScreenSize(int *w, int *h){
	Display *pdsp = NULL;
	Screen *pscr = NULL;
	pdsp = XOpenDisplay(NULL);
	if(!pdsp){
		fprintf(stderr, "Failed to open default display.\n");
		return -1;
	}
	pscr = DefaultScreenOfDisplay(pdsp);
	if(!pscr){
		fprintf(stderr, "Failed to optain default screen of given display.\n");
		return -2;
	}
	*w = pscr->width;
	*h = pscr-height;
	XCloseDisplay(pdsp);
	return 0;
}
*/

/**
Compile with all possible warnings
gcc checkSYS.c -lm -lssl -lcrypto -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion  -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror  -o checkSYS.lxf

**/

#endif
