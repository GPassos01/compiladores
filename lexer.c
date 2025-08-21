/* Analisador Léxico (Lexer)
 * Este arquivo implementa as funções de análise léxica para reconhecer:
 * - Identificadores (ID)
 * - Números decimais (DEC)
 * - Números octais (OCT)
 * - Números hexadecimais (HEX)
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  /* Para funções:
 * isalpha -> Verifica se o caractere é uma letra [A-Za-z]
 * isdigit -> Verifica se o caractere é um dígito [0-9]
 * isspace -> Verifica se o caractere é um espaço em branco [ ]
 * isalnum -> Verifica se o caractere é uma letra ou um dígito [A-Za-z0-9]
 * isxdigit -> Verifica se o caractere é um dígito hexadecimal [0-9A-Fa-f]
 * isupper -> Verifica se o caractere é uma letra maiúscula [A-Z]
 * islower -> Verifica se o caractere é uma letra minúscula [a-z]
 * isascii -> Verifica se o caractere é um caractere ASCII [0-127]
 * toupper -> Converte o caractere para maiúsculo
 * tolower -> Converte o caractere para minúsculo
 * getc -> Lê um caractere do arquivo
 * ungetc -> Devolve um caractere ao arquivo
 */


/* Enumeração dos códigos de tokens
 * Começando em 1024 para não conflitar com caracteres ASCII (0-255)
 * ID = 1024: Identificador
 * DEC = 1025: Número decimal
 * OCT = 1026: Número octal
 * HEX = 1027: Número hexadecimal
 */
enum {
	ID = 1024,
	DEC,
	OCT,
	HEX,
};

/* Função isID: Reconhece identificadores
 * Versão extendida de identificador Pascal
 * Gramática: ID = [A-Za-z][A-Za-z0-9]*
 * 
 * Um identificador deve:
 * - Começar com uma letra (maiúscula ou minúscula)
 * - Pode conter letras e dígitos após o primeiro caractere
 * 
 * Parâmetros:
 *   tape: ponteiro para o arquivo/stream de entrada
 * Retorno:
 *   ID (1024) se reconheceu um identificador
 *   0 se não é um identificador
 */
int isID(FILE *tape)
{
	int head;  /* Caractere atual sendo analisado */

	/* Lê o primeiro caractere e verifica se é uma letra */
	if (isalpha(head = getc(tape))) {
		/* Se é letra, continua lendo enquanto forem letras ou dígitos */
		while (isalnum(head = getc(tape)));
		/* Devolve o último caractere lido (que não é alfanumérico) */
		ungetc(head, tape);
		return ID;  /* Retorna o código do token ID */
	}

	/* Se não começou com letra, devolve o caractere e retorna 0 */
	ungetc(head, tape);
	return 0;
}

/* Função isDEC: Reconhece números decimais
 * Gramática: DEC = [1-9][0-9]* | '0'
 * 
 * Um número decimal pode ser:
 * - O dígito '0' sozinho
 * - Um dígito de 1-9 seguido de zero ou mais dígitos (0-9)
 * 
 * Diagrama de estados (autômato):
 *                           ------------------------------------------
 *                          |                      digit               |
 *                          |                    --------              |
 *                          |                   |        |             |
 *               digit      |     not zero      V        |  epsilon    V
 * -->(is DEC)--------->(is ZERO)---------->(isdigit)-------------->((DEC))
 *       |
 *       | epsilon
 *       |
 *       V
 *     ((0))
 * 
 * Parâmetros:
 *   tape: ponteiro para o arquivo/stream de entrada
 * Retorno:
 *   DEC (1025) se reconheceu um número decimal
 *   0 se não é um número decimal
 */
int isDEC(FILE *tape)
{
	int head;  /* Caractere atual sendo analisado */

	/* Verifica se o primeiro caractere é um dígito */
	if (isdigit(head = getc(tape)) ) {
		/* Caso especial: se é '0', é um decimal válido */
		if (head == '0') {
			return DEC;
		}
		/* Se não é '0', lê todos os dígitos seguintes */
		while ( isdigit(head = getc(tape)) );
		/* Devolve o último caractere não-dígito */
		ungetc(head, tape);
		return DEC;  /* Retorna o código do token DEC */
	}

	/* Se não começou com dígito, devolve o caractere e retorna 0 */
	ungetc(head, tape);
	return 0;
}

/* Função isOCT: Reconhece números octais
 * Gramática: OCT = '0'[0-7]+
 * 
 * Um número octal:
 * - Deve começar com '0'
 * - Deve ter pelo menos um dígito octal (0-7) após o '0'
 * - Exemplos: 077, 0123, 0755
 * 
 * Parâmetros:
 *   tape: ponteiro para o arquivo/stream de entrada
 * Retorno:
 *   OCT (1026) se reconheceu um número octal
 *   0 se não é um número octal
 */
