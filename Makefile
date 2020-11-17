CC 		 	:= clang
CFLAGS	:= -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -O3 -g
SRC			:= src
BIN     := allocTests
LIBS		:= -lcheck
OBJS		:=
HELPERS := src/helpers.h
OBJDIR	:= obj
TEST_DIR:= tests
OBJS    :=$(addprefix $(OBJDIR)/, header.o headAPI.o alloc.o )
TOBJS   :=$(addprefix $(OBJDIR)/, allocTests.o)

$(BIN): directories $(OBJS) $(TOBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LIBS)

$(OBJDIR) $(OBJDIR)/%.o: $(SRC)/%.c $(SRC)/%.h $(HELPERS)
	$(CC) $(CFLAGS) -c $< -o $@

directories:
	mkdir -p $(OBJDIR)

clean:
	rm -f *~ *.o $(TARGET)
	rm -rf obj

all: clean $(BIN)
