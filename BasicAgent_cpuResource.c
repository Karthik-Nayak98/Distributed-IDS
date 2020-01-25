#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define threshold 80

struct ProcDetails
{
    pid_t pid;
    char procName[100];
    float cpuUtil;
};

struct ProcDetails pt[2];

int main()
{
    FILE *fp;
    char *cpuPercent;

    system("ps -eo %cpu --sort=-%cpu | head -11 | tail -10 > procUsage.txt");

    fp = fopen("procUsage.txt", "r");

    while (!feof(fp))
    {
        fgets(fp, "%[^\n]", cpuPercent);
        if (atoi(cpuPercent) < 5)
        {
            printf("%s\n", cpuPercent);
            break;
        }
    }

        return 0;
}