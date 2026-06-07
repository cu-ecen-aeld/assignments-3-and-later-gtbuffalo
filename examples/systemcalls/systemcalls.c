#include "systemcalls.h"
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>

static int DO_EXEC_CALLED = 0;


/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
    /* Reference: The Linux Programming Interface p581 */

#define CANT_CREATE_CHILD -1  /* or termination status not retrieved */
    openlog(NULL, 0, LOG_USER);
    bool return_value = false;
    syslog(LOG_INFO, "Calling system() with cmd: %s", cmd);
    int status = system(cmd);
    if (CANT_CREATE_CHILD == status){
        syslog(LOG_ERR, "Calling system() with cmd: %s (%d)", cmd, status);
        perror("system");
        return_value = false;
    } else {
        syslog(LOG_INFO, "system() completed cmd: %s (%d)", cmd, status);
        return_value = true;
    }
   
    closelog();
    return return_value;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    char * child_args[count];
    bool return_value = false;
#define FAILURE -1
#define NO_WAIT_OPTIONS 0
    pid_t pid = FAILURE;
    int i;
    int status = FAILURE;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    for(i=0; i<count-1; i++)
    {
        child_args[i] = command[i+1];
    }
    child_args[count-1] = NULL;

 /*
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    openlog(NULL, 0, LOG_USER);
    DO_EXEC_CALLED+=1;
    syslog(LOG_INFO, "[%d] Calling fork() for system impl...", DO_EXEC_CALLED);
    pid = fork();
    if (FAILURE == pid){
        syslog(LOG_ERR, "[%d] Calling fork() from parent (%d)", DO_EXEC_CALLED, pid);
        perror("fork");
        return_value = false;
    } else if (0 == pid) {
        syslog(LOG_ERR, "[%d] fork() succeeded - in child (%d)", DO_EXEC_CALLED, pid);
        syslog(LOG_INFO, "[%d] Calling execv() in child with %s, %s", DO_EXEC_CALLED, command[0], child_args[0]);
        status = execv(command[0],child_args);
        if(FAILURE != waitpid(pid, &status, NO_WAIT_OPTIONS)) {
            if (WIFEXITED(status)){
              syslog(LOG_INFO, "[%d] execv() in child (%d) returned with status %d", DO_EXEC_CALLED, pid, WEXITSTATUS(status));
              return_value = true;
            }
        } else {
            syslog(LOG_ERR, "[%d] Calling execv() failed for child (%d)", DO_EXEC_CALLED, pid);
            perror("execv");
            return_value = false;
        }
    } else {
        syslog(LOG_INFO, "[%d] Calling waitpid() on child with %d", DO_EXEC_CALLED,pid);
        if(FAILURE != waitpid(pid, &status, NO_WAIT_OPTIONS)) {
            if (WIFEXITED(status)){
              syslog(LOG_INFO, "[%d] execv() exited to parent returning with status %d for %s %s %s", DO_EXEC_CALLED,WEXITSTATUS(status), command[0], child_args[0], count > 1 ? child_args[1] : "" );
              if(EXIT_SUCCESS == WEXITSTATUS(status)){
                return_value = true;
              } else {
                return_value = false;
              }
            }
        } else {
            syslog(LOG_ERR, "[%d] Calling execv() failed for child (%d)", DO_EXEC_CALLED, pid);
            perror("execv");
            return_value = false;
        }
    }
   
    va_end(args);
    closelog();
    syslog(LOG_INFO, "[%d] do_exec returning %s", DO_EXEC_CALLED, return_value ? "true" : "false");
    return return_value;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    va_end(args);

    return true;
}
