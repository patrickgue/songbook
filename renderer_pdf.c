
#include "renderer_pdf.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hpdf.h>

char _filename[64];
HPDF_Doc _document;

error_handler (HPDF_STATUS error_no, HPDF_STATUS detail_no, void*user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no, (HPDF_UINT)detail_no);
    exit(1);
}

void render_pdf_init(char *filename)
{
    strncpy(_filename, filename, sizeof(_filename));
    _document = HPDF_New(error_handler, NULL);
    if (!_document) {
        fprintf(stderr, "document creation failed\n");
        exit(1);
    }
}