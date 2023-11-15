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

/*
 * Argumentos do programa.
 */

int key;

/*
 * Imprime a versão do programa e encerra.
 */
static void version(void)
{
	printf("semget (Nanvix Coreutils) %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Direitos Autorais (C) 2011-2014 Pedro H. Penna\n");
	printf("Este é um software livre sob a ");
	printf("Licença Pública Geral GNU Versão 3.\n");
	printf("NÃO EXISTE GARANTIA, na medida do permitido por lei.\n\n");

	exit(EXIT_SUCCESS);
}

/*
 * Imprime o uso do programa e encerra.
 */
static void usage(void)
{
	printf("Uso: semget [opções] <id>\n\n");
	printf("Resumo: Cria um semáforo. Se já existir, retorna o identificador do semáforo.\n\n");
	printf("Opções:\n");
	printf("  --help             Exibe esta informação e sai\n");
	printf("  --version          Exibe a versão do programa e sai\n");

	exit(EXIT_SUCCESS);
}

/*
 * Obtém o número do semáforo.
 */
static void getargs(int argc, char *const argv[])
{
	int i;	   /* Índice do Loop.         */
	char *arg;  /* Argumento Atual.   */

	/* Lê os argumentos da linha de comando. */
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];

		/* Analisa o argumento da linha de comando. */
		if (!strcmp(arg, "--help"))
		{
			usage();
		}
		else if (!strcmp(arg, "--version"))
		{
			version();
		}
		else
		{
			key = atoi(arg);
		}
	}

	/* ID do semáforo inválido. */
	if (key < 0 || key > SEM_MAX)
	{
		fprintf(stderr, "semget: ID do semáforo incorreto. Por favor, escolha um número entre 0 e %d\n", SEM_MAX);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *const argv[])
{
	int ret; // retorno da função semget

	// Lendo os Argumentos
	getargs(argc, argv);
	ret = semget(key);
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
