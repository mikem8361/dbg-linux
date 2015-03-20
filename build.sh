export CORECLR_SRC=../coreclr/src
clang++  -DPAL_IMPLEMENTATION  -D__LINUX__ -I$CORECLR_SRC/pal/inc -pthread -std=c++11 -g dbg.cpp -o dbg -ldl -v