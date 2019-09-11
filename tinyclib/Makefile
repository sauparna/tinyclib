# subdirectory for objects
O=osx

CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LIBS = -lk

DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

SRC = \
	eprintf.c \
	khash.c   \
	klist.c   \
	kpost.c   \
	kstack.c  \
	ktree.c

OBJ = $(patsubst %.c,$(O)/%.o,$(SRC))

all: libk.a

libk.a: $(OBJ)
	ar ru $@ $^
	ranlib $@

test: test.o
	$(CC) $(CFLAGS) -I. -L. -o $@ $^ -lk

$(O)/%.o: %.c $(DEPDIR)/%.d
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRC)))

PREFIX = /usr/local

.PHONY: install
install: libk.a
	mkdir -p $(PREFIX)/include/kak
	cp -p *.h $(PREFIX)/include/kak/
	cp -p libk.a $(PREFIX)/lib/

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/lib/libk.a
	rm -f $(PREFIX)/include/kak/*.h

.PHONY: clean
clean:
	rm -f $(O)/*.o $(DEPDIR)/* libk.a test
