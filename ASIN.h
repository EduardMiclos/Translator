#pragma once

/** ================= */
int program();
int defVar();
int baseType();
int defFunc();
int block();
int funcParams();
int funcParam();
int instr();
int expr();
int exprLogic();
int exprAssign();
int exprComp();
int exprAdd();
int exprMul();
int exprPrefix();
int factor();
int consume(int);
void err(const char*);
/** ================= */

Atom *consumed;
Ret ret;

void err(const char *msg) {
    printf("Eroare in linia %d: %s\n", atomi[idxAtom].linie, msg);
    exit(EXIT_FAILURE);
}

int consume(int cod) {
	if(atomi[idxAtom].codAtom == cod){
        consumed = &atomi[idxAtom];
		idxAtom++;
		return TRUE;
	}

	return FALSE;
}

int factor() {
    int atomStart = idxAtom;

    if (consume(INT)) return TRUE;
    if (consume(REAL)) return TRUE;
    if (consume(STR)) return TRUE;
    if (consume(LPAR)) {
        if (expr()) {
            if (consume(RPAR))
                return TRUE;
            else err("Lipseste ')' la finalul expresiei!");
        }
    }
    else if (consume(ID)) {
        if (consume(LPAR)) {
            if (expr()) {
                while(1) {
                    if (consume(COMMA)) {
                        if (expr()) {}
                        else err("Lipseste parametrul functiei dupa virgula!");
                    }
                    else break;
                }
            }

            if (consume(RPAR)) return TRUE;
            else err("Lipseste ')' la sfarsitul apelului de functiei!");
        }

        return TRUE;
    }

    idxAtom = atomStart;
    return FALSE;
}

int exprPrefix() {
    int atomStart = idxAtom;

    if (consume(SUB) || consume(NOT)) {}
    if (factor()) return TRUE;

    idxAtom = atomStart;
    return FALSE;
}

int exprMul() {
    int atomStart = idxAtom;

    if (exprPrefix()) {
        while (1) {
            if (consume(MUL)) {
                if (exprPrefix()) {}
                else err("Lipseste operandul dupa operatorul de inmultire!");
            }
            else if (consume(DIV)) {
                if (exprPrefix()) {}
                else err("Lipseste operandul dupa operatorul de impartire!");
            }
            else break;
        }

        return TRUE;
    }

    idxAtom = atomStart;
    return FALSE;
}

int exprAdd() {
    int atomStart = idxAtom;

    if (exprMul()) {
        while (1) {
            if (consume(ADD)) {
                if (exprMul()) {}
                else err("Lipseste operandul dupa operatorul de adunare!");
            }
            else if (consume(SUB)) {
                if (exprMul()) {}
                else err("Lipseste operandul dupa operatorul de scadere!");
            }
            else break;
        }
        return TRUE;
    }

    idxAtom = atomStart;
    return FALSE;
}

int exprComp() {
    int atomStart = idxAtom;

    if (exprAdd()) {
        if (consume(LESS)) {
            if (exprAdd()) {}
            else err("Lipseste operandul dupa LESS!");
        }
        else if (consume(EQUAL)) {
            if (exprAdd()) {}
            else err("Lipseste operandul dupa EQUAL!");
        }

        return TRUE;
    }

    idxAtom = atomStart;
    return FALSE;
}

int exprAssign() {
    int atomStart = idxAtom;

    if (consume(ID)) {
        if (consume(ASSIGN)) {}
        else
            idxAtom = atomStart;
    }

    if (exprComp())
        return TRUE;

    idxAtom = atomStart;
    return FALSE;
}

int exprLogic() {
    int atomStart = idxAtom;

    if (exprAssign()) {
        while (1) {
            if (consume(AND)) {
                if (exprAssign()) {}
                else err("Lipseste operandul dupa AND!");
            }
            else if (consume(OR)) {
                if (exprAssign()) {}
                else err("Lipseste operandul dupa OR!");
            }
            else break;
        }
        return TRUE;
    }

    idxAtom = atomStart;
    return FALSE;
}

int expr() {
    int atomStart = idxAtom;

    if (exprLogic())
        return TRUE;

    idxAtom = atomStart;
    return FALSE;
}

int instr() {
    int startAtom = idxAtom;

    if (expr()) {
        if (consume(SEMICOLON)) return TRUE;
        else {
            idxAtom = startAtom;
            err("Lipseste ';' la finalul expresiei!");
        }
    }
    else if (consume(SEMICOLON)) return TRUE;

    if (consume(IF)) {
        if (consume(LPAR)) {
            if (expr()) {
                if (consume(RPAR)) {
                    if (block()) {
                        if (consume(ELSE)) {
                            if (block()) {}
                            else err("Lipseste blocul de instructiuni din ELSE!");
                        }

                        if (consume(END))
                            return TRUE;
                        else err("Lipseste END la sfarsitul blocului de instructiuni!");
                    }
                    else err("Lipseste blocul de instructiuni din IF!");
                }
                else err("Lipseste ')' dupa IF!");
            }
            else err("Lipseste conditia instructiunii IF!");
        }
        else err("Lipseste '(' dupa IF!");
    }
    else if (consume(RETURN)) {
        if (expr()) {
            if (consume(SEMICOLON)) 
                return TRUE;
            else err("Lipseste ';' la finalul instructiunii RETURN!");
        }
        else err("Lipseste o expresie dupa RETURN!");
    }
    else if (consume(WHILE)) {
        if (consume(LPAR)) {
            if (expr()) {
                if (consume(RPAR)) {
                    if (block()) {
                        if (consume(END))
                            return TRUE;
                    }
                    else err("Lipseste blocul de instructiuni din WHILE!");
                }
                else err("Lipseste ')' dupa WHILE!");
            }
            else err("Lipseste conditia instructiunii WHILE!");
        }
        else err("Lipseste '(' dupa WHILE!");
    }

    idxAtom = startAtom;
    return FALSE;
}

