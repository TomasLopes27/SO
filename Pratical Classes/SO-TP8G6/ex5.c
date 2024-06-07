#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>


int main(){
        int pipefd[2];
        pipe(pipefd);

    pid_t pid = fork();
    if(pid == 0){ //filho

        close(pipefd[1]);//fecha o extremo de escrita do pipe
        dup2(pipefd[0],0);
        close(pipefd[0]);


        execlp("wc", "wc","-l", NULL);
        if(execlp("wc", "wc","-l", NULL)== -1){
            perror("erro no exec");
        }
        _exit(0);
        
    }else{ //pai
        close(pipefd[0]);
        dup2(pipefd[1],1);
        close(pipefd[1]);

        execlp("ls","ls","/etc",NULL);
 
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