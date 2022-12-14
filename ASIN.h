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

    if (consume(INT)) {
        Text_write(crtCod, "%d", consumed->valoare.valoareInt);

        setRet(TYPE_INT, false);
        return TRUE;
    }
    if (consume(REAL)) {
        Text_write(crtCod, "%g", consumed->valoare.valoareFloat);

        setRet(TYPE_REAL, false);
        return TRUE;
    }
    if (consume(STR)) {
        Text_write(crtCod, "%s", consumed->valoare.valoareStr);

        setRet(TYPE_STR, false);
        return TRUE;
    }
    if (consume(LPAR)) {
        Text_write(crtCod, "(");

        if (expr()) {
            if (consume(RPAR)) {
                Text_write(crtCod, ")");
                return TRUE;
            }
            else err("Lipseste ')' la finalul expresiei!");
        }
    }
    else if (consume(ID)) {
        Simbol *s = cautaSimbol(consumed->valoare.valoareStr);
        Text_write(crtCod, "%s", s->nume);

        if (!s) {
            char err_str[300];

            sprintf(err_str, "Identificator necunoscut: %s", consumed->valoare.valoareStr);
            err(err_str);
        }
        if (consume(LPAR)) {
            Text_write(crtCod, "(");

            if (s->fel != FEL_FN) {
                char err_str[300];
                
                sprintf(err_str, "%s nu poate fi apelata, deoarece nu este o functie!", consumed->valoare.valoareStr);
                err(err_str);
            }

            Simbol *argDef = s->args;

            if (expr()) {
                if (!argDef) {
                    char err_str[300];

                    sprintf(err_str, "Functia %s este apelata cu prea multe argumente!", s->nume);
                    err(err_str);
                }

                if (argDef->tip != ret.tip) {
                    char err_str[300];

                    sprintf(err_str, "Tipul argumentului de la apelul functiei %s este diferit de cel de la definirea ei", s->nume);
                    err(err_str);
                }

                argDef = argDef->urm;

                while(1) {
                    if (consume(COMMA)) {
                        Text_write(crtCod, ",");

                        if (expr()) {
                            if (!argDef) {
                                char err_str[300];

                                sprintf(err_str, "Functia %s este apelata cu prea multe argumente!", s->nume);
                                err(err_str);
                            }

                            if (argDef->tip != ret.tip) {
                                char err_str[300];

                                sprintf(err_str, "Tipul argumentului de la apelul functiei %s este diferit de cel de la definirea ei", s->nume);
                                err(err_str);
                            }

                            argDef = argDef->urm;
                        }
                        else err("Lipseste parametrul functiei dupa virgula!");
                    }
                    else break;
                }
            }

            if (consume(RPAR)) {
                Text_write(crtCod, ")");

                if (argDef) {
                    char err_str[300];

                    sprintf(err_str, "Functia %s este apelata cu prea putine argumente!", s->nume);
                    err(err_str);
                }

                setRet(s->tip, false);
                return TRUE;
            }
            else {
                err("Lipseste ')' la sfarsitul apelului de functiei!");
            }
        }
        else {
            if (s->fel == FEL_FN) {
            char err_str[300];

            sprintf(err_str, "Functia %s se poate doar apela!", s->nume);
            err(err_str);
            }

            setRet(s->tip, true);
        }
        return TRUE;
    }

    idxAtom = atomStart;
    return FALSE;
}

int exprPrefix() {
    int atomStart = idxAtom;

    if (consume(SUB)) {
        Text_write(crtCod, "-");

        if (factor()) {
            if (ret.tip == TYPE_STR)
                err("Expresia lui - unar trebuia sa aiba tipul INT sau REAL!");
            ret.lval = false;
        }
    }
    else if (consume(NOT)) {
        Text_write(crtCod, "!");

        if (factor()) {
            if (ret.tip == TYPE_STR)
                err("Expresia lui ! unar trebuia sa aiba tipul INT sau REAL!");
            setRet(TYPE_INT, false);
        }
    }
    else if (factor()) return TRUE;

    idxAtom = atomStart;
    return FALSE;
}

