#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

int cp(char* origem, char* destino){
    char buffer[BUFFER_SIZE];
    ssize_t res_read;
    ssize_t res_write;

    int fd_origem = open(origem, O_RDONLY, 0640);
    int fd_destino = open(destino, O_WRONLY | O_CREAT , 0640);
    if(fd_destino<0){
        printf("erro\n"); 
    }

    while((res_read = read(fd_origem, buffer, BUFFER_SIZE))>0){
        ssize_t res_write = write(fd_destino, buffer , res_read);

    } 


    return 0;

    close(fd_origem);
    close(fd_destino);
    
}

int main(int argc,char* argv[]){
    mycp(argv[1],argv[2]);

    return 0;
}