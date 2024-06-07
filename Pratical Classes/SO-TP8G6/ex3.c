#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>


int main(){

    int infd = open("/etc/passwd", O_RDONLY);
    int outfd = open("saida.txt", O_CREAT | O_TRUNC | O_WRONLY,0666);
    int errfd = open("erros.txt", O_CREAT | O_TRUNC | O_WRONLY,0666);

  

    dup2(infd, 0); 
    close(infd);

    dup2(outfd,1); //aponta o stdout para a pasta saida.txt
    close(outfd);

    dup2(errfd,2); //aponta o error para a pasta de erro.txt
    close(errfd);




    execlp("wc", "wc", NULL);



    printf("terminei\n");
    perror("bla");

    return 0;

}