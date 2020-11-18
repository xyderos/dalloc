CC 		 	:= clang
CFLAGS	:= -g -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -O3
SHARED  := -shared
SRC			:= src
TEST_SRC:= tests
OBJDIR	:= obj
TOBJDIR := tobj
LIB     := libdalloc.so
LIBS		:= -lcheck
TEST    := test
OBJS    :=$(addprefix $(OBJDIR)/, header.o headAPI.o dalloc.o)
TOBJS   :=$(addprefix $(TOBJDIR)/, allocTests.o)

$(LIB): directories $(OBJS)
	$(CC) $(CFLAGS) $(SHARED) $(OBJS) -o $(LIB)

$(OBJDIR) $(OBJDIR)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST): $(TOBJS)
	$(CC) $(CFLAGS) $(LIB) $(TOBJS) -o $(TEST) $(LIBS) -L . -ldalloc

$(TOBJDIR) $(TOBJDIR)/%.o: $(TEST_SRC)/%.c $(TEST_SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p $(OBJDIR) $(TOBJDIR);

clean:
	rm -f *~ *.o $(TARGET)
	rm -rf obj tobj

all: clean $(LIB) $(TEST)
