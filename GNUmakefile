PROG=converter
BOOK_PROG=makebook
LIB=libsongbook.a
LIB_SRCS=songbook.c render.c renderer_html.c renderer_latex.c
APP_SRCS=converter.c
BOOK_SRCS=makebook.c
LIB_OBJS=$(LIB_SRCS:.c=.o)
APP_OBJS=$(APP_SRCS:.c=.o)
BOOK_OBJS=$(BOOK_SRCS:.c=.o)
MAN=songbook.5
MAN_CAT=songbook.cat5

CFLAGS+=-g -Wall

APP_LDFLAGS=-lsongbook -L.
BOOK_LDFLAGS=-lsongbook -L.

World: $(PROG) $(BOOK_PROG) $(LIB) $(MAN_CAT)

$(PROG): $(LIB) $(APP_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(APP_LDFLAGS)

$(BOOK_PROG): $(LIB) $(BOOK_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(BOOK_LDFLAGS)

$(LIB): $(LIB_OBJS)
	ar rcs libsongbook.a $^

%.o:%.c
	$(CC) -c $^ $(CFLAGS)

renderer_html_template.h: template.html
	xxd -i template.html > renderer_html_template.h

renderer_latex_template.h: template.tex
	xxd -i template.tex > renderer_latex_template.h

$(MAN_CAT): $(MAN)
	export MAN_KEEP_FORMATTING=1
	man ./$^ > $@

clean:
	rm -f $(APP_OBJS) $(PROG) $(LIB) $(LIB_OBJS) $(BOOK_PROG) $(BOOK_OBJS)


install: $(PROG) $(BOOK_PROG)
	cp $^ /usr/local/bin/

