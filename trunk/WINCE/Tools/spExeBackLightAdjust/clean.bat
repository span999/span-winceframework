@echo off

@echo ===============================================================
@echo "Clean work space new, and please remove binary yourself then!!"
@echo ===============================================================


del /F /Q *.sln
del /F /Q *.ncb
del /F /Q /A H *.suo
del /F /Q *.user