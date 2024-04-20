cd ..
cd Square
Rem Variant 1 for debugging DoPipeLine.exe
Rem Command: ..\Debug\DoPipeLine.exe
Rem Command Arguments:"Square<Floats5 2>Errors2|Sum>Results1"
Rem Current(Working) Directory: slnPipeLine\Square\
..\Debug\DoPipeLine.exe "Square<Floats5 2>Errors2|Sum>>Results22"
type Floats5.txt
type Errors2.txt
type Results22.txt
pause

Rem Variant 2
..\Debug\DoPipeLine.exe "Square<Floats 2>Errors2|Square>>Results22"
type Floats.txt
type Errors2.txt
type Results22.txt
pause

Rem Variant 3
..\Debug\DoPipeLine.exe "Sum<Floats 2>Errors2|SqureRoot>>Results22"
type Floats.txt
type Errors2.txt
type Results22.txt
pause

Rem End of Job
exit