INCLUDE_DIR = ../Include
SOURCE_DIR = ../Source
EXEC_DIR = ../Exec

# compile flags
VPATH += $(INCLUDE_DIR):$(SOURCE_DIR)
CXXFLAGS += -std=c++14 -g -I $(INCLUDE_DIR)

all: main

run:
	@$(EXEC_DIR)/main $(SHB_LOG_FILE_PATH)  # e.g.: make run SHB_LOG_FILE_PATH=/var/fpwork/shb.log

clean:
	$(RM) $(EXEC_DIR)/* *.o

main: main.o
	$(CXX) $^ -o $(EXEC_DIR)/$@