# Microsoft Developer Studio Generated NMAKE File, Based on regdelta.dsp
!IF "$(CFG)" == ""
CFG=regdelta - Win32 Debug_Unicode
!MESSAGE No configuration specified. Defaulting to regdelta - Win32 Debug_Unicode.
!ENDIF 

!IF "$(CFG)" != "regdelta - Win32 Release" && "$(CFG)" != "regdelta - Win32 Debug" && "$(CFG)" != "regdelta - Win32 Debug_Unicode" && "$(CFG)" != "regdelta - Win32 Release_Unicode"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "regdelta.mak" CFG="regdelta - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "regdelta - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "regdelta - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "regdelta - Win32 Debug_Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE "regdelta - Win32 Release_Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "regdelta - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\regdelta.exe"


CLEAN :
	-@erase "$(INTDIR)\regdelta.obj"
	-@erase "$(INTDIR)\regdelta.res"
	-@erase "$(INTDIR)\RegFile.obj"
	-@erase "$(INTDIR)\RegFileReader.obj"
	-@erase "$(INTDIR)\RegFileWriter.obj"
	-@erase "$(INTDIR)\RegValue.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\regdelta.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\lib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /U "UNICODE" /U "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\regdelta.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\regdelta.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=shared.lib Setupapi.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\regdelta.pdb" /machine:I386 /out:"$(OUTDIR)\regdelta.exe" 
LINK32_OBJS= \
	"$(INTDIR)\regdelta.obj" \
	"$(INTDIR)\RegFile.obj" \
	"$(INTDIR)\RegFileReader.obj" \
	"$(INTDIR)\RegFileWriter.obj" \
	"$(INTDIR)\RegValue.obj" \
	"$(INTDIR)\regdelta.res"

"$(OUTDIR)\regdelta.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\regdelta.exe" "$(OUTDIR)\regdelta.bsc"


CLEAN :
	-@erase "$(INTDIR)\regdelta.obj"
	-@erase "$(INTDIR)\regdelta.res"
	-@erase "$(INTDIR)\regdelta.sbr"
	-@erase "$(INTDIR)\RegFile.obj"
	-@erase "$(INTDIR)\RegFile.sbr"
	-@erase "$(INTDIR)\RegFileReader.obj"
	-@erase "$(INTDIR)\RegFileReader.sbr"
	-@erase "$(INTDIR)\RegFileWriter.obj"
	-@erase "$(INTDIR)\RegFileWriter.sbr"
	-@erase "$(INTDIR)\RegValue.obj"
	-@erase "$(INTDIR)\RegValue.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\regdelta.bsc"
	-@erase "$(OUTDIR)\regdelta.exe"
	-@erase "$(OUTDIR)\regdelta.ilk"
	-@erase "$(OUTDIR)\regdelta.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\lib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /U "UNICODE" /U "_UNICODE" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\regdelta.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\regdelta.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\regdelta.sbr" \
	"$(INTDIR)\RegFile.sbr" \
	"$(INTDIR)\RegFileReader.sbr" \
	"$(INTDIR)\RegFileWriter.sbr" \
	"$(INTDIR)\RegValue.sbr"

"$(OUTDIR)\regdelta.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=sharedd.lib Setupapi.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\regdelta.pdb" /debug /machine:I386 /out:"$(OUTDIR)\regdelta.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\regdelta.obj" \
	"$(INTDIR)\RegFile.obj" \
	"$(INTDIR)\RegFileReader.obj" \
	"$(INTDIR)\RegFileWriter.obj" \
	"$(INTDIR)\RegValue.obj" \
	"$(INTDIR)\regdelta.res"

"$(OUTDIR)\regdelta.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug_Unicode"

OUTDIR=.\Debug_Unicode
INTDIR=.\Debug_Unicode
# Begin Custom Macros
OutDir=.\Debug_Unicode
# End Custom Macros

ALL : "$(OUTDIR)\regdeltaU.exe" "$(OUTDIR)\regdelta.bsc"


CLEAN :
	-@erase "$(INTDIR)\regdelta.obj"
	-@erase "$(INTDIR)\regdelta.res"
	-@erase "$(INTDIR)\regdelta.sbr"
	-@erase "$(INTDIR)\RegFile.obj"
	-@erase "$(INTDIR)\RegFile.sbr"
	-@erase "$(INTDIR)\RegFileReader.obj"
	-@erase "$(INTDIR)\RegFileReader.sbr"
	-@erase "$(INTDIR)\RegFileWriter.obj"
	-@erase "$(INTDIR)\RegFileWriter.sbr"
	-@erase "$(INTDIR)\RegValue.obj"
	-@erase "$(INTDIR)\RegValue.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\regdelta.bsc"
	-@erase "$(OUTDIR)\regdeltaU.exe"
	-@erase "$(OUTDIR)\regdeltaU.ilk"
	-@erase "$(OUTDIR)\regdeltaU.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\lib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\regdelta.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\regdelta.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\regdelta.sbr" \
	"$(INTDIR)\RegFile.sbr" \
	"$(INTDIR)\RegFileReader.sbr" \
	"$(INTDIR)\RegFileWriter.sbr" \
	"$(INTDIR)\RegValue.sbr"

