#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>

void show_usage(char* program_name){
    printf("\nUsage: %s <writefile> <writestr>\n\n", program_name);
}

#define OPEN_ERROR -1

int main(int argc, char** argv) {
    openlog(NULL, 0, LOG_USER);

    if(argc < 3){
        syslog(LOG_ERR, "Invalid number of arguments %d", argc);
        closelog();

        fprintf(stderr,"Invalid number of arguments %d\n", argc);

        show_usage(argv[0]);
        return 1;
    }

    
    char* write_file = argv[1];
    char* write_string = argv[2];

    syslog(LOG_DEBUG,"arguments: write_file=%s, write_string=%s",write_file, write_string);
 
    errno = 0;
    int target_fd = open(write_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    const int error = errno;
    if(OPEN_ERROR == target_fd) {
        fprintf(stderr,"ERROR: Cannot open file for writing, %s: %s (%d)\n", write_file, strerror(error), error);
        syslog(LOG_ERR,"ERROR: Cannot open file for writing, %s: %s (%d)\n", write_file, strerror(error), error);
    }
    int bytes_written = 0;
        syslog(LOG_DEBUG,"Writing %s to file %s", write_string, write_file);
    if(bytes_written = write(target_fd,write_string, strlen(write_string))){
        syslog(LOG_DEBUG,"Wrote %d bytes to file, %s", bytes_written, write_file);
    } else {
        int error = errno;
        fprintf(stderr,"ERROR: Cannot write to file, %s: %s (%d)\n", write_file, strerror(error), error);
        syslog(LOG_ERR,"ERROR: Cannot write to file, %s: %s (%d)\n", write_file, strerror(error), error);
    }

    close(target_fd);
    closelog();
    return 0;
}