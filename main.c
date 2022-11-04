#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SPATII_TAB 4
#define MAX_DIM_CONTINUT 3001

#define avanseaza (pch++);
#define avanseazaColoana(spatii) {avanseaza; nrColoana += spatii;}
#define avanseazaLinie {avanseaza; nrLinie++; nrColoana = 1;}

/** Index-ul care garanteaza valoarea NULL in matricea de cuvinte cheie,
 * pentru optimizarea algoritmului in cazul in care se face accesarea cu
 * o litera ce nu se afla la inceputul niciunui cuvant cheie. Astfel, 
 * se va accesa charIndex pentru litera respectiva care va indica spre randul
 * plin de NULL. Dupa aceea, se va accesa lungimeToIndex cu modulo. In cazul in care
 * modulo ne duce totusi pe o valoare exista, avand deja un rand plin de NULL, avem
 * certitudinea ca oricum obtinem NULL daca primul caracter nu reprezinta inceputul unui
 * cuvant cheie. */
#define NULL_CHAR_TO_IDX 7

/** Analog. */
#define NULL_LUNGIME_TO_IDX 0

/** Coduri atomi Quick, fara SPACE si COMMENT. */
enum {ID, VAR, FUNCTION, IF, ELSE, WHILE, END, RETURN, 
      TYPE_INT, TYPE_REAL, TYPE_STR, INT, REAL, STR, 
      COMMA, COLON, SEMICOLON, LPAR, RPAR, FINISH,
      ADD, SUB, MUL, DIV, AND, OR, NOT, ASSIGN, EQUAL,
      NOTEQ, LESS};

static const char *strAtomi[] = {
    [ID] = "ID", 
    [VAR] = "VAR", 
    [FUNCTION] = "FUNCTION", 
    [IF] = "IF", 
    [ELSE] = "ELSE", 
    [WHILE] = "WHILE", 
    [END] = "END", 
    [RETURN] = "RETURN",
    [TYPE_INT] = "TYPE_INT",
    [TYPE_REAL] = "TYPE_REAL",
    [TYPE_STR] = "TYPE_STR",
    [INT] = "INT",
    [REAL] = "REAL",
    [STR] = "STR",
    [COMMA] = "COMMA",
    [COLON] = "COLON",
    [SEMICOLON] = "SEMICOLON",
    [LPAR] = "LPAR",
    [RPAR] = "RPAR",
    [FINISH] = "FINISH",
    [ADD] = "ADD",
    [SUB] = "SUB",
    [MUL] = "MUL",
    [DIV] = "DIV",
    [AND] = "AND",
    [OR] = "OR",
    [NOT] = "NOT",
    [ASSIGN] = "ASSIGN",
    [EQUAL] = "EQUAL",
    [NOTEQ] = "NOTEQ",
    [LESS] = "LESS"
    };

#define N_LUNGIME_TO_INDEX 9
int lungimeToIndex[] = {
    NULL_LUNGIME_TO_IDX, 
    NULL_LUNGIME_TO_IDX, 
    0, 
    1, 
    2, 
    3, 
    4, 
    NULL_LUNGIME_TO_IDX, 
    5};

int charToIndex[] = {
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    0, 
    1, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    2, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    3, 
    4, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    5, 
    6, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX, 
    NULL_CHAR_TO_IDX};

typedef struct {
    int codAtom;
    char *text;
} cuvantCheie;

