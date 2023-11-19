#include <nanvix/const.h>
#include <sys/sem.h>

/**
 * @brief Retorna o valor corrente do semáforo identificado por semid.
 * @param semid Identificador do semáforo.
 * @return O valor corrente do semáforo ou -1 se ocorrer um erro.
 */
int getval(int semid) {
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE) { 
            if (tabsem[i].id == semid) {
                if (valid(&tabsem[i]) == -1) return -1; // Verifica a validade do semáforo

                return tabsem[i].value; // Retorna o valor do semáforo
            }
        }
    }
    return -1; // Semáforo não encontrado
}

/**
 * @brief Define o valor do semáforo identificado por semid como val.
 * @param semid Identificador do semáforo.
 * @param val Novo valor a ser definido no semáforo.
 * @return 0 se bem-sucedido, -1 se ocorrer um erro.
 */
int setval(int semid, int val) {
    // Busca o semáforo com o id informado.
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE) {
            if (tabsem[i].id == semid) {
                if (valid(&tabsem[i]) == -1) return -1; // Verifica a validade do semáforo
                tabsem[i].value = val; // Define o novo valor do semáforo
                return 0; // Operação bem-sucedida
            }
        }
    }

    return -1; // Semáforo não encontrado
}

/**
 * @brief Diminui o valor do semáforo identificado por semid e o destrói se possível.
 * @param semid Identificador do semáforo.
 * @return 0 se bem-sucedido, -1 se ocorrer um erro.
 */
int ipc_rmid(int semid) {
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE) {
            if (tabsem[i].id == semid) {
                if (valid(&tabsem[i]) == -1) return -1; // Verifica a validade do semáforo

                struct sem *sem = &tabsem[i]; // Cria um ponteiro para o semáforo
                int table = sem->pos / 16; // Encontra em qual tabela o semáforo está
                int pos_table = sem->pos % 16; // Descobre a posição do semáforo na tabela
                int comp = 1;

                comp = comp << pos_table; // Posiciona o valor 1 no bit que será alterado

                int *b = &curr_proc->shared_sem[table]; // Pega a tabela em que o semáforo está

                *b = *b & (~comp); // Atribui o valor da tabela à operação de set bit

                sem->procusing--;

                if (sem->procusing == 0) {
                    sem->state = INACTIVE; // Define o semáforo como inativo
                    return 0; // Operação bem-sucedida
                }
                return 0; // Operação bem-sucedida
            }
        }
    }

    return -1; // Semáforo não encontrado
}

/**
 * @brief Executa uma série de operações no semáforo identificado por semid de acordo com o comando cmd.
 * @param semid Identificador do semáforo.
 * @param cmd Comando a ser executado.
 * @param val Valor associado ao comando (se aplicável).
 * @return Resultado da operação ou -1 em caso de erro.
 */
PUBLIC int sys_semctl(int semid, int cmd, int val) {
    // Verifica se o processo atual pode realizar operações no semáforo.

    switch (cmd) {
        case GETVAL:
            return getval(semid); // Retorna o valor do semáforo
        case SETVAL:
            return setval(semid, val); // Define um novo valor para o semáforo
        case IPC_RMID:
            return ipc_rmid(semid); // Remove o semáforo se possível
    }

    return -1; // Comando inválido
}
