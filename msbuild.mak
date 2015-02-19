.PHONY:	all clean realclean

all:
	MSBuild.exe regdelta.sln /t:Build /p:Configuration=Debug
	MSBuild.exe regdelta.sln /t:Build /p:Configuration=Release
	MSBuild.exe regdelta.sln /t:Build /p:Configuration=Debug_Unicode
	MSBuild.exe regdelta.sln /t:Build /p:Configuration=Release_Unicode

clean:
	MSBuild.exe regdelta.sln /t:Clean /p:Configuration=Debug
	MSBuild.exe regdelta.sln /t:Clean /p:Configuration=Release
	MSBuild.exe regdelta.sln /t:Clean /p:Configuration=Debug_Unicode
	MSBuild.exe regdelta.sln /t:Clean /p:Configuration=Release_Unicode

realclean: clean
	-cmd /c del /s *.bak *.bsc *.idb *.pdb *.lib *.ncb *.obj *.opt *.pch *.plg *.sbr
