#ifndef PARSER
#define PARSER

#include "headers.h"
#include "stack.h"

typedef struct ParserTokens {
    char token_name[100];
    char token_type[100];
} ParserTokens;

char parserTable[30][30][40];
int TokensCount = 0;
int stackCount = 0;

ParserTokens arr[501];

FILE* f1;

void parse(void);
void tableGenerator(void);
void intializeTable(void);
void printTokens(void);

int main(int argc, char const* argv[])
{
    f1 = fopen("lexerOut.txt", "r");
    char fileBuffer[100];
    char tokenName[100], tokenType[100];
    char c;
    int lenBuffer = 0;
    int i,k;

    while (!feof(f1)) { // CREATING ARRAY OF TOKENS FROM LEXER
        fgets(fileBuffer, 200, f1);
        lenBuffer = strlen(fileBuffer);
        fileBuffer[lenBuffer - 1] = '\0';
        for (i = 0; fileBuffer[i] != ' ' && i < lenBuffer; i++) {
            tokenName[i] = fileBuffer[i];
        }
        tokenName[i] = '\0';
        i++;
        int j = 0;
        while (i < lenBuffer) {
            tokenType[j] = fileBuffer[i];
            i++;
            j++;
        }
        tokenType[j] = '\0';
        if (TokensCount < 500) {
            strcpy(arr[TokensCount].token_name, tokenName);

            strcpy(arr[TokensCount].token_type, tokenType);
            TokensCount++;
        }
    }

    strcpy(arr[TokensCount].token_name, "$");
    strcpy(arr[TokensCount].token_type, "endLine");
    TokensCount++;

    intializeTable();
    tableGenerator(); // Creating parser table for rules to look into

    push("$");
    stackCount++;
    push("PROGRAM");
    stackCount++;
    //printTokens(); // to debug
    
    parse();

    return 0;
}

void printTokens()
{
    int k = 0;
    while (k < TokensCount) {
        printf("%s\n", arr[k].token_name);
        k++;
    }
}

void parse()
{
    int i = 0, z = 50, j;
    int row, column;
    char currentToken[200];
    char* garbage;
    int h;
    while (i < TokensCount) { // removed z counter even below also;
        if(i==TokensCount-2) i++;
        else{
        printf("\n\n\n");
        printf("Stack size -> %d\n", stackCount);
        printStack();

        if (stackCount == -1) {
            printf("done\n");
            break;
        }
        if (!strcmp(arr[i].token_name, "._init")) {
            strcpy(currentToken, "._init");
        } else if (!strcmp(arr[i].token_type, "id")) {

            strcpy(currentToken, "id");

        } else if (!strcmp(arr[i].token_type, "number")) {
            strcpy(currentToken, "number");

        } else if (!strcmp(arr[i].token_type, "float")) {
            strcpy(currentToken, "float");

        } else if (!strcmp(arr[i].token_type, "inst_code")) {
            strcpy(currentToken, "inst_code");

        } else if (!strcmp(arr[i].token_type, "macros")) {
            strcpy(currentToken, "macros");

        } else if (!strcmp(arr[i].token_type, "regs")) {
            strcpy(currentToken, "regs");

        } else {
            strcpy(currentToken, arr[i].token_name);
        }

        printf("Current Token: %s \n", currentToken);

        if (strcmp(readTop(), currentToken) == 0) {
            stackCount--;
            garbage = pop();
            i++;
           
        } else {

            for (j = 1; j < 30; j++) {
                if (!strcmp(readTop(), parserTable[j][0]))
                    row = j;
            }
            for (j = 1; j < 30; j++) {
                if (!strcmp(currentToken, parserTable[0][j]))
                    column = j;
            }

            printf("\n%d %d \n", row, column);

            if (!strcmp(parserTable[row][column], "empty"))
                stackCount--;

            else if ((strcmp(parserTable[row][column], "error")) && (strcmp(parserTable[row][column], "synch"))) {

                int len = strlen(parserTable[row][column]);
                char str[40], str1[40];
                strcpy(str, parserTable[row][column]);
                int check = 0;
                int kk;
                if (!strcmp(parserTable[row][column], "NULL")) {
                    pop();
                    stackCount--;
                } else {
                    stackCount--;
                    garbage = pop();
                    for (j = len - 1; j >= 0; j--) {
                        if (str[j] == ' ') {
                            for (h = j + 1, kk = 0; str[h] != ' ' && h < len; h++)
                                str1[kk++] = str[h];
                            str1[kk] = '\0';
                            stackCount++;
                            push(str1);
                        }
                    }

                    for (h = j + 1, kk = 0; str[h] != ' ' && h < len; h++) {
                        str1[kk++] = str[h];
                    }

                    str1[kk] = '\0';
                    stackCount++;
                    push(str1);
                }

            } else if (!strcmp(parserTable[row][column], "synch")) {
                stackCount--;
                garbage = pop();
            } else if (!strcmp(parserTable[row][column], "error")) {
                printf("error row : %d column : %d\n", row, column);
                i++;
            }
        }
        // z--;
        //z counter
    }
    }
}

