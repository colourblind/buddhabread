#include "png.h"

int save_png(char *filename, unsigned char *data, int width, int height)
{
    png_structp png_ptr;
    png_infop info_ptr;
    int i;
    FILE *file = fopen(filename, "wb");

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
       return 1;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
       png_destroy_write_struct(&png_ptr,
         (png_infopp)NULL);
       return 2;
    }

    png_init_io(png_ptr, file);

    if (setjmp(png_jmpbuf(png_ptr)))
    {
       png_destroy_write_struct(&png_ptr, &info_ptr);
       fclose(file);
       return 3;
    }

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
        return 4;

    for (i = 0; i < height; i ++)
        png_write_row(png_ptr, &(data[i * width]));

    if (setjmp(png_jmpbuf(png_ptr)))
        return 5;

    png_write_end(png_ptr, NULL);

    fclose(file);

    return 0;
}
