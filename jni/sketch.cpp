//Archivo de cabecera de SimDSP
#include <simdsp.h>


//Carga la frecuencia de muestre, tamaño del bloque y  coeficientes de los filtros 
//requeridos para implementar el algoritmo de Goertzel
#include "../matlab/coefs.h"

//Elementos de retardo de los filtros IIR intermedios usados por el algoritmo de Goertzel
short vk_n[N_FILT];
short vk_n_1[N_FILT];
short vk_n_2[N_FILT];

//Salidas acumuladas de cada uno de los filtros del algoritmo de Goertzel
float yk[N_FILT];

//Variables empleadas para la detección
int count_blks = 0;  //Contador para determinar cada cuantos bloques se realizará la detección
#define N_BLOCK_DETECT  40  //Establece cada cuantos bloques de N_DATA se mostrará la detección  
#define THRESHOLD   100000  //Umbral de detección para cada filtro

char keyfreq[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '9', 'D'}
};

//Rutina que simulará la ISR que se invoca una vez ha finalizado
//la transferencia por DMA, audio apunta a los datos que se procesarán
void process_data(short *x){

  //Para los ciclos for
  int n, k;

  //Para calcular la potencia de salida de cada filtro
  float re, im;

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
    re = vk_n[k] - wn_k_r[k] * vk_n_1[k];
    im =         - wn_k_i[k] * vk_n_1[k];
    yk[k] += re*re + im*im;
  }

  count_blks++;
}


//Funcion de inicializacion
void dsp_setup()
{
  int k;

  //Habilita sistema de audio del teléfono
  enableAudio(N_DATA,F_S);

  //Imprime tasa de muestreo actual
  println("Sampling frequency is %d Hz and buffer size is %d samples",F_S,N_DATA);

  //Inicializa los elementos de retardo de los filtros en cero
  for (k=0; k<N_FILT; k++) {
    vk_n_1[k] = 0;
    vk_n_2[k] = 0;
    yk[k] = 0;
  }

  //Inicia captura por DMA
  captureBlock(process_data );
}

void dsp_loop(){
    int k;
    int lowfreq_k, highfreq_k;
    float lowfreq_val, highfreq_val;
    char key;

    //Realiza la detección cada N_BLOCK_DETECT bloques procesados.
    //Si fs = 44100, N_DATA = 1024, y N_BLOCK_DETECT=40, se muestra el tono detectado 
    //en la consola cada segundo aproximadamente: N_DATA*N_BLOCK_DETECT/fs = 0,92 segundos
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
        key = keyfreq[lowfreq_k][highfreq_k-N_FILT/2];
        println(key);
      }

      //Reinicia los elementos usados en la detección
      for (k=0; k<N_FILT; k++) {
        yk[k] = 0;
      }
      count_blks = 0;
    }
}
