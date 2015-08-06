export CORECLR_SRC=../coreclr/src
dmcs -debug+ -nostdlib -r:$CORECLR_SRC/../bin/Product/Linux.x64.Debug/mscorlib.dll -out:Hello.exe -target:exe Hello.cs