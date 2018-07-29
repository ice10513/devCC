INCLUDE_DIR = ../Include
SOURCE_DIR = ../Source
EXEC_DIR = ../Exec
OBJS = StateTransitionLogHandler.o RespMessageLogHandler.o \
	PlainMessageLogHandler.o MessageLogHandler.o StateHandler.o DataRepository.o

# compile flags
VPATH += $(INCLUDE_DIR):$(SOURCE_DIR)
CXXFLAGS += -std=c++14 -g -W -I $(INCLUDE_DIR)

all: main

run: main
	@$(EXEC_DIR)/main $(SHB_LOG_PATH)  # e.g.: make run SHB_LOG_PATH=/var/fpwork/shb.log

clean:
	$(RM) $(EXEC_DIR)/* *.o

main: main.o $(OBJS)
	$(CXX) $^ -o $(EXEC_DIR)/$@