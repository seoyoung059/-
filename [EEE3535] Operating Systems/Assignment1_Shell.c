#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define buf_size 128    // Max length of user input
#define max_args 8      // Max number of arguments per process

// Read a shell input.
char* readcmd(char *buf) {
    // Read input from stdin.
    fprintf(stdout, "$ ");
    memset(buf, 0, buf_size);
    char *cmd = fgets(buf, buf_size, stdin);

    // Chop the tailing \n.
    cmd[strlen(cmd)-1] = 0;

    return cmd;
}

int runcmd(char *cmd); //forward declaration

int sing_func(char *cmd){//single function execution
	int f=0;//int to count parsed parts
	char*arg[buf_size]={NULL,};//argument array
	char*argptr=strtok(cmd," ");//parsing argument by space
	while (argptr != NULL){
		arg[f]=argptr;
		f++;
		argptr = strtok(NULL," ");
	}//while loop statement for parsing
	if(execvp(arg[0],arg)==-1){//execvp function for execution
		fprintf(stderr,"Command not found: %s\n",arg[0]);//invalid commands error
		exit(0);//kill fork
	}
	return 0;
}

int serial_func(char *cmd){//serialized commands execution
	int s=0;//int to count serialized block 
	char*ser_arg[20]={NULL,};//serialized block array
	char*ser_ptr=strtok(cmd,";");//parsing argument by ; sign
	while(ser_ptr != NULL){
		ser_arg[s]=ser_ptr;
		s++;
		ser_ptr=strtok(NULL,";");
	}//while loop statement for parsing
	for(int i=0; i<s; i++){//for loop statement to execute serialized argument block
		if(ser_arg[i]==NULL){break;}
		else{
			if(runcmd(ser_arg[i])==-1){return -1;}//if statement return -1, serial function returns -1 to exit
		}
	}
	return 0;
}
	

int pipe_func(char *cmd){//Piped commands execution
	int p=0;//int to count piped command
	char*pipe_arg[10]={NULL,};//piped commands array
	char*pipe_ptr=strtok(cmd,"|");//parsing argument by | sign
	while (pipe_ptr != NULL){
		pipe_arg[p]=pipe_ptr;
		p++;
		pipe_ptr=strtok(NULL,"|");
	}//while loop for parsing
	int pi[(p-1)*2];//fd for pipe
	for(int i=0; i<(p-1);i++){pipe(pi+2*i);}//pipe() func
	for(int i=0; i<p; i++){//for every piped commands
		int pipe_pid=fork();
		if(pipe_pid==0){//child process
			if(!((strlen(cmd)==2 && !strncmp(pipe_arg[i],"cd",2))||!strncmp(pipe_arg[i],"cd ",3)||!strcmp(pipe_arg[i],"exit"))){
				if(i!=0){dup2(pi[(i-1)*2],0);}
				if(i!=(p-1)){dup2(pi[(i*2)+1],1);}
			}//if cd or exit command, do not get input and give output
			for(int j=0;j<2*(p-1);j++){close(pi[j]);}//child closes
			runcmd(pipe_arg[i]);//runcmd to execute commands
			exit(0);//exit process
		}
	}
	for(int i=0;i<2*(p-1);i++){close(pi[i]);}//parent process closes
	for(int i=0;i<p;i++){wait(NULL);}//parent process wait for children
	return 0;
}


// Run a command.
int runcmd(char *cmd) {
    // Remove leading white space(s).
    while(*cmd == ' ') { cmd++; }

    //Serial Commands
    if(strchr(cmd,';')){
	    if(serial_func(cmd)==-1){return -1;}
    }

    //Piped commands
    else if(strchr(cmd,'|')){
	    pipe_func(cmd);
    }

    // cd command
    else if(strlen(cmd)==2 && !strncmp(cmd,"cd",2)){//'cd' command changes directory to home
	    chdir(getenv("HOME"));
    }
    else if(!strncmp(cmd, "cd ", 3)){
	    while(*(cmd+3)==' '){cmd++;}//remove spaces before  directory
	    strtok((cmd+3)," ");//remove spaces after directory
	    if(!strcmp(cmd+3,"~")){chdir(getenv("HOME"));}//~ directory to home directory
	    else if(chdir(cmd+3) < 0) { fprintf(stderr, "Cannot cd %s\n", cmd+3); }//change directory and directory error
    }
    // exit command: return -1 to terminate the while loop in main.
    else if(!strncmp(cmd, "exit ", 5)||(strlen(cmd)==4 && !strcmp(cmd,"exit"))) { return -1; }
    else {
        /* Incomplete shell */
	    int mainpid=fork();//fork function when single command execute
	    if(mainpid==0){sing_func(cmd);}//single command at child process
	    else if(mainpid>0){wait(NULL);}//parent process waits
    }
    return 0;
}

int main(void) {
    int fd = 0;
    char *cmd = 0;
    char buf[buf_size];

    // Ensure three file descriptors are open. Failure will hang the shell.
    while((fd = open("console", O_RDWR)) >= 0) {
        if(fd >= 3) { close(fd); break; }
    }

    fprintf(stdout, "EEE3535 Operating Systems: starting ysh\n");

    // Read and run input commands.
    while((cmd = readcmd(buf))) {
        // Run the input commands.
        if(*cmd && runcmd(cmd)) { break; }
    }

    return 0;
}

