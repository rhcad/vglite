set destpath=..\all.tmp\touchvg

del /Q/S  %destpath%

xcopy ..\..\core\include\geom\*.* %destpath%\geom\ /Y
xcopy ..\..\core\include\graph\*.* %destpath%\graph\ /Y
xcopy ..\..\core\include\shape\*.* %destpath%\shape\ /Y
xcopy ..\..\core\include\storage\*.* %destpath%\storage\ /Y
xcopy ..\..\core\include\command\*.* %destpath%\command\ /Y
xcopy ..\..\core\include\canvas\*.* %destpath%\canvas\ /Y
xcopy ..\..\core\include\view\*.* %destpath%\view\ /Y
xcopy ..\..\core\include\tradecmd\*.* %destpath%\tradecmd\ /Y

xcopy ..\..\core\src\geom\*.* %destpath%\geom\ /Y
xcopy ..\..\core\src\graph\*.* %destpath%\graph\ /Y
xcopy ..\..\core\src\shape\*.* %destpath%\shape\ /Y
xcopy ..\..\core\src\command\*.* %destpath%\command\ /Y
xcopy ..\..\core\src\json\*.* %destpath%\json\ /Y
xcopy ..\..\core\src\view\*.* %destpath%\view\ /Y
xcopy ..\..\core\src\tradecmd\*.* %destpath%\demotrade\ /Y
move /Y %destpath%\storage\mgjsonstorage.h %destpath%\json\

rem xcopy ..\..\win\include\canvas\*.* %destpath%\winview\ /Y
rem xcopy ..\..\win\src\canvas\*.* %destpath%\winview\ /Y
rem xcopy ..\..\win\include\view\*.* %destpath%\winview\ /Y
rem xcopy ..\..\win\src\view\*.* %destpath%\winview\ /Y

xcopy ..\..\ios\view\include\*.* %destpath%\iosview\ /Y
xcopy ..\..\ios\view\src\*.* %destpath%\iosview\ /Y

xcopy ..\..\android\demo\src\touchvg\view\*.java %destpath%\android\ /Y
xcopy ..\..\win\csharp\touchvglib\view\*.cs %destpath%\WPF\ /Y
