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
int value = -1; /* Valor para operações no semáforo. */
int cmd = -1; /* Opção. */

/*
 * Imprime a versão do programa e encerra.
 */
static void version(void)
{
	printf("semctl (Nanvix Coreutils) %d.%d\n\n", VERSION_MAJOR, VERSION_MINOR);
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
	printf("Uso: semctl [semid] [cmd] [value]\n\n");
	printf("Resumo: Configura diferentes opções para um semáforo.\n\n");
	printf("Opções:\n");
	printf("  --help    Exibir esta informação e sair\n");
	printf("  --version Exibir a versão do programa e sair\n");

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

	/* Obtenção dos argumentos do programa. */
	for (i = 1; i < argc; i++) {
		arg = argv[i];

		aux = atoi(arg);

		/* Exibir informações de ajuda. */
		if (!strcmp(arg, "--help"))
			usage(EXIT_SUCCESS);

		/* Exibir a versão do programa. */
		else if (!strcmp(arg, "--version"))
			version();

		/* Obter nomes de arquivos de origem/destino. */
		else {
			/* Obter semid */
			if (semid == -1){
				semid = aux;

                if (semid < 0 || semid > SEM_MAX){
                    fprintf(stderr, "semctl: ID do semáforo incorreto. Por favor, escolha um número entre 0 e %d.\n", SEM_MAX);
                    exit(EXIT_FAILURE);
                }
			}

			/* Obter cmd */
			else if (cmd == -1){
				cmd = aux;

                if (cmd != 0 && cmd != 1 && cmd != 3){
                    fprintf(stderr, "semctl: Comando incorreto para o semáforo. Por favor, escolha entre 0, 1 e 3.\n");
                    exit(EXIT_FAILURE);
                }

			}
	
            /* Obter valor */
            else if (value == -1){
				value = aux;

                if (value < 0){
                    fprintf(stderr, "semctl: Por favor, escolha um número positivo.\n");
                    exit(EXIT_FAILURE);
                }
			}
			/* Muitos argumentos. */
			else {
				fprintf(stderr, "semctl: muitos argumentos\n");
				usage(EXIT_FAILURE);
			}
		}
	}

	/* Argumentos faltando. */
	if ((semid == -1) || (cmd == -1) || (value == -1)) {
		fprintf(stderr, "semctl: argumentos faltando\n");
		usage(EXIT_FAILURE);
	}
}


int main(int argc, char *const argv[])
{
	int ret; // retorno da função semget

	// Lendo Argumentos
	getargs(argc, argv);

	ret = semctl(semid, cmd, value);
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
