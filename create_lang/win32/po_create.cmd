dir /B ..\..\src\*.cpp > files.txt
xgettext -a --no-location -s --no-wrap -D..\..\src -ffiles.txt -o..\qgen.po
del files.txt
