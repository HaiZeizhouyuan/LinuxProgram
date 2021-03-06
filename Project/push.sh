#!/bin/bash
nowtime=`date`
git add * 
git commit -a -m "${nowtime}" 
git push
