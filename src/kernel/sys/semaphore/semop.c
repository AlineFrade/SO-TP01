#include <nanvix/const.h>
#include <sys/sem.h>

/**
 * @brief Incrementa o valor do semáforo identificado por semid.
 * @param semid Identificador do semáforo.
 * @return 0 se bem-sucedido, -1 se ocorrer um erro.
 */
int up(int semid) {   
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE) { 
            if (tabsem[i].id == semid) {
                if(valid(&tabsem[i]) == -1) return -1; // Verifica a validade do semáforo

                if (tabsem[semid].value == 0)
                    wakeup(curr_proc->chain); // Acorda processos na cadeia de espera

                tabsem[i].value++; // Incrementa o valor do semáforo
                
                return 0; // Operação bem-sucedida
            }
        }
    } 
    return -1; // Semáforo não encontrado
}

/**
 * @brief Decrementa o valor do semáforo identificado por semid.
 * @param semid Identificador do semáforo.
 * @return 0 se bem-sucedido, -1 se ocorrer um erro.
 */
int down(int semid) {
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE) { 
            if (tabsem[i].id == semid) {
                if(valid(&tabsem[i]) == -1) return -1; // Verifica a validade do semáforo

                while (tabsem[i].value == 0) {
                    sleep(curr_proc->chain, curr_proc->priority); // Coloca o processo na cadeia de espera
                }

                tabsem[i].value--; // Decrementa o valor do semáforo
                
                return 0; // Operação bem-sucedida
            }
        }
    }
    return -1; // Semáforo não encontrado
}

/**
 * @brief Realiza uma operação em um semáforo identificado por semid, com base na operação op.
 * @param semid Identificador do semáforo.
 * @param op Operação a ser realizada (positiva para up, negativa para down).
 * @return Resultado da operação ou -1 em caso de erro.
 */
PUBLIC int sys_semop(int semid, int op) {
    if (op > 0)
        return up(semid); // Incrementa o semáforo
    
    if (op < 0)
        return down(semid); // Decrementa o semáforo
    
    return -1; // Operação inválida
}