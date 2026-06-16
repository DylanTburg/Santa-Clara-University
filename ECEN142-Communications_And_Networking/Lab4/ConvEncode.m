function [encoded] = ConvEncode(msgBits)
    encoded = zeros(1, 2*length(msgBits));
    x1=0;
    x2=0;

    for i = 1:length(msgBits)
            x=msgBits(i);
            p0 = mod(x+ x1 + x2, 2);
            p1 = mod(x + x1, 2);

            encoded(2*i -1) = p0;
            encoded(2*i) = p1;

            x2=x1;
            x1=x;
    end
end