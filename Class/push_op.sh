#!/bin/bash
nowtime=`date +"%Y-%m-%d %H:%M:%S %A"`
git add * 
git commit -a -m "${nowtime}" 
git push

