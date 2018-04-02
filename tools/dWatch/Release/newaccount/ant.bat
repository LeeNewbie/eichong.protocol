@echo off
D:
if "%1"=="ep" cd D:\idea\eichong\dEpGate
if "%1"=="dp" cd D:\idea\eichong\dPhoneGate
if "%1"=="nc" cd D:\idea\eichong\libNetCore
if "%1"=="co" cd D:\idea\eichong\libCooperate
if "%1"=="ec" cd D:\idea\eichong\libECCommon
if "%1"=="oc" cd D:\idea\eichong\ormcore
if "%1"=="ul" cd D:\idea\eichong\usrlayer
call ant
rem if "%2"=="oc" cd D:\idea\eichong\ormcore
rem if "%2"=="ul" cd D:\idea\eichong\usrlayer
rem call ant
rem pause