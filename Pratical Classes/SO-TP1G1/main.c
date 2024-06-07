#include <stdio.h>
#include <unistd.h>

int mycat(){
    char buffer[1024];
    ssize_t res_read;
    ssize_t res_write;

    while((res_read = read(0, buffer, 1024))>0){

        ssize_t res_write = write(1, buffer, res_read);

        if(res_read != res_write){
            return 1;
        }
    } 



    write(1,"fim",3);   
    return 0;
    
}

int main(int argc,char* argv[]){
    mycat();

    return 0;
}