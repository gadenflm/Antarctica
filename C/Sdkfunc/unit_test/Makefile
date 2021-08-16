#****************************************************************************
# arch configure
#****************************************************************************
LIBSDK_INC=../libsdk/output
LIBSDK_LIB=../libsdk/output

PLATLIB=${PLATFORM_OUTPUT_DIR}/library

#****************************************************************************
# Source 
#****************************************************************************
SUBDIRS=$(shell find ./ -maxdepth 2 -type d)

SRCS:=$(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.cpp))\
			$(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))

OBJS=$(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SRCS))) 

#****************************************************************************
# Include 
#****************************************************************************
INCDIRS=$(shell find ./ -maxdepth 4 -type d)
INCPRJ=$(foreach dir,$(INCDIRS),$(join -I,$(dir)))
INCS := ${INCPRJ}

INCS += -I${LIBSDK_INC}

#****************************************************************************
# Library 
#****************************************************************************
LIBS := -L${LIBSDK_LIB} -L${PLATLIB}
DYNAMIC_LIBS := -Wl,-Bdynamic -llua-5.3 -limg_aaa_flow_v5 -limg_dsp_v5_dyn -limg_algo_v5_dyn -lasound -lsdk -lm -lpthread
STATIC_LIBS := -Wl,-Bstatic 
LIBS += ${STATIC_LIBS} ${DYNAMIC_LIBS}

#****************************************************************************
# Output
#****************************************************************************
OUTPUT := ./unit_test

all: ${OUTPUT}
${OUTPUT}: ${OBJS}
	${CC} -fPIC -Wl,-rpath=. -Wl,--whole-archive ${OBJS} -Wl,--no-whole-archive -Wl,-soname -Wl,$@ -o $@ ${LIBS}
	# ${TP} ${OUTPUT}

#****************************************************************************
# Rule
#****************************************************************************
%.o : %.cpp
	${CXX} -c ${INCS} $< -o $@ ${CXXFLAGS}

%.o : %.c
	${CC} -c ${INCS} $< -o $@ ${CFLAGS} 

#****************************************************************************
# Other
#****************************************************************************
clean:
	rm ${OUTPUT}  ${OBJS}
