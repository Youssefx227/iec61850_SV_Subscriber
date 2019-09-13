clear all
close all
close figure
clc

t =0:1/3999:0.2;
V = 11000;
f_50 = 50;
for i=1:length(t)
  if(i<80*5)
  x (i)=100*sqrt(2)*cos(2*pi*f_50.*t(i))+ 20*cos(2*pi*1500.*t(i));
else
   x (i)=1000*sqrt(2)*cos(2*pi*f_50.*t(i)) + 20*cos(2*pi*1500.*t(i));
  end
endfor

fe = 4000; %[Hz]
fc = 500; %[Hz]

a = fc/fe;

%calcul coeff h
M= 3; %length of coefficient
sum =0;
for k=1:M+1
  if((k-M/2) ==0)
    h(k) =2*pi*a;
  else
    h(k) = sin(2*pi*(k-M/2)*a)/(k-M/2);
  end 
endfor

figure()
plot(h);
ylabel('amplitude')
xlabel('numéro d échantillon')
grid on


%fenêtre hamming 

for i =1:M+1
      %== hamming window ==%
   % w(i) =  0.54-0.46*cos(2*pi*i/N);
    %== Blackman window ==%
    w(i) =  0.42  - 0.5*cos(2*pi*i/M) + 0.08*cos(4*pi*i/M);
endfor

figure()
plot(w)
title('fenêtre de Blackman')
grid

 h = h.*w;

for i=1:M+1          % normaliser le filtre passe bas kernel pour avoir
  sum = sum + h(i);  % un gain unitaire en DC
endfor

for i=1:M+1
  h(i) = h(i)/sum;
endfor

figure()
plot(h)
title('windowed-filter kernel')
grid

[f,mod,phase] = reponseFreq(h,fc,fe);
figure()
subplot(2,1,1)
semilogx(f,20*log10(mod))
title('réponse fréquentielle filtre avec fenêtre blackman')
ylabel('module');
xlabel('f (Hz)')
grid on
subplot(2,1,2)
semilogx(f,20*log10(phase));
ylabel('phase');
xlabel('f (Hz)')
grid on

x_ = FIR_(h,x);
factor =1;
i =1;
if (i<=length(x_))
			for j=1:(length(x_)/factor)
				xds(j) = x_(i);	
				i+=factor;
			end
end

figure()
subplot(2,1,1)
plot(x,'r')
grid
ylabel('signal origine')
subplot(2,1,2)
plot(xds,'b')
ylabel('signal filtré')
grid

N=fe/(f_50*factor);
  sum1 = 0;
  sum2 = 0;
  for k=1:length(xds)
      angle = 2*pi/N;
      output_real_f(k) =  xds(k)*cos(k*angle); 
      output_imag_f(k) =  -xds(k)*sin(k*angle);
  end
  
  output_real = zeros(1,length(xds));
  output_imag = zeros(1,length(xds));
  
  for k=2:length(xds)
    if(k<=N)
      output_real(k) = (2*output_real_f(k)/N )+ output_real(k-1); 
      output_imag(k) = (2*output_imag_f(k)/N) + output_imag (k-1);
     else 
        for i=k-(N-1):k
          sum1  += output_real_f(i);
          sum2  += output_imag_f(i);
        end
         output_real(k) = (2/N)*sum1; 
         output_imag(k) = (-2/N)*sum2;
     end
     module(k)    = sqrt(output_real(k)^2 + output_imag(k)^2);
     argument (k) = atan(output_imag(k)/output_real(k)) * 180/pi;
     sum1=0;
     sum2=0;
  end
    
        
##
figure()
plot(module)
hold on
plot(xds)
grid
figure()
plot(argument)

























