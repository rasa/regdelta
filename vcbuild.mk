all:
	VCBuild.exe /nologo regdelta.vcproj /rebuild

clean:
	VCBuild.exe /nologo regdelta.vcproj /clean
	
upgrade:
	devenv regdelta.sln /upgrade

.PHONY:	all clean upgrade

