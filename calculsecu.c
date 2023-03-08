#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculsecu.h"

//passer d'un long en hexa en un tableau de bit
void hexatobinary(int *tabResult, long hexa, int nbrHexa){
	long tmp = hexa;
	int entier;
	int compteur = nbrHexa*4-1;
	for(int j=0;j<nbrHexa;j++){
		entier = tmp & 0xF;
		for(int i=0;i<4;i++){
			tabResult[compteur] = entier % 2;
			entier = entier / 2;
			compteur--;
		}
		tmp = tmp >> 4;
	}

}

//transformer un entier en un tableau de bit
void decimaltobinary(int *tabResult, int decimal, int nbrBit){
	int entier = decimal;
	for(int i=nbrBit-1; i>=0; i--){
		tabResult[i] = entier % 2;
		entier = entier /2;
	}
}

//élevé a la puissance
long puissance(int a,int b){
	if(b == 0){
		return 1;
	} else {
		return a*puissance(a,b-1);
	}
}

//transformer un tableau de bit en un entier
int TabtoInt(int *tab,int nbrBit){	
	int nombre=0;
	int i=0,j=nbrBit-1;
	while(j>=0){
		if(tab[j] != 0){
			nombre += puissance(2,i);
		}
		i++;
		j--;
	}
	return nombre;
}

//transformer un tableau de bit en un hexadecimal
long TabtoLong(int *tab,int nbrBit){	
	long nombre=0;
	int i=0,j=nbrBit-1;
	while(j>=0){
		if(tab[j] != 0){
			nombre += puissance(2,i);
		}
		i++;
		j--;
	}
	return nombre;
}

//gere toute les permutation y compris l'expension
void Permutation(int *resultat,int *aPermuter,const int *tablePermutation, int nbrBit){
	for(int i=0;i<nbrBit;i++){
		if(tablePermutation[i] != 0){
			resultat[i] = aPermuter[tablePermutation[i]-1];
		}
	}
}

//divise un tableau de bit en 2 tableau de meme longueur
void splitTab(int *completTab,int *leftTab,int *rightTab, int nbrBit){
	for(int i=0;i<nbrBit;i++){
		leftTab[i] = completTab[i];
		rightTab[i] = completTab[i+nbrBit];
	}
}

//applique un XOR entre 2 tableau de bit
void xor(int *tabResult, int *premierK, int *deuxiemeK, int nbrBit){
	for(int i=0;i<nbrBit;i++){
		tabResult[i] = premierK[i] ^ deuxiemeK[i];
	}
}

//trouve la position du bit fauter
int bitFauter(int *tabJuste, int *tabFaux){
	int tabxor[33] = {0};
	xor(tabxor, tabJuste, tabFaux, 32);
	for(int j=0;j<32;j++){
		if(tabxor[j] == 1){
			return j;
		}
	}
	return -1;
}

//la boite de substitution qui transforme 6 bit en 4 bit selon la S-box choisi
void Sboxfonc(int *resultat, int *entrer, int numSbox){
	int row = 0;
	int column = 0;
	row = entrer[0]*2+entrer[5];
	int i=0,j=4;
	while(j>0){
		if(entrer[j] != 0){
			column += puissance(2,i);
		}
		i++;
		j--;
	}
	long resultat4bit = sbox[numSbox][row][column];
	hexatobinary(resultat, resultat4bit,1);
}

//s'occupe de retrouver le L16 et le L16 fauter
void obtenirR16L16(long hexa,Message *m){
	m->chiffrerHexa = hexa;
	hexatobinary(m->chiffrerBinaire,hexa,16);
	Permutation(m->chiffrerBinairePermuter, m->chiffrerBinaire, ip, 64);
	splitTab(m->chiffrerBinairePermuter,m->leftChiffrer,m->rightChiffrer, 32);
	
}

//On extrer 6 bit une position bien defini au niveau des S-box
void extraire6Bits(Message *m, int position){
	for(int i=0; i<6; i++) {
		m->sbox6Bits[i] = m->rightChiffrerExp[6*position+i];
	}
}

//Compare deux tableau de meme taille
int egale(int *tab1, int *tab2, int nbrBit){
	for(int i=0; i<nbrBit; i++){
		if(tab1[i] != tab2[i])
			return 0;
	}
	return 1;
}

