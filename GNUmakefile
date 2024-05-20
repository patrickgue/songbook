PROG=songbook
SRCS=songbook.c render.c renderer_html.c renderer_latex.c
OBJS=$(SRCS:.c=.o)
MAN=songbook.5

CFLAGS+=-g -Wall

World: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^

%.o:%.c
	$(CC) -c $^ $(CFLAGS)

renderer_html_template.h: template.html
	xxd -i template.html > renderer_html_template.h

renderer_latex_template.h: template.tex
	xxd -i template.tex > renderer_latex_template.h


clean:
	rm -f $(OBJS) $(PROG)
