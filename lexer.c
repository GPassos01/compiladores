#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum {
    ID = 1024,
    DEC,
    OCT,
    HEX,
};

/* 
    Regex:
    *versão extendida de identificador pascal*
        ID = [A-Za-z][A-Za-z0-9]*
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

/*
    Regex:
        DEC = [1-9][0-9]* | '0'

    Representação de um automato para isDEC:
   -----------------------------------------------------------------------------------
   |                         --------------------------------------------------      |
   |                         |                       digit                    |      |
   |                         |                   -------------                |      |
   |                         |                   |           |                |      |
   |                         |                   |           |                |      |
   |             isdigit     |         !0        V           | epsilon        V      |
   | -->( isDEC )-------->( isZERO )-------->( isdigit )----------------> ( (DEC) )  |
   |        |                                                                        |
   |        | epsilon                                                                |
   |        |                                                                        |
   |        V                                                                        |
   |     ( (0) )                                                                     |
   -----------------------------------------------------------------------------------                                                                                                          
*/
int isDEC(FILE *tape){

    int head = getc(tape);

    if (isdigit(head)) {

        if (head == '0') {
            return DEC;
        }

        while (isdigit(head = getc(tape)));
        ungetc(head, tape);
        return DEC;
    }

    ungetc(head, tape);
    return 0;
}

/*
    Regex:
        OCT = '0'[0-7]*
*/
int isOCT(FILE *tape){
    int head = getc(tape);

    if (head == '0') {

        if((head = getc(tape)) >= '0' && head <='7') {

            while((head = getc(tape)) >= '0' && head <='7');
            ungetc(head, tape);
            return OCT;

        }
        ungetc(head, tape);
        ungetc('0', tape);
        return 0;
    }

    ungetc(head, tape);
    return 0;
}

/*
    Regex:
        HEX = '0'[Xx][0-9A-Fa-f]* 
    **isxdigit == [0-9A-Fa-f]
*/
int isHEX(FILE *tape) {
    int head = getc(tape);

    if (head == '0'){

        int X;
        head = getc(tape);

        if (head == 'X' || head == 'x'){

            if ( isxdigit(head = getc(tape)) ){

                while( isxdigit(head = getc(tape)) );
                ungetc(head, tape);
                return HEX;
            }

        }
        ungetc(head, tape);
        ungetc(X, tape);
        ungetc('0', tape);
        return 0;
    }

    ungetc(head, tape);
    return 0;
}

// skipspaces
void skipspaces(FILE *tape) {
    int head;
    while (isspace(head = getc(tape)));
    ungetc(head, tape);
}

int gettoken(FILE *source)
{
    int token;

    skipspaces(source);

    if ((token = isID(source))) return token;
    if ((token = isHEX(source))) return token;
    if ((token = isOCT(source))) return token;
    if ((token = isDEC(source))) return token;

    token = getc(source);

    // retornar um ASCII token
    return token;
}
