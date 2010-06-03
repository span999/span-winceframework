@echo off

cls

build -c

cd ..\..\csp\amss\drivers\siousb\
build -c
cd ..\..\..\..\shared\spCElog\

cd ..\..\csp\amss\drivers\sio\
build -c
cd ..\..\..\..\shared\spCElog\

cd ..\..\csp\amss\services\diag\
build -c
cd ..\..\..\..\shared\spCElog\

cd ..\..\Src\Drivers\windiag\
build -c
cd ..\..\..\shared\spCElog\


