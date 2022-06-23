% Este programa calcula los coeficientes requeridos para la implementación
% del algoritmo de Goertzel para el cálculo rápido del espectro en las
% frecuencias específicas de los tonos DTFM. 
% Los coeficientes se exportan al archivo de cabecera .h para ser 
% usado en la implementación del algoritmo en SimDSP.
% =============================================================================================

%Frecuencias de los tonos DTMF
dtmf_freqs = [697, 770, 852, 941, 1209, 1336, 1477, 1633];

N_DATA = 1024;  %Tamaño del bloque de procesamiento
fs = 44100;     %Frecuencia de muestreo

%Calcula los valores de k asociados a las frecuencias DTMF
k = fix( N_DATA * (dtmf_freqs/fs) );

%Calcula los coeficientes requeridos para cada k

r = 0.9995;     %Ubicación de los polos complejos conjugados
vk_c = 2*r*cos(2*pi*k/N_DATA);
wn_k = exp(-1i*2*pi*k/N_DATA);

%Genera el archivo de cabecera .h para el programa en C
fid = fopen('coefs.h', 'wt');
fprintf(fid, '#define F_S %d\n', fs);
fprintf(fid, '#define N_DATA %d\n', N_DATA);
fprintf(fid, '#define N_FILT %d\n', length(k));
fprintf(fid, 'float vk_c[] = {\n');
fprintf(fid, '%f,\n', vk_c);
fprintf(fid, '};\n');
fprintf(fid, 'float wn_k_r[] = {\n');
fprintf(fid, '%f,\n', real(wn_k));
fprintf(fid, '};\n');
fprintf(fid, 'float wn_k_i[] = {\n');
fprintf(fid, '%f,\n', imag(wn_k));
fprintf(fid, '};\n');
fclose(fid);
