set destpath=..\all.tmp\touchvg

del /Q/S  %destpath%

xcopy ..\..\core\include\*.* %destpath%\ /Y/S

xcopy ..\..\core\src\geom\*.* %destpath%\geom_src\ /Y
xcopy ..\..\core\src\graph\*.* %destpath%\graph_src\ /Y
xcopy ..\..\core\src\shape\*.* %destpath%\shape_src\ /Y
xcopy ..\..\core\src\shapedoc\*.* %destpath%\shapedoc_src\ /Y
xcopy ..\..\core\src\cmdbase\*.* %destpath%\cmdbase_src\ /Y
xcopy ..\..\core\src\cmdbasic\*.* %destpath%\cmdbasic_src\ /Y
xcopy ..\..\core\src\cmdmgr\*.* %destpath%\cmdmgr_src\ /Y
xcopy ..\..\core\src\json\*.* %destpath%\json\ /Y
xcopy ..\..\core\src\view\*.* %destpath%\view_src\ /Y
xcopy ..\..\core\src\educmds\*.* %destpath%\educmds\ /Y
move /Y %destpath%\storage\mgjsonstorage.h %destpath%\json\

rem xcopy ..\..\win\include\canvas\*.* %destpath%\winview\ /Y
rem xcopy ..\..\win\src\canvas\*.* %destpath%\winview_src\ /Y
rem xcopy ..\..\win\include\view\*.* %destpath%\winview\ /Y
rem xcopy ..\..\win\src\view\*.* %destpath%\winview_src\ /Y

xcopy ..\..\ios\view\include\*.* %destpath%\iosview\ /Y
xcopy ..\..\ios\view\src\*.* %destpath%\iosview_src\ /Y

xcopy ..\..\android\demo\src\touchvg\view\*.java %destpath%\android\ /Y
xcopy ..\..\win\csharp\touchvglib\view\*.cs %destpath%\WPF\ /Y
