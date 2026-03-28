// Starter file for Question 1 of HW1 for Fall 2024 CSCI 163.
// Implements and tests 3 different algorithms for computing Fibonacci numbers.
//
// Write your name, the date, and anyone you collaborated with here. Dylan Thornburg | 9/27/2024

#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

int fib1(int n);
int fib2(int n);
int fib3(int n);
void fibExperiments(int n, ostream* o);

int main() {
	// print experiment output to console
	fibExperiments(50,&cout);

	// print experiment output to a csv file
	ofstream ofile("output.csv");
	fibExperiments(50,&ofile);
	ofile.close();
	return 0;
}

// calculates F_i, i=0...n with each algorithm, outputting runtime to *o
void fibExperiments(int n, ostream* o) {
	clock_t start, end;
	int ans;
	for (int i=0; i<=n; i++) {
		*o << i << " ";
		start = clock();
		ans = fib1(i);
		*o << ans << " ";
		end = clock();
		*o << double(end-start)/CLOCKS_PER_SEC << " ";
		start = clock();
		if (fib2(i)!=ans) return;
		end = clock();
		*o << double(end-start)/CLOCKS_PER_SEC << " ";
		start = clock();
		if (fib3(i)!=ans) return;
		end = clock();
		*o << double(end-start)/CLOCKS_PER_SEC << endl;
	}
}

// TODO: implement the recursive algorithm from class
int fib1(int n) {
    if (n < 2)
    {
        return n;
    }
    return fib1(n - 2) + fib1(n - 1);
}

// TODO: implement the iterative algorithm from class
int fib2(int n) {
	if (n < 2)
    {
        return n;
    }
    int f[n+1];
    f[0] = 0;
    f[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        f[i] = f[i - 1] + f[i - 2];
    }
    return f[n];
}

// TODO: implement a new space-efficeint iterative algorithm without an array
int fib3(int n) {
	if (n < 2)
    {
        return n;
    }
    int x = 0;
    int y = 1;
    int z;
    for (int i = 2; i <= n; i++)
    {
        z = x+y;
        x=y;
        y=z;
    }
    return z;
}
