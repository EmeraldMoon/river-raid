CC:= gcc
CFLAGS:= -Wall -O3 -ansi -pedantic
LM:= -lm
GLLIBS:= -lGL -lGLU -lglut
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
TAR:= $(BIN).tar

.PHONY: clean distclean dump

all: $(BINDIR)/$(BIN)

$(BINDIR)/$(BIN): $(OBJ) | $(BINDIR)
	$(CC) $(LM) $(GLLIBS) $^ -o $@
	@echo "Generating C binary \033[1;32m"$@"\033[0m"

$(OBJ): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(CLIBS) -c $< -o $@

$(OBJDIR) $(BINDIR):
	$(MKDIR) $@

dump:
	@echo "src:" $(SRC)
	@echo "obj:" $(OBJ)

tar:
	mkdir River/
	cp -r include/ src/ doc/ Makefile *.md River/
	tar -czf $(TAR) River/
	rm -rf River/	
	@echo "Arquivo\033[1;32m" $(TAR) "\033[0mcriado com sucesso"

count:
	wc -l src/* include/*

clean:
	$(RMDIR) $(OBJDIR)

distclean: clean
	$(RMDIR) $(BINDIR)

tarclean:
	rm -r $(TAR)
