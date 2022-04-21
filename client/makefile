CXX=g++
LINUX_LIBS=-lX11 -lXtst
LINUX_FLAGS=
WIN_LIBS=
WIN_FLAGS=
UNAME=$(shell uname)

all:
	@echo -n "Compiling for "
ifeq ($(UNAME), Linux)
	@echo -n "Linux... "
	@${CXX} -o fb *.cpp src/*.cpp ${LINUX_LIBS} ${LINUX_FLAGS}
else
	@echo -n "Windows... "
	@windres.exe icon.rc icon.o
	@${CXX} -o fb.exe *.cpp src/*.cpp icon.o ${WIN_LIBS} ${WIN_FLAGS} -static-libstdc++ -static-libgcc -static
endif
	@echo "done"
