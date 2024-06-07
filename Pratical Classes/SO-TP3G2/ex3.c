#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main(){


  /*  
  for(int i=1; i<=10 ; i++){
    pid_t pid = fork();
    if(pid == 0){
  

        printf("Sou o filho: %d, o meu pid é: %d\n",i,getpid());

        _exit(-1);

    }else{


        int status;
        pid_t wait_pid = wait(&status);
        printf("PAI: wait_pid:  %d\n",wait_pid);
        if(WIFEXITED(status)){

        }else{
            printf("Erro ao terminar\n");
        }

    }    
   

  }
  */

 //aqui o pai cria 10 filhos e corre os todos ao mesmo tempo

    for(int i=1 ; i<=10 ;i++){
        pid_t pid = fork();
        if(pid == 0){

            printf("Filho: %d\n",i);
            sleep(2);


            _exit(i);

        }else{


            int status;
            pid_t wait_pid = wait(&status);
            printf("PAI: wait_pid:  %d\n",wait_pid);
            
            if(WIFEXITED(status)){
                printf("Valor exit filho: %d\n",WEXITSTATUS(status));


            }else{
                printf("Erro ao terminar\n");
            }

        }  

    }
    


  

   


    return 0;
}