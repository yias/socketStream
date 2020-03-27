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
reset:=$(shell tput sgr0)

# set output directory, where the executables will be built
OUTPUT_DIR = ./build

# set the compiler and its flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall 

# set include directories
INCLUDE_DIR = ./include ${CPP_DEPENDENCIES}/rapidjson/include
INCLUDE_PARAMS = $(foreach d, $(INCLUDE_DIR), -I $d)

# set the source directory
SRC_DIR = ./source

# set the libraries needed for compiling
LIBS = -lpthread


test: makeOutDir serverExample sendWithTime

# build a socketStream client for sending packets for specific time
serverExample:
	$(info $(greenC)$(boldT) --> building socketStream server example $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/serverExample.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/jsonWrapper.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/serverExample.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)
	$(info $(blueC)$(boldT) OK$(reset))


# build a socketStream client for sending packets for specific time
sendWithTime:
	$(info $(greenC)$(boldT) --> building socketStream client sendWithTime $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/sendingWithTime.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/sendWithTime.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)


# build the client example of the socketStream
clientExample:
	$(info $(greenC)$(boldT) --> building socketStream client example $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/example.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/clientExample.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)


# build the run diagnostics example
runDiagnostics:
	$(info $(greenC)$(boldT) --> building runDiagnostics client $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/runDiagnostics.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/jsonWrapper.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/runDiagnostics.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)


# build the example of the jsonWrapper
jsonWrapperExample:
	$(info $(greenC)$(boldT) --> building jsonWrapper example $(yellowC)==>$(reset))
	@$(CXX) $(CXXFLAGS) $(SRC_DIR)/jsonWrapper_example.cpp $(SRC_DIR)/socketStream.cpp $(SRC_DIR)/jsonWrapper.cpp $(SRC_DIR)/md5.cpp \
	-o ${OUTPUT_DIR}/jsonWrapperExample.exe \
	$(INCLUDE_PARAMS) \
	$(LIBS)


# echo the path of the cpp dependencies
echoEvnVariable:
	@echo ${CPP_DEPENDENCIES}


# create a directory for the builts, if it doesn't exist
makeOutDir:
	@if test -d ${OUTPUT_DIR}; \
	then \
		tput setaf 2; tput bold; printf " --> output folder "${OUTPUT_DIR}" alread exists"; tput setaf 3; printf " ==>\n";\
		tput sgr0;\
	else \
		tput setaf 2; printf " -->"; tput bold; printf " creating output folder ${OUTPUT_DIR} " ; tput setaf 3; printf " ==>\n"; tput sgr0;\
		mkdir -p ${OUTPUT_DIR}; \
	fi


.PHONY: clean

clean:
	rm ${OUTPUT_DIR}/*