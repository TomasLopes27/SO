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

void lookupNumber(int** matrix, int value, int* vector){
    

    int pipefd[2];
    pipe(pipefd);

        for(int i = 0; i < ROWS; i++){
            pid_t pid = fork();
               if(pid == 0){
                Minfo m;
                m.ocur_nr = 0;
                m.line_nr = i;

                close(pipefd[0]);//o filho n precisa de ler
                    for(int j = 0; j < COLUMNS; j++){
                        if(value == matrix[i][j]){
                            m.ocur_nr++;     
                        }

                    }
                    write(pipefd[1], &m,sizeof(m));
                    close(pipefd[1]);
                    _exit(0);
            
                }   
        }

        close(pipefd[1]);//o pai n precisa escrever

        Minfo res;
        while(read(pipefd[0], &res, sizeof(res)>0)){
            vector[res.line_nr] = res.ocur_nr;

        }

        for(int i = 0 ; i < ROWS ;i++){
            wait(NULL);
         
        }
        close(pipefd[0]);  
}

    //Hint - use the Minfo struct from matrix.h!
    

