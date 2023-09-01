#include <nanvix/pm.h>
#include <sys/types.h>

int sys_get_process_info(pid_t pid, struct process_buf *buf){
    do_get_process_info(pid_t pid, struct process_buf *buf);
}