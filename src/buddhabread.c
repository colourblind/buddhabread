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

int save_png(char *filename, char *data, int width, int height);

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

void save(unsigned int *counters, int width, int height, char *filename)
{
    int i, j;
    double colour;
    unsigned int total = 0;
    double luminosity;
    unsigned int maxCount = 0;
    unsigned char *data;

    printf("\nProcessing results\n");

    for (i = 0; i < width * height; i ++)
    {
        total += counters[i];
        maxCount = counters[i] > maxCount ? counters[i] : maxCount;
    }
    luminosity = (double)total / (width * height);

    printf("\nTotal points: %d\nMaximum     : %d\nAverage lum : %f\n", total, maxCount, luminosity);

    data = malloc(width * height);
    for (i = 0; i < height; i ++)
    {
        for (j = 0; j < width; j ++)
        {
            colour = (counters[j * width + i] / (luminosity * 10)) * 255.0;
            data[i * width + j] = (unsigned char)(colour > 255 ? 255 : colour);
        }
    }

    printf("\nSaving to %s\n", filename);
    save_png(filename, data, width, height);
    free(data);
}

int main(int argc, char **argv)
{
    int width = 600, height = 600;
    int maxIterations = 500;
    int samplesPerPixel = 1;
    double step;
    int a, b;
    double x, y;
    int x1, y1;
    unsigned int bufferSize;
    unsigned int *counters;
    complex z, c;
    complex *history;
    time_t start;
    double seconds;
    char filename[100];

    if (argc > 1)
        width = height = atoi(argv[1]);
    if (argc > 2)
        maxIterations = atoi(argv[2]);
    if (argc > 3)
        samplesPerPixel = atoi(argv[3]);

    step =  1.0 / sqrt(samplesPerPixel);
    bufferSize = sizeof(int) * width * height;
    counters = malloc(bufferSize);
    memset(counters, 0, bufferSize);

    history = malloc(sizeof(complex) * maxIterations);

    start = time(NULL);

    printf("Mediating");
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
                if ((z.re * z.re + z.im * z.im) > 4.05)
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
    printf("\nCalculations completed in %.0fs\n", seconds);

    sprintf(filename, "out_%d_%d_%d.png", width, maxIterations, samplesPerPixel); /* TODO: Potential buffer overflow */
    save(counters, width, height, filename);

    free(counters);
    free(history);

    return 0;
}
