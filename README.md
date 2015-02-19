regdelta 0.2 - 19 Jul 2006
Copyright (c) 2005-2006 Ross Smith II (http://smithii.com) All Rights Reserved

------------------------------------------------------------------------------

Usage: regdelta [options] [key | file.reg] [key | file.reg]
Options:
-a | --all                  Compare/export all keys (HKCC HKCU HKCR HKLM HKU)
-x | --export key(s) ex.reg Export key(s) to ex.reg
-o | --output output.reg    Set output filename to 'output.reg'
-c | --comments             Include original values as comments in diff
-r | --regedit              Export in RegEdit format (implies -u -w)
-u | --unsorted             Do not sort the keys or values, as RegEdit
-w | --wordwrap             Wordwrap at 78 chars, as RegEdit
-v | --verbose              Show warnings that are normally hidden
-V | --version              Show version & copyright information & quit
-? | --help                 Show this help message and quit

Examples:
 regdelta HKCU hkcu.reg              ; compare HKCU key with hkcu.reg
 regdelta HKCU\1 HKCU\2              ; compare HKCU\1 key with HKCU\2 key
 regdelta -o diff.reg HKCU hkcu.reg  ; compare HKCU key with hkcu.reg
 regdelta -x HKCU hkcu.reg           ; export HKCU key to hkcu.reg
 regdelta -xr HKCU\Software ex.reg   ; export HKCU to ex.reg in REGEDIT format
 regdelta -ax allkeys.reg            ; export all keys to allkeys.reg

------------------------------------------------------------------------------

Notes:

regdelta.exe  reads and writes ASCII .reg files (Windows 9x/NT 4)
regdeltau.exe reads and writes Unicode .reg files (Windows 2000/XP/2003)

'key' can be one of the following:

HKCC	HKEY_CURRENT_CONFIG 
HKCU	HKEY_CURRENT_USER 
HKCR	HKEY_CLASSES_ROOT 
HKLM	HKEY_LOCAL_MACHINE 
HKU		HKEY_USERS

Reads entire registry into memory for performance, so this may be an
issue in a low memory environment.

------------------------------------------------------------------------------

To Do List:

Compare currently only works on files created by regdelta. This will be
fixed in a related release.

Add HKA | HKEY_ALL key

Options:
-e | --execute 'cmd' ex.reg Execute cmd & save differences to ex.reg
-s | --sort in.reg out.reg  Sort in.reg to out.reg

Examples:
 regdelta -s input.reg output.reg    ; sort input.reg to output.reg
 regdelta -e 'setup.exe /S' ex.reg   ; run setup & save differences in ex.reg

To emulate the --execute option, I've used this batch script, called regdiff.cmd:

@echo off
del "%TEMP%\~regdiff_?.reg" >nul 2>nul
regdelta.exe -ax "%TEMP%\~regdiff_1.reg"
start /wait %*
regdelta.exe -ax "%TEMP%\~regdiff_2.reg"
regdelta.exe -o "%~1_redo.reg" "%TEMP%\~regdiff_1.reg" "%TEMP%\~regdiff_2.reg"
regdelta.exe -o "%~1_undo.reg" "%TEMP%\~regdiff_2.reg" "%TEMP%\~regdiff_1.reg"
del "%TEMP%\~regdiff_?.reg" >nul 2>nul

------------------------------------------------------------------------------

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

$Id$
