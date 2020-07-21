@echo off
title "start dubbo admin"
start javaw -jar "-Dserver.port=8091" dubbo-admin-server-0.2.0.jar 
echo ------------start------------
pause
exit
