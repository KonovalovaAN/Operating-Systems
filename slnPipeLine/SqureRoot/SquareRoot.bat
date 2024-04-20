
rem echo SquareRoot(Sum(Square(Floats5.txt))) > Results5.txt

rem echo SquareRoot(Sum(Square(Floats5.txt))) > Errors5.txt
rem cd ..
rem cd Square

del /P Errors5.txt Results5.txt

..\Debug\Square.exe < Floats5.txt  2>>Errors5.txt |..\Debug\Sum.exe |..\Debug\SqureRoot.exe >> Results5.txt

type Floats5.txt
type Errors5.txt
type Results5.txt

pause
rem exit
del /P Errors5.txt Results5.txt
..\Debug\Square.exe < ..\square\Floats.txt  2>>Errors5.txt |..\Debug\Sum.exe |..\Debug\SqureRoot.exe >> Results5.txt

type ..\square\Floats.txt
type Errors5.txt
type Results5.txt

pause


REM End of Job
pause