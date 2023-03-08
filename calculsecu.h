

static const long messageClaire = 0x40A7D989161A6223;

static const long chiffrerJuste = 0x864C804BB6B905BA;

static const long chiffrerFaux[32] = {
	0x8448800BB6B805BE, 
	0x865E804BB6B805BA, 
	0x864C824FB6B805BA, 
	0x861C8409A6B905BA, 
	0x860C804FA4B805BA, 
	0x860C844BB6BB05BA, 
	0x870C844BA6B907BA, 
	0x864C844AE6B905B8, 
	0x8F0C804AA6AD05BA, 
	0x8644804AF6AD05BA, 
	0x864C884BF6A905BA, 
	0xC64C9042F6A905BA, 
	0xC64C804AFEAD05BB, 
	0xC64C804BB6F105BA, 
	0x864C804BB6F90DBB, 
	0xC64C904BB6B904F3, 
	0xE64C804BB2B904FB, 
	0x866C804BB2B915FA, 
	0x864CA04BB6B914BA, 
	0x964CC16BB2B915BA, 
	0x824CC04B96B914FA, 
	0x924C814BB69905BA, 
	0x864CC14BB6B925BA, 
	0x964CC15BB6B9459A, 
	0x024CC05BB7B905BA, 
	0x86CC804BB7B901BA, 
	0x864C005BB6B945BA, 
	0x864880DBB7B901AA, 
	0x8648804B37B901AA, 
	0x8648804BB63905AE, 
	0x864D804BB6B985BE, 
	0x864C800BB6B8052E 
};

//Initial permutation
static const int ip[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7
};

//L'inverse de l'initial permutation
static const int ipMoin1[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
};

//Expansion
static const int e[48] = {
	32,1,2,3,4,5,
	4,5,6,7,8,9,
	8,9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32,1
};

//permutation dans la fonction interne f
static const int p[32] = {
	16,7,20,21,29,12,28,17,
	1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25
};

//permutation inverse dans la fonction interne f
static const int pMoin1[32] = {
	9,17,23,31,
	13,28,2,18,
	24,16,30,6,
	26,20,10,1,
	8,14,25,3,
	4,29,11,19,
	32,12,22,7,
	5,27,15,21
};

//Les 8 S-box contenu dans la fonction f
static const int sbox[8][4][16] = {
	{
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
	},
	{
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
	},
	{
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
	},
	{
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
	},
	{
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
	},
	{
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
	},
	{
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
	},
	{
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
	}
};

//permutation 1 dans le key schedul
static const int pc1[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
};

//permutation 1 inverse dans le key schedul
static const int pc1Moin1[64] = {
	8,16,24,56,52,44,36,0,
	7,15,23,55,51,43,35,0,	
	6,14,22,54,50,42,34,0,
	5,13,21,53,49,41,33,0,	
	4,12,20,28,48,40,32,0,	
	3,11,19,27,47,39,31,0,
	2,10,18,26,46,38,30,0,	
	1,9,17,25,45,37,29,0
};

//permutation 2 dans le key schedul qui fait passer la clef de 56 a 48 bits
static const int pc2[48] = {
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
};

//permutation 2 inverse dans le key schedul
static const int pc2Moin1[56] = {
	5,24,7,16,6,10,20,	
	18,0,12,3,15,23,1,
	9,19,2,0,14,22,11,	
	0,13,4,0,17,21,8,
	47,31,27,48,35,41,0,
	46,28,0,39,32,25,44,
	0,37,34,43,29,36,38,	
	45,33,26,42,0,30,40
};

//Cette structure est utiler dans la recherche de K16
typedef struct message {
	long chiffrerHexa;
	int chiffrerBinaire[64];
	int chiffrerBinairePermuter[64];
	int leftChiffrer[32];
	int rightChiffrer[32];
	int rightChiffrerExp[48];
	int sbox6Bits[6];
	int sbox6BitsXorer[6];
	int sbox4Bits[4]; 
} Message;

//Cette structure est utiliser dans les recherche des 16 sous clefs
typedef struct clef {
	int key48bit[48];
	int key56bit[56];
	int key64bitb[64];
	int key8bit[8];
} Key;

//Cette structure est utiliser dans le DES, 
//elle regroupe toute les variables necessaire au bon deroulement du DES
typedef struct des {
	int claireBinaire[64];
	int key64Bit[64];
	int claireBinaireIp[64];
	int right32Bit[32];
	int left32Bit[32];
	int right32BitPlus1[32];
	int left32BitPlus1[32];
	int right48Bit[48];
	int subKey[16][48];
	int chiffrerBinaire[64];
} DES;

void hexatobinary(int *tabResult, long hexa, int nbrHexa);
void decimaltobinary(int *tabResult, int decimal, int nbrBit);
long puissance(int a,int b);
int TabtoInt(int *tab,int nbrBit);
long TabtoLong(int *tab,int nbrBit);
void Permutation(int *resultat,int *aPermuter,const int *tablePermutation, int nbrBit);
void splitTab(int *completTab,int *leftTab,int *rightTab, int nbrBit);
void xor(int *tabResult, int *premierK, int *deuxiemeK, int nbrBit);
int bitFauter(int *tabJuste, int *tabFaux);
void Sboxfonc(int *resultat, int *entrer, int numSbox);
void obtenirR16L16(long hexa,Message *m);
void extraire6Bits(Message *m, int position);
int egale(int *tab1, int *tab2, int nbrBit);
long k16enHexa(int tabK16[8][64]);
long rechercheExostive(const long LechiffrerJuste, const long *LeschiffrerFaux);
void initTab(int *tab, int nbrBit);
void shiftGauche(int *resultat, int *tabAshifter, int nbrShift, int nbrBit);
void fusionTab(int *resultat, int *leftTab, int *rightTab, int nbrBit);
void generationSubKey(int Les16SubKey[][48], int *key64Bit);
void f(int *resultat, int *Ri, int *Ki);
void copieTab(int *resultat, int * aCopier, int nbrBit);
long fonctionDES(long claire, long k64);
long getK56bit(long claire, long chiffrer, long k16);
long getK64bitParite(long claire, long chiffrer, long k16);