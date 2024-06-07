#include "matrix.h"


int **createMatrix() {

    // seed random numbers
    srand(time(NULL));

    // Allocate and populate matrix with random numbers.
    printf("Generating numbers from 0 to %d...", MAX_RAND);
    int **matrix = (int **) malloc(sizeof(int*) * ROWS);
    for (int i = 0; i < ROWS; i++) {
        matrix[i] = (int*) malloc(sizeof(int) * COLUMNS);
        for (int j = 0; j < COLUMNS; j++) {
            matrix[i][j] = rand() % MAX_RAND;
        }
    }
    printf("Done.\n");

    return matrix;
}

void printMatrix(int **matrix) {

    for (int i = 0; i < ROWS; i++) {
        printf("%2d | ", i);
        for (int j = 0; j < COLUMNS; j++) {
            printf("%7d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// ex.5
int valueExists(int **matrix, int value) {
    
        for(int i = 0; i < ROWS; i++){
            pid_t pid = fork();
               if(pid == 0){
                    for(int j = 0; j < COLUMNS; j++){
                        if(value == matrix[i][j]){
                            _exit(1);
                        }

                    }
                    _exit(0);
            
                }    
        }
    for(int i = 0 ; i < ROWS ;i++){

        int status;  
        pid_t wait_pid = wait(&status);
        //printf("PAI: wait_pid:  %d\n",wait_pid);
        //printf("%d\n",WEXITSTATUS(status));    
        if(WEXITSTATUS(status) == 1){
            return 0;

        }
    }    
    return 1;
}


// ex.6 crio um array onde o pai guarda os pids dos filhos num array por ordem

void linesWithValue(int **matrix, int value) {

    int pids[ROWS];

    for(int i = 0; i < ROWS ; i++){
        pid_t pid = fork();

        if(pid == 0){
            for(int j = 0; j < COLUMNS ; j++){
                if(value == matrix[i][j]){
                    _exit(1);
                }
            }
            _exit(0);
        }else{
            pids[i]=pid;
        }
   
    }
    for(int i = 0 ; i < ROWS ;i++){
        int status;  
        pid_t wait_pid = waitpid(pids[i], &status,0);

        //printf("%d\n",WEXITSTATUS(status));    
        if(WEXITSTATUS(status) == 1){
            printf("Encontrei o valor %d na linha %d \n",value,i);
            

        }
        else{
            printf("Nao encontrei o valor %d na linha %d \n",value,i);
        }
    } 
    

    
}
