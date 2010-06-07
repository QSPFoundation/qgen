rd /s /q _debug
rd /s /q _release
xcopy misc\common _debug /E /I /EXCLUDE:_excludes_win32.txt
xcopy misc\common _release /E /I /EXCLUDE:_excludes_win32.txt
rem ---------------------------------
del win32_qgen.ncb
del /a:h win32_qgen.suo
del *.user
