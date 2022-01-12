#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define COLOR_RESET  "\x1B[0m"
int main()
{
    char hostname[50];
    char line[50];
    char path[1000];
    char *temp, *command;
    char *args[50];
    int pid;
    gethostname(hostname, sizeof(hostname));
    getcwd(path, sizeof(path));
    while(true)
    {
        printf("%s%s@%s:%s%s %s$ ",GREEN, getenv("USER"), hostname,BLUE,path, COLOR_RESET);
        fgets(line, sizeof(line), stdin);
        if(strcmp(line,"exit\n")==0)
        {
            exit(EXIT_SUCCESS);	
        }
        else
        {
            if (strcmp(line,"help\n")==0)
            {
                printf("%sAutor: ZOFIA BACZYK%s\n",MAGENTA,COLOR_RESET);
                printf("%sKomendy: cd, clear, help, mkdir, touch, whoami%s\n",MAGENTA,COLOR_RESET);
                printf("%sObsluga komend nie zaimplementowanych%s\n",MAGENTA,COLOR_RESET);
            }
            else
            {
                line[strlen(line)-1]='\0';
				temp=strtok(line, " ");
				int i=0;
				while(temp!=NULL)
				{
					args[i]=temp;
					i++;
					temp=strtok(NULL, " ");
				}
				command=args[0];
				args[i]=NULL;
				if(strcmp(command,"cd")==0)
				{
				    if(args[1]==NULL)
				    {
				        chdir(getenv("HOME"));
				    }
				    else
				    {
				        DIR *dir = opendir(args[1]);
				        if(dir)
				        {
				            chdir(args[1]);
				            closedir(dir);
				        }
				        else
				        {
				            printf("%sBlad: Brak katalogu: %s%s\n",RED,args[1],COLOR_RESET);
				        }
				    }
                    getcwd(path, sizeof(path));
				}
				else if(strcmp(command,"whoami")==0)
				    {
				        printf("%s\n",getenv("USER"));
				    }
				    else if(strcmp(command,"touch")==0)
				        {
				            if(args[1]==NULL)
				            {
				                printf("%sBlad:Brak sciezki do pliku.%s\n",RED,COLOR_RESET);
				            }
				            else if(access(args[1],F_OK)!=-1)
				            {
				                utime(args[1],NULL);
				            }
				            else
				            {
				                FILE *mojplik;
				                mojplik = fopen(args[1],"w");
				                fclose(mojplik);
				            }
				        }
				    else if(strcmp(command,"clear")==0)
				    {
				        printf("\033[H\033[J");
				    }
				    else if(strcmp(command,"mkdir")==0)
				       {
				           if(args[1]==NULL)
				            {
				                printf("%sBlad:Brak nazwy katalogu.%s\n",RED,COLOR_RESET);
				            }
				            else
				            {
				                mkdir(args[1],0777);
				            }
				        }
				    else
				    {
				        pid = fork();
				        if(pid>0) 
                        {
                        	wait(NULL);
                        }
				        if(pid==0)
				        {
					        execvp(command, args);
				        }
				    }				
            }
        }
    }
    return EXIT_SUCCESS;
}