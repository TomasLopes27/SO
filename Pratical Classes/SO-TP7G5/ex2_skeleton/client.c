#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"

int main (int argc, char * argv[]) {

    if (argc < 2) {
        printf("Missing argument.\n");
        exit(1);
    }

    // Popula a struct Msg
    Msg m;
    m.needle = atoi(argv[1]);
    m.pid = getpid();
    m.occurrences = 0;

    // Cria e abre o FIFO do cliente
    char fifoc_name[30];
    sprintf(fifoc_name, CLIENT "%d", m.pid);
    if (mkfifo(fifoc_name, 0666) == -1) {
        perror("mkfifo client");
        return -1;
    }

    int FDS = open(SERVER, O_WRONLY);
    if (FDS < 0) {
        perror("openaa");
        return -1;
    }

    // Escreve a mensagem no FIFO do servidor
    write(FDS, &m, sizeof(m));
    close(FDS);

    // Abre o FIFO do cliente para leitura
    int fdc = open(fifoc_name, O_RDONLY);
    if (fdc < 0) {
        perror("open leitura");
        return -1;
    }

    // Lê a resposta do servidor
    if (read(fdc, &m, sizeof(m)) == -1) {
        perror("read");
        return -1;
    }

    printf("Número de ocorrências: %d\n", m.occurrences);

    // Fecha o FIFO do cliente
    close(fdc);

    // Remove o FIFO do cliente
    unlink(fifoc_name);

    return 0;
}
