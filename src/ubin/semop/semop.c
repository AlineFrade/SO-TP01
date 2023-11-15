#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <nanvix/pm.h>
#include <unistd.h>
#include <sys/sem.h>

/* Versão do Software. */
#define VERSION_MAJOR 1 /* Versão Principal. */
#define VERSION_MINOR 0 /* Versão Secundária. */

/* Nome do Processo Padrão. */
#define PROCESS_DEFAULT 0

/* Parâmetros. */
int semid = -1;  /* Identificação do Semáforo. */
int op = -1; /* Opção. */

/*
 * Imprime a versão do programa e encerra.
 */
static void version(void)
{
	printf("semop (Nanvix Coreutils) %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Direitos Autorais (C) 2011-2014 Pedro H. Penna\n");
	printf("Este é um software livre sob a ");
	printf("Licença Pública Geral GNU Versão 3.\n");
	printf("NÃO EXISTE GARANTIA, na medida do permitido por lei.\n\n");

	exit(EXIT_SUCCESS);
}

/*
 * Imprime o uso do programa e encerra.
 */
static void usage(int status)
{
	printf("Usage: semop [semid] [op]\n\n");
	printf("Brief: Configura diferentes opções para um semáforo.\n\n");
	printf("Options:\n");
	printf("  --help    Exibe esta informação e sai\n");
	printf("  --version Exibe a versão do programa e sai\n");

	exit(status);
}

/*
 * Obtém o número do semáforo.
 */
static void getargs(int argc, char *const argv[])
{
	int i;     /* Índice do Loop.       */
	char *arg; /* Argumento de Trabalho. */
	int aux;

	/* Obtém os argumentos do programa. */
	for (i = 1; i < argc; i++) {
		arg = argv[i];

		aux = atoi(arg);

		/* Exibe informações de ajuda. */
		if (!strcmp(arg, "--help"))
			usage(EXIT_SUCCESS);

		/* Exibe a versão do programa. */
		else if (!strcmp(arg, "--version"))
			version();

		/* Obtém nomes de arquivos de origem/destino. */
		else {
			/* Obtém semid */
			if (semid == -1){
				semid = aux;

                if (semid < 0 || semid > SEM_MAX){
                    fprintf(stderr, "semop: ID do semáforo incorreto. Por favor, escolha um número entre 0 e %d.\n", SEM_MAX);
                    exit(EXIT_FAILURE);
                }
			}

			/* Obtém op */
			else if (op == -1){
				op = aux;

                // A opção não pode ser 0, pois não é positiva nem negativa
                if (op == 0){
                    fprintf(stderr, "semop: comando incorreto para o semáforo. Por favor, escolha entre 0, 1 e 3.\n");
                    exit(EXIT_FAILURE);
                }

			}
			/* Muitos argumentos. */
			else {
				fprintf(stderr, "semop: muitos argumentos\n");
				usage(EXIT_FAILURE);
			}
		}
	}

	/* Argumentos faltando. */
	if ((semid == -1) || (op == -1)) {
		fprintf(stderr, "semop: argumentos faltando\n");
		usage(EXIT_FAILURE);
	}
}


int main(int argc, char *const argv[])
{
	int ret; // retorno da função semget

	// Lendo os Argumentos
	getargs(argc, argv);
	ret = semop(semid, op);
	if (ret >= 0)
	{
		printf("%d\n", ret);
		return ret;
	}
	else
	{
		return -1;
	}
}
