#!/bin/bash
time_and_load=`date +"%Y-%m-%d__%H:%M:%S "``uptime | tr -s " " "\n" | tr -s "," " " | tail -3 | tr -s "\n" " "`
cpu_total1="$(cat /proc/stat | grep -w "cpu" | awk '{printf("%d", $2 + $3 + $4 + $5 + $6 + $7 + $8);}')";
cpu_idle1="$(cat /proc/stat | grep -w "cpu" | awk '{printf("%d", $5);}')";

sleep 0.5s

cpu_total2="$(cat /proc/stat | grep -w "cpu" | awk '{printf("%d", $2 + $3 + $4 + $5 + $6 + $7 + $8);}')";
cpu_idle2="$(cat /proc/stat | grep -w "cpu" | awk '{printf("%d", $5);}')";

((cpu_total = $cpu_total2 - $cpu_total1))
((cpu_idle = $cpu_idle2 - $cpu_idle1))

# 计算cpu占用率
cpu_occu=`echo "$cpu_total $cpu_idle" | awk '{printf("%.2f%%", ($1 - $2) * 100 / $1);}'`

#CPU温度

cpu_temp="$(cat /sys/class/thermal/thermal_zone0/temp | awk '
{
    if ($1 < 50000) {
        printf("%.2f normal", $1/1000);
    } else if ($1 <= 70000) {
        printf("%.2f note", $1/1000);
    } else {
        printf("%.2f warning", $1/1000);
    }

}
')";

echo "${time_and_load}${cpu_occu} ${cpu_temp}"
