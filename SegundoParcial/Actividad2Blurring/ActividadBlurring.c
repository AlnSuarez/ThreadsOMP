#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 10

void blur(char outputImage[], int sizeM);
void blur_inv(char outputImageName[], int sizeM);

int main(){
  #pragma omp parallel
    {
        #pragma omp sections
        {
        #pragma omp section
            blur("blurred1.bmp", 3);
        #pragma omp section
            blur("blurred2.bmp", 5);
        #pragma omp section
            blur("blurred3.bmp", 7);
        #pragma omp section
            blur("blurred4.bmp", 9);
        #pragma omp section
            blur("blurred5.bmp", 11);
        #pragma omp section
            blur("blurred6.bmp", 13);
        #pragma omp section
            blur("blurred7.bmp", 15);
        #pragma omp section
            blur("blurred8.bmp", 17);
        #pragma omp section
            blur("blurred9.bmp", 19);
        #pragma omp section
            blur("blurred10.bmp", 21);
        #pragma omp section
            blur_inv("blurred_inv1.bmp", 21);   
        #pragma omp section
            blur_inv("blurred_inv2.bmp", 19);
        #pragma omp section
            blur_inv("blurred_inv3.bmp", 17);
        #pragma omp section
            blur_inv("blurred_inv4.bmp", 15);
        #pragma omp section
            blur_inv("blurred_inv5.bmp", 13);
        #pragma omp section
            blur_inv("blurred_inv6.bmp", 11);
        #pragma omp section
            blur_inv("blurred_inv7.bmp", 9);
        #pragma omp section
            blur_inv("blurred_inv8.bmp", 7);  
        #pragma omp section
            blur_inv("blurred_inv9.bmp", 5); 
        #pragma omp section
            blur_inv("blurred_inv10.bmp", 3);     
        }
    }
  return 0;
}

void blur(char outputImageName[], int sizeM){
  FILE *image, *outputImage, *lecturas;
  image = fopen("6.bmp","rb");                
  outputImage = fopen(outputImageName,"wb");      
  long ancho;
  long alto;
  unsigned char r, g, b;    
  unsigned char* ptr;

  unsigned char xx[54];
  long cuenta = 0, anchoR = 0, altoR = 0, anchoM = 0, altoM = 0;
  long sum;
  int iR, jR;
  for(int i = 0; i < 54; i++) {
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage);      
  }
  ancho = (long)xx[20]*65536 + (long)xx[19]*256 + (long)xx[18];
  alto = (long)xx[24]*65536 + (long)xx[23]*256 + (long)xx[22];

  ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));
  omp_set_num_threads(NUM_THREADS);
    
  unsigned char foto[alto][ancho], fotoB[alto][ancho];
  unsigned char pixel;
  
    for(int i = 0; i < alto; i++){
      for(int j = 0; j < ancho; j++){
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);

        pixel = 0.21*r + 0.72*g + 0.07*b;
        foto[i][j] = pixel;
        fotoB[i][j] = pixel;
      }
    }

    anchoR = ancho/sizeM;
    altoR = alto/sizeM;
    anchoM = ancho%sizeM;
    altoM = alto%sizeM;

    int inicioX,inicioY,cierreX,cierreY,ladoX,ladoY;
    for(int i = 0; i < alto; i++){
      for(int j = 0; j < ancho; j++){
        if(i < sizeM){
          inicioX = 0;
          cierreX = i+sizeM;
          ladoX = i+sizeM;
        } else if(i >= alto-sizeM){
          inicioX = i-sizeM;
          cierreX = alto;
          ladoX = alto-i+sizeM;
        }else{
          inicioX = i-sizeM;
          cierreX = i+sizeM;
          ladoX = sizeM*2+1;
        }
        if(j < sizeM){
          inicioY = 0;
          cierreY = j+sizeM;
          ladoY = j+sizeM;
        }else if(j >= ancho-sizeM){
          inicioY = j-sizeM;
          cierreY = ancho;
          ladoY = ancho-j+sizeM;
        }else{
          inicioY = j-sizeM;
          cierreY = j+sizeM;
          ladoY = sizeM*2+1;
        }
        sum = 0;
        for(int x = inicioX; x < cierreX; x++){
          for(int y = inicioY; y < cierreY; y++){
            sum += foto[x][y];
          }
        }
        sum = sum/(ladoX*ladoY);
        fotoB[i][j] = sum;
      }
    }
   
  //asignacion a imagen
  cuenta = 0;
  for(int i = 0; i < alto; i++){
    for(int j = 0; j < ancho; j++){
      ptr[cuenta] = fotoB[i][j]; //b
      ptr[cuenta+1] = fotoB[i][j]; //g
      ptr[cuenta+2] = fotoB[i][j]; //r
      cuenta++;
    }
  }       

  //Grises
  const double startTime = omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < alto*ancho; ++i) {
      fputc(ptr[i], outputImage);
      fputc(ptr[i+1], outputImage);
      fputc(ptr[i+2], outputImage);
    }
  }

  const double endTime = omp_get_wtime();
  free(ptr);
  fclose(image);
  fclose(outputImage);
}