int funcParam() {
    int atomStart = idxAtom; 
    
    if (consume(ID)) {
        const char *nume = consumed->valoare.valoareStr;
        Simbol *s = cautaInDomeniulCurent(nume);
        if (s)
            err(strcat("Redefinire simbol: ", nume));

        s = adaugaSimbol(nume, FEL_ARG);
        Simbol *argFn = adaugaArgFn(crtFn, nume);

        if (consume(COLON)) {
            if (baseType()) {
                s->tip = ret.tip;
                argFn->tip = ret.tip;
                return TRUE;
            }
            else err("Tipul parametrului este invalid sau inexistent!");
        }
        else err("Lipseste ':' in declararea parametrului!");
    }
    
    idxAtom = atomStart;
    return FALSE;
}

int funcParams() {
    if (funcParam()) {
        while(1) {
            if (consume(COMMA)) {
                if (funcParam()) {}
                else err("Lipsesc parametrii dupa virgula!");
            }
            else break;
        }
    }
    return TRUE;
}

int block() {
    int atomStart = idxAtom;

    if (instr()) {
        while(1) {
            if (instr()) {}
            else break;
        }
        return TRUE;
    }
    
    idxAtom = atomStart;
    return FALSE;
}

int defFunc() {
    int atomStart = idxAtom;

    if (consume(FUNCTION)) {
        if (consume(ID)) {
            const char *nume = consumed->valoare.valoareStr;
            Simbol *s = cautaInDomeniulCurent(nume);

            if (s)
                err(strcat("Redefinire simbol: ", nume));
                
            crtFn = adaugaSimbol(nume, FEL_FN);
            crtFn->args = NULL;
            adaugaDomeniu();

            if (consume(LPAR)) {
                if (funcParams()) {
                    if (consume(RPAR)) {
                        if (consume(COLON)) {
                            if (baseType()) {
                                crtFn->tip = ret.tip;
                                while(1) {
                                    if (defVar()) {}
                                    else break;
                                }

                                if (block()) {
                                    if(consume(END)) {
                                        stergeDomeniu();
                                        crtFn = NULL;

                                        return TRUE;
                                    }
                                    else err("Lipseste END la sfarsitul blocului de instructiuni!");
                                }
                                else err("Lipseste blocul de instructiuni in definitia functiei!");
                            }
                            else err("Tipul returnat de functie este invalid sau inexistent!");
                        }
                        else err("Lipseste ':' in declararea functiei!");
                    }
                    else err("Lipseste ')' in declararea functiei!");
                }
            }
            else err("Lipseste '(' in declararea functiei!");
        }
        else err("Lipseste denumirea functiei!");
    }

    idxAtom = atomStart;
    return FALSE;
}

int baseType() {
    int atomStart = idxAtom;

    if (consume(TYPE_INT)) {
        ret.tip = TYPE_INT;
        return TRUE;
    }
    if (consume(TYPE_REAL)) {
        ret.tip = TYPE_REAL;
        return TRUE;
    }
    if (consume(TYPE_STR)) {
        ret.tip = TYPE_STR;
        return TRUE;
    }
    
    idxAtom = atomStart;
    return FALSE;
}

int defVar() {
    int atomStart = idxAtom;

    if (consume(VAR)) {
        if (consume(ID)) {
            const char *nume = consumed->valoare.valoareStr;
            Simbol *s = cautaInDomeniulCurent(nume);
            if (s) 
                err(strcat("Redefinire simbol: ", nume));

            s = adaugaSimbol(nume, FEL_VAR);
            s->local = crtFn != NULL;

            if (consume(COLON)) {
                if (baseType()) {
                    s->tip = ret.tip;
                    if(consume(SEMICOLON))
                        return TRUE;
                    else {
                        idxAtom = atomStart;
                        err("Lipseste ';' la finalul declaratiei de variabila!");
                    }
                }
                else err("Tip de variabila invalid sau inexistent!");
            }
            else err("Lipseste ':' in interiorul declaratiei de variabila!");
        }
        else err("Lipseste denumirea variabilei!");
    }

    idxAtom = atomStart;
    return FALSE;
}

int program() {
    int atomStart = idxAtom;

    adaugaDomeniu();

    while (1) {
        if (defVar()) {}
        else if (defFunc()) {}
        else if (block()) {}
        else break;
    }

    if (consume(FINISH)) {
        stergeDomeniu();
        return TRUE;
    }

    idxAtom = atomStart;
    return FALSE;
}
/** ================================== */