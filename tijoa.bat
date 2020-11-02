#!/bin/bash
title GIT commit bat
color 16
echo start code loacl res
echo current dir：%cd%

echo start add change
git add -A .
echo add end！
echo;
echo commit to local res
set /p declation=input change msg:
git commit -m "%declation%"
echo;
echo code change commit remote git server
git push origin master
echo;
echo push end！
echo;

pause