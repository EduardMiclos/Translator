#pragma once

#include <stddef.h>
#include <stdarg.h>

/** Implementare simpla a unui buffer dinamic in care se scriu caractere. 
 *  Pe masura ce se scriu caractere, buffer-ul creste. */
typedef struct {

    /** Buffer. */
	char *buf;

    /** Numar de caractere din buffer. */
	size_t n;
} Text;


/** ================= */ 

/** Analogul lui printf, dar caracterele sunt scrise 
 * in buffer-ul "text", nu pe ecran. */
void Text_write(Text *text, const char *fmt, ...);

/** Sterge caracterele dintr-un buffer. */
void Text_clear(Text *text);

/** Returneaza numele din C corespunzator lui type din Quick (ex: TYPE_REAL -> double). */
const char *cType(int type);

/** ================= */


Text tInceput,	/** Fisier antet si variabile globale. */
	 tMain,		/** In functia main din C se va genera codul global din Quick. */
	 tFunctii,	/** Functiile din Quick. */
	 tAntetFn;	/** Folosit temporar pentru generarea antetului unei functii Quick. */


/** Pointeri care pointeaza la diverse buffere,
 * in fuctie de domeniul curent (intr-o functie sau global). */
Text *crtCod;		/** Intr-o fn pointeaza la tFunctii, altfel la tMain. */
Text *crtVar;		/** Intr-o fn pointeaza la tFunctii, altfel la tInceput. */


void Text_write(Text *text, const char *fmt, ...) {
    va_list va;
    va_start(va, fmt);

    /** Functia vsnprintf, apelata cu buffer NULL sau nr max de caractere 0,
     * returneaza nr. total de caractere, fara \0, care se vor emite
     * daca exista un buffer corespunzator. */
    int n = vsnprintf(NULL, 0, fmt, va);

    char *p = (char*)realloc(text->buf, (text->n + n + 1) * sizeof(char));

	if(!p){
		puts("Memorie insuficienta!");
		exit(EXIT_FAILURE);
	}

	/** Adauga noile caractere la bufferul dinamic. */
	va_start(va, fmt);
	vsnprintf(p + text->n, n+1, fmt, va);
	text->buf = p;
	text->n += n;

	va_end(va);
}

void Text_clear(Text *text) {
	free(text->buf);
	text->buf = NULL;
	text->n = 0;
}

const char *cType(int type) {
	switch(type) {
		case TYPE_INT: 
            return "int";
		case TYPE_REAL: 
            return "double";
		case TYPE_STR: 
            return "str";
		default:
			printf("Tip eronat: %d\n", type);
			exit(EXIT_FAILURE);
	}
}