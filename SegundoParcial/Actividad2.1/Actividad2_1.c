#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 6
void flipHorizontal();

void flipVertical();

int main()
{

  double start;
  double end;
  start = omp_get_wtime();

  omp_set_num_threads(NUM_THREADS);
  flipHorizontal();
  flipVertical();


  end = omp_get_wtime();
  printf("Work took %f seconds\n", end - start);
  return 0;
}

void flipVertical()
{
  FILE *image, *outputImage, *lecturas;
  int row, col;
  image = fopen("6.bmp", "rb");        // Imagen original a transformar
  outputImage = fopen("img1_dd.bmp", "wb"); // Imagen transformada
  long ancho;
  int counter = 0;
  long alto;
  unsigned char r, g, b, Pixel, Pixel1, Pixel2, Pixel3; // Pixel
  // unsigned char* ptr;
  unsigned char xx[54];

  for (int i = 0; i < 54; i++)
  {
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage); // Copia cabecera a nueva imagen
  }

  ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
  alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);

  unsigned char threshold = 100;

  // Guardar imagen en una matriz
  unsigned char mat[ancho * 3][alto];


#pragma omp for schedule(static)
  for (row = 0; row < alto; row++)
  {
    for (col = 0; col < ancho * 3; col = col + 3)
    {
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);

      unsigned char pixel = 0.21 * r + 0.72 * g + 0.07 * b;

      mat[col][row] = pixel;
      mat[col + 1][row] = pixel;
      mat[col + 2][row] = pixel;
    }
  }



#pragma omp for schedule(static) 
  for (row = alto; row > 0; row--)
  {
    for (col = 0; col < ancho * 3; col++)
    {
      Pixel = mat[col][row];
      fputc(Pixel, outputImage);
      counter++;
    }
  }

  printf("%d\n", counter);

  fclose(image);
  fclose(outputImage);

}

void flipHorizontal()
{
  FILE *image, *outputImage, *lecturas;
  int row, col;
  image = fopen("6.bmp", "rb");        // Imagen original a transformar
  outputImage = fopen("img2_dd.bmp", "wb"); // Imagen transformada
  long ancho;
  int counter = 0;
  long alto;
  unsigned char r, g, b, Pixel, Pixel1, Pixel2, Pixel3; // Pixel
  // unsigned char* ptr;
  unsigned char xx[54];
  for (int i = 0; i < 54; i++)
  {
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage); // Copia cabecera a nueva imagen
  }
  ancho = (long)xx[20] * 65536 + (long)xx[19] * 256 + (long)xx[18];
  alto = (long)xx[24] * 65536 + (long)xx[23] * 256 + (long)xx[22];
  printf("largo img %li\n", alto);
  printf("ancho img %li\n", ancho);

  unsigned char threshold = 100;

  // Guardar imagen en una matriz
  unsigned char mat[ancho * 3][alto];

#pragma omp for schedule(static) 
  for (row = 0; row < alto; row++)
  {
    for (col = 0; col < ancho * 3; col = col + 3)
    {
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);

      unsigned char pixel = 0.21 * r + 0.72 * g + 0.07 * b;

      mat[col][row] = pixel;
      mat[col + 1][row] = pixel;
      mat[col + 2][row] = pixel;
    }
  }

#pragma omp for schedule(static) 
  for (row = 0; row < alto; row++)
  {
    for (col = ancho * 3; col > 0; col--)
    {
      Pixel = mat[col][row];
      fputc(Pixel, outputImage);
      counter++;
    }
  }

  printf("%d\n", counter);

  fclose(image);
  fclose(outputImage);
}