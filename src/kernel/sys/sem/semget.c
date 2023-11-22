#include <nanvix/const.h>
#include <sys/sem.h>

int associate_process_with_semaphore(struct sem *sem)
{
    for(int i = 0; i < PROC_MAX; i++){
        if(sem->procaccess[i] == 0){
            sem->procaccess[i] = curr_proc->pid;
            return sem->id;
        }
    }
   
    return -1;
}

int create_new_semaphore(unsigned key)
{
    struct sem *new_semaphore = NULL;

    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == INACTIVE) {
            new_semaphore = &tabsem[i];
            break;
        }
    }

    if (new_semaphore == NULL)
        return -1;
    
    new_semaphore->key = key;
    new_semaphore->value = 1;
    new_semaphore->state = ACTIVE;
    
    int ret = associate_process_with_semaphore(new_semaphore); 
    
    return ret;
        
}

PUBLIC int sys_semget(unsigned key)
{   
    for (int i = 0; i < SEM_MAX; i++) {
        if (tabsem[i].state == ACTIVE && tabsem[i].key == key) {
            return associate_process_with_semaphore(&tabsem[i]);
        }
    }

    // Creates a semaphore if not found.
    return create_new_semaphore(key);
}
