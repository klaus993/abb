#include <stdio.h>
#include <stdlib.h>

int main() {
	double c = 2 + rand() / (RAND_MAX / (1000 - 2) + 1),
		   d = 3 + rand() / (RAND_MAX / (1000 - 3) + 1);
	printf("%f\n%f\n", c, d);
	size_t largo = 50;
    for (unsigned i = 0; i < largo; i++) {
        // valores[i] = malloc(sizeof(int));
        // sprintf(claves[i], "%08d", i);//(int)(i + rand() / (RAND_MAX / (largo - i) + 1)));
        // // *valores[i] = i;
        // ok = abb_guardar(abb, claves[i], valores[i]);
        // if (!ok) break;
        printf("%d\n", (int)(i + rand() / (RAND_MAX / (largo - i) + 1)));
    }
}