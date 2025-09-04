#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ID 1024
#define DEC 1025

/* versão extendida de identificador pascal
ID = [A-Za-z][A-Za-z0-9]
*/
int isID(FILE *tape)
{
    int head = getc(tape);

    if (isalpha(head)) {
        while (isalnum(head = getc(tape)));
        ungetc(head, tape);
        return ID;
    }

    ungetc(head, tape);
    return 0;
}

int gettoken(FILE *source)
{
    int token;

    if ((token = isID(source))) return token;

    token = getc(source);

    return token;
}'
