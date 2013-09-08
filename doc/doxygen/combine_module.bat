set destpath=..\all.tmp\touchvg

del /Q/S  %destpath%

xcopy ..\..\core\include\geom\*.h %destpath%\geom\ /Y
xcopy ..\..\core\include\graph\*.h %destpath%\graph\ /Y
xcopy ..\..\core\include\shape\*.h %destpath%\shape\ /Y
xcopy ..\..\core\include\storage\*.h %destpath%\storage\ /Y
xcopy ..\..\core\include\command\*.h %destpath%\command\ /Y
xcopy ..\..\core\include\callback\*.h %destpath%\callback\ /Y
xcopy ..\..\core\include\view\*.h %destpath%\view\ /Y

xcopy ..\..\core\src\geom\*.h %destpath%\geom\ /Y
xcopy ..\..\core\src\geom\*.cpp %destpath%\geom\ /Y
xcopy ..\..\core\src\graph\*.h %destpath%\graph\ /Y
xcopy ..\..\core\src\graph\*.cpp %destpath%\graph\ /Y
xcopy ..\..\core\src\shape\*.h %destpath%\shape\ /Y
xcopy ..\..\core\src\shape\*.cpp %destpath%\shape\ /Y
xcopy ..\..\core\src\command\*.h %destpath%\command\ /Y
xcopy ..\..\core\src\command\*.cpp %destpath%\command\ /Y
xcopy ..\..\core\src\json\*.h %destpath%\json\ /Y
xcopy ..\..\core\src\json\*.cpp %destpath%\json\ /Y
xcopy ..\..\core\src\view\*.h %destpath%\view\ /Y
xcopy ..\..\core\src\view\*.cpp %destpath%\view\ /Y
move /Y %destpath%\storage\mgjsonstorage.h %destpath%\json\

rem xcopy ..\..\win\include\canvas\*.h %destpath%\winview\ /Y
rem xcopy ..\..\win\src\canvas\*.h %destpath%\winview\ /Y
rem xcopy ..\..\win\src\canvas\*.cpp %destpath%\winview\ /Y
rem xcopy ..\..\win\include\view\*.h %destpath%\winview\ /Y
rem xcopy ..\..\win\src\view\*.h %destpath%\winview\ /Y
rem xcopy ..\..\win\src\view\*.cpp %destpath%\winview\ /Y

xcopy ..\..\ios\view\include\*.h %destpath%\iosview\ /Y
xcopy ..\..\ios\view\src\*.mm %destpath%\iosview\ /Y

rem xcopy ..\..\android\demo\src\touchvg\view\*.java %destpath%\android\ /Y
rem xcopy ..\..\win\csharp\touchvglib\view\*.cs %destpath%\WPF\ /Y
