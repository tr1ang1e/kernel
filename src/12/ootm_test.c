#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ootm_main.h"


#define DEVICE_NODE_PATH "/dev/" DEVICE_NODE


int main() 
{
    int fd1, fd2, result = EXIT_SUCCESS;
    ssize_t rc;
    ootm_data data1, data2;
    const char* str1 = "first", * str2 = "second";
    size_t len1 = strlen(str1), len2 = strlen(str2);

    // test open device several times

    fd1 = open(DEVICE_NODE_PATH, O_RDWR);
    if (fd1 < 0) {
        printf("Error fd1: open(). %m\n");
        exit(EXIT_FAILURE);
    }

    fd2 = open(DEVICE_NODE_PATH, O_RDWR);
    if (fd2 < 0) {
        printf("Error fd2: open(). %m\n");
        exit(EXIT_FAILURE);
    }

    // test if the same data are initially read

    rc = read(fd1, data1.buf, BUF_SIZE);
    if (rc < 0) {
        printf("Error fd1: read(). %m\n");
        exit(EXIT_FAILURE);
    }

    rc = read(fd2, data2.buf, BUF_SIZE);
    if (rc < 0) {
        printf("Error fd2: read(). %m\n");
        exit(EXIT_FAILURE);
    }

    rc = strcmp(data1.buf, data2.buf);
    if (0 != rc) {
        printf("Error: different strings.\n");
        exit(EXIT_FAILURE);
    }

    // test writing unique data

    rc = write(fd1, str1, len1);
    if (rc < 0) {
        printf("Error fd1: write(). %m\n");
        exit(EXIT_FAILURE);
    }

    rc = write(fd2, str2, len2);
    if (rc < 0) {
        printf("Error fd2: write(). %m\n");
        exit(EXIT_FAILURE);
    }

    rc = read(fd1, data1.buf, BUF_SIZE);
    if (rc < 0) {
        printf("Error fd1: read(). %m\n");
        exit(EXIT_FAILURE);
    }

    rc = read(fd2, data2.buf, BUF_SIZE);
    if (rc < 0) {
        printf("Error fd2: read(). %m\n");
        exit(EXIT_FAILURE);
    }

    rc = strcmp(data1.buf, str1);
    if (0 != rc) {
        printf("Error fd1: different strings.\n");
        exit(EXIT_FAILURE);
    }

    rc = strcmp(data2.buf, str2);
    if (0 != rc) {
        printf("Error fd2: different strings.\n");
        exit(EXIT_FAILURE);
    }

    // close devices

    close(fd1);
    close(fd2);

exit:

   return result;
}
