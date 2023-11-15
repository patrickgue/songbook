PROG=songbook
SRCS=songbook.c render.c renderer_html.c
MAN=

renderer_html_template.h: template.html
	xxd -i template.html > renderer_html_template.h

.include<bsd.prog.mk>

