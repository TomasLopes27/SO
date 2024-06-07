#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "vector.h" // Inclua o cabeçalho "vector.h" se necessário

int main (int argc, char * argv[]) {
    // Inicializa o vetor, se necessário
    init_vector();
    print_vector();

    if (mkfifo(SERVER, 0666) == -1) {
        perror("mkfifo server");
        return -1;
    }

    while (1) {
        int fds;
        Msg m;
        if ((fds = open(SERVER, O_RDONLY)) == -1) {
            perror("open server fifo");
            return -1;
        }

        int byte;
        while ((byte = read(fds, &m, sizeof(m))) > 0) {
            // Processa a mensagem (não implementado)
            // Aqui você deve contar as ocorrências de m.needle no vetor
            // e atualizar m.occurrences com o número de ocorrências

            // Escreve a resposta no FIFO do cliente
            int fdc = open(CLIENT, O_WRONLY);
            if (fdc == -1) {
                perror("open client fifo for writing");
                break;
            } else {
                write(fdc, &m, sizeof(m));
                close(fdc);
            }
        }

        close(fds);
    }

    return 0;
}
