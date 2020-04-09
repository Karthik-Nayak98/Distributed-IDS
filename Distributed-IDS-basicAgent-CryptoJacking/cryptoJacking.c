#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

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
    float cpuPercent;
    int pid;
    char procName[100];

    char cmd[100] = {0};
    char base[100] = "bash processKill.sh ";

    sprintf(cmd, "%s", base);

    system("ps -eo pid,comm,%cpu --sort=-%cpu | head -11 | tail -10 > processDetails.txt");

    fp = fopen("processDetails.txt", "r");

    while (fscanf(fp, "%d %s %f", &pid, procName, &cpuPercent) != EOF)
    {
        if (cpuPercent > 7.0)
        {
            //system("sh process_kill.sh");
            sprintf(cmd, "%s %d %s", cmd, pid, procName);
            system(cmd);
            strcpy(cmd, "bash processKill.sh ");
        }
    }
    return 0;
}