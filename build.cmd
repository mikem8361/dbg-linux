setlocal
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64 
if not exist bin mkdir bin
pushd bin
cl.exe ..\dbg.cpp ..\cordebug_i.cpp -I.. /Zi /Fd:dbg.pdb /Fe:dbg.exe
popd
