CC:= gcc
CFLAGS:= -Wall -O3 -ansi -pedantic
LM:= -lm
OPENGL:= -lGL -lGLU -lglut
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
TAR:= $(BIN).tar

.PHONY: clean distclean dump

all: $(BINDIR)/$(BIN)

$(BINDIR)/$(BIN): $(OBJ) | $(BINDIR)
	$(CC) $(M32) $(LM) $(OPENGL) $^ -o $@
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

tar:
	mkdir River/
	cp -r include/ src/ doc/ Makefile *.md River/
	tar -czf $(TAR) River/
	rm -rf River/	
	@echo "Arquivo\033[1;32m" $(TAR) "\033[0mcriado com sucesso"

tarclean:
	rm -r $(TAR)

count:
	wc -l src/* include/*
