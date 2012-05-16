@ECHO OFF
SETLOCAL ENABLEEXTENSIONS

FOR /F "tokens=1,4" %%A in ('"git ls-files -s"') DO IF %%A==120000 CALL :LINKCREATE %%B
GOTO :DONE

:FOLDERCHECK
set DFLAG=
set ATTR2=%~a1
PUSHD %~dp1
set TMP1=%~f2
set ATTR=%~a2
IF /I "%ATTR2:~8,1%"=="l" GOTO :COMP
IF NOT EXIST %2 GOTO :COMP
IF /I "%ATTR:~0,1%"=="d" SET DFLAG=/D
IF EXIST %~nx1 DEL %~nx1
MKLINK %DFLAG% %~nx1 %2
git update-index --assume-unchanged %~nx1
:COMP
POPD
GOTO :DONE


:LINKCREATE
SET FNAME=%1
SET FNAME=%FNAME:/=\%
FOR /F %%I IN (%FNAME%) DO SET LNAME=%%I
SET LNAME=%LNAME:/=\%
call :FOLDERCHECK %FNAME% %LNAME%
:DONE