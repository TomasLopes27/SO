#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

int main(){

   int fd = open("fifo", O_WRONLY);
   if(fd < 0){
    perror("erro open");
   }

    printf("open writefifo\n");


    char buffer[1024];
    int res;

    while((res = read(0, buffer, 1024)) > 0){
        write(fd, buffer, res);


    }
    close(fd);

    return 0;
}