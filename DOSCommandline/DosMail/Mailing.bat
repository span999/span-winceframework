@echo off

set ML_MAILBODYFILE=%1
set ML_MAILLISTFILE=%2
set ML_SUBJECT=svn_notification
set ML_SMTP_SERVER=smtp.gmail.com
set ML_MY_MAILADDR=spanliu@gmail.com
set ML_MY_USERNAME=spanliu
set ML_MY_PASSWORD=gmail999

del /F /Q msgbody.txt
del /F /Q maillist.txt

if not "%ML_MAILBODYFILE%" == "" goto EXITP1CHECK
set ML_MAILBODYFILE=msgbody.txt
:EXITP1CHECK

if not "%ML_MAILLISTFILE%" == "" goto EXITP2CHECK
set ML_MAILLISTFILE=maillist.txt
:EXITP2CHECK

if not exist %ML_MAILBODYFILE% (
set ML_MAILBODYFILE=msgbody.txt
@echo This is a fake message !!! >> msgbody.txt
)

if not exist %ML_MAILLISTFILE% (
set ML_MAILLISTFILE=maillist.txt
@echo span.liu@hp.com >> maillist.txt
)


@echo install stunnel service
stunnel -install -quiet

@echo activate stunnel service
net start stunnel

@echo sent mail...
blat -install %ML_SMTP_SERVER% %ML_MY_MAILADDR% >> blat.log
REM blat %ML_MAILBODYFILE% -subject "%ML_SUBJECT%" -to span.liu@hp.com -u spanliu -pw gmail999 -f spanliu -debug -server 127.0.0.1:1099 >> blat.log
blat %ML_MAILBODYFILE% -subject "%ML_SUBJECT%" -tf %ML_MAILLISTFILE% -u %ML_MY_USERNAME% -pw %ML_MY_PASSWORD% -f %ML_MY_USERNAME% -debug -server 127.0.0.1:1099 >> blat.log

@echo deactivate stunnel service
net stop stunnel

@echo uninstall stunnel service
stunnel -uninstall -quiet

