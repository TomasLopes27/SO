#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "defs.h"

#define server_to_client "/tmp/server_to_client"
#define client_to_server "/tmp/client_to_server"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s execute <time> -u '<prog-a> [args]' \n", argv[0]);
    }


    

    // Popula a struct
    program task;
    task.time = atoi(argv[2]);
    strncpy(task.type, argv[3], sizeof(task.type));    
    task.pid = getpid();
    strcpy(task.program, argv[4]);

    // Abrir o FIFO do servidor para escrita
    int fd_client_to_server = open(client_to_server, O_WRONLY);
    if (fd_client_to_server < 0) {
        perror("Erro ao abrir o FIFO de escrita do servidor");
        return -1;
    }


    // Escrever a struct no FIFO
    if (write(fd_client_to_server, &task, sizeof(task)) < 0) {
        perror("Erro ao escrever no FIFO do servidor");
        return -1;
    }

    // Fechar o FIFO de escrita do servidor
    close(fd_client_to_server);

    int fd_server_to_client= open(server_to_client, O_RDONLY);
    if (fd_server_to_client < 0) {
        perror("Erro ao abrir o FIFO de escrita do servidor");
        return -1;
    }

    char task_id[256];
    if(read(fd_server_to_client, task_id, sizeof(task_id))==-1){
        perror("erro id");
        close(server_to_client);
        return -1;
    }
    close(server_to_client);
    printf("Task ID: %s\n", task_id);

    printf("Tarefa submetida com sucesso.\n");

    return 0;
}