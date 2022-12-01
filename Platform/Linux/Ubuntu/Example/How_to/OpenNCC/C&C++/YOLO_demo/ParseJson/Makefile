#CROSS_COMPILE =aarch64-linux-gnu-
#CROSS_COMPILE =arm-hisiv500-linux-
CC = $(CROSS_COMPILE)gcc 
CPP = $(CROSS_COMPILE)g++ -std=c++11
CXX = $(CROSS_COMPILE)g++ -std=c++11
AR = $(CROSS_COMPILE)ar 
LINK=$(CROSS_COMPILE)g++

OPENCV = `pkg-config opencv --cflags --libs`
INCS =  -I./inc
COMMFLAG=  -DUSE_WD

LDFLAGS =   $(OPENCV) -lrt -ldl  -lpthread  
# -lzlib
CFLAGS = -D_GNU_SOURCE -Wall  -O2 -g

PWD=$(shell pwd)

#TARGET =  $(notdir $(PWD))
TARGET =  Openncc

# change to you project file dir

# the obj dir
OBJDIR = obj
BINDIR = bin
###########################################################################

TEST_SOURCES = $(wildcard *.c*)
TEST_OBIS = $(TEST_SOURCES:%.c*=%.o)
# head files

CFLAGS += $(INCS) $(COMMFLAG)
CXXFLAGS += $(INCS) $(COMMFLAG)

help:
	@echo
	@echo "\t|--------------------------------------------------------------|"
	@echo "\t| Help                                                         |"
	@echo "\t| ----                                                         |"
	@echo "\t| make clone : Clone or update lib                             |"
	@echo "\t| make all   : Clone lib and build application                 |"
	@echo "\t| make run   : Clone lib and run application after build       |"
	@echo "\t| make clean : Clean application                               |"
	@echo "\t|--------------------------------------------------------------|"
	@echo

all: $(TARGET)

$(TARGET): $(TEST_OBIS)  	
	@echo "TEST_SOURCES: $(TEST_SOURCES)"
	@test -d $(BINDIR) | mkdir -p $(BINDIR)
	$(CXX) -o $@ $^ $(TARGET_LIB) $(CXXFLAGS) $(LDFLAGS) 
	mv $@ $(BINDIR)/
	
%.o: %.c
	$(CC) -c $(CFLAGS)-o $@ $<

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<
	
clean:
	rm -rf $(OBJDIR) *.o
	rm -f  $(BINDIR)/$(TARGET)

