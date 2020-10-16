!/bin/bash
nowtime=$(date +"%Y-%m-%d__%H:%M:%S")
last_all=$(ps -aux | awk '
BEGIN{
    num = 0;
} 
{
    if ($3 > 50 || $4 > 50) {
        if (num > 0) {
            printf("|");
        }
        printf("%s", $2);
        num++;

    }
}')

sleep 5

ps -aux | awk '{ 
    if ($2 ~ last_all && ($3 > 50 || $4 > 50)) { 
        printf("%s %s %s %s %s %s\n", nowtime, $11, $2, $1, $3, $4);
    }
}'

