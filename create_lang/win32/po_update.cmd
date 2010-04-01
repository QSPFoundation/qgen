dir /B ..\..\src\*.cpp > files.txt
xgettext -a --no-location -s --no-wrap -j -D..\..\src -ffiles.txt -o..\qgen.po
del files.txt
