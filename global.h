#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SPATII_TAB 4
#define MAX_DIM_CONTINUT 30001

#define TRUE 1
#define FALSE 0

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