#define ROWS_CUVINTE_CHEIE 8
#define COLS_CUVINTE_CHEIE 6
cuvantCheie cuvinteCheie[ROWS_CUVINTE_CHEIE][COLS_CUVINTE_CHEIE] = {
    /** Litera inceput\Lungime  2     3    4     5     6     8    */
    /*       e        */       {{0},        {END, "end"}, {ELSE, "else"}, {0},              {0},                {0}},
    /*       f        */       {{0},        {0},          {0},            {0},              {0},                {FUNCTION, "function"}},
    /*       i        */       {{IF, "if"}, {TYPE_INT, "int"}, {0},            {0},              {0},                {0}},
    /*       r        */       {{0},        {0},          {TYPE_REAL, "float"}, {0},              {RETURN, "return"}, {0}},
    /*       s        */       {{0},        {TYPE_STR, "str"}, {0},            {0},              {0},                {0}},
    /*       v        */       {{0},        {VAR, "var"}, {0},            {0},              {0},                {0}},
    /*       w        */       {{0},        {0},          {0},            {WHILE, "while"}, {0},                {0}},
    /*      NULL      */       {{0},        {0},          {0},            {0},              {0},                {0}}
};

typedef struct {
    char *start, *final;
} IndicatorString;

#define MAX_STR 100
typedef struct {
    unsigned int tipCurent : 2;

    union {
        float valoareFloat;
        int valoareInt;
        char valoareStr[MAX_STR];
    };
} valoareAtom;

typedef struct{
    int linie;
	int codAtom;
    valoareAtom valoare;
} Atom;

/** Vectorul cu atomii extrasi din fisierul de intrare. */
Atom atomi[10000];	

/** Numarul de atomi din vectorul atomi. */
int nAtomi;

/** Index-ul liniei de cod curente. */
int nrLinie = 1;

/** Index-ul coloanei curente. */
int nrColoana = 1;

/** Continutul fisierului sursa. */
char continut[MAX_DIM_CONTINUT];

/** Pointer la caracterul curent din continutul fisierului. */
char *pch;

/** Indicatorul care ne ajuta sa copiem continutul atomului. */
IndicatorString indicator;

void addIntToAtom(int val) {
    atomi[nAtomi].valoare.tipCurent = 1;
    atomi[nAtomi].valoare.valoareInt = val;
}

void addFloatToAtom(float val) {
    atomi[nAtomi].valoare.tipCurent = 2;
    atomi[nAtomi].valoare.valoareFloat = val;
}

void addStringToAtom(char *str) {
    atomi[nAtomi].valoare.tipCurent = 3;
    strcpy(atomi[nAtomi].valoare.valoareStr, str);
}

void addAtom(int cod) {
    atomi[nAtomi].codAtom = cod;
    atomi[nAtomi].linie = nrLinie;
    nAtomi++;
}

int esteCuvantCheie(char *str) {
    char primaLitera = str[0];
    int lungime = strlen(str);

    int i = charToIndex[primaLitera - 97];
    int j = lungimeToIndex[lungime % N_LUNGIME_TO_INDEX];

    cuvantCheie cuv = cuvinteCheie[i][j];

    if (cuv.text != NULL)
        return !strcmp(str, cuv.text) ? cuv.codAtom : -1;
    
    return -1;
}

