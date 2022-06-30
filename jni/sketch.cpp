//Archivo de cabecera de SimDSP
#include <simdsp.h>


//Carga la frecuencia de muestre, tamaño del bloque y  coeficientes de los filtros 
//requeridos para implementar el algoritmo de Goertzel
#include "../matlab/coefs.h"


//Elementos de retardo de los filtros IIR intermedios usados por el algoritmo de Goertzel
double vk_n[N_FILT];
double vk_n_1[N_FILT];
double vk_n_2[N_FILT];

//Salidas acumuladas de cada uno de los filtros del algoritmo de Goertzel
double yk[N_FILT];

//Variables empleadas para la detección
int count_blks = 0;  //Contador para determinar cada cuantos bloques se realizará la detección
#define N_BLOCK_DETECT  1  //Establece cada cuantos bloques de N_DATA se mostrará la detección  
#define THRESHOLD   100e3  //Umbral de detección para cada filtro

char keyfreq[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


void resetDelays() {
  int k;
  //Inicializa los elementos de retardo de los filtros en cero
  for (k=0; k<N_FILT; k++) {
    vk_n_1[k] = 0;
    vk_n_2[k] = 0;
    yk[k] = 0;
  }
}


void displayDetection(){
    int k;
    int lowfreq_k, highfreq_k;
    float lowfreq_val, highfreq_val;
    char key[2];

    //Realiza la detección cada N_BLOCK_DETECT bloques procesados.
    //Si fs = 8000, N_DATA = 1024, y N_BLOCK_DETECT=8, se muestra el tono detectado 
    //en la consola cada segundo aproximadamente: N_DATA*N_BLOCK_DETECT/fs = 1,024 segundos
    if (count_blks == N_BLOCK_DETECT) {

      //Determina el tono bajo
      lowfreq_k = -1;
      lowfreq_val = 0;
      for (k=0; k<N_FILT/2; k++) {
        if ( (yk[k] > THRESHOLD) && (yk[k] > lowfreq_val) ) {
          lowfreq_val = yk[k];
          lowfreq_k = k;
        } 
      }

      //Determina el tono alto
      highfreq_k = -1;
      highfreq_val = 0;
      for (k=N_FILT/2; k<N_FILT; k++) {
        if ( (yk[k] > THRESHOLD) && (yk[k] > highfreq_val) ) {
          highfreq_val = yk[k];
          highfreq_k = k;
        } 
      } 

      //Determina la tecla y la muestra en la consola
      if ( (lowfreq_k!=-1) && (highfreq_k!=-1) ) {    
        key[0] = keyfreq[lowfreq_k][highfreq_k-N_FILT/2];
        key[1] = 0;
        println(key);
      }

      //Reinicia los elementos usados en la detección
      resetDelays();
      count_blks = 0;
    }
}


//Rutina que simulará la ISR que se invoca una vez ha finalizado
//la transferencia por DMA, audio apunta a los datos que se procesarán
void process_data(short *x){

  //Para los ciclos for
  int n, k;

  //Para calcular la potencia de salida de cada filtro
  double re, im;

  //Invoca captura por DMA nuevamente
  captureBlock(process_data );

  //Calcula los filtros IIR intermedios del algoritmo de Goertzel
  for(n=0; n<N_DATA; n++) {
    for (k=0; k<N_FILT; k++) {
      vk_n[k] = x[n] + vk_c[k]*vk_n_1[k] - vk_n_2[k];
      vk_n_2[k] = vk_n_1[k];
      vk_n_1[k] = vk_n[k];
    }
  }

  //Calcula la salida del filtro de Goertzel, al ser un valor complejo, 
  //acumula en yk la magnitud, para ser posteriormente usada por el detector
  for (k=0; k<N_FILT; k++) {
    re = wn_k_r[k] * vk_n_1[k] - vk_n_2[k];
    im = wn_k_i[k] * vk_n_1[k];
    re = re/16384.0;
    im = im/16384.0;
    yk[k] += re*re + im*im;
  }

  count_blks++;
  displayDetection();
}


//Funcion de inicializacion
void dsp_setup()
{
  //Habilita sistema de audio del teléfono
  enableAudio(N_DATA,F_S);

  //Imprime tasa de muestreo actual
  println("Sampling frequency is %d Hz and buffer size is %d samples",F_S,N_DATA);

  resetDelays();

  //Inicia captura por DMA
  captureBlock(process_data );
}
