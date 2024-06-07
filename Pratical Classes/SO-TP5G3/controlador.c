#include <stdio.h>
#include "mysystem.h"

//neste exercicio criamos 3 filhos que vao correr um comando
//e enquanto n der 0 vai incrementar

//este exericio tem um problema pq se o filho correr
//mais de 255 vezes o exit so representa 8 bits ou seja
//vai dar um valor sempre entre 0 255, ex se correr 256,
//o exit vai retornar 1

void controller(int N, char** commands) {
    int pids[N];
    int fork_ret, status;

    for(int i = 0; i < N; i++){
        int aux = 0;
        fork_ret= fork();


        if(fork_ret == 0){
		    while (mysystem(commands[i])!= 0){
			    aux++;
		    }
		    _exit(aux);

        }else{
            pids[i]=fork_ret;//adiciona o pid ao array de pids

        }
    }
    //espera por todos os filhos
    for(int i = 0; i < N; i++){   
        waitpid(pids[i], &status, 0); 

        if(WIFEXITED(status)){
            printf("Comand %s executed %d times\n", commands[i], WEXITSTATUS(status));
        }else{
            printf("Monitor for commands %s was interropted",commands[i]);
        }

    }
}
	
    


int main(int argc, char* argv[]) {

	char *commands[argc-1];
	int N = 0;
	for(int i=1; i < argc; i++){
		commands[N] = strdup(argv[i]);
		printf("command[%d] = %s\n", N, commands[N]);
		N++;
	}

	controller(N, commands);

	return 0;
}