void blur_inv(char outputImageName[], int sizeM){
  FILE *image, *outputImage, *lecturas;
  image = fopen("6.bmp","rb");                 
  outputImage = fopen(outputImageName,"wb");    
  long ancho;
  long alto;
  unsigned char r, g, b;    
  unsigned char* ptr;

  unsigned char xx[54];
  long cuenta = 0, anchoR = 0, altoR = 0, anchoM = 0, altoM = 0;
  long sum;
  int iR, jR;
  for(int i = 0; i < 54; i++) {
    xx[i] = fgetc(image);
    fputc(xx[i], outputImage);      
  }
  ancho = (long)xx[20]*65536 + (long)xx[19]*256 + (long)xx[18];
  alto = (long)xx[24]*65536 + (long)xx[23]*256 + (long)xx[22];

  ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));
  omp_set_num_threads(NUM_THREADS);
    
  unsigned char foto[alto][ancho], fotoB[alto][ancho];
  unsigned char pixel;
  
    for(int i = 0; i < alto; i++){
      for(int j = 0; j < ancho; j++){
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);

        pixel = 0.21*r + 0.72*g + 0.07*b;
        foto[i][j] = pixel;
        fotoB[i][j] = pixel;
      }
    }

    anchoR = ancho/sizeM;
    altoR = alto/sizeM;
    anchoM = ancho%sizeM;
    altoM = alto%sizeM;

    int inicioX,inicioY,cierreX,cierreY,ladoX,ladoY;
    for(int i = alto; i > 0; i--){
      for(int j = ancho; j > 0; j--){
        if(i < sizeM){
          inicioX = alto;
          cierreX = 0;
          ladoX = i+sizeM;
        } else if(i >= alto-sizeM){
          inicioX = i+sizeM;
          cierreX = 0;
          ladoX = alto-i+sizeM;
        }else{
          inicioX = i-sizeM;
          cierreX = i+sizeM;
          ladoX = sizeM*2+1;
        }
        if(j < sizeM){
          inicioY = ancho;
          cierreY = j+sizeM;
          ladoY = j+sizeM;
        }else if(j >= ancho-sizeM){
          inicioY = j-sizeM;
          cierreY = ancho;
          ladoY = ancho-j+sizeM;
        }else{
          inicioY = j-sizeM;
          cierreY = j+sizeM;
          ladoY = sizeM*2+1;
        }
        sum = 0;
        for(int x = inicioX; x < cierreX; x++){
          for(int y = inicioY; y < cierreY; y++){
            sum += foto[x][y];
          }
        }
        sum = sum/(ladoX*ladoY);
        fotoB[i][j] = sum;
      }
    }
   
  cuenta = 0;


  for(int i = 0; i<alto ; i++){
    for(int j = 0; j<ancho; j++){
      ptr[cuenta] = fotoB[i][ancho-j]; //b
      ptr[cuenta+1] = fotoB[i][ancho-j]; //g
      ptr[cuenta+2] = fotoB[i][ancho-j]; //r
      cuenta++;
    }
  }       
                
  const double startTime = omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < alto*ancho; ++i) {
      fputc(ptr[i], outputImage);
      fputc(ptr[i+1], outputImage);
      fputc(ptr[i+2], outputImage);
    }
  }

  const double endTime = omp_get_wtime();
  free(ptr);
  fclose(image);
  fclose(outputImage);
}