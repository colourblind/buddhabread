/*
 * BuddhaBread - Buddhabrot Generator
 * Algorithm: Melinda Green
 * Implementation: Tom Milsom
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

const int SAMPLES_PER_PIXEL = 4;

typedef struct complex
{
    double re;
    double im;
} complex;

complex mult(complex a, complex b)
{
    complex result;
    result.re = (a.re * b.re) - (a.im * b.im);
    result.im = (a.re * b.im) + (a.im * b.re);
    return result;
}

complex add(complex a, complex b)
{
    complex result;
    result.re = a.re + b.re;
    result.im = a.im + b.im;
    return result;
}

double length(complex a)
{
    return sqrt(a.re * a.re + a.im * a.im);
}

void save(unsigned int *counters, int width, int height)
{
    FILE *out;
    int i, j;
    double colour;
    unsigned char c;
    unsigned int total = 0;
    double luminosity;
    unsigned int maxCount = 0;

    for (i = 0; i < width * height; i ++)
    {
        total += counters[i];
        maxCount = counters[i] > maxCount ? counters[i] : maxCount;
    }
    luminosity = (double)total / (width * height);

    printf("\nTotal: %d\nMax  : %d\nLumin: %f\n", total, maxCount, luminosity);

    out = fopen("out.raw", "wb");
    for (i = 0; i < height; i ++)
    {
        for (j = 0; j < width; j ++)
        {
            colour = (counters[j * width + i] / (luminosity * 10)) * 255.0;
            c = (unsigned char)(colour > 255 ? 255 : colour);
            fwrite(&c, 1, 1, out);
        }
    }
    fclose(out);
}

int main(int argc, char **argv)
{
    int width, height, maxIterations;
    int a, b;
    double x, y;
    double step = 1.0 / sqrt(SAMPLES_PER_PIXEL);
    int x1, y1;
    unsigned int bufferSize;
    unsigned int *counters;
    complex z, c;
    complex *history;
    time_t start;
    double seconds;

    if (argc < 4)
        return 1;

    width = atoi(argv[1]);
    height = atoi(argv[2]);
    maxIterations = atoi(argv[3]);

    bufferSize = sizeof(int) * width * height;
    counters = malloc(bufferSize);
    memset(counters, 0, bufferSize);

    history = malloc(sizeof(complex) * maxIterations);

    start = time(NULL);

    for (x = 0; x < width; x += step)
    {
        printf(".");
        for (y = 0; y < height; y += step)
        {
            c.re = (x * 3.0 / width - 2);
            c.im = (y * 3.0 / height - 1.5);
            z.im = z.re = 0;

            memset(history, 0, sizeof(complex) * maxIterations);

            a = 0;
            while (a < maxIterations)
            {
                z = add(mult(z, z), c);
                history[a] = z;
                if (length(z) > 2.1)
                {
                    for (b = 1; b <= a; b ++)
                    {
                        x1 = (int)((history[b].re + 2) * width / 3);
                        y1 = (int)((history[b].im + 1.5) * height / 3);
                        if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
                            counters[x1 * width + y1] ++;
                    }
                    break;
                }
                a ++;
            }
        }
    }

    seconds = difftime(time(NULL), start);
    printf("\nProcessing completed in %.0fs\nSaving raw\n", seconds);

    save(counters, width, height);

    free(counters);
    free(history);

    return 0;
}
