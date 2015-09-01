export CORECLR_SRC=../coreclr/src
clang++-3.5 -DPAL_IMPLEMENTATION -DFEATURE_PAL -D__LINUX__ -DPLATFORM_UNIX -DPAL_STDCPP_COMPAT -I$CORECLR_SRC/pal/inc -pthread -std=c++11 -g dbg.cpp cordebug_i.cpp -o dbg -ldl -L$CORECLR_SRC/../bin/Product/Linux.x64.Debug/lib  -lunwind -lpalrt -lcoreclrpal 

