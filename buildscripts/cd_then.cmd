@echo off
cd /d %~1
set _tail=%*
call set _tail=%%_tail:*%1=%%
@echo on
%_tail%