int exprMul() {
    int atomStart = idxAtom;

    if (exprPrefix()) {
        while (1) {
            if (consume(MUL)) {
                Ret tipStanga = ret;
                Text_write(crtCod, "*");

                if (tipStanga.tip == TYPE_STR)
                    err("Operanzii lui * nu pot fi de tip STR!");

                if (exprPrefix()) {
                    if (tipStanga.tip != ret.tip)
                        err("Tipuri diferite pentru operanzii lui *!");
                    ret.lval = false;
                }
                else err("Lipseste operandul dupa operatorul de inmultire!");
            }
            else if (consume(DIV)) {
                Ret tipStanga = ret;
                Text_write(crtCod, "/");

                if (tipStanga.tip == TYPE_STR)
                    err("Operanzii lui / nu pot fi de tip STR!");

                if (exprPrefix()) {
                    if (tipStanga.tip != ret.tip)
                        err("Tipuri diferite pentru operanzii lui /!");
                    ret.lval = false;
                }
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
                Ret tipStanga = ret;
                Text_write(crtCod, "+");

                if (tipStanga.tip == TYPE_STR)
                    err("Operanzii lui + nu pot fi de tip STR!");

                if (exprMul()) {
                    if (tipStanga.tip != ret.tip)
                        err("Tipuri diferite pentru operanzii lui +!");
                    ret.lval = false;
                }
                else err("Lipseste operandul dupa operatorul de adunare!");
            }
            else if (consume(SUB)) {
                Ret tipStanga = ret;
                Text_write(crtCod, "-");

                if (tipStanga.tip == TYPE_STR)
                    err("Operanzii lui - nu pot fi de tip STR!");

                if (exprMul()) {
                    if (tipStanga.tip != ret.tip)
                        err("Tipuri diferite pentru operanzii lui -!");
                    ret.lval = false;
                }
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
            Ret tipStanga = ret;
            Text_write(crtCod, "<");

            if (exprAdd()) {
                if (tipStanga.tip != ret.tip)
                    err("Tipuri diferite pentru operanzii lui <!");
                setRet(TYPE_INT, false);
            }
            else err("Lipseste operandul dupa LESS!");
        }
        else if (consume(EQUAL)) {
            Ret tipStanga = ret;
            Text_write(crtCod, "==");

            if (exprAdd()) {
                if (tipStanga.tip != ret.tip)
                    err("Tipuri diferite pentru operanzii lui ==!");
                setRet(TYPE_INT, false);
            }
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
        const char *nume = consumed->valoare.valoareStr;

        if (consume(ASSIGN)) {
            Text_write(crtCod, "%s=", nume);

            if (exprComp()) {
                Simbol *s = cautaSimbol(nume);
                
                if (!s) {
                    char err_str[300];
                    sprintf(err_str, "Identificator necunoscut: %s", nume);

                    err(err_str);
                }

                if (s->tip == FEL_FN) {
                    char err_str[500];
                    sprintf(err_str, "O functie (%s) nu poate fi folosita ca destinatie a unei atriburi", nume);

                    err(err_str);
                }

                if (s->tip != ret.tip)
                    err("Sursa si destinatia atribuirii au tipuri diferite!");

                ret.lval = false;
                return TRUE;
            }
        }
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
            if (consume(AND)) {
                Ret tipStanga = ret;

                Text_write(crtCod, "&&");

                if (tipStanga.tip == TYPE_STR)
                    err("Operandul stang al lui && sau || nu poate fi de tip STR");

                if (exprAssign()) {
                    if (ret.tip == TYPE_STR)
                        err("Operandul drept al lui && sau || nu poate fi de tip STR");
                    setRet(TYPE_INT, false);
                }
                else err("Lipseste operandul dupa AND!");
            }
            else if (consume(OR)) {
                Ret tipStanga = ret;

                Text_write(crtCod, "||");
                if (tipStanga.tip == TYPE_STR)
                    err("Operandul stang al lui && sau || nu poate fi de tip STR");

                if (exprAssign()) {
                    if (ret.tip == TYPE_STR)
                        err("Operandul drept al lui && sau || nu poate fi de tip STR");
                    setRet(TYPE_INT, false);
                }
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
        if (consume(SEMICOLON)) {
            Text_write(crtCod, ";\n");

            return TRUE;
        }
        else {
            idxAtom = startAtom;
            err("Lipseste ';' la finalul expresiei!");
        }
    }
    else if (consume(SEMICOLON)) return TRUE;

    if (consume(IF)) {
        if (consume(LPAR)) {
            Text_write(crtCod, "if(");

            if (expr()) {
                if (ret.tip == TYPE_STR)
                    err("Conditia lui IF trebuie sa aiba tipul INT sau REAL.");

                if (consume(RPAR)) {
                    Text_write(crtCod, "){\n");

                    if (block()) {
                        Text_write(crtCod, "}\n");

                        if (consume(ELSE)) {
                            Text_write(crtCod, "else{\n");

                            if (block()) {
                                Text_write(crtCod, "}\n");
                            }
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
        Text_write(crtCod, "return ");

        if (expr()) {
            if (!crtFn)
                err("RETURN poate fi folosit doar intr-o functie!");
            
            if (ret.tip != crtFn->tip)
                err("Tipul lui RETURN este diferit de tipul returnat de functie!");

            if (consume(SEMICOLON)) {
                Text_write(crtCod, ";\n");

                return TRUE;
            }
            else err("Lipseste ';' la finalul instructiunii RETURN!");
        }
        else err("Lipseste o expresie dupa RETURN!");
    }
    else if (consume(WHILE)) {
        Text_write(crtCod, "while(");

        if (consume(LPAR)) {
            if (expr()) {
                if (ret.tip == TYPE_STR)
                    err("Conditia lui WHILE trebuie sa aiba tipul INT sau REAL.");

                if (consume(RPAR)) {
                    Text_write(crtCod, "){\n");

                    if (block()) {
                        if (consume(END)) {
                            Text_write(crtCod, "}\n");

                            return TRUE;
                        }
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

                Text_write(&tAntetFn, "%s %s", cType(ret.tip), nume);

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
                Text_write(&tAntetFn, ",");

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

            crtCod = &tFunctii;
            crtVar = &tFunctii;
            Text_clear(&tAntetFn);
            Text_write(&tAntetFn, "%s(",nume);

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

                                Text_write(&tFunctii, "\n%s %s){\n", cType(ret.tip), tAntetFn.buf);

                                while(1) {
                                    if (defVar()) {}
                                    else break;
                                }

                                if (block()) {
                                    if(consume(END)) {
                                        stergeDomeniu();
                                        crtFn = NULL;

                                        Text_write(&tFunctii, "}\n");
                                        crtCod = &tMain;
                                        crtVar = &tInceput;

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
    
            if (s) { 
                char err_str[100];
                sprintf(err_str, "Redefinire simbol: %s", nume);

                err(err_str);
            }

            s = adaugaSimbol(nume, FEL_VAR);
            s->local = (crtFn != NULL);

            if (consume(COLON)) {
                if (baseType()) {
                    s->tip = ret.tip;
                

                    if (consume(SEMICOLON)) {
                        Text_write(crtVar, "%s %s;\n", cType(ret.tip), nume);
                        return TRUE;
                    }
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
    adaugaFnPredefinite();

    crtCod = &tMain;
    crtVar = &tInceput;
    Text_write(&tInceput, "#include \"quick.h\"\n\n");
    Text_write(&tMain, "\nint main(){\n");

    while (1) {
        if (defVar()) {}
        else if (defFunc()) {}
        else if (block()) {}
        else break;
    }

    if (consume(FINISH)) {
        Text_write(&tMain, "return 0;\n}\n");
        FILE *fis;
        
        if(!(fis = fopen("1.c", "w"))) {
            printf("Nu se poate scrie in fisierul 1.c\n");
            exit(EXIT_FAILURE);
        }

        fwrite(tInceput.buf, sizeof(char), tInceput.n, fis);
        fwrite(tFunctii.buf, sizeof(char), tFunctii.n, fis);
        fwrite(tMain.buf, sizeof(char), tMain.n, fis);
        fclose(fis);

        stergeDomeniu();
        return TRUE;
    }

    idxAtom = atomStart;
    return FALSE;
}
/** ================================== */