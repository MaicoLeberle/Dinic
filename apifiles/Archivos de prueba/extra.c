#include <stdio.h>

struct a;
struct a {
	int b;
};

struct c {
	typedef struct {
		int d;
	} estructura;
	estructura e;
};

int main() {
	// struct a pepe;
	// pepe.b = 5;
	// printf("Holaaa! %d \n", pepe.b);
	struct c nueva_estructura;
	nueva_estructura.e.d = 10;
	printf("d = %d\n", nueva_estructura.e.d);
	return 0;
}