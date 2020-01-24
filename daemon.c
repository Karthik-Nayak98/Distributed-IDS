// Creating a daemon process
// 1. Fork a child process that is the replica of the child process
// 2. Terminate the parent process.
// 3. Set the umask.
// 4. Call setid() to provide new session and new group for the child.
// 5. Now change the working directory to root(/) and close stdin, stdout, stderr.

#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t pid, sid;
    FILE *fp;

    pid = 0;
    sid = 0;

    pid = fork();

    if (pid < 0)
    {

        printf("Fork failed\n");
        exit(1);
    }
    else if (pid > 0)
    {
        printf("PID of the process is %d", pid);
        exit(0);
    }

    umask(0);

    sid = setid();

    if (sid < 0)
    {
        // Return failure;
        exit(1);
    }

    // Change the current working directory to root (Optional but for safety)
    chdir("/");

    // Closing stdin, stdout, stderr so it is not able to access the daemon
    close(stdin);
    close(stdout);
    close(stderr);

    fp = fopen("syslog.txt", "w+");

    // Actual functionalities of the daemon
    while (1)
    {
        }

    return 0;
}
