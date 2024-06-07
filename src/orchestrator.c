#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "defs.h"
#include <errno.h>

#define server_to_client "/tmp/server_to_client"
#define client_to_server "/tmp/client_to_server"
#define MAX_TASKS 10

typedef struct
{
    program tasks[MAX_TASKS];
    int count;
} TaskQueue;

TaskQueue task_queue; // fila de tarefas

char **parse(char *string)
{
    char *token = strtok(string, " ");
    char **strings = NULL;
    int i = 0, num_strings = 1;
    while (token != NULL)
    {
        strings = (char **)realloc(strings, num_strings * sizeof(char *));
        strings[i] = token;
        token = strtok(NULL, " ");
        i++, num_strings++;
    }
    return strings;
}

int parser(char **exec_args, char *str) {
    char *comando = strdup(str);
    if (!comando) {
        perror("Failed to allocate memory for comando");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    char *string = strsep(&comando, " ");
    while (string) {
        if (strcmp("", string) != 0) {
            exec_args[i++] = string;
        }
        string = strsep(&comando, " ");
    }
    exec_args[i] = NULL;
    return i;
}

int pipeline(char **commands, int nc) {
    int pipefds[2 * (nc - 1)]; // array pra guardar os descritores do pai
    int pid;
    int status;

    // Create pipes
    for (int i = 0; i < nc - 1; i++) {
        if (pipe(pipefds + i*2) < 0) {
            perror("Failed to create pipe");
            exit(EXIT_FAILURE);
        } 
    }

    for (int i = 0; i < nc; i++) {
        pid = fork();
        if (pid == 0) { // filho
            // se n for o ultimo comando rederecionar o input para o proximo pipe
            if (i > 0) {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0) {
                    perror("dup2 input");
                    _exit(EXIT_FAILURE);
                }
            }
            // se n for o ultimo comando redirecionar para o proximo pipe
            if (i < nc - 1) {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0) {
                    perror("dup2 output");
                    _exit(EXIT_FAILURE);
                }
            }

            // Cfechar todos os pipes fds
            for (int j = 0; j < 2 * (nc - 1); j++) {
                close(pipefds[j]);
            }

            // Parse dos argumentos comando
            char *exec_args[20]; // aumentar tamanho se necessaro
            parser(exec_args, commands[i]);
            execvp(exec_args[0], exec_args);
            perror("execvp failed");
            _exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("Failed to fork");
            exit(EXIT_FAILURE);
        }
    }

    // fechar todos os pipes no pai
    for (int i = 0; i < 2 * (nc - 1); i++) {
        close(pipefds[i]);
    }


    while ((pid = wait(&status)) > 0) {
    }

    return status;
}
int parse_pipeline(char **args, char *cmd_str) {
    char *token;
    token = strtok(cmd_str, "|");
    int i = 0;
    while (token != NULL) {
        args[i] = token;
        token = strtok(NULL, "|");
        i++;
    }
    return i; 
}

// Função para adicionar uma tarefa à fila
void add_task(program task)
{
    if (task_queue.count < MAX_TASKS)
    {
        task_queue.tasks[task_queue.count++] = task;
    }
    else
    {
        printf("A fila de tarefas está cheia.\n");
    }
}

// Função para remover e retornar a próxima tarefa da fila
program pop_task()
{
    if (task_queue.count > 0)
    {
        program task = task_queue.tasks[0];
        for (int i = 0; i < task_queue.count - 1; i++)
        {
            task_queue.tasks[i] = task_queue.tasks[i + 1];
        }
        task_queue.count--;
        return task;
    }
    else
    {
        program empty_task; // Retorna uma tarefa vazia se a fila estiver vazia
        empty_task.time = -1;
        return empty_task;
    }
}

