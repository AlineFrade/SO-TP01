#include <nanvix/const.h>
#include <sys/sem.h>

int get_semaphore_value(int semid)
{
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;

            return tabsem[i].value;
        }
    }
    return -1; 
}

int set_semaphore_value(int semid, int val)
{
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;

            tabsem[i].value = val; // Sets the value of the semaphore.
            return 0;
        }
    }
    return -1; 
}

int decrement_and_destroy_semaphore(int semid)
{
    int flag = 0;
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE && tabsem[i].id == semid) {
            // Checks if the process is associated with the semaphore.
            if (valid(&tabsem[i]) == -1)
                return -1;
            for(int j = 0; j < PROC_MAX; j++){
                if(tabsem[i].procaccess[j] != 0){
                    flag++;
                }
                if(tabsem[i].procaccess[j] == curr_proc->pid){
                    tabsem[i].procaccess[j] = 0;
                }
            }

            if (flag > 1) {
                return 0; 
            }else{
                tabsem[i].state = INACTIVE;
            }
        }
    }
    return -1; 
}

PUBLIC int sys_semctl(int semid, int cmd, int val)
{
    
    switch (cmd) {
        case GETVAL:
            return get_semaphore_value(semid); // Gets the current value of the semaphore.
        case SETVAL:
            return set_semaphore_value(semid, val); // Sets the value of the semaphore.
        case IPC_RMID:
            return decrement_and_destroy_semaphore(semid); // Destroys the semaphore if possible.
    }

    return -1; 
}