#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/klib.h>
#include <nanvix/pm.h>
#include <sys/sem.h>

PUBLIC void down_sem(struct semaphore *sem, int semid){

    if (sem->current_value > 0)
        semtab[semid].current_value--;
    else
        sleep(curr_proc->chain, PRIO_USER);

}

PUBLIC void up_sem(struct semaphore *sem, int semid){

    if (sem->current_value == 0 && sem->current_value < sem->value){
        wakeup(curr_proc->chain);
        semtab[semid].current_value++;
    } else if (sem->current_value > 0 && sem->current_value < sem->value)
        semtab[semid].current_value++;

}

PUBLIC int sys_semop(int semid, int op){

    struct semaphore *sem;

    //Searching for semid
    for (sem = SEM_FIRST; sem < SEM_LAST; sem++){
        if (sem->id == semid){
            if (op < 0) {
                down_sem(sem, semid);
            } else if (op > 0) {
                up_sem(sem, semid);
            }
        }
    }
    return 0;
}