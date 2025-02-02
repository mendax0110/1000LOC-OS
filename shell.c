#include "user.h"

void main(void)
{
    while (1)
    {
        prompt:

        printf("mendax:~$ ");
        char cmdline[128];

        for (int i = 0; ; i++)
        {
            char ch = getchar();
            putchar(ch);

            if (i == sizeof(cmdline) - 1)
            {
                printf("\ncommand line too long\n");
                goto prompt;
            }
            else if (ch == '\r')
            {
                printf("\n");
                cmdline[i] = '\0';
                break;
            }
            else
            {
                cmdline[i] = ch;
            }
        }

        if (strcmp(cmdline, "hello") == 0)
        {
            printf("Hello World from shell!\n");
        }
        else if (strcmp(cmdline, "exit") == 0)
        {
            exit();
        }
        else if (strcmp(cmdline, "readfile") == 0)
        {
            char buf[128];
            int len = readfile("hello.txt", buf, sizeof(buf));
            buf[len] = '\0';
            printf("read %d bytes: %s\n", len, buf);
        }
        else if (strcmp(cmdline, "writefile") == 0)
        {
            writefile("hello.txt", "Hello World from file!\n", 24);
            printf("write file\n");
        }
        else if (strcmp(cmdline, "ls") == 0)
        {
            list_files();
        }
        else if (strncmp(cmdline, "cat ", 4) == 0)
        {
            char buf[256];
            int len = readfile(cmdline + 4, buf, sizeof(buf));
            buf[len] = '\0';
            printf("%s\n", buf);
        }
        else if (strncmp(cmdline, "echo ", 5) == 0)
        {
            printf("%s\n", cmdline + 5);
        }
        else if (strcmp(cmdline, "clear") == 0)
        {
            clear_screen();
        }
        else if (strncmp(cmdline, "exec ", 5) == 0)
        {
            exec(cmdline + 5);
        }
        else if (strcmp(cmdline, "help") == 0)
        {
            printf("commands:\n");
            printf("  hello\n");
            printf("  exit\n");
            printf("  readfile\n");
            printf("  writefile\n");
            printf("  ls\n");
            printf("  cat\n");
            printf("  echo\n");
            printf("  clear\n");
            printf("  exec\n");
            printf("  help\n");
        }
        else
        {
            printf("unknown command: %s\n", cmdline);
        }
    }
}