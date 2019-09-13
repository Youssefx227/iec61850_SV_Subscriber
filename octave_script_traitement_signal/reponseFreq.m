
function[f_,mod,phase] = reponseFreq(h,fc,fe) 
  N = length(h);
  g=0;
  for f=1:1:2000
    g = 0;
    for k=1:N
        g = g + h(k)*exp(-i*2*pi*k*(f/fe));
        f_(f) = f;
    endfor
     mod(f)   = abs(g);
     phase(f) = angle(g);
  endfor
end


