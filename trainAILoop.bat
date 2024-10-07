@echo off

set "count=20"

:train_ai
for /L %%i in (1,1,%count%) do (
    start /min "" "GameTetris++.exe" aitrain
)

:check_if_tetris1_running
tasklist /FI "IMAGENAME eq GameTetris++.exe" 2>NUL | find /I "GameTetris++.exe" >NUL
if "%ERRORLEVEL%"=="0" (
    timeout /T 5 /NOBREAK >NUL
    goto check_if_tetris1_running
)

for /L %%i in (1,1,%count%) do (
    start /min "" "GameTetris++.exe" aitrain
)

:check_if_tetris2_running
tasklist /FI "IMAGENAME eq GameTetris++.exe" 2>NUL | find /I "GameTetris++.exe" >NUL
if "%ERRORLEVEL%"=="0" (
    timeout /T 5 /NOBREAK >NUL
    goto check_if_tetris2_running
)

start /min "" "GameTetris++.exe" evaluate skipgame

:check_if_tetrisEvaluate_running
tasklist /FI "IMAGENAME eq GameTetris++.exe" 2>NUL | find /I "GameTetris++.exe" >NUL
if "%ERRORLEVEL%"=="0" (
    timeout /T 5 /NOBREAK >NUL
    goto check_if_tetrisEvaluate_running
)

goto train_ai

exit
