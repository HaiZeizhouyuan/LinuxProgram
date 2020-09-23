/*************************************************************************
	> File Name: mmap.c
	> Author: 
	> Mail: 
	> Created Time: Sun Aug  9 19:02:07 2020
 ************************************************************************/

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(msg) {\
    do {\
        perror(msg); \
        exit(EXIT_FAILURE);\
    } while (0);\
}

int main(int argc, char *argv[]) {
    char *addr;
    int fd;
    struct stat sb;
    off_t offset, pa_offset;
    size_t length;
    ssize_t s;

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "%s file offset [length]\n", argv[0]);
        exit(EXIT_FAILURE);
        //exit(1);
    }

    fd = open(argv[1], O_RDONLY);//int open(const char *pathname, int flags);打开文件argv[1]并只读;
    if (fd == -1)
        handle_error("open()");//成功返回文件描述符，　失败返回-1;

    if (fstat(fd, &sb) == -1)   //int fstat(int fd, struct stat a*stabuf)获取文件状态，成功返回0，失败返回－１，设置errno        /* To obtain file size */获取文件大小
        handle_error("fstat()");

    offset = atoi(argv[2]);//字符串转整数　
    pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);//sysconf在运行时获取配置信息，获取页面大小，pa_offset是获取需要映射到虚拟空间的大小, 页对齐;
    /* offset for mmap() must be page aligned */

    if (offset >= sb.st_size) {//当截取的字节的起始位置大于读取文件时报错
        fprintf(stderr, "offset is past end of file\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 4) {
        length = atoi(argv[3]);//打印的字节长度
        if (offset + length > sb.st_size) {//打印范围超过文件的大小时
            length = sb.st_size - offset;
        }
        /* Can't display bytes past end of file */
    } else {/* No length arg ==> display to end of file */
        length = sb.st_size - offset;
    }

    //起始地址为空,映射空间的长度为length + offset - pa_offset,内存保护标志是页面可读取,映射对象类型flags
    //是创建一个私有的写时复制映射
    addr = mmap(NULL, length + offset - pa_offset, PROT_READ, MAP_PRIVATE, fd, pa_offset);
    if (addr == MAP_FAILED)//成功时，mmap（）返回一个指向映射区域的指针。错误时，返回MAP_FAILED
        handle_error("mmap");

    s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
    if (s != length) {
        if (s == -1)
            handle_error("write");

        fprintf(stderr, "partial write");
        exit(EXIT_FAILURE);
    }

    munmap(addr, length + offset - pa_offset);
    close(fd);

    exit(EXIT_SUCCESS);
}


