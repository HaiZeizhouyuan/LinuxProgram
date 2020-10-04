#!/bin/bash
free -m | grep "^Mem" | awk -v last_ave=$1 -v ntime=$(date +"%Y-%m-%d__%H:%M:%S") '
{   printf("%-20s %7s %9s %8s %36s\n", "time", "total", "available", "used(%)", "mic average of occupancy percentage");
    now_ave=$3*100/$2;
    printf("%-20s %6sM %8sM %7.1f%% %35.1f%%\n", ntime, $2, $7, now_ave, 0.3 * last_ave + 0.7 * now_ave);
}
'
