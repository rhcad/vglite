set destpath=..\all.tmp\touchvg

del /Q/S  %destpath%

xcopy ..\..\core\include\*.* %destpath%\ /Y/S

xcopy ..\..\core\src\geom\*.* %destpath%\geom\ /Y
xcopy ..\..\core\src\graph\*.* %destpath%\graph\ /Y
xcopy ..\..\core\src\shape\*.* %destpath%\shape\ /Y
xcopy ..\..\core\src\shapedoc\*.* %destpath%\shapedoc\ /Y
xcopy ..\..\core\src\cmdbase\*.* %destpath%\cmdbase\ /Y
xcopy ..\..\core\src\cmdbasic\*.* %destpath%\cmdbasic\ /Y
xcopy ..\..\core\src\cmdmgr\*.* %destpath%\cmdmgr\ /Y
xcopy ..\..\core\src\json\*.* %destpath%\json\ /Y
xcopy ..\..\core\src\view\*.* %destpath%\view\ /Y
xcopy ..\..\core\src\educmds\*.* %destpath%\educmds\ /Y
move /Y %destpath%\storage\mgjsonstorage.h %destpath%\json\

del /Q/S %destpath%\cmdobserver
del /Q/S %destpath%\canvas
del /Q/S %destpath%\storage

rem xcopy ..\..\win\include\canvas\*.* %destpath%\winview\ /Y
rem xcopy ..\..\win\src\canvas\*.* %destpath%\winview\ /Y
rem xcopy ..\..\win\include\view\*.* %destpath%\winview\ /Y
rem xcopy ..\..\win\src\view\*.* %destpath%\winview\ /Y

xcopy ..\..\ios\view\include\*.* %destpath%\iosview\ /Y
xcopy ..\..\ios\view\src\*.* %destpath%\iosview\ /Y

xcopy ..\..\android\demo\src\touchvg\view\*.java %destpath%\android\ /Y
xcopy ..\..\win\csharp\touchvglib\view\*.cs %destpath%\WPF\ /Y
copy dummy.cpp %destpath%\android\
copy dummy.cpp %destpath%\WPF\
