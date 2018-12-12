#define  _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFFERSIZE 10
char buffer[BUFFERSIZE],*input,*path,*str,*p;
int ff(char *ss,char *ff);
int gg(char *a,char *b);
int is_fileexist(char *comm);

int splitShell(char ** arg,char * str);
int main()
{
    char lc_char;
    char *arg[BUFFERSIZE],li_inputlen[BUFFERSIZE],BUFFER[BUFFERSIZE][BUFFERSIZE];
    int count,num,is_bj=0,is_back,i,j,k,pid,status,mm=0;
    while(1)
    {
        if(mm == 1) break;
        printf("\n%s>$",get_current_dir_name());
        //now get the commands
        memset(li_inputlen,0,sizeof(li_inputlen));
        lc_char = getchar();
        num=0;
        count=0;
        while(lc_char != '\n' && count < BUFFERSIZE)//enter->execute this command
        {
            if(lc_char != ';')//split the commands 
                BUFFER[num][li_inputlen[num] ++] = lc_char;
            else num++;
            lc_char = getchar();
            count++;
        }
        // check the length of command_char
        if(count > BUFFERSIZE)// define a buffer for bin/ less than 10
        {
            printf("define a bufferlength for /usr/bin/ less than 10\n");
            count = 0; //clear the command
            continue;
        }
        else
        {
            for(count = 0; count <= num; count++)
            {
                BUFFER[count][li_inputlen[count]] = '\0';//the buffer_end = '\0',end it
            }
        }
        for(count = 0; count <= num; count++)
        {
            strcpy(buffer,BUFFER[count]);
            char *arr[BUFFERSIZE];
            int shel_count = splitShell(arr,buffer);
            j = 0;
            k = 0;
            str = (char*)malloc(sizeof(char)*10);
            strcpy(str,BUFFER[count]);
            int split = splitShell(arg,str);
            //inner commands
            if(strcmp(arg[0],"bye") == 0)
            {
                printf("shell have quitted,goodbye\n");
                mm=1;
                break;
            }else if(strcmp(arg[0],"down")==0){
                char *halt ="ls";
            	execl("/bin/ls",halt);
            }else if(strcmp(arg[0],"who")==0){
            	printf("1607020223 wangfuyan\n");
            }else if(strcmp(arg[0],"pwd")==0){
            	printf("%s  (from innercommand)\n",get_current_dir_name());
            }
            //outer commands
            else if(is_bj == 0)
            {
                if(is_fileexist(arg[0]) == -1)
                {
                    printf("command not found\n");
                    for(i = 0; i < k; i++)
                        free(arg[i]);
                    continue;
                }
                // printf("%s  %s\n",arg[0],arg[1]);
                if((pid = fork()) == 0){
                	//place a new thread for the followed in recent system environments
                    execv(buffer,arg);// from <unistd.h>
                    is_back=1;
                    continue;
                }
                else if(is_back == 0)
                    waitpid(pid,&status,0);//from <sys/type.h&wait.h>
                for(i = 0; i < k; i++)
                    free(arg[i]);
            }
        }
    }
}


int is_fileexist(char *comm)
{
    char * path, * p;
    int i;
    i = 0;
    path = getenv("PATH");
    p = path;
    while (*p != '\0')
    {
        if (*p != ':')
            buffer[i++] = *p;
        else
        {
            buffer[i++] = '/';
            buffer[i] = '\0';
            strcat(buffer,comm);
            // printf("%s\n", buffer);
            if (access(buffer,F_OK) == 0)// in <stdio.h>
                return 0;
            else
                i = 0;
        }
        p++;
    }
    return -1;
}
int splitShell(char ** arg,char * str){
    int splcnt = 0;
    char *s =NULL;
    s=strtok(str," ");
    while(s != NULL)
    {
       *arg++ = s;
       splcnt ++;
       s = strtok(NULL," ");
    }
    return splcnt;
}