/* Declaração externa da função gettoken
 * Permite que outros arquivos usem a função gettoken definida em lexer.c
 * Parâmetro: FILE* - ponteiro para arquivo ou stream de entrada
 * Retorno: int - código do token identificado
 */
extern int gettoken(FILE *);