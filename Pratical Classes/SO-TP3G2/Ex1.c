#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){

  

    pid_t pid = fork();

  

    if(pid == 0){
  

        
        printf("FILHO o meu pid: %d\n",getpid());
        printf("FILHO pid do meu pai: %d\n",getppid());


        _exit(-1);

    }
    
    
    
        printf("PAI o meu pid: %d\n",getpid());
        printf("PAI pid do meu pai: %d\n",getppid());

        int status;
        pid_t wait_pid = wait(&status);

        printf("wait_pid:  %d\n",wait_pid);
        if(WIFEXITED(status)){
            printf("Valor exit filho: %d\n",WEXITSTATUS(status));

        }else{
            printf("Erro ao terminar\n");
        }
   


  

   


    return 0;
}