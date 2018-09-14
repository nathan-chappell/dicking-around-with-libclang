CPP = g++ --std=c++14
CPPFLAGS = -g -O0 -Wall -I/usr/lib/llvm-4.0/include/ -L/usr/lib/llvm-4.0/lib/ -lclang
COMP = $(CPP) $^ $(CPPFLAGS) -o $@

cxcursor_info : cxcursor_info.o parse_cxcursor_info_options.o
	$(COMP)


