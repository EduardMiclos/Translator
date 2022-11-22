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
/** ================= */


void err (const char *msg) {
    printf("Eroare in linia %d: %s\n", atomi[idxAtom].linie, msg);
    exit(EXIT_FAILURE);
}

int consume(int cod) {
	if(atomi[idxAtom].codAtom == cod){
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
            if (consume(MUL) || consume(DIV)) {
                if (exprPrefix()) {}
                else err("Lipseste operandul in cadrul inmultirii/impartirii!");
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
            if (consume(ADD) || consume(SUB)) {
                if (exprMul()) {}
                else err("Lipseste operandul in cadrul adunarii/scaderii!");
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
        if (consume(LESS) || consume(EQUAL)) {
            if (exprAdd()) {}
            else err("Lipseste operandul in cadrul compararii!");
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
        else idxAtom = atomStart;
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
            if (consume(AND) || consume(OR)) {
                if (exprAssign()) {}
                else err("Lipseste operandul in cadrul expresiei logice!");
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

    if (expr()) {}

    if (consume(SEMICOLON)) return TRUE;
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
        if (consume(COLON)) {
            if (baseType()) return TRUE;
            else err("Lipseste tipul parametrului!");
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
            if (consume(LPAR)) {
                if (funcParams()) {
                    if (consume(RPAR)) {
                        if (consume(COLON)) {
                            if (baseType()) {
                                while(1) {
                                    if (defVar()) {}
                                    else break;
                                }

                                if (block()) {
                                    if(consume(END))
                                        return TRUE;
                                    else err("Lipseste END la sfarsitul blocului de instructiuni!");
                                }
                                else err("Lipseste blocul de instructiuni in definitia functiei!");
                            }
                            else err("Tip invalid de variabila!");
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

    if (consume(TYPE_INT)  || 
        consume(TYPE_REAL) || 
        consume(TYPE_STR)) 
        return TRUE;
    
    idxAtom = atomStart;
    return FALSE;
}

int defVar() {
    int atomStart = idxAtom;

    if (consume(VAR)) {
        if (consume(ID)) {
            if (consume(COLON)) {
                if (baseType()) {
                    if(consume(SEMICOLON))
                        return TRUE;
                    else err("Lipseste ';' la finalul declaratiei de variabila!");
                }
                else err("Tip invalid de variabila!");
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

    while (1) {
        if (defVar()) {}
        else if (defFunc()) {}
        else if (block()) {}
        else break;
    }

    if (consume(FINISH))
        return TRUE;

    idxAtom = atomStart;
    return FALSE;
}
/** ================================== */