//Cette fonction traite les resultat de la recherche exhaustive pour trouver le bon K16
long k16enHexa(int tabK16[8][64]){
	long resultat = 0;
	int tab[8] = {0};
	int tabclef[6] = {0};
	int tabresult[64] = {0};
	for(int i=0; i<8; i++){
		for(int j=0;j<64;j++){
			if(tabK16[i][j] == 6)
				tab[i] = j;
		}
		printf("%d\n", tab[i]);
		decimaltobinary(tabclef, tab[i], 6);
		for(int j=0; j<6;j++){
			tabresult[i*6+j] = tabclef[j];
		}
	}
	resultat = TabtoLong(tabresult,48);
	return resultat;
}

//Fait la recherche exhaustive pour trouver la clef K15
long rechercheExostive(const long LechiffrerJuste, const long *LeschiffrerFaux){
	Message juste;
	Message faux;
	long aRetourner = 0;
	int resultat[8][64] = {0};
	obtenirR16L16(LechiffrerJuste,&juste);
	int leftPmoin1[32] = {0};
	int resultatxorLeft[32] = {0};
	for(int w=0;w<32;w++){
		obtenirR16L16(LeschiffrerFaux[w],&faux);
		xor(resultatxorLeft,juste.leftChiffrer,faux.leftChiffrer,32);
		Permutation(leftPmoin1, resultatxorLeft, pMoin1, 32);
		int bitfaux = bitFauter(juste.rightChiffrer,faux.rightChiffrer);
		printf("bit faux : %d\n", bitfaux);
		Permutation(juste.rightChiffrerExp,juste.rightChiffrer,e,48);
		Permutation(faux.rightChiffrerExp,faux.rightChiffrer,e,48);
		int resSbox[4] = {0};
		int resLeftJuste[4] = {0};
		int key[6] = {0};
		for(int i=0; i<48; i++){
			if(e[i] == (bitfaux+1)){
				extraire6Bits(&juste,i/6);
				extraire6Bits(&faux,i/6);
				for(int y=0; y<4; y++){
					resLeftJuste[y] = leftPmoin1[4*(i/6)+y];
				}
				for(int j=0; j<64; j++){
					decimaltobinary(key,j,6);
					xor(juste.sbox6BitsXorer,juste.sbox6Bits,key,6);
					decimaltobinary(key,j,6);
					xor(faux.sbox6BitsXorer,faux.sbox6Bits,key,6);
					Sboxfonc(juste.sbox4Bits, juste.sbox6BitsXorer, i/6);
					Sboxfonc(faux.sbox4Bits, faux.sbox6BitsXorer, i/6);
					xor(resSbox,juste.sbox4Bits,faux.sbox4Bits,4);
					if(egale(resLeftJuste,resSbox,4)) {
						resultat[i/6][TabtoInt(key,6)]++;
					}
				}
			}
		}
	}
	aRetourner = k16enHexa(resultat);
	return aRetourner;
}

//initiatilse le tableau de bit a 0
void initTab(int *tab, int nbrBit){
	for(int i=0;i<nbrBit;i++){
		tab[i] = 0;
	}
}

//S'occupe de shifter les bit de nbrShift fois vers la gauche
void shiftGauche(int *resultat, int *tabAshifter, int nbrShift, int nbrBit){
	for(int i=-nbrShift; i<nbrBit-nbrShift; i++){
		if(i<0) {
			resultat[i+nbrBit] = tabAshifter[i+nbrShift];
		} else {
			resultat[i] = tabAshifter[i+nbrShift];	
		}
	}
}

//On fusionne deux tableau de meme dimension
void fusionTab(int *resultat, int *leftTab, int *rightTab, int nbrBit){
	for(int i=0; i<nbrBit; i++){
		resultat[i] = leftTab[i];
		resultat[i+nbrBit] =rightTab[i];
	}
}

