#include <stdio.h>
#include <stdlib.h>


#define XBUF_LEN 256
#define YBUF_LEN 16
#define B_LEN 160
#define A_LEN 12

//General IIR Filter
float xbuffer[XBUF_LEN] = {0};
float ybuffer[YBUF_LEN] = {0};
float bcoeff[B_LEN]; // Initialize with MATLAB-generated values
float acoeff[A_LEN]; // a0 is 1 and not stored here

int time_index = 0;

void filter_step(float input_sample) {
    xbuffer[time_index] = input_sample;
    float output = .0f;
    //current and past inputs (b coef)
    for (int i = 0; i < B_LEN; i++) {
        int idx = (time_index - i + XBUF_LEN) % XBUF_LEN;
        output += bcoeff[i] * xbuffer[idx];
    }
    //past outputs (a coef). also skip a0
    for (int i = 1; i <= A_LEN; i++) {
        int idx = (time_index - i + YBUF_LEN) % YBUF_LEN;
        output -= acoeff[i - 1] * ybuffer[idx];
    }
    ybuffer[time_index % YBUF_LEN] = output;
    time_index = (time_index + 1) % XBUF_LEN;
}

int main() {
    printf("Hello world!\n");
    return 0;
}
