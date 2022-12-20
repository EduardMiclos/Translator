#pragma once

#include <stdbool.h>

Atom *consumed;
Ret ret;

/** ================= */

/** Adauga in tabela de simboluri o functie care are un argument.
    Argumentul are tipul tipArg, iar functia are tipul returnat tipRet. */
Simbol *adaugaFn1Arg(const char *numeFn, int tipArg, int tipRet);

/** Adauga in TS functiile predefinite din exemplu: puti, putr, puts. 
    Altfel, la intalnirea acestor identificatori s-ar fi emis mesaj de eroare
    ca ei sunt necunoscuti. */
void adaugaFnPredefinite();

/** Seteaza variabila globala "ret" cu tipul rezultat dintr-o regula. */
void setRet(int tip,bool lval);

/** ================= */

Simbol *adaugaFn1Arg(const char *numeFn, int tipArg, int tipRet){
    Simbol *fn = adaugaSimbol(numeFn, FEL_FN);

    fn->tip = tipRet;
    fn->args = NULL;
    Simbol *arg= adaugaArgFn(fn, "arg");
    arg->tip = tipArg;
    return fn;
}

void adaugaFnPredefinite() {
    adaugaFn1Arg("puti", TYPE_INT, TYPE_INT);
    adaugaFn1Arg("putr",TYPE_REAL, TYPE_REAL);
    adaugaFn1Arg("puts",TYPE_STR, TYPE_STR);
}

void setRet(int tip, bool lval){
    ret.tip = tip;
    ret.lval = lval;
}