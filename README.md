## Operating Systems | Sistemas Operativos

This project was developed during the second semester of the 2nd year.

In the Operating Systems , we were challenged to create a service where users must use a client program to submit to the server their intention to execute a task, specifying the required duration in milliseconds and the task (i.e., program or set/pipeline of programs) to be executed. Each task is assigned a unique identifier that should be sent to the client as soon as the server receives it. The server is responsible for scheduling and executing the users' tasks. Output produced by the tasks for standard output or standard error should be redirected by the server to a file named after the task's identifier.

## Installing and running the project


#### Compiling
```bash
$ cd SO
$ make
```

#### Running
```bash
$ ./bin/client <time> execute -u "command"
$ ./bin/client <time> execute -p "command1 | command2 | command3"
```
