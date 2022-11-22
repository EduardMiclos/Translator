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

    /** Analizorul sintactic. */
    idxAtom = 0;
    if (program())
        printf("Analiza sintactica completa!");

    return 0;
}

