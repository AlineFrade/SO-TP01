## Semget:

Esta função invoca `semget` para transferir para o espaço do kernel e valida os parâmetros: o valor deve estar dentro do intervalo de IDs permitidos para semáforos, situando-se entre 0 e `SEM_MAX`.

## Semop:

Realiza chamadas a `semop` para transferir para o espaço do kernel e valida os parâmetros: `semid` deve estar no intervalo dos IDs de semáforos, enquanto `op` pode ser qualquer número positivo ou negativo, exceto 0.

## Semctl:

Utiliza `semctl` para transferir para o espaço do kernel e valida os parâmetros: `semid` deve estar dentro do intervalo de valores permitidos para semáforos; `cmd` precisa ser um dos valores 0, 1 ou 3; `val` pode ser qualquer número maior que 0.

### Chamadas do Sistema na Implementação do Semáforo

O arquivo `sem.h` contém a estrutura do semáforo e a tabela que mantém todos os semáforos, definindo um número máximo de semáforos, por exemplo, 10, mas modificável em `SEM_MAX`.

### Chamadas do Sistema

#### Semget

Recebe um número inteiro como chave e procura na tabela de semáforos por essa chave. Se encontrar, retorna o valor correspondente; caso contrário, cria um novo semáforo com essa chave.

#### Semctl

É responsável pelo controle do semáforo, recebendo o ID do semáforo, a operação a ser realizada (`cmd`) e um valor. Os comandos possíveis são `GETVAL`, `SETVAL` e `IPC_RMID`.

#### Semop

Executa operações de incremento e decremento do semáforo, realizadas pelas funções `up_sem()` e `down_sem()`.

### Função para Leitores e Escritores da `semaphore_test4`

Demonstra um cenário simples de comunicação entre processos usando semáforos. Cria um buffer e dois processos: um leitor e um escritor. Os semáforos são utilizados para controlar o acesso ao buffer compartilhado e sincronizar as operações entre os processos.

### Retorno

Retorna 0 em caso de execução bem-sucedida e -1 em caso de erro.

## Depuração

### Criar Arquivo de Buffer

Abre ou cria um arquivo chamado "buffer" com permissões de leitura e escrita. Retorna -1 em caso de erro.

### Criar Semáforos

Cria três semáforos: `mutex`, `empty` e `full`.

### Inicializar Semáforos

Configura os semáforos: `full` para 0, `empty` para o tamanho do buffer e `mutex` para 1.

### Criar Processo Filho

Cria um novo processo usando `fork`.

### Processos Leitor e Escritor

O processo pai representa o escritor e o processo filho representa o leitor. Semáforos são usados para controlar o acesso ao buffer e sincronizar as operações.

### Destruir Semáforos

Desaloca os semáforos criados.

### Fechar e Limpar

Fecha o arquivo de buffer e o exclui.

### Retornar Sucesso

Retorna 0 para indicar uma execução bem-sucedida.