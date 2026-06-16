function [BER] = ComputeBER(refBits,actualBits)
    if ~isequal(size(refBits), size(actualBits))
        error('refBits and actualBits must have the same size.');
    end

    num_errors = sum(refBits ~= actualBits);
    num_bits = length(refBits);

    BER = num_errors/num_bits;
end