#!/bin/bash
#时间
sys_time=`date +"%Y-%m-%d__%H:%M:%S"`
#主机名
sys_hostname=`hostname`
#OS版本
sys_os=`cat /etc/issue | awk '{printf("%s_%s_%s", $1, $2, $3);}'`
#内核版本
sys_kernel=`uname -r`
#运行时间
run=`uptime -p | tr -s " " "_"`
#平均负载
load=`cat /proc/loadavg | awk '{printf("%s %s %s", $1, $2, $3);}'`
#磁盘

disk_total=`df -l | grep '^/dev/' | awk '
BEGIN{
    all_size=0;
    all_bava=0;
    all_use=0;
}
{    all_size+=$2;
    all_use+=$3;
    all_bava+=$4;
}
END{
    disk_p=(all_use * 100) / (all_use + all_bava) + 1;
    printf("%d %d%%\n", all_size, disk_p);
}
'
`
#内存
mem=`free -m | grep '^Mem' | awk '{
    mem_p=$3*100/$2;
    printf("%dM %.1d%%\n", $2, mem_p);
}'`

#CPU的温度
cpu_temp=`cat /sys/class/thermal/thermal_zone0/temp | awk '{printf("%.2d", $1/1000);}'`

if [[ $disk_p -ge 0 && $disk_p -le 80 ]]; then
    disk_class="normal"
elif [[ $disk_p -ge 81 && $disk_p -le 90 ]]; then
    disk_class="note"
else
    disk_class="warning"
fi

if [[ $cpu_temp -ge 0 && $cpu_temp -le 50 ]]; then
    cpu_class="normal"
elif [[ $cpu_temp -ge 51 && $cpu_temp -le 70 ]]; then
    cpu_class="note"
else
    cpu_class="warning"
fi

if [[ $mem_p -ge 0 && $mem_p -le 70 ]]; then
    mem_class="normal"
elif [[ $mem_p -ge 71 && $mem_p -le 80 ]]; then
    mem_class="note"
else 
    mem_class="warning"
fi

echo "${sys_time}" " ${sys_hostname}" "${sys_os}"　"${sys_kernel}" "${run}" "${load}" "${disk_total}" "${mem}" "${cpu_temp}" $disk_class $mem_class $cpu_class 

