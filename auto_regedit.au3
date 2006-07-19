; Application Name - AutoIt unattended installation script
; Last tested with version x.xx

_init()

$_exe = 'c:\\windows\\regedit.exe'
$_max_runtime	= 600

$title	= _next_arg('Registry Editor')

_run()

While _still_running()
	Select
		Case _WinExists($title, 'My Computer', 0, 1)
			Send('{HOME}')
			Send('!FE!nALL_8_regedit.reg!tW!S!Y')
			Sleep(1000)
			While MouseGetCursor() = 15
				Sleep(1)
			Wend
			
			Send('{HOME}')
			Send('{DOWN}')
			Send('!FE!nHKCR_8_regedit.reg!tW!S!Y')
			Sleep(1000)
			While MouseGetCursor() = 15
				Sleep(1)
			Wend

			Send('{HOME}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('!FE!nHKCU_8_regedit.reg!tW!S!Y')
			Sleep(1000)
			While MouseGetCursor() = 15
				Sleep(1)
			Wend

			Send('{HOME}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('!FE!nHKLM_8_regedit.reg!tW!S!Y')
			Sleep(1000)
			While MouseGetCursor() = 15
				Sleep(1)
			Wend

			Send('{HOME}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('!FE!nHKU_8_regedit.reg!tW!S!Y')
			Sleep(1000)
			While MouseGetCursor() = 15
				Sleep(1)
			Wend

			Send('{HOME}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('{DOWN}')
			Send('!FE!nHKCC_8_regedit.reg!tW!S!Y')
			Sleep(1000)
			While MouseGetCursor() = 15
				Sleep(1)
			Wend

			ExitLoop

		Case Else
			_WinGetText($title)
	EndSelect
Wend

_fini()

; zinstall AU3 library 1.0.101 - 03-Mar-06, do not modify anything below, 
; or your changes will be overwritten the next time the library is updated
; Copyright (c) 2006, Ross Smith II (http://smithii.com) All Rights Reserved

Func _WinExists($title, $text, $current_state = 0, $new_state = 0)
	Global $_state

	_log(StringFormat('_WinExists(''%s'', ''%s'', %s, %s)', $title, $text, $current_state, $new_state))

	If $current_state <> $_state Then 
;		_log(StringFormat('_WinExists: current_state(%s) != $_state(%s)', $current_state, $_state))
		Return 0
	EndIf
	
	If Not WinExists($title, $text) Then 
;		_log(StringFormat('_WinExists: WinExists(''%s'', ''%s'') returned 0', $title, $text))
		Local $gettext = WinGetText($title)
;		_log(StringFormat('_WinExists: WinGetText(''%s'')=''%s''', $title, $gettext))
		Return 0
	EndIf

	_logvar('_WinExists', '$_state', $_state)
	_log(StringFormat('_WinExists: Found ''%s'' in window ''%s''', $text, $title))

	If $new_state > 0 Then
		$_state = $new_state
		_logvar('_WinExists', '$_state', $_state)
	EndIf
	
	Return 1
EndFunc

Func _WinGetText($title)
	Global $_state

	_logvar('_WinGetText', '$_state',	$_state)
	_logvar('_WinGetText', '$title',	$title)

	_log(StringFormat('_WinGetText: WinGetText($title)  returned ''%s''', WinGetText($title)))
	_log(StringFormat('_WinGetText: WinGetTitle($title) returned ''%s''', WinGetTitle($title)))
	_log(StringFormat('_WinGetText: WinGetTitle('''') returned ''%s''', WinGetTitle('')))
EndFunc

Func _argv($n)
	Local $rv

	If $CmdLine[0] < $n Then
		$rv = ''
	Else
		$rv = _trim(_unquote($CmdLine[$n]))
	EndIf

	_logvar('_argv', '$n', $n)
	_logvar('_argv', '$rv', $rv)

	Return $rv
EndFunc

Func _get_exe()
	Global $_exe
	
	return $_exe
EndFunc

Func _change_state($inc = 1, $substate = -1)
	Global $_state
	Global $_substate

	_logvar('_change_state', '$inc',		$inc)
	_logvar('_change_state', '$substate',	$substate)
	_logvar('_change_state', '$_state',		$_state)
	_logvar('_change_state', '$_substate',	$_substate)
	
	If $inc <> 0 Then
		$_state = $_state + $inc
		$_substate = 0
		_logvar('_change_state', '$_state',		$_state)
		_logvar('_change_state', '$_substate',	$_substate)
		Return $_state
	EndIf
	
	If $substate = -1 Then
		$_substate = $_substate + 1
	Else
		$_substate = $substate
	EndIf

	_logvar('_change_state', '$_substate',	$_substate)

	Return $_state
EndFunc

Func _find_exe($glob = '*.exe')
	_logvar('_find_exe', '$glob', $glob)

	Local $executable = _next_arg()

	If $executable > '' And Not FileExists($executable) Then 
		_msgbox(StringFormat('The file ''%s'' does not exist in %s', $executable, @WorkingDir))
		Exit(1)
	Endif

	If $executable = '' Then
		Local $search = FileFindFirstFile($glob)

		If $search <> -1 Then
			While $executable = ''
				Local $file = FileFindNextFile($search) 
				If @error Then 
					ExitLoop
				EndIf
				$file = StringLower($file)
				_logvar('_find_exe', '$file', $file)
				If StringInStr($file, '_install.') Then 
					ContinueLoop
				EndIf
				$executable = $file
			Wend

			FileClose($search)
		EndIf

		If $executable = '' Then
			_msgbox(StringFormat('No files matching %s were found in %s', $glob, @WorkingDir))
			Exit(2)
		Endif
	Endif
	
	return $executable
EndFunc

Func _fini()
	Global $_exitval
	Global $_pid
	Global $_still_time

	If ProcessExists($_pid) Then 
		_sleep()
		_log(StringFormat('_fini: Closing PID %s', $_pid))
		ProcessClose($_pid)
	Else
		_log(StringFormat('_fini: PID %s does not exit', $_pid))
	Endif

	If Not $_still_time Then 
		$_exitval = 3
	EndIf

	_logvar('_fini', '$_exitval', $_exitval)

	Exit($_exitval)
EndFunc

Func _init()
	Global $_exe			= ''
	Global $_exitval		= 0
	Global $_initialized	= 0
	Global $_log_name		= @TempDir & '\' & @ScriptName & '.log'
	Global $_max_runtime	= 60	; seconds to run before aborting
	Global $_msgbox_time	= 10	; seconds to display messages
	Global $_next_arg		= 0
	Global $_pause_time		= 1000	; milliseconds to sleep before retry
	Global $_pid			= 0
	Global $_start_time		= 0
	Global $_still_time		= 0
	Global $_state			= 0
	Global $_substate		= 0

	If FileExists($_log_name) Then 
		FileDelete($_log_name)
	EndIf

	_logvar('_init', '@ScriptFullPath', @ScriptFullPath)
	_logvar('_init', '@WorkingDir', @WorkingDir)
	_logvar('_init', '@OSLang', @OSLang)
	_logvar('_init', '@OSVersion', @OSVersion)
	_logvar('_init', '@OSBuild', @OSBuild)
	_logvar('_init', '@OSServicePack', @OSServicePack)
	
	_logvar('_init', '$CmdLine[0]', $CmdLine[0])
	Local $i
	For $i = 1 to $CmdLine[0]
		_logvar('_init', StringFormat('CmdLine[%d]', $i), $CmdLine[$i])
	Next

	$_exe = _find_exe()
	$_initialized = 1
EndFunc

Func _log($s)
	Global $_log_name

	Local $fh = FileOpen($_log_name, 1)
	If $fh = -1 Then 
		SetError(1)
		Return 0
	EndIf

	Local $rv = FileWriteLine($fh, @YEAR & "-" & @MON & "-" & @MDAY & " " & @HOUR & ":" & @MIN & ":" & @SEC & ": " & $s)
	If $rv < 0 Then
		SetError(2)
	EndIf

	FileClose($fh)
	Return $rv
EndFunc

Func _logvar($func, $var, $value)
	_log(StringFormat('%s: %-20s =''%s'''))
EndFunc

Func _msgbox($s)
	Global $_msgbox_time

	_log(StringFormat('%s: %s', @ScriptName, $s))

	MsgBox(0x1030, @ScriptName, $s, $_msgbox_time)
EndFunc

Func _next_arg($default = '')
	Global $_next_arg

	_logvar('_next_arg', '$default', $default)
	
	If $CmdLine[0] > $_next_arg Then 
		$_next_arg = $_next_arg + 1
		Local $rv = _argv($_next_arg)
		_log(StringFormat('_next_arg: Returning _argv(%s)=''%s''', $_next_arg, $rv))
		Return $rv;
	EndIf

	_log(StringFormat('_next_arg: Returning ''%s''', $default))

	Return $default
EndFunc

Func _ProcessExists()
	Global $_pid

	If Not ProcessExists($_pid) Then
		_log(StringFormat('_ProcessExists: PID %s was not found', $_pid))
		Return (1 = 0)
	EndIf
	
	Return (1 = 1)
EndFunc

Func _run($exe = '', $args = '')
	Global $_exe
	Global $_pid
	Global $_initialized
	
	If (not $_initialized) Then
		_init()
	EndIf
	
	_logvar('_run', '$exe', $exe)
	_logvar('_run', '$args', $args)

	if (StringLen($_exe) > 0) Then
		$exe = $_exe
	EndIf

	Local $version = FileGetVersion($exe)
	Local $cmd

	If StringLower(StringRight($exe, 4)) = '.msi' Then
		$cmd = 'msiexec.exe /i "' & $exe & '"'
		If FileExists(@SystemDir & '\msiexec.exe') Then
			$cmd = @SystemDir & '\' + $cmd
		Endif
	Else
		$cmd = $exe
	EndIf

	If StringLen($args) > 0 Then
		$cmd = $cmd & ' ' & $args
	EndIf

	_logvar('_run', '$cmd', $cmd)
	_logvar('_run', '$version', $version)
	
	$_pid = Run($cmd)

	_logvar('_run', '$_pid', $_pid)

	Return $_pid
EndFunc

Func _set_exitval($exitval)
	Global $_exitval

	Local $rv = $_exitval

	$_exitval = $exitval;
	
	return $rv;
EndFunc

Func _sleep($millis = 0)
	Global $_pause_time

	If @NUMPARAMS > 0 Then
		Sleep($millis)
	Else
		Sleep($_pause_time)
	EndIf
EndFunc

Func _still_running()
	If Not _ProcessExists() Then
		Return (1=0)
	EndIf
	
	Return _still_time()
EndFunc

Func _still_time()
	Global $_max_runtime
	Global $_start_time
	Global $_still_time

	Local $initialized = $_start_time > 0
	
	If Not $initialized Then
		$_start_time = TimerInit()
		_logvar('_still_time', '$start_time', $_start_time)
	EndIf
	
	Local $diff = TimerDiff($_start_time)
	$_still_time = $diff < ($_max_runtime * 1000)
	If $_still_time > 0 And $initialized > 0 Then
		_sleep()
	EndIf

	_logvar('_still_time', '$diff', $diff)
	_logvar('_still_time', '$still_time', $_still_time)

	Return $_still_time
EndFunc

Func _trim($s)
	return StringStripWS($s, 3)
EndFunc

Func _unquote($s)
	If StringLeft($s, 1) = '"' Then 
		$s = StringTrimLeft($s, 1)
	EndIf
	If StringRight($s, 1) = '"' Then 
		$s = StringTrimRight($s, 1)
	EndIf
	return $s
EndFunc

Func OnAutoItStart()
    ; A 2nd script instance will exit.
    If StringInStr($cmdlineraw, '/dummy') Then 
    	Exit(-1)
    EndIf
    Local $interpreter = StringTrimRight(@ScriptName, 4) & ' Script Interpreter'
    If WinExists($interpreter) Then 
    	Exit(-1)
    EndIf
    AutoItWinSetTitle($interpreter)
EndFunc
