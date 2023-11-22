#include <sys/sem.h>
#include <nanvix/config.h>


PUBLIC struct sem tabsem[SEM_MAX];

void sem_init()
{
    // Initializes the semaphore table.
    for (int i = 0; i < SEM_MAX; i++) {
        tabsem[i].state = INACTIVE; // Sets the semaphore state to inactive.
        tabsem[i].id = i;           // Sets the identifier of the semaphore.
        for(int j = 0; j < PROC_MAX; j++){
            tabsem[i].procaccess[j] = 0;  
        }
    }
    
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
    for(int i = 0; i < PROC_MAX; i++){
        if(sem->procaccess[i] == curr_proc->pid){
            return 0;
        }
    }
    return -1;
}
