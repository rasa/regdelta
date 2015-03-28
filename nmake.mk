all:
	nmake /nologo /f regdelta.mak CFG="regdelta - Win32 Release"		NO_EXTERNAL_DEPS=1	all
	nmake /nologo /f regdelta.mak CFG="regdelta - Win32 Debug"		NO_EXTERNAL_DEPS=1	all
	nmake /nologo /f regdelta.mak CFG="regdelta - Win32 Debug_Unicode"	NO_EXTERNAL_DEPS=1	all
	nmake /nologo /f regdelta.mak CFG="regdelta - Win32 Release_Unicode"	NO_EXTERNAL_DEPS=1	all

clean:
	nmake /nologo /f regdelta.mak CFG="regdelta - Win32 Release"		clean
	nmake /nologo /f regdelta.mak CFG="regdelta - Win32 Debug"		clean
	nmake /nologo /f regdelta.mak CFG="regdelta - Win32 Debug_Unicode"	clean
	nmake /nologo /f regdelta.mak CFG="regdelta - Win32 Release_Unicode"	clean
	
.PHONY:	all clean
