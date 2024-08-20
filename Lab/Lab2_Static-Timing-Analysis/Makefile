# compiler settings 
CXX = g++
CXXFLAGS = -I ./inc -std=c++11 -static -fopenmp -DPRINTTIME=true -DTESTMODE=true -DNDEBUG 
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
NETLIST = hidden_1.v
LIBRARY = test_lib.lib
PATTERN = hidden_1.pat

TARGET = 312510224

all: $(TARGET)

$(OBJDIR):
	@mkdir $(OBJDIR)

$(TARGET): main.cpp $(OBJS)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(WARNINGS) $(CXXFLAGS) $(OPTFLAGS) -c $< -o $@

run:
	time ./$(TARGET)  $(CASEDIR)/$(NETLIST) -l $(CASEDIR)/$(LIBRARY) -i $(CASEDIR)/$(PATTERN)

c17:
	time ./$(TARGET)  $(CASEDIR)/c17.v -l $(CASEDIR)/$(LIBRARY) -i $(CASEDIR)/c17.pat
	diff -w --ignore-blank-lines 312510224_c17_load.txt Test_Case/golden/golden_c17_load.txt
	diff -w --ignore-blank-lines 312510224_c17_delay.txt Test_Case/golden/golden_c17_delay.txt
	diff -w --ignore-blank-lines 312510224_c17_path.txt Test_Case/golden/golden_c17_path_1.txt
	diff -w --ignore-blank-lines 312510224_c17_gate_info.txt Test_Case/golden/golden_c17_gate_info.txt

c17_cmt:
	time ./$(TARGET)  $(CASEDIR)/c17_comment.v -l $(CASEDIR)/$(LIBRARY) -i $(CASEDIR)/c17.pat
	diff -w --ignore-blank-lines 312510224_c17_comment_load.txt Test_Case/golden/golden_c17_load.txt
	diff -w --ignore-blank-lines 312510224_c17_comment_delay.txt Test_Case/golden/golden_c17_delay.txt
	diff -w --ignore-blank-lines 312510224_c17_comment_path.txt Test_Case/golden/golden_c17_path_1.txt
	diff -w --ignore-blank-lines 312510224_c17_comment_gate_info.txt Test_Case/golden/golden_c17_gate_info.txt

hellish:
	time ./$(TARGET)  $(CASEDIR)/hellish.v -l $(CASEDIR)/$(LIBRARY) -i $(CASEDIR)/c17.pat
	diff -w --ignore-blank-lines 312510224_hellish_load.txt Test_Case/golden/golden_c17_load.txt
	diff -w --ignore-blank-lines 312510224_hellish_delay.txt Test_Case/golden/golden_c17_delay.txt
	diff -w --ignore-blank-lines 312510224_hellish_path.txt Test_Case/golden/golden_c17_path_1.txt
	diff -w --ignore-blank-lines 312510224_hellish_gate_info.txt Test_Case/golden/golden_c17_gate_info.txt

c432:
	time ./$(TARGET)  $(CASEDIR)/c432.v -l $(CASEDIR)/$(LIBRARY) -i $(CASEDIR)/c432.pat
#	diff -w --ignore-blank-lines 312510224_c432_load.txt Test_Case/golden/golden_c432_load.txt
	diff -w --ignore-blank-lines 312510224_c432_delay.txt Test_Case/golden/golden_c432_delay.txt
	diff -w --ignore-blank-lines 312510224_c432_gate_info.txt Test_Case/golden/golden_c432_gate_info.txt

c123456:
	time ./$(TARGET)  $(CASEDIR)/c123456.v -l $(CASEDIR)/$(LIBRARY) -i $(CASEDIR)/c123456.pat
	diff -w --ignore-blank-lines 312510224_c123456_load.txt Test_Case/golden/golden_c123456_load.txt
	diff -w --ignore-blank-lines 312510224_c123456_delay.txt Test_Case/golden/golden_c123456_delay.txt
	diff -w --ignore-blank-lines 312510224_c123456_gate_info.txt Test_Case/golden/golden_c123456_gate_info.txt

clean:
	rm -rf $(OBJDIR) $(TARGET) *.txt