"$(OUTDIR)\regdelta.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=shareddu.lib Setupapi.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\regdeltaU.pdb" /debug /machine:I386 /out:"$(OUTDIR)\regdeltaU.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\regdelta.obj" \
	"$(INTDIR)\RegFile.obj" \
	"$(INTDIR)\RegFileReader.obj" \
	"$(INTDIR)\RegFileWriter.obj" \
	"$(INTDIR)\RegValue.obj" \
	"$(INTDIR)\regdelta.res"

"$(OUTDIR)\regdeltaU.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "regdelta - Win32 Release_Unicode"

OUTDIR=.\Release_Unicode
INTDIR=.\Release_Unicode
# Begin Custom Macros
OutDir=.\Release_Unicode
# End Custom Macros

ALL : "$(OUTDIR)\regdeltaU.exe"


CLEAN :
	-@erase "$(INTDIR)\regdelta.obj"
	-@erase "$(INTDIR)\regdelta.res"
	-@erase "$(INTDIR)\RegFile.obj"
	-@erase "$(INTDIR)\RegFileReader.obj"
	-@erase "$(INTDIR)\RegFileWriter.obj"
	-@erase "$(INTDIR)\RegValue.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\regdeltaU.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\lib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "UNICODE" /D "_UNICODE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\regdelta.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\regdelta.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=sharedu.lib Setupapi.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\regdeltaU.pdb" /machine:I386 /out:"$(OUTDIR)\regdeltaU.exe" 
LINK32_OBJS= \
	"$(INTDIR)\regdelta.obj" \
	"$(INTDIR)\RegFile.obj" \
	"$(INTDIR)\RegFileReader.obj" \
	"$(INTDIR)\RegFileWriter.obj" \
	"$(INTDIR)\RegValue.obj" \
	"$(INTDIR)\regdelta.res"

"$(OUTDIR)\regdeltaU.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("regdelta.dep")
!INCLUDE "regdelta.dep"
!ELSE 
!MESSAGE Warning: cannot find "regdelta.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "regdelta - Win32 Release" || "$(CFG)" == "regdelta - Win32 Debug" || "$(CFG)" == "regdelta - Win32 Debug_Unicode" || "$(CFG)" == "regdelta - Win32 Release_Unicode"
SOURCE=.\regdelta.cpp

!IF  "$(CFG)" == "regdelta - Win32 Release"


"$(INTDIR)\regdelta.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug"


"$(INTDIR)\regdelta.obj"	"$(INTDIR)\regdelta.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug_Unicode"


"$(INTDIR)\regdelta.obj"	"$(INTDIR)\regdelta.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Release_Unicode"


"$(INTDIR)\regdelta.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RegFile.cpp

!IF  "$(CFG)" == "regdelta - Win32 Release"


"$(INTDIR)\RegFile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug"


"$(INTDIR)\RegFile.obj"	"$(INTDIR)\RegFile.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug_Unicode"


"$(INTDIR)\RegFile.obj"	"$(INTDIR)\RegFile.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Release_Unicode"


"$(INTDIR)\RegFile.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RegFileReader.cpp

!IF  "$(CFG)" == "regdelta - Win32 Release"


"$(INTDIR)\RegFileReader.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug"


"$(INTDIR)\RegFileReader.obj"	"$(INTDIR)\RegFileReader.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug_Unicode"


"$(INTDIR)\RegFileReader.obj"	"$(INTDIR)\RegFileReader.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Release_Unicode"


"$(INTDIR)\RegFileReader.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RegFileWriter.cpp

!IF  "$(CFG)" == "regdelta - Win32 Release"


"$(INTDIR)\RegFileWriter.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug"


"$(INTDIR)\RegFileWriter.obj"	"$(INTDIR)\RegFileWriter.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug_Unicode"


"$(INTDIR)\RegFileWriter.obj"	"$(INTDIR)\RegFileWriter.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Release_Unicode"


"$(INTDIR)\RegFileWriter.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RegValue.cpp

!IF  "$(CFG)" == "regdelta - Win32 Release"


"$(INTDIR)\RegValue.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug"


"$(INTDIR)\RegValue.obj"	"$(INTDIR)\RegValue.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Debug_Unicode"


"$(INTDIR)\RegValue.obj"	"$(INTDIR)\RegValue.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "regdelta - Win32 Release_Unicode"


"$(INTDIR)\RegValue.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\regdelta.rc

"$(INTDIR)\regdelta.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

