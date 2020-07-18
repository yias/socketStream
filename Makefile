# 	makefile of the socketStream package
# 
#	Developer:  Iason Batzianoulis
#	email:      iasonbatz@gmail.com
#	License:    GNU GPLv3


# define collors for printing to the console
redC := $(shell tput setaf 1)
greenC := $(shell tput setaf 2)
yellowC := $(shell tput setaf 3)
blueC := $(shell tput setaf 4)
boldT := $(shell tput bold)
reset :=$(shell tput sgr0)

# get current directory
CURRENT_DIR = $(shell pwd)

# set output directories, where the binaries will be built
OUTPUT_DIR = ./build
OUTPUT_OBJ_DIRx64 = $(OUTPUT_DIR)/obj/x64
OUTPUT_OBJ_DIRx86 = $(OUTPUT_DIR)/obj/x86
BIN_DIR = ./bin
LINUX_LIB_DIR = $(BIN_DIR)/linux
LINUX_LIB_X64 = $(LINUX_LIB_DIR)/x64
LINUX_LIB_X86 = $(LINUX_LIB_DIR)/x86
INSTALL_PREFIX = /usr/local

# set the compiler and its flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -DMAKEDLL
CXX_x86_FLAG = -m32
CXX_x64_FLAG = -m64

# set include directories
HEADERS_DIR = ./include
RAPIDJSON_INCLUDE = ./rapidjson/include
INCLUDE_DIR = $(HEADERS_DIR)
INCLUDE_DIR += $(RAPIDJSON_INCLUDE)
INCLUDE_PARAMS = $(foreach d, $(INCLUDE_DIR), -I $d)

# set the source directory
SRC_DIR = ./src
EXAMPLES_DIR = ./examples

# set objects to be created
_OBJS = socketStream.o jsonWrapper.o md5.o
_OBJSx64 = $(patsubst %,$(OUTPUT_OBJ_DIRx64)/%,$(_OBJS))
_OBJSx86 = $(patsubst %,$(OUTPUT_OBJ_DIRx86)/%,$(_OBJS))

# set the libraries needed for compiling
LIBS = -lpthread

generateLibs: generateObjects generateSOx64 generateStaticLibx64 generateSOx86 generateStaticLibx86

generateObjects: makeDirs generateObjectsx64 generateObjectsx86 

makeDirs: makeOutDir makeBinDir makeLxDir makeLx64Dir makeLx86Dir makeObj64Dir makeObj86Dir

test: makeOutDir serverExample sendWithTime serverExampleSendMsg clientListener

all: makeOutDir serverExample sendWithTime clientExample runDiagnostics jsonWrapperExample serverExampleSendMsg clientListener


$(OUTPUT_OBJ_DIRx64)/%.o: $(SRC_DIR)/%.cpp 
	$(CXX) -c -o $@ $< $(INCLUDE_PARAMS) $(CXXFLAGS) $(CXX_x64_FLAG) -fpic
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi

