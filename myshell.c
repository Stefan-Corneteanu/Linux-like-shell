// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported
#define SUPERSECRETKEY 7 // key for Caesar's Cipher

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// Greeting shell during startup
void init_shell()
{
    clear();
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t****MY SHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}

// Function to take input
int takeInput(char* str)
{
    char* buf;

    buf = readline("\n>>> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}

// Function to print Current Directory.
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork();

    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL);
        return;
    }
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0) {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } else {
        // Parent executing
        p2 = fork();

        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } else {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

// Help command builtin
void openHelp()
{
    puts("\n***WELCOME TO MY SHELL HELP***"
        "\nCopyright @ 1221EA Anghel Denisa Florentina, Corneteanu Stefan"
        "\n-Use the shell at your own risk..."
        "\nList of Commands supported:"
        "\n>cd"
        "\n>ls"
        "\n>exit"
        "\n>help"
        "\n>hello"
        "\n>pwd"
        "\n>czr"
        "\n>randfunc"
        "\n>print"
        "\n>countdown");

    return;
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 10, i, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;

    ListOfOwnCmds[0] = "exit"; //exits shell -L
    ListOfOwnCmds[1] = "cd"; //change directory -L
    ListOfOwnCmds[2] = "help"; //open help menu -L
    ListOfOwnCmds[3] = "hello"; //greets user -O
    ListOfOwnCmds[4] = "ls"; //current directory files -L
    ListOfOwnCmds[5] = "pwd"; //print working directory -L
    ListOfOwnCmds[6] = "czr"; //applies Caesar's Cipher on a given text -O
    ListOfOwnCmds[7] = "randfunc"; //opens task manager -O
    ListOfOwnCmds[8] = "print"; //our attempt at implementing echo, but without dealing with variables -O
    ListOfOwnCmds[9] = "countdown"; //counts down to zero from a given number -O
    //10 commands, 5 Linux, 5 of our own

    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }
    i=1; //starting from 1 to jump over "print" or "czr"
    int j,len; //to be used for string parsing
    char c; //will be used for a yes or no context

    //TO BE USED IN THE PRINT FUNCTION

    int qflag = 0; //is a word enclosed between double quotes (2), single quotes (1), or unquoted (0) (default will be 0)
    bool escflag = 0; //should we escape a character (1) or not(0) (default will be 0)

    switch (switchOwnArg) {
    case 1: //exit
        printf("\nGoodbye\n");
        kill(0, SIGKILL);

    case 2: //cd
        chdir(parsed[1]);
        return 1;

    case 3: //help
        openHelp();
        return 1;

    case 4: //hello
        username = getenv("USER");
        printf("\nHello %s.\nMind that this is "
            "not a place to play around."
            "\nUse help to know more..\n",
            username);
        return 1;

    case 5: //ls
        system("ls");
        return 1;

    case 6: //pwd
        printDir();
        return 1;

    case 7: //czr
        printf("\nDo you want to encrypt or to decrypt your text? Press 0 for encrypt or 1 for decrypt\n");
        bool encrypted; //true: encrypted, we must decrypt; false: plain text, we must encrypt
        c=getchar();
        while (c!='1'&&c!='0'){
            printf("Error, invalid option. Do you want to encrypt or to decrypt your text? Press 0 for encrypt or 1 for decrypt\n");
            c=getchar();
        }
        if (c=='1'){
            encrypted=1;
        }
        else{
            encrypted=0;
        }
        while (parsed[i]!=NULL){
            int j;
            int len=strlen(parsed[i]);
            for (j=0;j<len;j++){
                if (!encrypted){
                    if ('a'<=parsed[i][j]&&parsed[i][j]<='z'){
                        parsed[i][j]+=SUPERSECRETKEY;
                        if (parsed[i][j]>'z')
                            parsed[i][j]-=26;
                    }
                    if ('A'<=parsed[i][j]&&parsed[i][j]<='Z'){
                        parsed[i][j]+=SUPERSECRETKEY;
                        if (parsed[i][j]>'Z')
                            parsed[i][j]-=26;
                    }
                }
                else{
                    if ('a'<=parsed[i][j]&&parsed[i][j]<='z'){
                        parsed[i][j]-=SUPERSECRETKEY;
                        if (parsed[i][j]<'a')
                            parsed[i][j]+=26;
                    }
                    if ('A'<=parsed[i][j]&&parsed[i][j]<='Z'){
                        parsed[i][j]-=SUPERSECRETKEY;
                        if (parsed[i][j]<'A')
                            parsed[i][j]+=26;
                    }
                }
            }
            printf("%s ",parsed[i]);
            i++;

        }
        return 1;

    case 8: //randfunc
        start:
        srand(time(0));
        int fidx;
        fidx = 1 + rand() % 10;
        printf("\nRandfunc has randomly called the function:\n");
        switch (fidx) {
            case 1:
                printf("exit. Do you want to exit the shell? y/n\n");
                c=getchar();
                while (c!='y'&&c!='Y'&&c!='n'&&c!='N'){
                    printf("\nError, invalid choice. Do you want to exit the shell? y/n\n");
                    c=getchar();
                }
                if(c=='y'||c=='Y'){
                    printf("Goodbye\n");
                    kill(0, SIGKILL);
                }
                else{
                    return 1;
                }
            case 2:
                printf("cd. Please input the directory you want to go to\n");
                char *  buf;
                buf = readline("");
                chdir(buf);
                return 1;
            case 3:
                printf("help. Redirecting to help menu\n");
                openHelp();
                return 1;
            case 4:
                printf("hello.\n");
                username = getenv("USER");
                printf("\nHello %s.\nMind that this is "
                "not a place to play around."
                "\nUse help to know more..\n",
                username);
                return 1;
            case 5:
                printf("ls. This is the content of your current working directory:\n");
                system("ls");
                return 1;
            case 6:
                printf("pwd. This is the directory where you are right now:\n");
                printDir();
                return 1;
            case 7:
                printf("czr, but you don't have a text to encrypt");
                return 1;
            case 8:
                printf("randfunc... isn't this a lucky coincidence -_-. Do you want to try and run the command again to get something else? y/n\n");
                c=getchar();
                while (c!='y'&&c!='Y'&&c!='n'&&c!='N'){
                    printf("\nError, invalid choice. Do you want to try and run the command again to get something else? y/n\n");
                    c=getchar();
                }
                if(c=='y'||c=='Y'){
                    goto start;
                }
                else{
                    return 1;
                }

            case 9:
                printf("print, but you have no text to print.\n");
                return 1;
            case 10:
                printf("countdown. You have 10 seconds to defuse the bomb or the computer will explode\n");
                int num = 10;
                while (num!=0){
                    printf("\n%d",num--);
                    sleep(1);
                }
                printf("\nBOOM\n");
                return 1;
            default: //something went wrong
                printf("error in creating randfunc!");
                return 1;
        }

    case 9: //print
        while (parsed[i]!=NULL){ //while there are still words to parse
            len = strlen(parsed[i]);
            for (j = 0; j < len; j++){

                if (parsed[i][j]=='\"'&& qflag == 0 && escflag == 0){ //if we have an unescaped double quote and we didnt have one before,
                    //we enter in a double quoted string
                    qflag = 2;
                    continue; //we dont want to print unescaped quotes
                }

                if (parsed[i][j]=='\"'&& qflag ==2 && escflag == 0){ //if we have an unescaped double quote and we had one before,
                    //we exit a double quoted string
                    qflag = 0;
                    continue; //we dont want to print unescaped quotes
                }

                if (parsed[i][j]=='\''&& qflag == 0 && escflag == 0){ //if we have an unescaped single quote and we didnt have one before,
                    //we enter a single quoted string
                    qflag = 1;
                    continue; //we dont want to print unescaped quotes
                }

                if (parsed[i][j]=='\''&& qflag == 1){ //if we have an unescaped single quote and we had one before,
                    //we exit a single quoted string
                    //did not include the 2nd and because inside a single quoted string the backslash is no longer an escape character
                    qflag = 0;
                    continue; //we dont want to print unescaped quotes
                }
                if (parsed[i][j]=='\\' && qflag !=1){
                    if (escflag == 0){
                        escflag = 1;

                    }
                    else{
                        escflag = 0;
                        printf("\\");
                    }
                    continue;
                    //we dont want to print the escape character or to print twice a backslash
                }
                if (escflag){
                    //this wouldve looked better with a switch case but oh well
                    if (parsed[i][j]=='n'){
                        printf("\n"); //new line
                    }
                    else if (parsed[i][j]=='t'){
                        printf("\t"); //horizontal tab
                    }
                    else if (parsed[i][j]=='r'){
                        printf("\r"); //carriage return
                    }
                    else if (parsed[i][j]=='v'){
                        printf("\v"); //vertical tab
                    }
                    else if (parsed[i][j]=='f'){
                        printf("\f"); //form feed
                    }
                    else if (parsed[i][j]=='\"'){
                        printf("\""); //double quote
                    }
                    else if (parsed[i][j]=='\''){
                        printf("\'"); //single quote
                    }
                    else{
                        printf("\\%c",parsed[i][j]); //nonexistent escaping character, default is backslash and character
                    }
                    escflag = 0;
                }
                else{
                    printf("%c",parsed[i][j]); //deals even with escaped quotes
                }
            }
            printf(" ");
            i++;
        }
        printf("\n>>>");
        return 1;

    case 10: //countdown
        //can't have more than 1 parameter
        if(parsed[2]!=NULL){
            printf("\nThis function accepts only one parameter\n");
            return 0;
        }
        //checking if the given parameter is a nonnegative number
        i = 0;
        len = strlen(parsed[1]);
        for (;i<len;i++){
            if (!('0'<=parsed[1][i]&&parsed[1][i]<='9')){
                printf("\nThis function accepts only nonnegative numbers\n");
                return 0;
            }
        }
        int num = atoi(parsed[1]);
        //we got a nonnegative number lets countdown
        while (num!=0){
            printf("%d\n",num--);
            sleep(1);
        }
        printf("done\n");
        return 1;
    }

    return 0;
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0; // returns zero if no pipe is found.
    else {
        return 1;
    }
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;

    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char* str, char** parsed, char** parsedpipe)
{

    char* strpiped[2];
    int piped = 0;

    piped = parsePipe(str, strpiped);

    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);

    } else {

        parseSpace(str, parsed);
    }

    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}

int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST], *commands[MAXLIST], *p;
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0, no_commands, i;
    init_shell();

    while (1) {
        // print shell line
        printDir();
        // take input
        if (takeInput(inputString))
            continue;
        // separate input string into commands (separator is ";")
        p = strtok(inputString, ";");
        // for each command, process the string and execute the command
        while (p!=NULL){
            // process
            execFlag = processString(p,
            parsedArgs, parsedArgsPiped);
            // execflag returns zero if there is no command
            // or it is a builtin command,
            // 1 if it is a simple command
            // 2 if it is including a pipe.

            // execute
            if (execFlag == 1)
                execArgs(parsedArgs);

            if (execFlag == 2)
                execArgsPiped(parsedArgs, parsedArgsPiped);

            p=strtok(NULL,";");
        }
    }
    return 0;
}
