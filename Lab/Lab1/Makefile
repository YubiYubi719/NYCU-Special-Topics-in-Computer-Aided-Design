# compiler settings 
CXX = g++
CXXFLAGS = -I ./inc -std=c++11
OPTFLAGS = -march=native -flto -funroll-loops -finline-functions -ffast-math -O3
WARNINGS = -g -Wall

# Source files and object files
SRCDIR = src
OBJDIR = obj
INCDIR = inc
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# testcase file
CASEDIR = testcase/OpenCase
OUTPUTDIR = output

# change here!!
CASEIDX = 6
TESTINPUT = input$(CASEIDX).txt
TESTOUTPUT = output$(CASEIDX).txt

TARGET = 312510224.o

all: $(TARGET)

$(OBJDIR):
	@mkdir $(OBJDIR)

$(OUTPUTDIR):
	@mkdir $(OUTPUTDIR)

$(TARGET): 312510224.cpp $(OBJS)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) -c $< -o $@

run: | $(OUTPUTDIR)
	./$(TARGET) $(CASEDIR)/$(TESTINPUT) $(OUTPUTDIR)/$(TESTOUTPUT)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(OUTPUTDIR)