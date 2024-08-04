#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ootm_main.h"


#define DEVICE_NODE_PATH "/dev/ioctl"


int main() 
{
    int rc, result = EXIT_SUCCESS;
    int fd;
    ootm_ioctl_buf_t buf = { 0 };
    
    fd = open(DEVICE_NODE_PATH, O_RDWR);
    if (fd < 0) {
        printf("Error: open(). %m\n");
        exit(EXIT_FAILURE);
    }

    rc = ioctl(fd, IOCTL_GET_STRING, &buf);
    if (rc != IOCTL_SUCCESS) {
        printf("Error: ioctl(). %m\n");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", (char*)&buf);
    close(fd);

exit:

   return result;
}
