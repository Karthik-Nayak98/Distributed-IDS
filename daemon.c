// Creating a daemon process
// 1. Fork a child process that is the replica of the child process
// 2. Terminate the parent process.
// 3. Set the umask.
// 4. Call setid() to provide new session and new group for the child.
// 5. Now change the working directory to root(/) and close stdin, stdout, stderr.

#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>

void *crypto(void *vargp)
{
    char *args[] = {"./Distributed-IDS-basicAgent-CryptoJacking/crypto", NULL};
    execvp(args[0], args);
}
void *denialOfService(void *vargp)
{
    char *args[] = {"./Distributed-IDS-basicAgent-DOS/dos", NULL};
    execvp(args[0], args);
}

void bruteForce()
{
}
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
        printf("PID of the parent process is %d\n", pid);
        exit(0);
    }

    umask(0);

    sid = setsid();

    if (sid < 0)
    {
        // Return failure;
        exit(1);
    }

    // Change the current working directory to root (Optional but for safety)
    chdir("/");

    // Closing stdin, stdout, stderr so it is not able to access the daemon
    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

    fp = fopen("syslog.txt", "w+");

    //system(" zenity-- warning-- title = \" Warning High CPU usage \" --text = \" Do you want to kill the process $2 \" --width = 300 --height = 100");

    pthread_t thread1, thread2, thread3;

    // Actual functionalities of the daemon
    while (1)
    {
        pthread_create(&thread1, NULL, crypto, NULL);
        pthread_create(&thread2, NULL, denialOfService, NULL);
        //pthread_create(&thread3, NULL, bruteForce, NULL);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        //pthread_join(thread3, NULL);
    }

    return 0;
}
