#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>

int main(){

   int fd = open("fifo", O_RDONLY);
   if(fd < 0){
    perror("erro open");
   }


   printf("open writefifo\n");

    char buffer[1024];
    int res;

    while((res = read(fd, buffer, 1024)) > 0){
        write(1, buffer, res);


    }
   //unlink("fifo");
return 0;


}