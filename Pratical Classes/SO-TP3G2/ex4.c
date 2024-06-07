#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//neste exemplo primeiro o pai cria os filhos primeiro e so depois é que o pai vai correr cada filho um
//de cada vez esperando por eles 1 por 1

   int main(){

    for(int i=1 ; i<=10 ;i++){
        pid_t pid = fork();
        if(pid == 0){

            printf("Filho: %d\n",i);
            sleep(1);


            _exit(i);
        }        

    }
    for(int i=1 ; i<=10 ;i++){


        int status;     
        pid_t wait_pid = wait(&status);
        printf("PAI: wait_pid:  %d\n",wait_pid);
            
        if(WIFEXITED(status)){
            printf("Valor exit filho: %d\n",WEXITSTATUS(status));


        }else{
            printf("Erro ao terminar\n");
        }

    }  

    
    
   


    return 0;
}
