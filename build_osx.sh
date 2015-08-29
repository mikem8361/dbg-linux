export CORECLR_SRC=../coreclr/src
mkdir bin
clang++ -DPAL_IMPLEMENTATION -DFEATURE_PAL -DBIT64 -D__APPLE__ -DPLATFORM_UNIX -DPAL_STDCPP_COMPAT -I$CORECLR_SRC/inc -I$CORECLR_SRC/pal/inc -I$CORECLR_SRC/pal/inc/rt -I$CORECLR_SRC/pal/prebuilt/inc -pthread -std=c++11 -g dbg.cpp cordebug_i.cpp -o bin/dbg -ldl -L$CORECLR_SRC/../bin/Product/OSx.x64.Debug/lib  -lpalrt -lcoreclrpal -framework CoreFoundation -framework CoreServices -framework Security -framework System

