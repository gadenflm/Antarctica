LIBS_PATH := -L./out/
# CROSS_COMPILE :=arm-linux-ohoseabi-

AR := $(CROSS_COMPILE)ar
CC := $(CROSS_COMPILE)gcc
# STRIP :=$(CROSS_COMPILE)strip

CFLAGS := -O0 -fPIC 

# SHARE := -fPIC -shared -o
STATIC := -rc

# LIBS_BIN :=$(LIBS_PATH) libhello_test.a

LIBS_BIN :=$(LIBS_PATH) -lhello_test

TARGET_AR := libhello_test.a
TARGET_BIN := sta_test


SOURCE_AR := lib_func.c \
			 hello_tvt.c


OBJS_AR := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE_AR)))

OBJS_BIN := hello_test.c


.PHONY :all clean

all: $(TARGET_AR) $(TARGET_BIN)

ar: $(TARGET_AR)

bin: $(TARGET_BIN)

$(TARGET_AR): $(OBJS_AR)
	$(AR) $(STATIC) $@ $(OBJS_AR)
	@mkdir out
	cp -f $(TARGET_AR) ./out/

$(TARGET_BIN): $(OBJS_BIN)
	$(CC) -o $@ $(OBJS_BIN) $(LIBS_BIN)
	cp -f $(TARGET_BIN) ./out/

clean:
	rm -fr ./out
	rm -fr *.o
	rm -fr $(TARGET_AR)
	rm -fr $(TARGET_BIN)