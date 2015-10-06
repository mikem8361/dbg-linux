if [ $CoreClrRoot = "" ]; then
    export CORECLR_SRC=~/coreclr/src
else
    export CORECLR_SRC=$CoreClrRoot/src
fi
mkdir bin > /dev/null 2>&1

OSName=$(uname -s)
case $OSName in
    Linux)
        __BuildOS=Linux
        ;;

    Darwin)
        __BuildOS=OSX
        ;;

    FreeBSD)
        __BuildOS=FreeBSD
        ;;

    OpenBSD)
        __BuildOS=OpenBSD
        ;;

    NetBSD)
        __BuildOS=NetBSD
        ;;

    *)
        echo "Unsupported OS $OSName detected, configuring as if for Linux"
        __BuildOS=Linux
        ;;
esac

if [ $__BuildOS = "Linux" ]; then

    clang++-3.5 -DPAL_IMPLEMENTATION -DFEATURE_PAL -D__LINUX__ -DPLATFORM_UNIX -DPAL_STDCPP_COMPAT -I./inc -I$CORECLR_SRC/inc -I$CORECLR_SRC/pal/inc -I$CORECLR_SRC/pal/inc/rt -pthread -std=c++11 -g dbg.cpp cordebug_i.cpp -o bin/dbg -L$CORECLR_SRC/../bin/Product/Linux.x64.Debug/lib -ldl -lunwind-x86_64 -lunwind-generic -lunwind-ptrace -lunwind -luuid -lpalrt -lcoreclrpal

elif [ $__BuildOS = "OSX" ]; then

    clang++ -DPAL_IMPLEMENTATION -DFEATURE_PAL -DBIT64 -D__APPLE__ -DPLATFORM_UNIX -DPAL_STDCPP_COMPAT -I./inc -I$CORECLR_SRC/inc -I$CORECLR_SRC/pal/inc -I$CORECLR_SRC/pal/inc/rt -I$CORECLR_SRC/pal/prebuilt/inc -pthread -std=c++11 -g dbg.cpp cordebug_i.cpp -o bin/dbg -L$CORECLR_SRC/../bin/Product/OSx.x64.Debug/lib -ldl -lpalrt -lcoreclrpal -framework CoreFoundation -framework CoreServices -framework Security -framework System

else
    echo "Unsupported OS detected"
    exit 1
fi

