#!/bin/bash
df -l | grep '^/dev/' | awk -v ntime=$(date +"%Y-%m-%d__%H:%M:%S") '
BEGIN{
    all_size=0;
    all_bava=0;
    all_use=0;
}
{    all_size+=$2;
    all_use+=$3;
    all_bava+=$4;
    printf("%s 1 %s %s %s %s\n", ntime, $6, $2, $4, $5);
}
END{
printf("%s 0 disk %s %s %.2f%%\n", ntime, all_size, all_bava, (all_use * 100)/(all_use+all_bava) + 1);
}
'
