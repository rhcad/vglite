set destpath=..\all.tmp\touchvg

del /Q/S  %destpath%

xcopy ..\..\core\include\geom\*.* %destpath%\geom\ /Y
xcopy ..\..\core\include\graph\*.* %destpath%\graph\ /Y
xcopy ..\..\core\include\shape\*.* %destpath%\shape\ /Y
xcopy ..\..\core\include\command\*.* %destpath%\command\ /Y
xcopy ..\..\core\include\callback\*.* %destpath%\callback\ /Y
xcopy ..\..\core\include\view\*.* %destpath%\view\ /Y

xcopy ..\..\core\src\geom\*.* %destpath%\geom_src\ /Y
xcopy ..\..\core\src\graph\*.* %destpath%\graph_src\ /Y
xcopy ..\..\core\src\shape\*.* %destpath%\shape_src\ /Y
xcopy ..\..\core\src\command\*.* %destpath%\command_src\ /Y
xcopy ..\..\core\src\json\*.* %destpath%\json_src\ /Y
xcopy ..\..\core\src\view\*.* %destpath%\view_src\ /Y

xcopy ..\..\win\include\canvas\*.* %destpath%\winview\ /Y
xcopy ..\..\win\src\canvas\*.* %destpath%\winview_src\ /Y
xcopy ..\..\win\include\view\*.* %destpath%\winview\ /Y
xcopy ..\..\win\src\view\*.* %destpath%\winview_src\ /Y

xcopy ..\..\ios\view\include\*.h %destpath%\iosview\ /Y
xcopy ..\..\ios\view\src\*.* %destpath%\iosview_src\ /Y

xcopy ..\..\android\demo\src\touchvg\jni\*.java %destpath%\AndroidJNI\ /Y
xcopy ..\..\android\demo\src\touchvg\view\*.java %destpath%\AndroidView\ /Y
