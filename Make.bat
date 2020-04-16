call egcc.bat
::definex.py rb.cc rb-x.cc
::gcc %CCFLAGS2% dftree.cc test.cc
gcc %CCFLAGS2% dftree.cc -c
ar rcs %PROGRAMS%\local\lib32\libexshit.a dftree.o
copy /Y dftree.h %PROGRAMS%\local\include
