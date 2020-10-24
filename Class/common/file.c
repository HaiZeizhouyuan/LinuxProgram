/*************************************************************************
	> File Name: file.c
	> Author: zhouyuan
	> Mail: 3294207721@qq.com 
	> Created Time: 2020年09月19日 星期六 10时39分32秒
 ************************************************************************/

#include "head.h"
char *re_filename(char *filename);

void send_file(const char *filename, int sockfd) {
    FILE *fp = NULL;
    size_t size;
    struct FileMsg filemsg;
    char *p;
    DBG(BLUE"filname is %s!\n"NONE, filename);
    if ((fp = fopen(filename, "rb")) == NULL) {//'b'以二进制打开
        perror("fopen");
        return ;
    }
    memset(&filemsg, 0, sizeof(filemsg));
    fseek(fp, 0L, SEEK_END);//将指针移到文件末尾
    filemsg.size = ftell(fp);//获取当前文件指针,得到文件的大小
    fseek(fp, 0L, SEEK_SET);//将指针移到文件起始位置
    strcpy(filemsg.name, (p = strrchr(filename, '/')) ? p + 1 : filename);// ./
    while((size = fread(filemsg.buff, 1, sizeof(filemsg.buff), fp))) {
        send(sockfd, (void *)&filemsg, sizeof(filemsg), 0);
        usleep(100);
        memset(filemsg.buff, 0, sizeof(filemsg.buff));
    }
    DBG(GREEN"end send!\n"NONE);
    return ;
}

void recv_file(int sockfd) {
    DBG(RED"start recv!\n"NONE);
    size_t recv_size, total_size = 0, write_size = 0;
    long left_size;
    struct FileMsg packet_t, packet, packet_pre;
    int packet_size = sizeof(struct FileMsg);
    long offset = 0, cnt = 0, sum = 0;
    FILE *fp = NULL;
    char new_name[512];
    bzero(&packet, 0);
    bzero(&packet_pre, 0);
    bzero(&packet_t, 0);
    while(1) {
        if (offset) {
            memcpy(&packet, &packet_pre, offset);
        }
        while((recv_size = recv(sockfd, (void *)&packet_t, packet_size, 0)) > 0) {
            DBG(RED"packet_t.size : %ld!\n"NONE, packet_t.size); 
            if (!sum) left_size = packet_t.size;
            sum++;
            left_size -= sizeof(packet_t.buff);
            DBG(GREEN" recv_size : %ld, buff_size : %ld, left size is %ld!\n"NONE,
                recv_size, sizeof(packet_t.buff), left_size);
            
            if (recv_size + offset == packet_size) {
                memcpy((char *)&packet + offset, &packet_t, recv_size);
                offset = 0;
                DBG(L_GREEN"整包!\n"NONE);
                break;
            } else if (recv_size + offset < packet_size) {
                memcpy((char *)&packet + offset, &packet_t, recv_size);
                offset += recv_size;
                DBG(BLUE"拆包!, size : %ld\n"NONE, packet_t.size);
                //if (left_size <= 0) break;
                continue;
            } else {
                int need = packet_size - offset;
                memcpy((char *)&packet + offset, &packet_t, need);
                offset = recv_size - need;
                memcpy((char *)&packet_pre, (char *)&packet_t + need, offset);
                DBG(RED"粘包!\n"NONE);
                break;
            }
        }
        DBG(RED"offset : %ld!\n"NONE,offset);
        if (!cnt) {
            char path[30];
            sprintf(new_name, "%s", re_filename(packet.name));
            sprintf(path, "./data/%s", new_name);
            DBG(GREEN"create file!, file name is %s\n"NONE, new_name);
            if ((fp = fopen(path, "wb")) == NULL) {
                perror("fopen file failed");
                return ;
            } 
        }
        cnt += 1;
        if (packet.size - total_size >= sizeof(packet.buff)) {
            write_size = fwrite(packet.buff, 1, sizeof(packet.buff), fp);
            DBG(BLUE"write a packet to %s!\n"NONE, new_name);
        } else {
            write_size  = fwrite(packet.buff, 1, packet.size - total_size, fp);
            DBG(YELLOW"write helf packet to %s!\n"NONE, new_name);
        }
        
        total_size += write_size;
        memset(packet_t.buff, 0, sizeof(packet.buff));
        DBG(RED"total_size : %ld!\n\n"NONE, total_size);
        if (total_size >= packet.size) {
            DBG(RED"recv finish!"NONE);
            break;
        }
    }
    DBG(BLUE"have finish send!\n"NONE);
    fclose(fp);
    return ;
}

char *re_filename(char *filename) {
    FILE *fp;
	char *tmp = (char *)malloc(sizeof(char) * 512);
    char *f_name = (char *)malloc(sizeof(char) * 512);
    char *b_name = (char *)malloc(sizeof(char) * 512);
    char path[30];
    sprintf(path, "./data/%s", filename);
    fp = fopen(path,"r");
    if (fp != NULL) {
        int ans = 1;
        int i = strlen(filename) - 1;
        for (; i >= 0; i--) {
            if (filename[i] == '.') break;
        }
        strncpy(f_name, filename, i);
        strncpy(b_name, filename + i, strlen(filename) - i);
        while(1) {
            DBG(RED"create %s is failed!\n"NONE, filename);
            sprintf(tmp, "./data/%s%d%s", f_name, ans, b_name);
            if ((fp = fopen(tmp, "r")) == NULL) {
                memset(filename, 0, sizeof(filename));
                sprintf(filename, "%s%d%s", f_name, ans, b_name);
                break;                 
            }
            memset(tmp, 0, sizeof(tmp));
            ans += 1;       
        }     
    }
    free(tmp);
    free(f_name);
    free(b_name);
    return filename;
}
