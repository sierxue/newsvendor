
CC := g++ 
SRCDIR := src
BUILDDIR := build
TARGET := bin/runner
TEST_IN := test/input.txt
TEST_OUT := test/output.csv

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g # -Wall
LIB := 
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
	rm -f $(TESTS) gtest.a gtest_main.a *.o

run:
	@echo " $(TARGET) $(TEST_IN) $(TEST_OUT)"; $(TARGET) $(TEST_IN) $(TEST_OUT)
	@echo " more $(TEST_OUT)"; more $(TEST_OUT)

