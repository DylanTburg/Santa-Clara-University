function H = WalshHadamard(n)
    if n == 0
        H = 1;
    else
        H_sub = WalshHadamard(n-1);
        H = [H_sub H_sub; H_sub -H_sub];
    end
end