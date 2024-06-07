#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

void filho_to_pai(){
int pipefd[2];
    int ret = pipe(pipefd); //[0]->ler [1]->escrever
    if (ret < 0){
        perror("erro a criar pipe");
    }
    
    if(fork()==0){
        //filho
        int valor = 10;
        close(pipefd[0]);

        write(pipefd[1], &valor, sizeof(int));
        close(pipefd[1]);

        _exit(0);
        
    }else{
        //pai
        int res;
        close(pipefd[1]);

        read(pipefd[0], &res, sizeof(int));
        close(pipefd[0]);

        printf("Pai -> Recebi %d\n", res);

        int status;
        wait(&status);
        if (WIFEXITED(status)){
            printf("filho terminou\n");
        }else{
            printf("filho não terminou\n");
        }
        
    }
}

void pai_to_filho(){
    int pipefd[2];
    int ret = pipe(pipefd); //[0]->ler [1]->escrever
    if (ret < 0){
        perror("erro a criar pipe");
    }
    
    if(fork()==0){
        //filho
        int res;
        close(pipefd[1]);

        read(pipefd[0], &res, sizeof(int));
        close(pipefd[0]);

        printf("Filho -> Recebi %d\n", res);

        _exit(0);
    }else{
        //pai
        int valor = 10;
        close(pipefd[0]);

        write(pipefd[1], &valor, sizeof(int));
        close(pipefd[1]);

        int status;
        wait(&status);
        if (WIFEXITED(status)){
            printf("filho terminou\n");
        }else{
            printf("filho não terminou\n");
        }
        
    }
}

int main(){

    //pai_to_filho();
    filho_to_pai2();
    return 0;
}