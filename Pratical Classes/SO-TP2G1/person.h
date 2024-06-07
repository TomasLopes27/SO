#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#define FILENAME "file_pessoas"



typedef struct person{
    char name[100];
    int age;
}Person;


int add_person(char* name, int age);

int list_person(int nr);

int change_agev1(char* name, int newage);
int change_agev2(long pos, int newage);//fazer este exercicio
//ideia desta funçao mudar a idade do registo (ex:0,1,3,4) o pos
//equivale ao registo, implementar com Lseeks andando na memoria
//