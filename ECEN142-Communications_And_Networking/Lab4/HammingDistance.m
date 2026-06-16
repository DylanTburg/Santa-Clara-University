function [distance] = HammingDistance(x,y)
    sum = 0;
    for i = 1:length(x)
        if x(i) ~= y(i)
            sum = sum + 1;
        end
    end
    distance = sum;
end