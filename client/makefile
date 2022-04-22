CXX=g++ -std=c++20

LINUX_LIBS=-lX11 -lXtst
LINUX_FLAGS=
LINUX_GUI_LIBS=
LINUX_GUI_FLAGS=

WIN_LIBS=
WIN_FLAGS=
WIN_GUI_LIBS=`pkg-config --libs gtkmm-3.0 | sed 's/ -I/ -isystem /g'`
WIN_GUI_FLAGS=`pkg-config --cflags gtkmm-3.0 | sed 's/ -I/ -isystem /g'`

CLI_SOURCE_FILES=main.cpp src/fb_events.cpp src/fb_keymap.cpp src/fb_parser.cpp src/footbutton.cpp
GUI_SOURCE_FILES=${CLI_SOURCE_FILES} src/fb_gui.cpp src/resources.cpp

UNAME=$(shell uname)

all: cli gui
	@echo "All done"

gui:
ifeq ($(UNAME), Linux)
	@echo -n "Linux... "
	@${CXX} -o fb_cli *.cpp src/*.cpp ${LINUX_LIBS} ${LINUX_FLAGS} -D FB_CLIENT_CLI
else
	@windres.exe res/icon.rc icon.o
	@echo -n "Compiling Windows GUI client... "
	@cd res; glib-compile-resources --target=resources.cpp --generate-source resources.gresource.xml; cd ..
	@mv res/resources.cpp src/resources.cpp
	@${CXX} -o fb.exe ${GUI_SOURCE_FILES} icon.o ${WIN_GUI_LIBS} ${WIN_GUI_FLAGS} -static-libstdc++ -static-libgcc -D FB_CLIENT_GUI
	@echo "done"
	@rm icon.o
endif

cli:
ifeq ($(UNAME), Linux)
	@echo -n "Linux... "
	@${CXX} -o fb_cli *.cpp src/*.cpp ${LINUX_LIBS} ${LINUX_FLAGS} -D FB_CLIENT_CLI
else
	@echo -n "Compiling Windows CLI client... "
	@windres.exe res/icon.rc icon.o
	@${CXX} -o fb_cli.exe ${CLI_SOURCE_FILES} icon.o ${WIN_LIBS} ${WIN_FLAGS} -static-libstdc++ -static-libgcc -static -D FB_CLIENT_CLI
	@echo "done"
	@rm icon.o
endif

clean:
	@rm *.exe
	@rm fb fb_cli	
