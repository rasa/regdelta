.PHONY:	all clean realclean

all:
	$(MAKE) /f regdelta.mak CFG="regdelta - Win32 Release"		all
	$(MAKE) /f regdelta.mak CFG="regdelta - Win32 Debug"		all
	$(MAKE) /f regdelta.mak CFG="regdelta - Win32 Debug_Unicode"	all
	$(MAKE) /f regdelta.mak CFG="regdelta - Win32 Release_Unicode"	all

clean:
	$(MAKE) /f regdelta.mak CFG="regdelta - Win32 Release"		clean
	$(MAKE) /f regdelta.mak CFG="regdelta - Win32 Debug"		clean
	$(MAKE) /f regdelta.mak CFG="regdelta - Win32 Debug_Unicode"	clean
	$(MAKE) /f regdelta.mak CFG="regdelta - Win32 Release_Unicode"	clean

realclean: clean
	-cmd /c del /s *.bak *.bsc *.idb *.pdb *.lib *.ncb *.obj *.opt *.pch *.plg *.sbr

