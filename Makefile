CFLAGS += -fPIC
CFLAGS += -shared -Wl,-undefined,dynamic_lookup
CFLAGS += -Wall
CFLAGS += -I../rtlua/src -I../rtlua/vendor/lua -I../realtalk/src

all: dylib.so

dylib.so: dylib.c
	clang -o $@ $^ $(CFLAGS)
