CC:= gcc
CFLAGS:= -Wall -O3 -ansi -pedantic
LM:= -lm
M32:= -m32
RM:= rm -f
RMDIR:= rm -rf
MKDIR:= mkdir -p
BINDIR:= bin
SRCDIR:= src
INCDIR:= include
OBJDIR:= obj
BIN:= River
SRC:= $(wildcard $(SRCDIR)/*.c)
OBJ:= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
CLIBS:= -I$(INCDIR)

.PHONY: clean distclean dump

all: $(BINDIR)/$(BIN)

$(BINDIR)/$(BIN): $(OBJ) | $(BINDIR)
	$(CC) $(M32) $(LM) $^ -o $@
	@echo "Generating C binary \033[1;32m"$@"\033[0m"

$(OBJ): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(CLIBS) $(M32) -c $< -o $@

$(OBJDIR) $(BINDIR):
	$(MKDIR) $@

clean:
	$(RMDIR) $(OBJDIR)

distclean: clean
	$(RMDIR) $(BINDIR)	

dump:
	@echo "src:" $(SRC)
	@echo "obj:" $(OBJ)
