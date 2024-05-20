PROG=converter
LIB=libsongbook.a
LIB_SRCS=songbook.c render.c renderer_html.c renderer_latex.c
APP_SRCS=converter.c
LIB_OBJS=$(LIB_SRCS:.c=.o)
APP_OBJS=$(APP_SRCS:.c=.o)
MAN=songbook.5

CFLAGS+=-g -Wall

APP_LDFLAGS=-lsongbook -L.

World: $(PROG) $(LIB)

$(PROG): $(LIB) $(APP_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(APP_LDFLAGS)

$(LIB): $(LIB_OBJS)
	ar rcs libsongbook.a $^

%.o:%.c
	$(CC) -c $^ $(CFLAGS)

renderer_html_template.h: template.html
	xxd -i template.html > renderer_html_template.h

renderer_latex_template.h: template.tex
	xxd -i template.tex > renderer_latex_template.h


clean:
	rm -f $(APP_OBJS) $(PROG) $(LIB) $(LIB_OBJS)
