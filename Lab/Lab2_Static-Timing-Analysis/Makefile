# compiler settings 
CXX = g++
CXXFLAGS = -I ./inc -std=c++11 -DPRINTTIME=true #-DNDEBUG 
OPTFLAGS = -flto -funroll-loops -finline-functions -O3 -ffast-math
WARNINGS = -g -Wall
# DEFINE = -DPRINTTIME=true

# Source files and object files
SRCDIR = src
OBJDIR = obj
INCDIR = inc
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# testcase file
CASEDIR = Test_Case

# change here!!
NETLIST = c432.v
LIBRARY = test_lib.lib
PATTERN = c432.pat

TARGET = 312510224

all: $(TARGET)

$(OBJDIR):
	@mkdir $(OBJDIR)

$(TARGET): main.cpp $(OBJS)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $(DEFINE) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) -c $< -o $@

run:
	./$(TARGET)  $(CASEDIR)/$(NETLIST) -l $(CASEDIR)/$(LIBRARY) -i $(CASEDIR)/$(PATTERN)

clean:
	rm -rf $(OBJDIR) $(TARGET) *.txt