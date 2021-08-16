#****************************************************************************
# arch configure
#****************************************************************************
include ../arch.mk

HARDWARE_INC=../hardware/include
PUBINC=../include

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
INCDIRS=$(shell find ./ -maxdepth 2 -type d)
INCPRJ=$(foreach dir,$(INCDIRS),$(join -I,$(dir)))
INCS := ${INCPRJ}

PUB_INCDIRS=$(shell find $(PUBINC) -maxdepth 2 -type d)
PUB_INCPRJ=$(foreach dir,$(PUB_INCDIRS),$(join -I,$(dir)))
INCS += ${PUB_INCPRJ}

HARDWARE_INCDIRS=$(shell find $(HARDWARE_INC) -maxdepth 2 -type d)
HARDWARE_INCPRJ=$(foreach dir,$(HARDWARE_INCDIRS),$(join -I,$(dir)))
INCS += ${HARDWARE_INCPRJ}
#****************************************************************************
# Library 
#****************************************************************************
# LIBS := -L${PUBLIB}
# DYNAMIC_LIBS := -Wl,-Bdynamic -limg_dsp_v5_dyn -limg_algo_v5_dyn 
# STATIC_LIBS :=   -Wl,-Bstatic
# LIBS += ${STATIC_LIBS} ${DYNAMIC_LIBS}

#****************************************************************************
# Output
#****************************************************************************
OUTPUT_STATIC    := ./libservice.a

all: ${OUTPUT_STATIC}

${OUTPUT_STATIC}: ${OBJS}
	${AR} rcs $@ $^

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
	${RM} ${OUTPUT_STATIC} ${OBJS}
