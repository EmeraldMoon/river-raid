CPPC   = g++
CFLAGS = -Wall -Wextra -pedantic -Wno-unused-result -Wno-unused-parameter -std=c++14 -g
LIBS   = -lm -lGL -lGLU -lglut -lsfml-graphics -lsfml-window -lsfml-system
BINDIR = bin
SRCDIR = src
INCDIR = include
OBJDIR = obj
DOCDIR = doc
TEXDIR = texture
MODDIR = model
SRC    = $(wildcard $(SRCDIR)/*.cpp)
OBJ    = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))
TAR    = $(BIN).tar.gz
BIN    = River

.PHONY: dump count tar clean new pull push amend

all: $(BINDIR)/$(BIN)

$(BINDIR)/$(BIN): $(OBJ) | $(BINDIR)
	$(CPPC) $^ $(LIBS) -o $@
	@echo "\033[1m> Generating C++ binary \033[32m"$@"\033[0m"

$(OBJ): | $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CPPC) $(CFLAGS) -I$(INCDIR)/ -c $< -o $@

$(OBJDIR) $(BINDIR):
	mkdir -p $@

dump:
	@echo "src:" $(SRC)
	@echo "obj:" $(OBJ)

count:
	wc -l $(SRCDIR)/* $(INCDIR)/*

tar:
	mkdir -p $(BIN)/
	cp -r `ls | grep -v "$(BINDIR)\|$(OBJDIR)\|$(BIN)"` $(BIN)/
	tar -czf $(TAR) $(BIN)/
	rm -rf $(BIN)/
	@echo "\033[1m> Arquivo\033[32m" $(TAR) "\033[37mcriado com sucesso.\033[0m"

clean:
	rm -rf $(OBJDIR)/ $(BINDIR)/ $(TAR)

new: clean | all

pull:
	git pull

push:
	git add . && git commit && git push

amend:
	git add . && git commit --amend && git push -f
