#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int main(){

    int infd = open("/etc/passwd", O_RDONLY);
    int outfd = open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY,0666);
    int errfd = open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY,0666);

    int fdi = dup(0);
    int fdo = dup(1);
    int fde = dup(2);

    dup2(infd, 0); 
    close(infd);

    dup2(outfd,1); //aponta o stdout para a pasta saida.txt
    close(outfd);

    dup2(errfd,2); //aponta o error para a pasta de erro.txt
    close(errfd);


    char buffer[1024];
    int read_res;
    while((read_res = read(0,buffer,1024))>0){
        write(1, buffer, read_res);
        write(2, buffer, read_res);
    }

    dup2(fdi,0); //volta a apontar o stdin para o 0, guardamos o valor do stdi no fdi
    dup2(fdo,1);
    dup2(fde,2);

    printf("terminei\n");
    perror("bla");

    return 0;

}