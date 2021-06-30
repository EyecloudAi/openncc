@echo off&cls
cd /d %~dp0
set /p xz=Please make sure you have backed up what you need?(y or n):
if /i "%xz%"=="y" goto y
if /i "%xz%"=="n" goto n
:y
echo y
rd /q /s Source
rd /q /s Example
rd /q /s Viewer
echo mkdir Source....
md Source
echo copy Library....
md Source\Library
copy "..\..\SDK\Source\Library\Windows\C&C++" Source\Library\
echo copy Model....
md Source\Model
md Source\Model\2020.3
xcopy ..\..\SDK\Source\Model\2020.3\ Source\Model\2020.3\ /s/e/y
echo copy Firmware......
md Source\Firmware
xcopy ..\..\SDK\Source\Firmware\ Source\Firmware\ /s/e/y
echo copy Example.....
md Example
xcopy ..\..\SDK\Example\ Example\ /s/e/y
echo copy Viewer.....
md Viewer
xcopy ..\..\SDK\Viewer\ Viewer\ /s/e/y
rd /q /s Viewer\OpenNcc_Linux
rd /q /s Viewer\OpenNCC_Raspberry
echo "copy lib to Viewer"
del Viewer\QT_Package\OpenNCC\ncc_sdk\OpenNCC.lib
copy Source\Library\OpenNCC.lib Viewer\QT_Package\OpenNCC\ncc_sdk\
pause
:n
echo n
pause