//Genere les 16 clef de K0 a K15 dans le DES
void generationSubKey(int Les16SubKey[][48], int *key64Bit){
	int v[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
	int key56bit[56] = {0};
	int key48bit[48] = {0};
	int tabSplit[2][28] = {0};
	int tabSplitRes[2][28] = {0};
	Permutation(key56bit, key64Bit, pc1, 56);
	for(int i=0; i<16; i++) {
		splitTab(key56bit,tabSplit[0],tabSplit[1], 28);
		shiftGauche(tabSplitRes[0], tabSplit[0], v[i], 28);
		shiftGauche(tabSplitRes[1], tabSplit[1], v[i], 28);
		fusionTab(key56bit, tabSplitRes[0], tabSplitRes[1], 28);
		Permutation(Les16SubKey[i], key56bit, pc2, 48);
	}
}

//Fonction interne F du DES
void f(int *resultat, int *Ri, int *Ki){
	int right48b[48] = {0};
	int resultatXor[48] = {0};
	int entrerSbox[6] = {0};
	int sortiSbox[4] = {0};
	int sorti32bit[32] = {0};
	Permutation(right48b,Ri,e,48);
	xor(resultatXor,right48b,Ki,48);
	for(int j=0;j<8;j++){
		for(int i=0; i<6; i++) {
			entrerSbox[i] = resultatXor[6*j+i];
		}
		Sboxfonc(sortiSbox, entrerSbox, j);
		for(int i=0;i<4;i++){
			sorti32bit[j*4+i] = sortiSbox[i];
		}
	}
	Permutation(resultat, sorti32bit, p, 32);
}

//Sert a copier un tableau dans un autre de meme dimension
void copieTab(int *resultat, int * aCopier, int nbrBit){
	for(int i=0; i<nbrBit; i++){
		resultat[i] = aCopier[i];
	}
}

//DES : Prend en entrer un message claire et une clef pour donner un message chiffrer
long fonctionDES(long claire, long k64){
	DES d;
	int resultatF[32] = {0};
	int resultatConcat[64] = {0};
	hexatobinary(d.claireBinaire, claire, 16);
	hexatobinary(d.key64Bit, k64, 16);
	Permutation(d.claireBinaireIp, d.claireBinaire, ip, 64);
	splitTab(d.claireBinaireIp, d.left32Bit, d.right32Bit, 32);
	generationSubKey(d.subKey, d.key64Bit);
	for(int i=0; i<16; i++){
		copieTab(d.left32BitPlus1, d.right32Bit, 32);
		f(resultatF, d.right32Bit, d.subKey[i]);
		xor(d.right32BitPlus1, d.left32Bit, resultatF, 32);
		copieTab(d.left32Bit, d.left32BitPlus1, 32);
		copieTab(d.right32Bit, d.right32BitPlus1, 32);
	}
	fusionTab(resultatConcat, d.right32Bit, d.left32Bit, 32);
	Permutation(d.chiffrerBinaire, resultatConcat, ipMoin1, 64);
	return TabtoLong(d.chiffrerBinaire,64);
}

//Sert a obtenir les 56 bit juste de la clef de chiffrement
//(il restera que les bit de parité a trouver)
long getK56bit(long claire, long chiffrer, long k16){
	Key k;
	initTab(k.key48bit,48);
	initTab(k.key56bit,56);
	initTab(k.key64bitb,64);
	hexatobinary(k.key48bit,k16,12);
	Permutation(k.key56bit, k.key48bit, pc2Moin1, 56);
	Permutation(k.key64bitb, k.key56bit, pc1Moin1, 64);
	int position8bit[8] = {14,15,19,20,51,54,58,60};
	int i=0;
	while(i<256) {
		decimaltobinary(k.key8bit,i,8);
		for(int j=0;j<8;j++){
			k.key64bitb[position8bit[j]-1] = k.key8bit[j];
		}
		long clef = TabtoLong(k.key64bitb,64);
		if(chiffrer == fonctionDES(claire,clef)){
			return clef;
		}
		i++;
	}
	return 0;
}

//Renvoi la clef de chiffrement complete de 64 bits
long getK64bitParite(long claire, long chiffrer, long k16){
	int compteur = 0;
	int tabClefB[64] = {0};
	hexatobinary(tabClefB, getK56bit(claire, chiffrer, k16), 16);
	for(int i=0; i<64;i++){
		printf("%d", tabClefB[i]);
		if((i+1)%8 == 0)
			printf(" ");
	}
	printf("\n");
	for(int i=1; i<65; i++) {
		if((i%8) == 0) {
			if(compteur%2 == 1){
				tabClefB[i-1] = 0;
			}else {
				tabClefB[i-1] = 1;
			}
			compteur = 0;
		}else {
			compteur += tabClefB[i-1];
		}
	}
	for(int i=0; i<64;i++){
		printf("%d", tabClefB[i]);
		if((i+1)%8 == 0)
			printf(" ");
	}
	printf("\n");
	return TabtoLong(tabClefB,64);
}