int mysystem(program p)
{

    // Parsing do comando
    char *comando;
    comando = strdup(p.program);
    int i = 0;
    char *string;
    char *exec_args[20];
    string = strsep(&comando, " ");
    while (string != NULL)
    {
        exec_args[i] = string;
        string = strsep(&comando, " ");
        i++;
    }
    exec_args[i] = NULL;

    int status;

    pid_t pid = fork();
    // Executar o comando
    if (pid == 0)
    {
        execvp(exec_args[0], exec_args);
        perror("Erro ao executar comando");
        _exit(EXIT_FAILURE);
    }
    else
    {
        wait(&status);
        p.pid = pid;
    }
}

int main()
{
    
    int outfd = open("saida.txt", O_CREAT | O_APPEND| O_WRONLY, 0666);
    dup2(outfd, 1); // Redireciona stdout para o arquivo saida.txt

    int errfd = open("erros.txt", O_CREAT | O_APPEND | O_WRONLY,0666);
    dup2(errfd,2); //aponta o error para a pasta de erro.txt

    

    // Inicializar a fila de tarefas
    task_queue.count = 0;

    program task;

    // Criar o FIFO principal (MainFifo) se não existir
    if (mkfifo(server_to_client, 0666) == -1 && errno != EEXIST)
    {
        perror("erro no server to client");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(client_to_server, 0666) == -1 && errno != EEXIST)
    {
        perror("erro no client to server");
        exit(EXIT_FAILURE);
    }

    // Abrir o FIFO do cliente para leitura
    int fd_client_to_server = open(client_to_server, O_RDONLY);
    if (fd_client_to_server < 0)
    {
        perror("Erro ao abrir o FIFO de entrada do cliente");
        exit(EXIT_FAILURE);
    }




    // Loop para receber e processar as tarefas
    size_t bytes_read;
    int server_to_client_fd = -1;

    while (1)
    {
        ssize_t bytes_read = read(fd_client_to_server, &task, sizeof(task));
        if(bytes_read >0){
        if (bytes_read < 0)
        {
            perror("Erro ao ler do FIFO do servidor");
            exit(EXIT_FAILURE);
        }

        if (server_to_client_fd == -1)
        {
            server_to_client_fd = open(server_to_client, O_WRONLY);
            if (server_to_client_fd == -1)
            {
                perror("erro a abrir o server to client");
                exit(EXIT_FAILURE);
            }
        }
        // Adicionar a tarefa recebida à fila
        add_task(task);

        // Executar as tarefas da fila
        while (task_queue.count > 0)
        {
            program next_task = pop_task();

            if (strcmp(next_task.type, "-u") == 0)
            {
                struct timeval time;
                gettimeofday(&time, NULL);
                task.ms = time.tv_usec;
                task.sec = time.tv_sec;

                mysystem(next_task);

                gettimeofday(&time, NULL);
                long ms = ((task.sec - time.tv_sec) * 1000) + ((task.ms - time.tv_usec) / 1000);
            }
            else if (strcmp(next_task.type, "-p") == 0)
            {
                struct timeval time;
                gettimeofday(&time, NULL);
                task.ms = time.tv_usec;
                task.sec = time.tv_sec;
                
                char *args[100];

                int tamanho = parse_pipeline(args, next_task.program);

                fflush(stdout);
                pipeline(args, tamanho);

                gettimeofday(&time, NULL);
                long ms = ((task.sec - time.tv_sec) * 1000) + ((task.ms - time.tv_usec) / 1000);
            }
            // Enviar mensagem de confirmação para o cliente
            char response_message[20];
            sprintf(response_message, "%d", next_task.pid);
            ssize_t bytes_written = write(server_to_client_fd, response_message, strlen(response_message) + 1);
            if (bytes_written < 0)
            {
                perror("Erro ao escrever no FIFO de saída do servidor");
                exit(EXIT_FAILURE);
            }
        }

        // bytes_read = read(fd_client_to_server, &task, sizeof(task));

        // Fechar o FIFO do cliente após processar cada tarefa
        // close(fd_server_to_client);
        }
    }

    // Fechar o FIFO do cliente após processar todas as tarefas
    close(server_to_client_fd);
    close(fd_client_to_server);

    // Remover o FIFO principal (MainFifo)
    unlink(server_to_client);
    unlink(client_to_server);

    return 0;
}