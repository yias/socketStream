# 	nMakefile of the socketStream package
# 
#	Developer:  Iason Batzianoulis
#	email:      iasonbatz@gmail.com
#	License:    GNU GPLv3


# set output directories, 
# where the executables will be built
OUTPUT_DIR = $(MAKEDIR)\build
OUTPUT_OBJ_DIRx64 = $(OUTPUT_DIR)\obj\x64
OUTPUT_OBJ_DIRx86 = $(OUTPUT_DIR)\obj\x86
OUT_DLL64_DIR = $(MAKEDIR)\bin\win32\x64
OUT_DLL86_DIR = $(MAKEDIR)\bin\win32\x86


# set the compiler and its flags
# CXX = g++
CXX = cl
CXXFLAGS = /Zi /EHsc
DLL_FLAGS = /LD /DMAKEDLL

# set include directories
INCLUDE_DIR= $(MAKEDIR)\include $(MAKEDIR)\rapidjson\include

INCLUDE_PARAMS= -I $(INCLUDE_DIR: = -I )

# set the source directory
SRC_DIR = $(MAKEDIR)\src
EXAMPLES_DIR = $(MAKEDIR)\examples

# set the libraries needed for compiling
LIBS = -lws2_32

# set objects to be created
# _CLIENT_OBJS = socketStream.obj jsonWrapper.obj md5.obj
SRC_FILES = $(SRC_DIR)\socketStream.cpp $(SRC_DIR)\jsonWrapper.cpp $(SRC_DIR)\md5.cpp
# CLIENT_OBJSx64 = $(OUTPUT_OBJ_DIRx64)\$(_CLIENT_OBJS: = $(OUTPUT_OBJ_DIRx64)\)
# CLIENT_OBJSx86 = $(OUTPUT_OBJ_DIRx86)\$(_CLIENT_OBJS: = $(OUTPUT_OBJ_DIRx86)\)


generateLib: createDirs generateSLib

createDirs: createBuildFolder createDLLDirx64 createDLLDirx86 createObjFolder64 createObjFolder86



serverExample: 
	@echo generating serverexample in $(OUTPUT_DIR)
	@($(CXX) $(INCLUDE_PARAMS) $(CXXFLAGS)\
	/Fe: $(OUTPUT_DIR)\serverExample.exe \
	$(EXAMPLES_DIR)\serverExample.cpp $(SRC_FILES))


generateSLib:
	@echo generating dll
	@!IF %PROCESSOR_ARCHITECTURE%==AMD64 \
		($(CXX) $(INCLUDE_PARAMS) $(CXXFLAGS) $(DLL_FLAGS)\
		/Fe: $(OUT_DLL64_DIR)\socketStream.dll \
		/Fo: $(OUTPUT_OBJ_DIRx64)\ \
		$(SRC_FILES) /link /DLL /OUT:$(OUT_DLL64_DIR)\socketStream.dll)\
	ELSE \
		($(CXX) $(INCLUDE_PARAMS) $(CXXFLAGS) $(DLL_FLAGS)\
		/Fe: $(OUT_DLL86_DIR)\socketStream.dll \
		/Fo: $(OUTPUT_OBJ_DIRx86)\ \
		$(SRC_FILES) /link /DLL /OUT:$(OUT_DLL86_DIR)\socketStream.dll)
	@echo OK

# createStaticLibDirx86:
# 	@!IF exist $(OUT_STATIC86_DIR) \
# 		(echo $(greenC)Output directory for static lib x86 already exists) $(reset) \
# 	ELSE \
# 		(echo Creating output directory for static lib x86 && mkdir $(OUT_STATIC86_DIR) )
	

# createStaticLibDirx64:
# 	@!IF exist $(OUT_STATIC64_DIR) \
# 		(echo Output directory for static lib x64 already exists) \
# 	ELSE \
# 		(echo Creating output directory for static lib x64 && mkdir $(OUT_STATIC64_DIR) )

createDLLDirx64:
	@!IF exist $(OUT_DLL64_DIR) \
		(echo Output directory for dll x64 already exists) \
	ELSE \
		(echo Creating output directory for dll x64 && mkdir $(OUT_DLL64_DIR) )

createDLLDirx86:
	@!IF exist $(OUT_DLL86_DIR) \
		(echo Output directory for dll x86 already exists) \
	ELSE \
		(echo Creating output directory for dll x64 && mkdir $(OUT_DLL86_DIR) )


createBuildFolder:
	@!IF exist $(OUTPUT_DIR) \
		(echo Output directory already exists) \
	ELSE \
		(echo Creating output directory for static lib x64 && mkdir $(OUTPUT_DIR) )

createObjFolder64:
	@!IF exist $(OUTPUT_OBJ_DIRx64) \
		(echo Output directory already exists) \
	ELSE \
		(echo Creating output directory for static lib x64 && mkdir $(OUTPUT_OBJ_DIRx64) )

createObjFolder86:
	@!IF exist $(OUTPUT_OBJ_DIRx86) \
		(echo Output directory already exists) \
	ELSE \
		(echo Creating output directory for static lib x64 && mkdir $(OUTPUT_OBJ_DIRx86) )

