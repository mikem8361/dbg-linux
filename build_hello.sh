export CORECLR_SRC=../coreclr/src
mkdir bin
dmcs -debug+ -nostdlib -r:$CORECLR_SRC/../bin/Product/Linux.x64.Debug/mscorlib.dll -out:bin/Hello.exe -target:exe Hello.cs
