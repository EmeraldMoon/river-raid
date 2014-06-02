CC:= gcc
CFLAGS:= -Wall -O3 -ansi -pedantic -std=c99
LIBS:= -lm -lGL -lGLU -lglut
MKDIR:= mkdir -p
RMDIR:= rm -rf
RM:= rm -rf
BINDIR:= bin
SRCDIR:= src
INCDIR:= include
OBJDIR:= obj
DOCDIR:= doc
BIN:= River
SRC:= $(wildcard $(SRCDIR)/*.c)
OBJ:= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
INC:= -I$(INCDIR)
TAR:= $(BIN).tar

.PHONY: dump tar count clean distclean tarclean

all: $(BINDIR)/$(BIN)

$(BINDIR)/$(BIN): $(OBJ) | $(BINDIR)
	$(CC) $(LIBS) $^ -o $@
	@echo "Generating C binary \033[1;32m"$@"\033[0m"

$(OBJ): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJDIR) $(BINDIR):
	$(MKDIR) $@

dump:
	@echo "src:" $(SRC)
	@echo "obj:" $(OBJ)

tar:
	$(MKDIR) $(BIN)/
	cp -r $(INCDIR)/ $(SRCDIR)/ $(DOCDIR)/ Makefile *.md $(BIN)/
	tar -czf $(TAR) $(BIN)/
	$(RM) $(BIN)/
	@echo "Arquivo\033[1;32m" $(TAR) "\033[0mcriado com sucesso"

count:
	wc -l $(SRCDIR)/* $(INCDIR)/*

clean:
	$(RMDIR) $(OBJDIR)/

distclean: clean
	$(RMDIR) $(BINDIR)/

tarclean:
	$(RM) $(TAR)
