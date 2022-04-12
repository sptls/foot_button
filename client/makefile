CXX=g++
LINUX_LIBS=-lX11 -lXtst
LINUX_FLAGS=
WIN_LIBS=
WIN_FLAGS=

all:
	@echo "make linux/windows"

linux:
	@echo -n "Compiling... "
	@${CXX} -o fb *.cpp src/*.cpp ${LINUX_LIBS} ${LINUX_FLAGS}
	@echo "done"
windows:
	@echo -n "Compiling... "
	@${CXX} -o fb *.cpp src/*.cpp ${WIN_LIBS} ${WIN_FLAGS}
	@echo "done"