/** La fiecare apel returneaza codul unui atom. */
int getNextTk() {
    /** Starea curenta. */
	int state = 0;

    /** Valoarea propriu-zisa a atomului. */
    char valoareAtom[MAX_STR];

	for(;;) {
        /** Caracterul curent, de analizat. */
		char ch = *pch;

		/** Exista cate un case pentru fiecare stare din diagrama. */
		switch(state){
			case 0:
				if (isalpha(ch) || ch == '_' ){
                    state = 1;

                    indicator.start = pch; 
                    avanseazaColoana(1)
                }
				else if (isdigit(ch)){
                    state = 3;

                    indicator.start = pch;
                    avanseazaColoana(1)
                }
                else if (ch == '"') {
                    state = 5;

                    indicator.start = pch;
                    avanseazaColoana(1)
                }
                else if (ch == ',') {
                    state = 10;
                    avanseazaColoana(1)
                }
                else if (ch == '.') {
                    state = 11;
                    avanseazaColoana(1)
                }
                else if (ch == ';') {
                    state = 12;
                    avanseazaColoana(1)
                }
                else if (ch == '(') {
                    state = 13;
                    avanseazaColoana(1)
                }
                else if (ch == ')') {
                    state = 14;
                    avanseazaColoana(1)
                }
                else if (ch == '+') {
                    state = 15;
                    avanseazaColoana(1)
                }
                else if (ch == '-') {
                    state = 16;
                    avanseazaColoana(1)
                }
                else if (ch == '*') {
                    state = 17;
                    avanseazaColoana(1)
                }
                else if (ch == '/') {
                    state = 18;
                    avanseazaColoana(1)
                }
                else if (ch == '&') {
                    state = 19;
                    avanseazaColoana(1)
                } 
                else if (ch == '|') {
                    state = 21;
                    avanseazaColoana(1)
                }
                else if (ch == '!') {
                    state = 23;
                    avanseazaColoana(1)
                }
                else if (ch == '=') {
                    state = 26;
                    avanseazaColoana(1)
                }
                else if (ch == '<') {
                    state = 29;
                    avanseazaColoana(1)
                }
                else if (ch == '\0') {
                    state = 30;
                    avanseazaColoana(1)
                }
                else if (ch == '#') {
                    state = 31;
                    avanseazaColoana(1)
                }
				else if (ch == ' '|| ch == '\r') {
                    avanseazaColoana(1)
                }
                else if (ch == '\t') {
                    avanseazaColoana(SPATII_TAB);
                }
                else if (ch == '\n') {
                    avanseazaLinie
                }
				break;
			case 1:
				if (isalnum(ch) || ch == '_')
                    avanseazaColoana(1)
				else
                    state = 2;
				break;
			case 2:
                indicator.final = pch;

                memcpy(valoareAtom, indicator.start, indicator.final - indicator.start);
                valoareAtom[indicator.final - indicator.start] = '\0';

                int codCuvantCheie;

                if ((codCuvantCheie = esteCuvantCheie(valoareAtom)) != -1) {
                    addAtom(codCuvantCheie);
                    return codCuvantCheie;
                }
                else {
                    addStringToAtom(valoareAtom);
                    addAtom(ID);

                    return ID;
                }

                return ID;
            case 3:
                if (isdigit(ch))
                    avanseazaColoana(1)
                else if (ch == '.') {
                    state = 6;
                    avanseazaColoana(1);
                }
                else
                    state = 4;
                break;
            case 4:
                indicator.final = pch;
                memcpy(valoareAtom, indicator.start, indicator.final - indicator.start);
                valoareAtom[indicator.final - indicator.start] = '\0';

                addIntToAtom(atoi(valoareAtom));
                addAtom(INT);
                return INT;
            case 5:
                if (ch != '"' && ch != '\0')
                    avanseazaColoana(1)
                else if (ch == '"') {
                    state = 9;
                    avanseazaColoana(1);
                }
                else {
                    printf("Eroare: Programul s-a incheiat dar sirul a ramas neterminat! [Linia: %d, coloana: %d]\n", nrLinie, nrColoana);
                    exit(EXIT_FAILURE);
                }
                break;
            case 6:
                if (isdigit(ch)) {
                    state = 7;
                    avanseazaColoana(1)
                }
                else {
                    printf("Eroare: Se asteapta o cifra dupa punctul zecimal! [Linia: %d, coloana: %d]\n", nrLinie, nrColoana);
                    exit(EXIT_FAILURE);
                }
                break;
            case 7:
                if (isdigit(ch))
                    avanseazaColoana(1)
                else
                    state = 8;
                break;
            case 8:
                indicator.final = pch;
                memcpy(valoareAtom, indicator.start, indicator.final - indicator.start);
                valoareAtom[indicator.final - indicator.start] = '\0';

                addFloatToAtom(atof(valoareAtom));
                addAtom(REAL);
                return REAL;
            case 9:
                indicator.final = pch;

                memcpy(valoareAtom, indicator.start, indicator.final - indicator.start);
                valoareAtom[indicator.final - indicator.start] = '\0';

                addStringToAtom(valoareAtom);
                addAtom(STR);
                return STR;
			case 10:
                addAtom(COMMA);
                return COMMA;
            case 11:
                addAtom(COLON);
                return COLON;
            case 12:
                addAtom(SEMICOLON);
                return SEMICOLON;
            case 13:
                addAtom(LPAR);
                return LPAR;
            case 14:
                addAtom(RPAR);
                return RPAR;
            case 15:
                addAtom(ADD);
                return ADD;
            case 16:
                addAtom(SUB);
                return SUB;
            case 17:
                addAtom(MUL);
                return MUL;
            case 18:
                addAtom(DIV);
                return DIV;
            case 19:
                if (ch == '&') {
                    state = 20;
                    avanseazaColoana(1)
                }
                else {
                    printf("Eroare: Se asteapta inca un '&'! [Linia: %d, coloana: %d]\n", nrLinie, nrColoana);
                    exit(EXIT_FAILURE);
                }
                break;
            case 20:
                addAtom(AND);
                return AND;
            case 21:
                if (ch == '|') {
                    state = 22;
                    avanseazaColoana(1)
                }
                else {
                    printf("Eroare: Se asteapta inca un '|'! [Linia: %d, coloana: %d]\n", nrLinie, nrColoana);
                    exit(EXIT_FAILURE);
                }
                break;
            case 22:
                addAtom(OR);
                return OR;
            case 23:
                if (ch == '=') {
                    state = 25;
                    avanseazaColoana(1)
                }
                else
                    state = 24;
                break;
            case 24:
                addAtom(NOT);
                return NOT;
            case 25:
                addAtom(NOTEQ);
                return NOTEQ;
            case 26:
                if (ch == '=') {
                    state = 27;
                    avanseazaColoana(1) 
                }
                else
                    state = 28;
                break;
            case 27:
                addAtom(EQUAL);
                return EQUAL;
            case 28:
                addAtom(ASSIGN);
                return ASSIGN;
            case 29:
                addAtom(LESS);
                return LESS;
            case 30:
                addAtom(FINISH);
                return FINISH;
            case 31:
                if (ch == '\n' || ch == '\r' || ch == '\0')
                    state = 0;
                else 
                    avanseazaColoana(1)
                break; 
			default: 
                printf("Stare invalida: %d!\n [Linia: %d, coloana: %d]\n", state, nrLinie, nrColoana);
                exit(EXIT_FAILURE);
			}
		}
}

