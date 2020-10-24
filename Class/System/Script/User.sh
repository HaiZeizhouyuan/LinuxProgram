#!/bin/bash
nowtime=`date +"%Y-%m-%d__%H:%M:%S"`
usernum=`awk -F ":" '
BEGIN{
    num=0;
}
{
    if ($3 >= 1000) {
        num++;
    }
}
END{
    printf("%d\n", num);
}
' /etc/passwd`

active_user=`last | awk NF | grep -v "system" | grep -v "wtmp" | cut -d " " -f 1 | uniq -c | sort -n -r | head -3 | awk '
BEGIN{
    num = 0;
}
{
    if (num > 0) {
        printf(",");
    }
    printf("%s", $2);
    num++;
}
'`

all_root_user=`cat /etc/group | grep "^sudo" | cut -d ":" -f 4`

now_login=`w | awk '
BEGIN{
    num=0
} 
{
    if (NR > 2) {
        if (num > 0) {
            printf(",");
        }
        printf("%s_%s_%s",$1, $3, $2);
    }
}'`
echo "${nowtime} ${usernum} [${active_user}] [${all_root_user}] [${now_login}]" 


