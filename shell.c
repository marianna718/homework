
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SH_RL_BUFSIZE 1024
#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIM "\t\r\n\a"


char *sh_read_line(void)
{
    int bufsize = SH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;
    
    if (!buffer)
    {
        fprintf(stderr, "allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    while(1)
    {
        c = getchar();
        
        if(c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        position++;
        
        if(position >= bufsize)
        {
            bufsize += SH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if(!buffer)
            {
                fprintf(stderr, "allocation error\n");
                exit(EXIT_FAILURE);
                
            }
        }
    }
}


char **sh_split_line(char *line)
{
    int bufsize = SH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    
    if(!tokens)
    {
        fprintf(stderr, "allocaiton error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, SH_TOK_DELIM);
    
    while(token != NULL)
    {
        tokens[position] = token;
        position++;
    
    
    if(position >= bufsize)
    {
        bufsize += SH_TOK_BUFSIZE;
        tokens = realloc(tokens, bufsize * sizeof(char*));
        if(!tokens)
        {
            fprintf(stderr, "allocaiton error \n");
            exit(EXIT_FAILURE);
            
        }
    }
    
    token = strtok(NULL,SH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
    
}



int sh_launch(char **args)
{
    pid_t pid , wpid;
    int status;
    
    pid = fork();
    if (pid == 0) 
    {
        if(execvp(args[0], args) == -1)
        {
            perror("shl");
        }
        exit(EXIT_FAILURE);
        
    }
    else if(pid < 0)
    {
        perror("shl");
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
            
        }
        while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

//built-in-ish functions

int sh_cd(char **args);
int sh_help(char **args);
int sh_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_help,
    &sh_exit
};

int sh_num_builtins() 
{
    return sizeof(builtin_str) / sizeof(char*);
    
}

//implementations

int sh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "expected argument to \"cd\"\n");
    }
    else 
    {
        if(chdir(args[1]) != 0)
        {
            perror("shl");
        }
    }
    return 1;
}


int sh_help(char **args)
{
    int i;
    printf("This is some kind of shell\n");
    printf("Type program names and arguments and then hit enter.\n");
    printf("here are built in :\n");
    
    for(i =0; i < sh_num_builtins();i++)
    {
        printf("  %s\n", builtin_str[i]);
    }
    
    printf("Use the man command for information on other programs.\n");
    return 1;
}


int sh_exit(char **args)
{
    return 0;
}


int sh_execute(char **args)
{
    int i;
    
    if(args[0] == NULL)
    {
        return 1;
    }
    
    for(i = 0; i < sh_num_builtins(); i++)
    {
        if(strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }
    
    return sh_launch(args);
}




void sh_loop(void)
{
    char *line;
    char  **args;
    int status;
    
    do {
        printf("> ");
        line = sh_read_line();
        args = sh_split_line(line);
        status = sh_execute(args);
        
        free(line);
        free(args);
        
    } while (status);
    
}

int main()
{

}

