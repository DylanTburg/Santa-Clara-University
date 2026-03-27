#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *f = fopen("data.mem", "w");
	int i;
	for (i=0; i < 1024; i++) {
		int val = ((i*99) % 1000);
		fprintf(f, "%03x\n", val); //for readmemh
	}
	fclose(f);
	return 0;
}
