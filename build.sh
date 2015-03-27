export CORECLR_SRC=../coreclr/src
clang++-3.5  -DPAL_IMPLEMENTATION  -D__LINUX__ -DPLATFORM_UNIX -I$CORECLR_SRC/pal/inc -pthread -std=c++11 -g dbg.cpp cordebug_i.cpp -o dbg -ldl