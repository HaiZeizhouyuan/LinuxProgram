#include "head.h"

unsigned long all_size, all_use, all_free, all_bavail;


int main(int argc, char *argv[]) {
    FILE* sfile;
    struct mntent *mount_entry;
    struct statfs sfs;
    unsigned long blocks_used;
    unsigned blocks_percent_used;

    if ((sfile = setmntent("/etc/mtab", "r")) == NULL) {
        fprintf(stderr, "set mount entry error/n");
        return -1;        
    }

    printf("%-20s  %-2s  %-20s %-9s %-9s %5s%%\n", "Date ans times", "Kind", "Filesystem", "Size", "Last", "Use");
    char **buff = (char **)calloc(100 , sizeof(char*));
    int ans = 0;
    while (1) {
        buff[ans] = (char *)calloc(100, sizeof(char));
        const char *device;
        const char *mount_point;
        if(( mount_entry = getmntent(sfile)) == NULL) {
            endmntent(sfile);
            break;                            
        }                   
        device = mount_entry->mnt_fsname;
        mount_point = mount_entry->mnt_dir;

        //fprintf(stderr, "device = %s   mountpoint = %s\n", device, mount_point);

        if (statfs(mount_point, &sfs) != 0) continue;
        if ((sfs.f_blocks > 0)) {
            blocks_used = sfs.f_blocks - sfs.f_bfree;
            blocks_percent_used = 0;
            if (sfs.f_blocks - sfs.f_bavail != 0) {
                blocks_percent_used = (blocks_used * 100) / (blocks_used + sfs.f_bavail) + 1;                
            }
           
            sprintf(buff[ans++], "%-20s %-9ld %-9ld %3u%%", device, 4 * sfs.f_blocks, 4 * sfs.f_bavail,blocks_percent_used);
            all_size += sfs.f_blocks;
            all_free += sfs.f_bfree;
            all_bavail += sfs.f_bavail;
        }   
    }

    char *my_time = (char *)malloc(sizeof(char) * 30);
    struct tm *tms;
    time_t t;
    time(&t);
    tms = localtime(&t);  

    sprintf(my_time, "%4d-%02d-%02d__%02d:%02d:%02d", tms->tm_year + 1900, tms->tm_mon, tms->tm_mday, tms->tm_hour, tms->tm_min, tms->tm_sec);
    for (int i = 0; i < ans; i++) {
        printf("%s  %-5d %s\n", my_time, 1, buff[i]);
    }

    blocks_percent_used = (all_size - all_free) * 100 / (all_size - all_free + all_bavail) + 1; 
    printf("%s  %-5d %-20s %-9ld %-9ld %3u%%\n" , my_time, 0, "disk", 4 * all_size, 4 * all_bavail, blocks_percent_used);
    free(buff);
    free(my_time);
    return 0;

}
