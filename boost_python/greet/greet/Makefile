all: libgreet.so

CC = g++

FRAMEWORK_PATH=/opt/local/Library/Frameworks/Python.framework/Versions/2.7
INCLUDE_PATH_1=$(FRAMEWORK_PATH)/Headers
INCLUDE_PATH_2=/opt/local/include/
LIB_PATH_1=$(FRAMEWORK_PATH)/lib
LIB_PATH_2=/opt/local/lib

greet.o: greet.cpp
	$(CC) -Wall -O2 -fPIC -c $< -I$(INCLUDE_PATH_1) -I$(INCLUDE_PATH_2)

libgreet.so: greet.o
	$(CC) -shared -o $@ $< -I$(INCLUDE_PATH_1) -I$(INCLUDE_PATH_2) -L$(LIB_PATH_1) -L$(LIB_PATH_2) -lboost_python-mt -lpython2.7 #  -F$(FRAMEWORK_PATH) -framework Python
