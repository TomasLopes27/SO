#include <stdio.h>
#include <unistd.h>


int main(int agrc, char* argv[]){

    char *arr[3] = {"ls","-l", NULL};

    //int ret = execl("/bin/ls", "ls", "-l", NULL);
    //int ret = execlp("ls", "ls", "-l", NULL);

    int ret = execv("/bin/ls", arr);
    //int ret = execvp("ls", arr);


    if (ret ==-1 ){
        perror("erro exec");
    }

    printf("%s\n",argv[0]);


    return 0;


}