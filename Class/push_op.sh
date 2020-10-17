#!/bin/bash
nowtime=`date +"%Y-%m-%d %H:%M:%S %a "`
git add * 
git commit -a -m "${nowtime}" 
git push

