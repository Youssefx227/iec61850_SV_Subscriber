

function bufferFIR = FIR_(h,signal_to_filter) 
 bufferFIR = zeros(1,length(signal_to_filter));
	for n=1:length(signal_to_filter)
		for k=1:length(h)
    if((n-k)>0)
			bufferFIR(n) = bufferFIR(n)+ h(k)*signal_to_filter((n-k));
    end
		endfor
	endfor
end
