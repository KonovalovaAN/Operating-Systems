REM Square(Square(Floats.txt)) > Results.txt
REM Square(Square(Floats.txt)) > Errors.txt

del /P Errors.txt Results.txt

..\Debug\Square.exe < Floats.txt  2>Errors.txt |..\Debug\Square.exe > Results.txt

type Floats.txt
type Errors.txt
type Results.txt

pause
exit

REM Sum(Square(Floats5.txt)) >> Results.txt
REM Sum(Square(Floats5.txt)) >> Errors.txt

..\Debug\Square.exe < Floats5.txt  2>>Errors.txt |..\Debug\Sum.exe >> Results.txt

REM Square(Square(Square(Floats5.txt))) >> Results.txt
REM Square(Square(Square(Floats5.txt))) >> Errors.txt

..\Debug\Square.exe < Floats5.txt  2>>Errors.txt |..\Debug\Square.exe |..\Debug\Square.exe >> Results.txt

type Results.txt

REM End of Job
pause