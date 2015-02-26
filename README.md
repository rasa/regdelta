# regdelta [![Flattr this][flatter_png]][flatter]

Diff, export, and sort Windows registry files.

## Usage

````
regdelta [options] [key | file.reg] [key | file.reg]

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
````

## Examples

````
 regdelta HKCU hkcu.reg              ; compare HKCU key with hkcu.reg
 regdelta HKCU\1 HKCU\2              ; compare HKCU\1 key with HKCU\2 key
 regdelta -o diff.reg HKCU hkcu.reg  ; compare HKCU key with hkcu.reg
 regdelta -x HKCU hkcu.reg           ; export HKCU key to hkcu.reg
 regdelta -xr HKCU\Software ex.reg   ; export HKCU to ex.reg in REGEDIT format
 regdelta -ax allkeys.reg            ; export all keys to allkeys.reg
````

## Notes

````
regdelta.exe  reads and writes ASCII .reg files (Windows 9x/NT 4)
regdeltau.exe reads and writes Unicode .reg files (Windows 2000/XP/2003)

'key' can be one of the following:

HKCC	HKEY_CURRENT_CONFIG
HKCU	HKEY_CURRENT_USER
HKCR	HKEY_CLASSES_ROOT
HKLM	HKEY_LOCAL_MACHINE
HKU		HKEY_USERS
````

Reads entire registry into memory for performance, so this may be an
issue in a low memory environment.

## To Do

Compare currently only works on files created by regdelta. This will be
fixed in a related release.

Add HKA | HKEY_ALL key

Options:
````
-e | --execute 'cmd' ex.reg Execute cmd & save differences to ex.reg
-s | --sort in.reg out.reg  Sort in.reg to out.reg
````

Examples:
````
 regdelta -s input.reg output.reg    ; sort input.reg to output.reg
 regdelta -e 'setup.exe /S' ex.reg   ; run setup & save differences in ex.reg
````

To emulate the --execute option, I've used this batch script, called regdiff.cmd:

````
@echo off
del "%TEMP%\~regdiff_?.reg" >nul 2>nul
regdelta.exe -ax "%TEMP%\~regdiff_1.reg"
start /wait %*
regdelta.exe -ax "%TEMP%\~regdiff_2.reg"
regdelta.exe -o "%~1_redo.reg" "%TEMP%\~regdiff_1.reg" "%TEMP%\~regdiff_2.reg"
regdelta.exe -o "%~1_undo.reg" "%TEMP%\~regdiff_2.reg" "%TEMP%\~regdiff_1.reg"
del "%TEMP%\~regdiff_?.reg" >nul 2>nul
````

## Contributing

To contribute to this project, please see [CONTRIBUTING.md](CONTRIBUTING.md).

## Bugs

To view existing bugs, or report a new bug, please see [issues](../../issues).

## Changelog

To view the version history for this project, please see [CHANGELOG.md](CHANGELOG.md).

## License

This project is [MIT licensed](LICENSE).

## Contact

This project was created and is maintained by [Ross Smith II][] [![endorse][endorse_png]][endorse]

Feedback, suggestions, and enhancements are welcome.

[Ross Smith II]: mailto:ross@smithii.com "ross@smithii.com"
[flatter]: https://flattr.com/submit/auto?user_id=rasa&url=https%3A%2F%2Fgithub.com%2Frasa%2Fregdelta
[flatter_png]: http://button.flattr.com/flattr-badge-large.png "Flattr this"
[endorse]: https://coderwall.com/rasa
[endorse_png]: https://api.coderwall.com/rasa/endorsecount.png "endorse"

