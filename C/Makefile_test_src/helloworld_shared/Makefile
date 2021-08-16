LIBS_PATH := -Lout
# CROSS_COMPILE :=arm-linux-ohoseabi-

CC := $(CROSS_COMPILE)gcc
STRIP :=$(CROSS_COMPILE)strip

CFLAGS := -O0 -fPIC 

SHARE := -fPIC -shared -o
LIBS_BIN :=$(LIBS_PATH) -lhello_test

TARGET_SO := libhello_test.so
TARGET_BIN := hello_test


SOURCE_SO := lib_func.c \
			 hello_tvt.c


OBJS_SO := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE_SO)))

OBJS_BIN := hello_test.c


.PHONY :all clean

all: $(TARGET_SO) $(TARGET_BIN)

$(TARGET_SO): $(OBJS_SO)
	$(CC) $(CFLAGS) $(SHARE) $@ $(OBJS_SO)
	$(STRIP) $(TARGET_SO)
	@mkdir out
	cp -f $(TARGET_SO) ./out/

$(TARGET_BIN): $(OBJS_BIN)
	$(CC) -o $@ $(OBJS_BIN) $(LIBS_BIN)
	cp -f $(TARGET_BIN) ./out/

clean:
	rm -fr ./out
	rm -fr *.o
	rm -fr $(TARGET_SO)
	rm -fr $(TARGET_BIN)