/* Programa Principal - Teste do Analisador Léxico
 * Este programa testa o funcionamento do analisador léxico,
 * lendo tokens da entrada padrão (stdin) e exibindo seus códigos.
 */

#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>   /* Para função isascii() */
#include "main.h"    /* Para declaração da função gettoken() */

/* Função main: Ponto de entrada do programa
 * 
 * Funcionamento:
 * 1. Lê um token da entrada padrão
 * 2. Se for um caractere ASCII imprimível, mostra o caractere e seu código
 * 3. Se for um token especial (ID, DEC, OCT, HEX), mostra apenas o código
 *    e lê um segundo token para demonstração
 * 
 * Parâmetros: void (sem parâmetros)
 * Retorno: 0 indica execução bem-sucedida
 */
int main(void)
{
	/* Lê o primeiro token da entrada padrão */
	int token;
    token = gettoken(stdin);
    /* Verifica se o token é um caractere ASCII (0-127) */
    if (isascii(token) ) {
        /* Se for ASCII, imprime o caractere e seu código */
        printf("\'%c\': %d\n", token, token);
    } else {
        /* Se não for ASCII (é ID, DEC, OCT ou HEX) */
        /* Imprime o código do token */
        printf("%d\n", token);
        /* Lê e imprime um segundo token para demonstração */
        printf("%d\n", token = gettoken(stdin));
    }
	return 0;
}