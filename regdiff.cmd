@echo off
del "%TEMP%\~regdiff_?.reg" >nul 2>nul
regdelta.exe -ax "%TEMP%\~regdiff_1.reg"
start /wait %*
regdelta.exe -ax "%TEMP%\~regdiff_2.reg"
regdelta.exe -o "%~1_redo.reg" "%TEMP%\~regdiff_1.reg" "%TEMP%\~regdiff_2.reg"
regdelta.exe -o "%~1_undo.reg" "%TEMP%\~regdiff_2.reg" "%TEMP%\~regdiff_1.reg"
del "%TEMP%\~regdiff_?.reg" >nul 2>nul
