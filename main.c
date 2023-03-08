#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculsecu.h"

int main(){

	long K_16 = rechercheExostive(chiffrerJuste,chiffrerFaux);
	printf("K16 : %lx\n", K_16); 
	printf("Clef : %lx\n", getK64bitParite(messageClaire, chiffrerJuste, K_16));
		
	return 0;
}