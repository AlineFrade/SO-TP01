#include <nanvix/pm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Software versioning. */
#define VERSION_MAJOR 1 /* Major version. */
#define VERSION_MINOR 0 /* Minor version. */

pid_t pid = 0;

static void get_proc_args(int argc, char *const argv[])
{
	int i;         /* Loop index.         */
	char *arg;     /* Current argument.   */

	/* Read command line arguments. */
	for (i = 1; i < argc; i++)
	{
		arg = argv[i];
		pid = atoi(arg);
	}
}

int main(int argc, char *const argv[]) {
    /*Declaração da estrutura para armazenar as informações do processo.*/ 
    struct process_buf buf;

    /*Chamada de função para obter os argumentos passados ao programa.*/ 
    get_proc_args(argc, argv);

    /*Tentativa de obter informações do processo usando a chamada de sistema.*/ 
    if (get_process_info(pid, &buf) < 0) {
        printf("Erro! Tente novamente!\n");
        return (errno); // Retorna o código de erro correspondente.
    }

    /*Impressão das informações do processo.*/ 
    printf("Process id: %d\n", buf.id);
    printf("Process State: %u\n", buf.state);
    printf("Process Priority: %d\n", buf.priority);
    printf("Process User Time: %u\n", buf.utime);
    printf("Process Kernel Time: %u\n", buf.ktime);

    return (EXIT_SUCCESS); /*Retorno indicando sucesso.*/ 
}