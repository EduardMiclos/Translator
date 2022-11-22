#pragma once


/** ================= */
void addIntToAtom(int);
void addFloatToAtom(float);
void addStringToAtom(char *);
void addAtom(int);
int esteCuvantCheie(char *);
int getNextTk();
void vizualizareAtomi();
/** ================= */


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

/** Index-ul atomului curent, folosit in cadrul analizorului sintactic. */
int idxAtom;

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
                else if (ch == ':') {
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
            linieCurenta = atomi[i].linie;    
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
    printf("Analiza lexicala completa!\n");
}