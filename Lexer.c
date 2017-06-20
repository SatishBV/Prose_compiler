#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checker(char* p);
void printLine(int tok, char* p, int lineNum);
void delChk(char p);

int i = 0, lno = 1;
char strBuf[100];
FILE* f1;
FILE* f2;

char instr[174][15] = { "noop", "stack/push", "stack/pull", "stack/lock", "stack/unlock", "stack/flush", "obj/cmp", "obj/def",
    "obj/clone", "obj/edit", "obj/commit", "obj/del", "tree/del", "obj/addr", "obj/pa", "obj/child", "obj/dump",
    "class/add", "class/del", "class/load", "attr/load", "class/load()", "attr/load()", "class/test", "attr/add",
    "attr/del", "attr/mod", "attr/mvadd", "attr/mvdel", "attr/mvmod", "attr/test", "attr/mvtest", "attr/copy",
    "attr/copy()", "attr/index", "attr/addr", "attr/xcopy", "attr/xcopy()", "attr/direct", "attr/cmp", "attr/def",
    "attr/xconv", "op/incr", "op/decr", "op/add", "opo/add", "opx/add", "op/sub", "opo/sub", "opx/sub", "op/mult",
    "opo/mult", "opx/mult", "op/div", "opo/div", "opx/div", "op/mod", "opo/mod", "opx/mod", "op/not", "opo/not",
    "opx/not", "op/and", "opo/and", "opx/and", "op/or", "opo/or", "opx/or", "op/xor", "opo/xor", "opx/xor", "op/shl",
    "opo/shl", "opx/shl", "op/shr", "opo/shr", "opx/shr", "op/mask", "op/swap", "list/def", "error/def", "error/now",
    "error/jmp", "error/clr", "func/addr", "var/addr", "func/def", "func/call", "func/bcall", "func/rtn", "local/jmp()",
    "local/jsr()", "local/jmp", "local/jsr", "local/rtn", "reg/load", "reg/move", "reg/jmpeq", "reg/jmpneq",
    "reg/jmplt", "reg/jmple", "reg/jmpgt", "reg/jmpge", "reg/jsreq", "reg/jsrneq", "reg/jsrlt", "reg/jsrle",
    "reg/jsrgt", "reg/jsrge", "reg/dump", "reg/load()", "reg/clr", "reg/index", "reg/cmp", "reg/save()", "reg/copy",
    "reg/copy()", "reg/conv", "reg/lcmp", "reg/rcmp", "reg/scan", "reg/type", "reg/roll", "var/def", "mtx/def",
    "itree/def", "itree/bdef", "xtree/def", "xtree/bdef", "var/local", "mtx/local", "itree/local", "itree/blocal",
    "xtree/local", "xtree/blocal", "var/static", "mtx/static", "itree/static", "itree/bstatic", "xtree/static",
    "xtree/bstatic", "var/global", "mtx/global", "itree/global", "itree/bglobal", "xtree/global", "xtree/bglobal",
    "tree/clone", "tree/bclone", "opa/add", "opa/sub", "opa/mult", "opa/div", "opa/mod", "opa/not", "opa/and",
    "opa/or", "opa/xor", "opa/shl", "opa/shr", "mtx/size", "mtx/bsize", "mtx/dim", "mtx/resize", "mtx/bresize", "mtx/set",
    "itree/addr", "xtree/addr", "itree/set", "xtree/set", "itree/xconv", "xtree/iconv", "debug/source", "debug/level" };

char macros[6][5] = { "EQUB", "EQUW", "EQUD", "EQUS", "EQUP", "EQUT" };

char regs[26][5] = { "P0", "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8", "P9", "P10", "P11", "P12", "P13", "P14", "P15", "PERR", "A", "SFLG",
    "SCMP", "LOCK", "PCTX", "PEEK", "PULL", "PUSH", "NULL" };

char seps[] = { ' ', '~', '.', '(', ')', '[', ']', '{', '}', ',' };

char oper[] = { '!', '&', '#' };

char reserveWord[] = { "._init" };

