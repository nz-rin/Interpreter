# Compiler
CXX = g++
CXXFLAGS = -g -Wall -Wextra -Iinclude
CXXDBFLAG = -DDLOG

# DIR
ROOT = $(PWD)
Build = $(ROOT)/build
SRC = $(ROOT)/src
DB = $(ROOT)/debug

# Files
src = $(wildcard $(SRC)/*.cxx)
obj = $(patsubst $(SRC)/%.cxx, $(Build)/%.o, $(src))
target = $(Build)/main

dbtarget = $(DB)/main.debug
dbobj = $(patsubst $(SRC)/%.cxx, $(DB)/%.o, $(src))


# Default
all: $(target)

# Link obj
$(target): $(obj)
	$(CXX) $(CXXFLAGS) -o $(target) $(obj)

# Compile obj files
$(Build)/%.o: $(SRC)/%.cxx | $(Build)
	$(CXX) $(CXXFLAGS) -c $< -o $@


#Debug build
debug: $(dbtarget)
	mkdir -p debug

$(dbtarget): $(dbobj)
	$(CXX) $(CXXFLAGS) $(CXXDBFLAG) -o $(dbtarget) $(dbobj)

$(DB)/%.o: $(SRC)/%.cxx | $(DB)
	$(CXX) $(CXXFLAGS) $(CXXDBFLAG) -c $< -o $@

clean:
	rm -rf $(Build)/*
	rm -rf $(DB)/*

run:
	$(target) $(ARGS)

debug-run:
	$(dbtarget) $(ARGS)

