#pragma once

#include <stdbool.h>

/** Datele returnate de una dintre regulile sintactice. */
typedef struct {
    int tip;
    bool lval;
} Ret;

typedef enum {FEL_VAR, FEL_ARG, FEL_FN} FEL;

/** Un simbol reprezinta un identificator folosit in cadrul unui program. */
typedef struct Simbol Simbol;
struct Simbol {
    /** Referinta la numele stocat intr-un atom. */
    const char *nume;

    FEL fel;
    int tip;

    union {
        /** Pentru functii: lista cu argumentele functiei. */
        Simbol *args;

        /** Pentru variabile: daca variabila este locala.*/
        bool local;
    };

    /** Simbolul urmator (se utilizeaza liste simplu inlantuite). */
    Simbol *urm;
};

/** Un domeniu este alcatuit din unul sau mai multe simboluri locale. */
typedef struct Domeniu Domeniu;
struct Domeniu {
    /** Referinta catre domeniul parinte. */
    Domeniu *parinte;

    /** Lista simplu inlantuita de simboluri. */
    Simbol *simboluri;
} ;

/** Tabela de simboluri, definind multimea tuturor domeniilor. */
Domeniu *tabelaSim;

/** Pointer la simbolul functiei curente, daca se parseaza o functie.
    Altfel, NULL (in afara functiilor). */
Simbol *crtFn;



/** ================= */

/** Adauga un nou domeniu in TS si il seteaza ca fiind domeniul curent. */
Domeniu *adaugaDomeniu();

/** Sterge domeniul curent din TS si revine la domeniul parinte. */
void stergeDomeniu();

/** Cautare exclusiv in domeniul curent. */
Simbol *cautaInDomeniulCurent(const char*);

/** Cautare in toate domeniile. */
Simbol *cautaSimbol(const char*);

/** Adaugare simbol in domeniul curent. */
Simbol *adaugaSimbol(const char*, int);

/** Adauga un argument la simbolul Fn. */
Simbol *adaugaArgFn(Simbol*, const char*);

/** Cautare in lista. */
Simbol* cautaInLista(Simbol*, const char*);

/** Sterge un simbol. */
void stergeSimbol(Simbol*);

/** Sterge o lista de simboluri. */
void stergeSimboluri(Simbol*);
/** ================= */



Domeniu *adaugaDomeniu() {
    Domeniu *domeniu;
    printf("Creare domeniu nou!\n");

    if (!(domeniu = (Domeniu*)malloc(sizeof(Domeniu)))) {
        printf("Eroare la alocarea in memorie a domeniului curent!");
        exit(EXIT_FAILURE);
    }

    domeniu->parinte = tabelaSim;
    domeniu->simboluri = NULL;
    tabelaSim = domeniu;
    return domeniu; 
}

void stergeSimbol(Simbol *s) {
    printf("\tStergere simbol: %s\n", s->nume);

	if (s->fel == FEL_FN)
		stergeSimboluri(s->args);
	free(s);
}

void stergeSimboluri(Simbol * lista) {
    for (Simbol *s1 = lista, *s2; s1; s1 = s2) {
		s2 = s1->urm;
		stergeSimbol(s1);
	}
}

void stergeDomeniu() {
    printf("Stergere domeniu curent!\n");

    if (tabelaSim == NULL) return;
    Domeniu *domeniuCurent = tabelaSim;
    tabelaSim = tabelaSim->parinte;

    stergeSimboluri(domeniuCurent->simboluri);
    free(domeniuCurent);

    printf("Revenire in domeniul parinte!\n");
}

Simbol* cautaInLista(Simbol *lista, const char *nume) {
    for (Simbol *s = lista; s; s = s->urm) {
		if (!strcmp(s->nume,nume)) return s;
	}
	return NULL;
}

Simbol *cautaInDomeniulCurent(const char *nume){
	return cautaInLista(tabelaSim->simboluri, nume);
}

Simbol *cautaSimbol(const char *nume){
    for (Domeniu *d = tabelaSim; d; d = d->parinte) {
        Simbol *s = cautaInLista(d->simboluri,nume);
        if (s) return s;
    }
    return NULL;
}

Simbol *creeazaSimbol(const char *nume, int fel){
	Simbol *s;

    if (!(s= (Simbol*)malloc(sizeof(Simbol)))) {
        printf("Eroare la alocarea in memorie a domeniului curent!\n");
        exit(EXIT_FAILURE);
    }
	s->nume=nume;
	s->fel=fel;
	return s;
}

Simbol *adaugaSimbol(const char *nume, int fel){
	printf("\tAdaugare simbol: %s\n", nume);

	Simbol *s = creeazaSimbol(nume, fel);
	s->urm = tabelaSim->simboluri;
	tabelaSim->simboluri = s;
	return s;
}

Simbol *adaugaArgFn(Simbol *fn, const char *argName) {
	printf("\tSe adauga simbolul %s ca si argument\n", argName);
    
	Simbol *s = creeazaSimbol(argName, FEL_ARG);
	s->urm = NULL;

	if (fn->args) {
		Simbol *p;
		for (p = fn->args; p->urm; p = p->urm) {}
		p->urm = s;
	}
    else
		fn->args = s;
	return s;
}