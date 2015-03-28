all:
	MSBuild.exe /nologo regdelta.sln /p:Configuration=Debug
	MSBuild.exe /nologo regdelta.sln /p:Configuration=Release
	MSBuild.exe /nologo regdelta.sln /p:Configuration=Debug_Unicode
	MSBuild.exe /nologo regdelta.sln /p:Configuration=Release_Unicode

clean:
	MSBuild.exe /nologo regdelta.sln /p:Configuration=Debug           /t:clean
	MSBuild.exe /nologo regdelta.sln /p:Configuration=Release         /t:clean
	MSBuild.exe /nologo regdelta.sln /p:Configuration=Debug_Unicode   /t:clean
	MSBuild.exe /nologo regdelta.sln /p:Configuration=Release_Unicode /t:clean

upgrade:
	devenv regdelta.sln /upgrade
	
.PHONY:	all clean upgrade