void vizualizareAtomi() {
    int linieCurenta = atomi[0].linie - 1;

    for (int i = 0; i < nAtomi; i++) {
        if (atomi[i].linie != linieCurenta) {
            printf("\nLinia %d: ", atomi[i].linie);
            linieCurenta = atomi[i].linie ;    
        }

        printf(" %s", strAtomi[atomi[i].codAtom]);
            
        if (atomi[i].valoare.tipCurent == 1) {
            printf(":%d ", atomi[i].valoare.valoareInt);
        }
        else if (atomi[i].valoare.tipCurent == 2) {
            printf(":%f ", atomi[i].valoare.valoareFloat);
        }
        else if (atomi[i].valoare.tipCurent == 3) {
            printf(":%s ", atomi[i].valoare.valoareStr);
        }
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Eroare argument: Specificati fisierul sursa!\n");
        exit(EXIT_FAILURE);
    }

    /** Pointer-ul fisierului de intrare (fisier sursa). */
	FILE *f;

    /** Dimensiunea in octeti a fisierului f, deteriminata in urma citirii. */
    int fdim;

    if (!(f = fopen(argv[1], "rb"))) {
        printf("Eroare la deschiderea fisierului sursa: %s!\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    fdim = fread(continut, sizeof(char), MAX_DIM_CONTINUT, f);
    continut[fdim] = '\0';

    /** Inchidem fisierul in urma citirii. */
	fclose(f);
	
    /** Pointer-ul pch pointeaza acum la inceputul sirului de octeti. */
    pch = continut;
	
    /** Extragem, pe rand, atomii. */
	while(getNextTk() != FINISH) {};

    vizualizareAtomi();
}