#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <lexer.h>

char lexeme[MAXIDLEN + 1];

/* 
    Regex:
    *versão extendida de identificador pascal*
        ID = [A-Za-z][A-Za-z0-9]*
*/
int isID(FILE *tape)
{

    if (isalpha(lexeme[0] = getc(tape))) {
        int i = 1;
        while (isalnum(lexeme[i] = getc(tape))) i++;
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return ID;
    }

    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
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

    if (isdigit(lexeme[0] = getc(tape))) {

        if (lexeme[0] == '0') {
            return DEC;
        }

        int i = 1;
        while (isdigit(lexeme[i] = getc(tape))) i++;
        ungetc(lexeme[i], tape);
        lexeme[i] = 0;
        return DEC;
    }

    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}

/*
    Regex:
        OCT = '0'[0-7]*
*/
int isOCT(FILE *tape){

    if ((lexeme[0] = getc(tape)) == '0') {

        if((lexeme[1] = getc(tape)) >= '0' && lexeme[1] <='7') {

            int i = 2;
            while((lexeme[i] = getc(tape)) >= '0' && lexeme[i] <='7') i++;
            ungetc(lexeme[i], tape);
            lexeme[i] = 0;
            return OCT;

        }
        ungetc(lexeme[1], tape);
        lexeme[1] = 0;
        ungetc(lexeme[0], tape);
        lexeme[0] = 0;
        return 0;
    }

    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
    return 0;
}

/*
    Regex:
        HEX = '0'[Xx][0-9A-Fa-f]* 
    **isxdigit == [0-9A-Fa-f]
*/
int isHEX(FILE *tape) {

    if ((lexeme[0] = getc(tape)) == '0'){

        int X;

        if ((lexeme[1] = getc(tape)) == 'X' || lexeme[1] == 'x'){

            if ( isxdigit(lexeme[2] = getc(tape)) ){

                int i = 3;
                while( isxdigit(lexeme[i] = getc(tape)) ) i++;
                ungetc(lexeme[i], tape);
                lexeme[i] = 0;
                return HEX;
            }
            ungetc(lexeme[2], tape);
            lexeme[2] = 0;
        }
        ungetc(lexeme[1], tape);
        lexeme[1] = 0;
        ungetc(lexeme[0], tape);
        lexeme[0] = 0;
        return 0;
    }

    ungetc(lexeme[0], tape);
    lexeme[0] = 0;
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
