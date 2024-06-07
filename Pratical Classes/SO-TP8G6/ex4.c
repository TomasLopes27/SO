#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>


int main(){
        int pipefd[2];
        pipe(pipefd);

    pid_t pid = fork();
    if(pid ==0){ //filho
        close(pipefd[1]);//fecha o extremo de escrita do pipe

        dup2(pipefd[0],0);
        close(pipefd[0]);


        execlp("wc", "wc", NULL);
        if(execlp("wc", "wc", NULL)== -1){
            perror("erro no exec");
        }
        _exit(0);
        
    }else{ //pai
        char buffer[1024];
        int read_res;
        while((read_res = read(0,buffer,1024))>0){
            write(pipefd[1], buffer, read_res); 
        }
        close(pipefd[1]); 
        wait(NULL);
    

        int status;
            wait(&status);
            if(WIFEXITED(status)){
                printf("filho terminou");
            }else{
                perror("filho interrompido");
            }

        }

    return 0;

}