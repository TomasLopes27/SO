#include <stdio.h>
#include <unistd.h>
#include <aio.h>
#include <sys/wait.h>



int main(int agrc, char* argv[]){

    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("erro no fork");
        break;

    case 0:{
        int ret = execlp("ls", "ls", "-l", NULL);
        _exit(ret);
        break;


        
    }
    default:{
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            printf("%d\n", WEXITSTATUS(status));
        }else{
            perror("filho interrompido");
        }
        break;
    }
        
       
    }

  


    return 0;


}