generateObjectsx64: $(_OBJSx64)
	$(info $(greenC)$(boldT) --> generating objects x64 $(yellowC)==>$(reset))
	$(CXX) -c -o $^ $(CXXFLAGS) $(CXX_x64_FLAG) $(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi

# generate shared libraries
generateSOx64:
	$(info $(greenC)$(boldT) --> generating shared object x64 $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(CXX_x64_FLAG) \
	-shared -o $(LINUX_LIB_X64)/libsocketStream.so.1.0.1 \
	$(_OBJSx64) \
	$(INCLUDE_PARAMS) -lc $(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi

# generate static libraries
generateStaticLibx64:
	$(info $(greenC)$(boldT) --> generating static library x64 $(yellowC)==>$(reset))
	@ar rcs $(LINUX_LIB_X64)/libsocketStream.a $(_OBJSx64)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi


$(OUTPUT_OBJ_DIRx86)/%.o: $(SRC_DIR)/%.cpp 
	$(CXX) -c -o $@ $< $(INCLUDE_PARAMS) $(CXXFLAGS) $(CXX_x86_FLAG) -fpic
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi

generateObjectsx86: $(_OBJSx86)
	$(info $(greenC)$(boldT) --> generating objects x86 $(yellowC)==>$(reset))
	$(CXX) -c -o $^ $(CXXFLAGS) $(CXX_x86_FLAG) $(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi

# generate shared libraries
generateSOx86:
	$(info $(greenC)$(boldT) --> generating shared object x86 $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(CXX_x86_FLAG) \
	-shared -o ${LINUX_LIB_X86}/libsocketStream.so \
	$(_OBJSx86) \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi

# generate static libraries
generateStaticLibx86:
	$(info $(greenC)$(boldT) --> generating static library x86 $(yellowC)==>$(reset))
	@ar rcs $(LINUX_LIB_X86)/libsocketStream.a $(_OBJSx86)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi



# build a socketStream server for listening to clients and retrieve the signals of a client
serverExample:
	$(info $(greenC)$(boldT) --> building socketStream server example $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(EXAMPLES_DIR)/serverExample.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/jsonWrapper.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/serverExample.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi

# build a socketStream client for sending packets for specific time
serverExampleSendMsg:
	$(info $(greenC)$(boldT) --> building socketStream serverSendMsg example $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(EXAMPLES_DIR)/serverExampleSendMsg.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/jsonWrapper.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/serverExampleSendMsg.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi


# build a socketStream client for sending packets for specific time
sendWithTime:
	$(info $(greenC)$(boldT) --> building socketStream client sendWithTime $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(EXAMPLES_DIR)/sendingWithTime.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/sendWithTime.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi


# build the client example of the socketStream
clientExample:
	$(info $(greenC)$(boldT) --> building socketStream client example $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(EXAMPLES_DIR)/example.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/clientExample.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi


# build the run diagnostics example
runDiagnostics:
	$(info $(greenC)$(boldT) --> building runDiagnostics client $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(EXAMPLES_DIR)/runDiagnostics.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/jsonWrapper.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/runDiagnostics.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi

# build a socketStream client for sending packets for specific time
clientListener:
	$(info $(greenC)$(boldT) --> building socketStream clientListener example $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(EXAMPLES_DIR)/clientListenerExample.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/jsonWrapper.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/clientListener.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi


# build the example of the jsonWrapper
jsonWrapperExample:
	$(info $(greenC)$(boldT) --> building jsonWrapper example $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(EXAMPLES_DIR)/jsonWrapper_example.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/jsonWrapper.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/jsonWrapperExample.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	@if test $$? -eq 0;\
	then tput setaf 4; tput bold; echo " --> OK"; tput sgr0; \
	fi


# echo the path of the cpp dependencies
echoEvnVariable:
	@echo ${CPP_DEPENDENCIES}


# create a directory for the builts, if it doesn't exist
makeOutDir:
	@if test -d ${OUTPUT_DIR}; \
	then \
		tput setaf 4; tput bold; printf " --> output folder "${OUTPUT_DIR}" alread exists"; tput setaf 3; printf " ==>\n";\
		tput sgr0;\
	else \
		tput setaf 2; printf " -->"; tput bold; printf " creating output folder ${OUTPUT_DIR} " ; tput setaf 3; printf " ==>\n"; tput sgr0;\
		mkdir -p ${OUTPUT_DIR}; \
	fi

# create a directory for the library, if it doesn't exist
makeBinDir:
	@if test -d ${BIN_DIR}; \
	then \
		tput setaf 4; tput bold; printf " --> output folder "${BIN_DIR}" alread exists"; tput setaf 3; printf " ==>\n";\
		tput sgr0;\
	else \
		tput setaf 2; printf " -->"; tput bold; printf " creating output folder ${OUTPUT_DIR} " ; tput setaf 3; printf " ==>\n"; tput sgr0;\
		mkdir -p ${BIN_DIR}; \
	fi

# create a directory for the linux binaries, if it doesn't exist
makeLxDir:
	@if test -d ${LINUX_LIB_DIR}; \
	then \
		tput setaf 4; tput bold; printf " --> output folder "${LINUX_LIB_DIR}" alread exists"; tput setaf 3; printf " ==>\n";\
		tput sgr0;\
	else \
		tput setaf 2; printf " -->"; tput bold; printf " creating output folder ${LINUX_LIB_DIR} " ; tput setaf 3; printf " ==>\n"; tput sgr0;\
		mkdir -p ${LINUX_LIB_DIR}; \
	fi

# create a directory for the linux x64 binaries, if it doesn't exist
makeLx64Dir:
	@if test -d ${LINUX_LIB_X64}; \
	then \
		tput setaf 4; tput bold; printf " --> output folder "${LINUX_LIB_X64}" alread exists"; tput setaf 3; printf " ==>\n";\
		tput sgr0;\
	else \
		tput setaf 2; printf " -->"; tput bold; printf " creating output folder ${LINUX_LIB_X64} " ; tput setaf 3; printf " ==>\n"; tput sgr0;\
		mkdir -p ${LINUX_LIB_X64}; \
	fi

# create a directory for the linux x86 binaries, if it doesn't exist
makeLx86Dir:
	@if test -d ${LINUX_LIB_X86}; \
	then \
		tput setaf 4; tput bold; printf " --> output folder "${LINUX_LIB_X86}" alread exists"; tput setaf 3; printf " ==>\n";\
		tput sgr0;\
	else \
		tput setaf 2; printf " -->"; tput bold; printf " creating output folder ${LINUX_LIB_X86} " ; tput setaf 3; printf " ==>\n"; tput sgr0;\
		mkdir -p ${LINUX_LIB_X86}; \
	fi

# create a directory for the x64 objects, if it doesn't exist
makeObj64Dir:
	@if test -d ${OUTPUT_OBJ_DIRx64}; \
	then \
		tput setaf 4; tput bold; printf " --> output folder "${OUTPUT_OBJ_DIRx64}" alread exists"; tput setaf 3; printf " ==>\n";\
		tput sgr0;\
	else \
		tput setaf 2; printf " -->"; tput bold; printf " creating output folder ${OUTPUT_OBJ_DIRx64} " ; tput setaf 3; printf " ==>\n"; tput sgr0;\
		mkdir -p ${OUTPUT_OBJ_DIRx64}; \
	fi

# create a directory for the x86 objects, if it doesn't exist
makeObj86Dir:
	@if test -d ${OUTPUT_OBJ_DIRx86}; \
	then \
		tput setaf 4; tput bold; printf " --> output folder "${OUTPUT_OBJ_DIRx86}" alread exists"; tput setaf 3; printf " ==>\n";\
		tput sgr0;\
	else \
		tput setaf 2; printf " -->"; tput bold; printf " creating output folder ${OUTPUT_OBJ_DIRx86} " ; tput setaf 3; printf " ==>\n"; tput sgr0;\
		mkdir -p ${OUTPUT_OBJ_DIRx86}; \
	fi


install:
	ln -sf $(CURRENT_DIR)/$(LINUX_LIB_X64)/libsocketStream.so.1.0.1 $(LINUX_LIB_X64)/libsocketStream.so.1
	ln -sf $(CURRENT_DIR)/$(LINUX_LIB_X64)/libsocketStream.so.1.0.1 $(INSTALL_PREFIX)/lib/libsocketStream.so.1
	ln -sf $(CURRENT_DIR)/$(LINUX_LIB_X64)/libsocketStream.so.1.0.1 $(LINUX_LIB_X64)/libsocketStream.so
	ln -sf $(CURRENT_DIR)/$(LINUX_LIB_X64)/libsocketStream.so.1.0.1 $(INSTALL_PREFIX)/lib/libsocketStream.so
	cp $(HEADERS_DIR)/socketStream.h $(INSTALL_PREFIX)/include
	cp $(HEADERS_DIR)/jsonWrapper.hpp $(INSTALL_PREFIX)/include
	cp $(HEADERS_DIR)/md5.h $(INSTALL_PREFIX)/include

.PHONY: clean

clean:
	@tput setaf 2; printf " -->"; tput bold; printf " cleaning up ${OUTPUT_DIR} folder\n"; tput sgr0;
	@rm -r ${OUTPUT_DIR}/*
	@tput setaf 4; printf " -->"; tput bold; printf " output folder ${OUTPUT_DIR} is empty\n"; tput sgr0;
	@tput setaf 2; printf " -->"; tput bold; printf " cleaning up ${LINUX_LIB_DIR} folder\n"; tput sgr0;
	@rm -r ${LINUX_LIB_DIR}/*
	@tput setaf 4; printf " -->"; tput bold; printf " output folder ${LINUX_LIB_DIR} is empty\n"; tput sgr0;

