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

int main()
{
    char lc_char;
    char *arg[BUFFERSIZE],li_inputlen[BUFFERSIZE],BUFFER[BUFFERSIZE][BUFFERSIZE];
    int count,num,is_bj=0,is_back,i,j,k,pid,status,mm=0;
    while(1)
    {
        if(mm == 1) break;
        printf("You must put your document in these path:\n");
        p = getenv("PATH");
        printf("\n%s\n",p);
        path=get_current_dir_name();
        printf("\n%s>$",path);
        memset(li_inputlen,0,sizeof(li_inputlen));
        lc_char = getchar();
        num=0;
        count=0;
        while(lc_char != '\n' && count < BUFFERSIZE)
        {
            if(lc_char != ';')
                BUFFER[num][li_inputlen[num] ++] = lc_char;
            else num++;
            lc_char = getchar();
            count++;
        }
        printf("%d\n",count);
        if(count > BUFFERSIZE)
        {
            printf("Your command too long ! Please reenter your command !\n");
            count = 0;
            continue;
        }
        else
        {
            for(count = 0; count <= num; count++)
            {
                BUFFER[count][li_inputlen[count]] = '\0';
            }
        }

        for(count = 0; count <= num; count++)
        {

            strcpy(buffer,BUFFER[count]);
            j = 0;
            k = 0;
            str = (char*)malloc(sizeof(char)*10);
            strcpy(str,BUFFER[count]);
            for(i = 0; i <= li_inputlen[count]; i++)
            {
                if(str[i] == ' ' || str[i] == '\t' || str[i] == '\0')
                {
                    if(j == 0) continue;
                    else
                    {
                        buffer[j++] = '\0';
                        arg[k] = (char*)malloc(sizeof(char)*j);
                        strcpy(arg[k],buffer);
                        k++;
                    }
                }
                else
                {
                    if(str[i] == '&' && str[i+1] == '\0')
                    {
                        is_back = 1;
                        continue;
                    }
                    buffer[j++] = str[i];
                }
            }
            free(str);
            if(strcmp(arg[0],"bye") == 0 || strcmp(arg[0],"exit") ==0)
            {
                printf("see later,luck!\n");
                mm=1;
                break;
            }

            if(is_bj == 0)
            {
                arg[k] = (char*)0;
                if(is_fileexist(arg[0]) == -1)
                {
                    printf("This command is not found!\n");
                    for(i = 0; i < k; i++)
                        free(arg[i]);
                    continue;
                }
                if((pid = fork()) == 0)
                    execv(buffer,arg);
                else if(is_back == 0)
                    waitpid(pid,&status,0);
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
            if (access(buffer,F_OK) == 0)
                return 0;
            else
                i = 0;
        }
        p++;
    }
    return -1;
}
