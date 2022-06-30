%Programa que genera y reproduce tonos DTMF 
%en el parlante del computador

dtmf_vals = {
    struct('key','1', 'f1',697, 'f2',1209);
    struct('key','2', 'f1',697, 'f2',1336);
    struct('key','3', 'f1',697, 'f2',1477);
    struct('key','A', 'f1',697, 'f2',1633);
    struct('key','4', 'f1',770, 'f2',1209);
    struct('key','5', 'f1',770, 'f2',1336);
    struct('key','6', 'f1',770, 'f2',1477);
    struct('key','B', 'f1',770, 'f2',1633);
    struct('key','7', 'f1',852, 'f2',1209);
    struct('key','8', 'f1',852, 'f2',1336);
    struct('key','9', 'f1',852, 'f2',1477);
    struct('key','C', 'f1',852, 'f2',1633);
    struct('key','*', 'f1',941, 'f2',1209);
    struct('key','0', 'f1',941, 'f2',1336);
    struct('key','#', 'f1',941, 'f2',1477);
    struct('key','D', 'f1',941, 'f2',1633)
   };
    
disp('Generador de tonos DTMF');
s = upper( input('Escriba una cadena de caracteres 0-9, A-D, *, #:','s') );

for n=1:length(s)
    for k=1:length(dtmf_vals)
        if (s(n) == dtmf_vals{k}.key)
            x = play_dtmf(dtmf_vals{k}.f1, dtmf_vals{k}.f2);
        end
    end  
end

function [x] = play_dtmf(f1, f2)
    fs = 44100;
    n = 0:fs-1;
    Oc1  = 2*pi*f1/fs;
    Oc2  = 2*pi*f2/fs;
    x = 0.5* (sin(n*Oc1) + sin(n*Oc2));
    sound(x,fs);
    pause(1.5);
end