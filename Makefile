PROG=songbook
SRCS=songbook.c render.c renderer_html.c renderer_latex.c
MAN=songbook.5

renderer_html_template.h: template.html
	xxd -i template.html > renderer_html_template.h

renderer_latex_template.h: template.tex
	xxd -i template.tex > renderer_latex_template.h

.include<bsd.prog.mk>

