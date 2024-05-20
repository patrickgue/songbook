PROG=songbook
LIB=libsongbook.a
SRCS=songbook.c render.c renderer_html.c renderer_latex.c
APP_SRCS=$(SRCS) main.c
OBJS=$(SRCS:.c=.o)
APP_OBJS=$(APP_SRCS:.c=.o)
MAN=songbook.5

CFLAGS+=-g -Wall

World: $(PROG) $(LIB)

$(PROG): $(APP_OBJS)
	$(CC) -o $@ $(LDFLAGS) $^

$(LIB): $(OBJS)
	ar rcs libsongbook.a $^

%.o:%.c
	$(CC) -c $^ $(CFLAGS)

renderer_html_template.h: template.html
	xxd -i template.html > renderer_html_template.h

renderer_latex_template.h: template.tex
	xxd -i template.tex > renderer_latex_template.h


clean:
	rm -f $(APP_OBJS) $(PROG) $(LIB)
