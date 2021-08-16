############################################################################################
#	1st_Author: K_gadenflm     2021.07
############################################################################################
# Path define:
SO_OUT_PATH := -Llib
SO_SLIB_SRC_PATH := src/ServerLib
SO_CLIB_SRC_PATH := src/ClientLib
SO_SBIN_SRC_PATH := src/ServerManager
SO_CBIN_SRC_PATH := src/Client

INCLUDE_PATH := include

# Compile tools:
CROSS_COMPILE := 

CC := $(CROSS_COMPILE)gcc
CXX:= $(CROSS_COMPILE)g++
STRIP :=$(CROSS_COMPILE)strip

# Flags define:

CFLAGS := -O0 -fPIC -I $(INCLUDE_PATH) -Wall -Wl,-rpath=. -std=c99

CXXFLAGS := -O0 -fPIC -I $(INCLUDE_PATH) -Wall -Wl,-rpath=. -std=c++11

SHARE := -fPIC -shared -o

BIN_S_LDFLAGS :=$(SO_OUT_PATH) -lWKLServer
BIN_C_LDFLAGS :=$(SO_OUT_PATH) -lWKLClient

# Target define

SO_S_TARGET := libWKLServer.so
SO_C_TARGET := libWKLClient.so
BIN_S_TARGET := WKL_Server_manager 
BIN_C_TARGET :=	WKL_Client	

# Src define
SO_S_SRC := $(wildcard $(SO_SLIB_SRC_PATH)/*.c) \
			$(wildcard $(SO_SLIB_SRC_PATH)/*.cpp) 

# SO_C_SRC := $(SO_CLIB_SRC_PATH)/*.c

BIN_S_SRC := $(wildcard $(SO_SBIN_SRC_PATH)/*.c) \
			 $(wildcard $(SO_SBIN_SRC_PATH)/*.cpp) 

# BIN_C_SRC := $(SO_CBIN_SRC_PATH)/*.c		  

# Obj define
SO_S_OBJ := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SO_S_SRC)))
SO_C_OBJ := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SO_C_SRC)))

BIN_S_OBJ := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(BIN_S_SRC)))
BIN_C_OBJ := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(BIN_C_SRC)))


.PHONY :all clean mk_debug

all: $(SO_S_TARGET) $(BIN_S_TARGET)

$(SO_S_TARGET): $(SO_S_OBJ)
	$(CC) $(CFLAGS) $(SHARE) $@ $(SO_S_OBJ)
# 	$(STRIP) $(SO_S_TARGET)
	@cp -f $(SO_S_TARGET) out/
	@mv -f $(SO_S_TARGET) lib/

$(BIN_S_TARGET): $(BIN_S_OBJ)
	$(CC) $(CFLAGS) -o $@ $(BIN_S_OBJ) $(BIN_S_LDFLAGS)
	@mv -f $(BIN_S_TARGET) out/

# Rule
%.o : %.cpp
	${CXX} -c ${CXXFLAGS} $< -o $@ 

%.o : %.c
	${CC} -c ${CFLAGS} $< -o $@ 


clean:
	@echo "********************************************************************"
	@echo "Clean"
	@echo "********************************************************************"
	rm -rf out/*
	rm -rf lib/$(SO_S_TARGET)
	rm -rf lib/$(SO_C_TARGET)
	rm -rf $(SO_S_OBJ)
	rm -rf $(SO_C_OBJ)
	rm -rf $(BIN_S_OBJ)
	rm -rf $(BIN_C_OBJ)

mk_debug: 
	@echo "********************************************************************"
	@echo "SO_SRC"
	@echo "********************************************************************"
	@echo $(SO_S_SRC)
	@echo
	@echo $(SO_C_SRC)
	@echo
	@echo "********************************************************************"
	@echo "BIN_SRC"
	@echo "********************************************************************"
	@echo $(BIN_S_SRC)
	@echo
	@echo $(BIN_C_SRC)
	@echo
	@echo "********************************************************************"
	@echo "SO_OBJ"
	@echo "********************************************************************"
	@echo $(SO_S_OBJ)
	@echo
	@echo $(SO_C_OBJ)
	@echo	
	@echo "********************************************************************"
	@echo "BIN_OBJ"
	@echo "********************************************************************"
	@echo $(BIN_S_OBJ)
	@echo
	@echo $(BIN_C_OBJ)