void intializeTable()
{
    int i, j;
    for (i = 1; i < 30; i++)
        for (j = 1; j < 30; j++)
            strcpy(parserTable[i][j], "error");
}

void tableGenerator()
{
    strcpy(parserTable[1][0], "STRING");
    strcpy(parserTable[2][0], "WORDS");
    strcpy(parserTable[3][0], "WORD");
    strcpy(parserTable[4][0], "PROGRAM");
    strcpy(parserTable[5][0], "CLASSES");
    strcpy(parserTable[6][0], "CLASS");
    strcpy(parserTable[7][0], "MACROSTMT");
    strcpy(parserTable[8][0], "GROUP");
    strcpy(parserTable[9][0], "ASSIGN2");
    strcpy(parserTable[10][0], "ITEMS");
    strcpy(parserTable[11][0], "ITEM");
    strcpy(parserTable[12][0], "ASSIGN4");
    strcpy(parserTable[13][0], "MODULES");
    strcpy(parserTable[14][0], "FUNCTIONS");
    strcpy(parserTable[15][0], "FUNCTION");
    strcpy(parserTable[16][0], "LABEL");
    strcpy(parserTable[17][0], "STATEMENTLIST");
    strcpy(parserTable[18][0], "STMT");
    strcpy(parserTable[19][0], "ARGUMENTS");
    strcpy(parserTable[20][0], "REGISTER");
    strcpy(parserTable[21][0], "ASSIGN");
    strcpy(parserTable[22][0], "ASSIGN3");
    strcpy(parserTable[0][1], "id");
    strcpy(parserTable[0][2], "~");
    strcpy(parserTable[0][3], "macros");
    strcpy(parserTable[0][4], "{");
    strcpy(parserTable[0][5], "}");
    strcpy(parserTable[0][6], "LOFT.UTIL");
    strcpy(parserTable[0][7], "&");
    strcpy(parserTable[0][8], "[");
    strcpy(parserTable[0][9], "]");
    strcpy(parserTable[0][10], ".");
    strcpy(parserTable[0][11], "._init");
    strcpy(parserTable[0][12], "inst_code");
    strcpy(parserTable[0][13], "#");
    strcpy(parserTable[0][14], "regs");
    strcpy(parserTable[0][15], "(");
    strcpy(parserTable[0][16], "!");
    strcpy(parserTable[0][17], ")");
    strcpy(parserTable[0][18], "$");
    strcpy(parserTable[1][1], "WORDS");
    strcpy(parserTable[1][6], "WORDS");
    strcpy(parserTable[1][9], "WORDS");
    strcpy(parserTable[2][1], "WORD WORDS");
    strcpy(parserTable[2][6], "WORD WORDS");
    strcpy(parserTable[2][9], "NULL");
    strcpy(parserTable[2][18], "NULL");
    strcpy(parserTable[3][1], "id");
    strcpy(parserTable[3][6], "LOFT.UTIL");
    strcpy(parserTable[4][2], "CLASSES MODULES");
    strcpy(parserTable[4][11], "CLASSES MODULES");
    strcpy(parserTable[4][18], "CLASSES MODULES");
    strcpy(parserTable[5][2], "CLASS CLASSES");
    strcpy(parserTable[5][11], "NULL");
    strcpy(parserTable[6][2], "~ id MACROSTMT");
    strcpy(parserTable[7][3], "macros GROUP");
    strcpy(parserTable[8][4], "{ ASSIGN2");
    strcpy(parserTable[9][2], "ITEMS }");
    strcpy(parserTable[9][5], "ITEMS }");
    strcpy(parserTable[9][6], "LOFT.UTIL}");
    strcpy(parserTable[9][7], "ITEMS }");
    strcpy(parserTable[9][8], "ITEMS }");
    strcpy(parserTable[9][11], "ITEMS }");
    strcpy(parserTable[10][5], "NULL");
    strcpy(parserTable[10][7], "ITEM ITEMS");
    strcpy(parserTable[10][8], "ITEM ITEMS");
    strcpy(parserTable[11][7], "& [ ASSIGN4");
    strcpy(parserTable[11][8], "[ STRING ]");
    strcpy(parserTable[12][2], "~ id");
    strcpy(parserTable[12][10], ". id");
    strcpy(parserTable[13][11], "._init STATEMENTLIST FUNCTIONS");
    strcpy(parserTable[14][10], "FUNCTION FUNCTIONS");
    strcpy(parserTable[14][18], "NULL");
    strcpy(parserTable[15][10], "LABEL STATEMENTLIST");
    strcpy(parserTable[16][10], ". id");
    strcpy(parserTable[17][10], "NULL");
    strcpy(parserTable[17][12], "STMT STATEMENTLIST");
    strcpy(parserTable[17][18], "NULL");
    strcpy(parserTable[18][12], "inst_code ARGUMENTS");
    strcpy(parserTable[19][7], "REGISTER ARGUMENTS");
    strcpy(parserTable[19][8], "REGISTER ARGUMENTS");
    strcpy(parserTable[19][10], "NULL");
    strcpy(parserTable[19][12], "NULL");
    strcpy(parserTable[19][13], "REGISTER ARGUMENTS");
    strcpy(parserTable[19][14], "REGISTER ARGUMENTS");
    strcpy(parserTable[19][15], "REGISTER ARGUMENTS");
    strcpy(parserTable[19][18], "NULL");
    strcpy(parserTable[20][7], "& [ ASSIGN3");
    strcpy(parserTable[20][8], "[ STRING ]");
    strcpy(parserTable[20][13], "# id");
    strcpy(parserTable[20][14], "regs");
    strcpy(parserTable[20][15], "( ASSIGN");
    strcpy(parserTable[21][14], "regs )");
    strcpy(parserTable[21][16], "! [ . id ] )");
    strcpy(parserTable[22][10], ". id ]");
    strcpy(parserTable[22][11], "._init ]");


    strcpy(parserTable[3][9], "synch");
    strcpy(parserTable[6][11], "synch");
    strcpy(parserTable[7][2], "synch");
    strcpy(parserTable[7][11], "synch");
    strcpy(parserTable[8][2], "synch");
    strcpy(parserTable[8][11], "synch");
    strcpy(parserTable[11][5], "synch");
    strcpy(parserTable[12][5], "synch");
    strcpy(parserTable[12][7], "synch");
    strcpy(parserTable[12][8], "synch");
    strcpy(parserTable[13][18], "synch");
    strcpy(parserTable[15][18], "synch");
    strcpy(parserTable[16][12], "synch");
    strcpy(parserTable[16][18], "synch");
    strcpy(parserTable[18][10], "synch");
    strcpy(parserTable[18][18], "synch");
    strcpy(parserTable[20][10], "synch");
    strcpy(parserTable[20][12], "synch");
    strcpy(parserTable[20][18], "synch");
    strcpy(parserTable[21][7], "synch");
    strcpy(parserTable[21][8], "synch");
    strcpy(parserTable[21][10], "synch");
    strcpy(parserTable[21][12], "synch");
    strcpy(parserTable[21][13], "synch");
    strcpy(parserTable[21][15], "synch");
    strcpy(parserTable[21][18], "synch");
    strcpy(parserTable[22][7], "synch");
    strcpy(parserTable[22][8], "synch");
    strcpy(parserTable[22][12], "synch");
    strcpy(parserTable[22][13], "synch");
    strcpy(parserTable[22][14], "synch");
    strcpy(parserTable[22][15], "synch");
    strcpy(parserTable[2][18], "synch");
}

#endif