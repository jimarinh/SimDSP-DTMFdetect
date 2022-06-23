%Implementación en Matlab del algoritmo de Goertzel

%Frecuencias de los tonos DTMF
dtmf_freqs = [697, 770, 852, 941, 1209, 1336, 1477, 1633];

N_DATA = 1024;  %Tamaño del bloque de procesamiento
fs = 44100;     %Frecuencia de muestreo

%Calcula los valores de k asociados a las frecuencias DTMF
k = fix( N_DATA * (dtmf_freqs/fs) );

%Genera un tono DTMF articial
n = 0:fs-1;
Oc1  = 2*pi*697/fs;
Oc2  = 2*pi*1477/fs;
x = 0.5* (sin(n*Oc1) + sin(n*Oc2));

%Implementa el algoritmo de Goertzel
close all;

r = 0.9995;
yk = zeros( length(dtmf_freqs), fix(length(x)/N_DATA)+1 );

for n=1:length(dtmf_freqs)
    vk = filter( 1, [1 -2*r*cos(2*pi*k(n)/N_DATA) 1], x );
    for nn = N_DATA:N_DATA:length(vk)
        ykk = vk(nn) - exp(-1i*2*pi*k(n)/N_DATA) * vk(nn-1);
        yk(n, nn/N_DATA) = abs(ykk).^2;
    end
end

%plot(yk')
%legend('697', '770', '852', '941', '1209', '1336', '1477', '1633')

sum(yk,2)