int isOCT(FILE *tape)
{
	int head = getc(tape);  /* Lê o primeiro caractere */

	/* Verifica se começa com '0' */
	if (head == '0') {
		/* Lê o próximo caractere e verifica se é dígito octal */
		if ((head = getc(tape)) >= '0' && head <= '7') {
			/* Continua lendo enquanto forem dígitos octais */
			while ((head = getc(tape)) >= '0' && head <= '7');
			/* Devolve o último caractere não-octal */
			ungetc(head, tape);
			return OCT;  /* Retorna o código do token OCT */
		}
		/* Se não tinha dígitos octais após '0', devolve ambos caracteres */
		ungetc(head, tape);
		ungetc('0', tape);
	} else {
		/* Se não começou com '0', devolve o caractere */
		ungetc(head, tape);
	}
	return 0;
}

/* Função isHEX: Reconhece números hexadecimais
 * Gramática: HEX = '0'[Xx][0-9A-Fa-f]+
 * 
 * Um número hexadecimal:
 * - Deve começar com '0'
 * - Seguido de 'x' ou 'X'
 * - Deve ter pelo menos um dígito hexadecimal (0-9, A-F, a-f)
 * - Exemplos: 0xFF, 0x1A3B, 0xdeadbeef
 * 
 * Nota: isxdigit() verifica se é [0-9A-Fa-f]
 * 
 * Parâmetros:
 *   tape: ponteiro para o arquivo/stream de entrada
 * Retorno:
 *   HEX (1027) se reconheceu um número hexadecimal
 *   0 se não é um número hexadecimal
 */
int isHEX(FILE *tape)
{
	int head = getc(tape);  /* Lê o primeiro caractere */

	/* Verifica se começa com '0' */
	if ( head == '0' ) {
		int X;  /* Armazena o caractere 'x' ou 'X' */
		/* Verifica se o próximo é 'x' ou 'X' */
		if ( toupper(X = getc(tape)) == 'X' ) {
			/* Verifica se há pelo menos um dígito hexadecimal */
			if ( isxdigit(head = getc(tape)) ) {
				/* Continua lendo enquanto forem dígitos hexadecimais */
				while ( isxdigit(head = getc(tape)) );
				/* Devolve o último caractere não-hexadecimal */
				ungetc(head, tape);
				return HEX;  /* Retorna o código do token HEX */
			}
			/* Se não tinha dígitos hex, devolve todos os caracteres lidos */
			ungetc(head, tape);
			ungetc(X, tape);
			ungetc('0', tape);
			return 0;
		}
		/* Se não tinha 'x' ou 'X', devolve o caractere lido e o '0' */
		ungetc(X, tape);
		ungetc('0', tape);
		return 0;
	}
	/* Se não começou com '0', devolve o caractere */
	ungetc(head, tape);
	return 0;
}

/* Função skipspaces: Ignora espaços em branco
 * 
 * Remove todos os caracteres de espaçamento (espaços, tabs, quebras de linha)
 * até encontrar um caractere não-espaço
 * 
 * Parâmetros:
 *   tape: ponteiro para o arquivo/stream de entrada
 * Retorno:
 *   void - apenas avança o ponteiro do arquivo
 */
void skipspaces(FILE *tape)
{
	int head;  /* Caractere atual sendo analisado */
	/* Lê e descarta todos os espaços em branco */
	while (isspace(head = getc(tape)));
	/* Devolve o primeiro caractere não-espaço */
	ungetc(head, tape);
}

/* Função gettoken: Função principal do analisador léxico
 * 
 * Esta função tenta reconhecer o próximo token na entrada.
 * A ordem de verificação é importante:
 * 1. Identificadores (ID)
 * 2. Hexadecimal (HEX) - deve vir antes de OCT e DEC pois começa com '0'
 * 3. Octal (OCT) - deve vir antes de DEC pois começa com '0'
 * 4. Decimal (DEC)
 * 5. Caractere ASCII individual (operadores, pontuação, etc.)
 * 
 * Parâmetros:
 *   source: ponteiro para o arquivo/stream de entrada
 * Retorno:
 *   Código do token reconhecido (ID, DEC, OCT, HEX)
 *   ou código ASCII do caractere se não for nenhum dos anteriores
 */
int gettoken(FILE *source)
{
	int token;  /* Armazena o código do token reconhecido */
    if (source == NULL) return -1;
	/* Primeiro, pula todos os espaços em branco */
	skipspaces(source);

	/* Tenta reconhecer cada tipo de token, na ordem correta */
	if ((token = isID(source))) return token;   /* Verifica identificador */
	if ((token = isHEX(source))) return token;  /* Verifica hexadecimal */
	if ((token = isOCT(source))) return token;  /* Verifica octal */
	if ((token = isDEC(source))) return token;  /* Verifica decimal */

	/* Se não é nenhum dos tokens acima, lê um caractere ASCII */
	token = getc(source);

	/* Retorna o código ASCII do caractere (operador, pontuação, etc.) */
	return token;
}