///******* SET OF FUNCTIONS PRESENT BELOW******///

void lexerProse();
void checkEmptyLines(char x);
void checker(char* p);
void checkDelimeter(char x);
void cleanBuf();
void loadBuffer(char x);
void delChk(char p);
void printLine(int tok, char* p, int lineNum);
int checkComments(char x);
int checkOperators(char x);
char getNextChar();
void finalWordPrint();
int chkReserve(char x);
int isNumber(char c);

int main(int argc, char const* argv[])
{
    char ch, fname[100];

    printf("Enter the location of file \n");
    scanf("%s", &fname);

    f1 = fopen(fname, "r");
    f2 = fopen("lexerOut.txt", "w");

    if (f2 == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    if (f1 == NULL) {
        printf("file not found\n");
    }

    lexerProse();

    return 0;
}

char getNextChar()
{
    return fgetc(f1);
}

void lexerProse()
{
    char ch;
    while ((ch = getNextChar()) != EOF) {

        checkEmptyLines(ch);

        if (checkComments(ch)) {
            continue;
        }

        if (checkOperators(ch)) {
            continue;
        }

        checkDelimeter(ch);

        loadBuffer(ch);
    }
    finalWordPrint();
}

int isNumber(char c)
{
    if (c >= 48 && c <= 57) {
        return 1;
    }
    return 0;
}

int isAlphaNumeric(char* p)
{ // returns 0->NUM 	1->VALID ALPHANUM 	2->FLOAT 	-1->INVALID TOKEN STARTING WITH NUM 	-2->INVALID Token 	-3-> Invalid float
    int i = 0;

    int flagInt = 0;
    int length = strlen(p);
    if (isNumber(p[0])) {
        do {
            if (p[i + 1] == '.' && flagInt == 0) {
                i++;
                flagInt = 1;
            } else if (p[i + 1] == '.') {
                return -3;
            }
            i++;
        } while (isNumber(p[i]) && (i < length));

        if (i < length) {
            return -1;
        }

        if (flagInt == 1) {
            return 2;

        } else if (flagInt == 0) {
            return 0;
        }

    } else {
        if (!strcmp(p, "._init")) {
            return 1;
        }
        if (p[0] == '_' || (p[0] >= 65 && p[0] <= 90) || (p[0] >= 97 && p[0] <= 122)) {
            return 1;
        } else {
            return -2;
        }
    }
}

void finalWordPrint()
{
    strBuf[i] = '\0';
    checker(strBuf);
}

void cleanBuf()
{
    i = -1;
}

void loadBuffer(char x)
{
    if (i != -1) {
        strBuf[i] = x;
        i++;
    } else {
        i = 0;
    }
}

void checkEmptyLines(char x)
{
    if (x == '\n' || x == '\r') {
        if (x == '\n') {
            lno++;
        }
        strBuf[i] = '\0';

        if (strBuf[0] != '\0') {
            checker(strBuf);
        }

        cleanBuf();
    }
}

int checkComments(char x)
{
    char gg;
    gg = x;
    if (gg == '%') {
        while (gg != '\n') {
            gg = getNextChar();
        }

        lno++;
        return 1;
    } else {
        return 0;
    }
}

int checkOperators(char x)
{
    char gg = x;
    int j;

    int localTok = 208; //operator token num
    for (j = 0; j <= 2; j++) {
        if (gg == oper[j]) {
            strBuf[j] = '\0';
            //printLine(208, gg, lno); // 208 token num for operators;
            printf("Token %d string %c line number %d \n", localTok, gg, lno);

            fprintf(f2, "%c operators\n", gg);

            return 1;
        }
    }

    return 0;
}

void checkDelimeter(char x)
{
    char gg = x, localBuf[100];
    int j;
    int k = 0;
    for (j = 0; j <= 9; j++) {
        if (i == -1) {
            break;
        }

        if (gg == seps[j]) {

            if (gg == '.') {

                k = 0;
                while (gg != ' ' && gg != '\n' && gg != ',' && gg != '\t' && gg != '\r') {
                    localBuf[k] = gg;
                    k++;
                    gg = getNextChar();
                }

                localBuf[k] = '\0';
                if (!strcmp(localBuf, "._init")) {
                    checker(localBuf);

                } else {
                    delChk('.');
                    int hk = 0;
                    while (localBuf[hk] != '\0') {
                        localBuf[hk] = localBuf[hk + 1];
                        hk++;
                    }
                }
                if (gg == '\n') {
                    lno++;
                }
                localBuf[0] = '\0';
                cleanBuf();
                break;
            }

            if (gg == '~') {
                delChk(gg);
                k = 0;
                while (gg != ' ' && gg != '\n' && gg != ',' && gg != '\t' && gg != '\r') {
                    if (k == 0) {
                        gg = getNextChar(); /* code */
                    }
                    localBuf[k] = gg;
                    k++;
                    gg = getNextChar();
                }
                localBuf[k] = '\0';
                checker(localBuf);

                if (gg == '\n') {
                    lno++;
                }
                localBuf[0] = '\0';
                cleanBuf();
                break;
            }

            if (gg == '[') {
                delChk(gg);
                k = 0;
                gg = getNextChar();

                if (gg == '.') {
                    delChk(gg);
                    gg = getNextChar();
                    while (gg == ' ') {
                        gg = getNextChar();
                    }
                }
                if (gg == '~') {
                    delChk(gg);
                    gg = getNextChar();
                }
                while (gg != ']') {

                    localBuf[k] = gg;
                    k++;
                    gg = getNextChar();
                }

                localBuf[k] = '\0';
                checker(localBuf);
                delChk(gg);
                localBuf[0] = '\0';
                cleanBuf();
                break;
            }

            strBuf[i] = '\0';
            if (strBuf[0] != '\0') {
                checker(strBuf);
            }
            if (gg != ' ') {
                delChk(gg);
            }
            cleanBuf();
        }
    }
}

void checker(char* p)
{
    if (*p == NULL) {

        return;
    }

    int k = 0, flag = 0;
    int g = -4;
    for (k = 0; k < 174; k++) {
        if (strcmp(instr[k], p) == 0) {
            printLine(k + 1, p, lno); // k+1 indicates token number
            fprintf(f2, "%s inst_code\n", p);
            flag = 1;
            break;
        }
    }

    if (flag == 0) {

        for (k = 0; k < 26; k++) {

            if (strcmp(regs[k], p) == 0) {
                printLine(k + 176, p, lno); // k+176 indicates token number
                fprintf(f2, "%s regs\n", p);
                flag = 2;
                break;
            }
        }

        for (k = 0; k < 6; k++) {
            if (strcmp(macros[k], p) == 0) {
                printLine(k + 202, p, lno); // k+202 indicates token number
                fprintf(f2, "%s macros\n", p);
                flag = 2;
                break;
            }
        }
    }
    if (flag == 0) {

        g = isAlphaNumeric(p);
    }
    if (g == -1) {
        printf("%s -> INVALID identifier starting with int at line num %d\n", p, lno);
    }
    if (g == -2) {
        printf("%s -> INVALID identifier token at line num %d\n", p, lno);
    }
    if (g == -3) {
        printf("%s -> INVALID FLOAT at line num %d\n", p, lno);
    }
    if (flag == 0 && g == 1) {
        if (*p != NULL) {
            printLine(209, p, lno);
            fprintf(f2, "%s id\n", p); // 209 token num for identifiers;
        }
    }
}

void delChk(char p)
{

    int i;
    for (i = 0; i <= 9; i += 1) {
        if (seps[i] == p) {
            //printLine(210+i, &p, lno);//(210)+i token num for delimiters
            if (p != ',') {

                fprintf(f2, "%c seperator\n", p);
            }
            printf("Token %d  string %c line number %d \n", 210 + i, p, lno);
        }
    }
}

void printLine(int tok, char* p, int lineNum)
{ //printf statement

    printf("Token %d  string %s line number %d \n", tok, p, lineNum);
}