#include <nanvix/pm.h>
#include <sys/types.h>

// Implementação da função sys_get_process_info com os parâmetros pid e buf
int sys_get_process_info(pid_t pid, struct process_buf *buf) {
    // Chama a função do_get_process_info para obter informações do processo com o ID 'pid' e armazená-las em 'buf'
    do_get_process_info(pid, buf);
    return 0;
}
