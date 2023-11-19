#include <sys/sem.h>

PUBLIC struct sem tabsem[SEM_MAX];

void sem_init()
{
    // Initializes the semaphore table.
    for (int i = 0; i < SEM_MAX; i++) {
        tabsem[i].state = INACTIVE; // Sets the semaphore state to inactive.
        tabsem[i].procusing = 0;    // Initializes the count of processes using the semaphore to zero.
        tabsem[i].pos = i;          // Sets the position of the semaphore in the table.
        tabsem[i].id = i;           // Sets the identifier of the semaphore.
    }
    // Calculates the number of semaphore tables needed and initializes them in the current process.
    int table_num = SEM_MAX / 16;
    for (int i = 0; i < table_num; i++)
        curr_proc->shared_sem[i] = 0;
}

/**
 *  @brief Checks if the process is associated with
 *  the semaphore.
 *
 *  @param sem The semaphore to be checked.
 *  @return 0 if the process is associated with the semaphore, -1 otherwise.
 */
int valid(struct sem *sem)
{
    int table = sem->pos / 16;  // Calculates the table index for the semaphore.
    int pos_table = sem->pos % 16; // Calculates the position within the table.
    int comp = 1;
    
    comp = comp << pos_table; // Shifts the bit corresponding to the semaphore position.

    int *b = &curr_proc->shared_sem[table]; // Points to the shared semaphore table in the current process.
    
    comp = *b & comp; // Performs a bitwise AND operation.

    if (comp == 0) {
        return -1; // Indicates that the process is not associated with the semaphore.
    }

    return 0; // Indicates that the process is associated with the semaphore.
}
