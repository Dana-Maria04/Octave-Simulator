// Copyright Caruntu Dana-Maria 311CAa 2023-2024
#include <stdio.h>
#include <stdlib.h>
#define MODULO 10007

int **alloc_matrix(int n, int m)
{
	// functie pentru alocarea dinamica a matricii
	int i;
	// alocarea matricii
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) {
		//realizarea defensiva a alocarii
		printf("first malloc failed!\n");
		return NULL;
	}
	// alocare pe linii
	for (i = 0; i < n; i++) {
		a[i] = (int *)malloc(m * sizeof(int));
		if (!a[i]) {
			printf("allocating every array failed\n");
			while (--i >= 0)
				free(a[i]);
			free(a);
			return NULL;
		}
	}
	return a;
}

void realloc_matrix(int ****matrix, int **idx_c, int **idx_l,
					int size_matrix, int *capacity, int n)
{
	if (size_matrix >= (*capacity)) {
		// verifica daca nr de matrici depaseste capacitatea vectorului
		(*capacity) = (*capacity) * n + 1;
		// daca da , mareste capacitatea cu un n necesar
		// si realizeaza realocarea
		(*matrix) = realloc((*matrix), (*capacity) * sizeof(int **));
		(*idx_c) = realloc((*idx_c), (*capacity) * sizeof(int));
		(*idx_l) = realloc((*idx_l), (*capacity) * sizeof(int));
		if (!(*matrix) || !(*idx_c) || !(*idx_l)) {
			printf("realloc failed\n");
			free(matrix);
			free(idx_l);
			free(idx_c);
			exit(-1);
		}
	}
}

void free_matrix(int **a, int n)
{
	// functie de eliberare a matricii
	for (int i = 0; i < n; i++)
		free(a[i]);
	free(a);
}

void read_matrix(int **a, int n, int m)
{
	// functie care imi citeste matricea
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			scanf("%d", &a[i][j]);
	}
}

void multiply_matrix(int ***matrix, int nr_matrix1, int nr_matrix2,
					 int *idx_l, int *idx_c,
					 int *ptr_size_matrix)
{
	int size_matrix = *ptr_size_matrix;
	// ptr_size_matrix este un pointer catre size matrix
	if (nr_matrix1 < 0 || nr_matrix1 >= size_matrix || nr_matrix2 < 0 ||
		nr_matrix2 >= size_matrix) {
		// verifica daca matricile se afla in vector
		printf("No matrix with the given index\n");
		return;
	}
	// verifica daca se poate realiza inmultirea intre ele
	if (idx_c[nr_matrix1] != idx_l[nr_matrix2]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	// Alocare memorie pentru matricea rezultat
	int **prod = alloc_matrix(idx_l[nr_matrix1],
							  idx_c[nr_matrix2]);
	// inmultirea matricilor
	for (int i = 0; i < idx_l[nr_matrix1]; i++) {
		for (int j = 0; j < idx_c[nr_matrix2]; j++) {
			prod[i][j] = 0;// initializez matricea rezultat
			for (int k = 0; k < idx_c[nr_matrix1]; k++) {
				prod[i][j] =
					(prod[i][j] + matrix[nr_matrix1][i][k] *
					matrix[nr_matrix2][k][j]) % MODULO;
				prod[i][j] = ((prod[i][j]) % MODULO + MODULO) % MODULO;
				// pentru a evita probleme de overflow
			}
		}
	}
	idx_l[size_matrix] = idx_l[nr_matrix1];
	idx_c[size_matrix] = idx_c[nr_matrix2];
	matrix[size_matrix] = prod;
	// adaug la final matricea rezultat in vectorul de matrici si in vectorul
	// de index de linie si coloana
	*ptr_size_matrix = size_matrix + 1;
	// incrementez size matrix deorece adaug matricea rezultat la final
}

void trans(int ***matrix, int nr_matrix, int mat_crt,
		   int *idx_l, int *idx_c)
{
scanf("%d", &mat_crt);
if (mat_crt < 0 || mat_crt >= nr_matrix) {
	printf("No matrix with the given index\n");
	} else {
		// alocare memorie pentru matricea transpusa
		int **transpose = alloc_matrix(idx_c[mat_crt],
									idx_l[mat_crt]);
		// Transpunerea matricei
		for (int i = 0; i < idx_l[mat_crt]; i++) {
			for (int j = 0; j < idx_c[mat_crt]; j++)
				transpose[j][i] = matrix[mat_crt][i][j];
		}
		// Eliberarea memoriei matricei vechi și actualizarea cu noua matrice
		// transpusa
		free_matrix(matrix[mat_crt], idx_l[mat_crt]);
		matrix[mat_crt] = transpose;
		// Actualizare dimensiuni
		int tmp = idx_l[mat_crt];
		idx_l[mat_crt] = idx_c[mat_crt];
		idx_c[mat_crt] = tmp;
	}
}

int sum(int **a, int *idx_l, int *idx_c, int mat_crt)
{
	// functie care imi realizeaza suma elementelor
	// dintr-o matrice
	int s = 0;
	for (int i = 0; i < idx_l[mat_crt]; i++) {
		for (int j = 0; j < idx_c[mat_crt]; j++) {
			s += a[i][j] % MODULO;
			s = ((s % MODULO) + MODULO) % MODULO;
		}
	}
	return s;
}

void sort(int ***matrix, int *idx_l, int *idx_c,
		  int size_matrix)
{
	int **aux1;
	for (int i = 0; i < size_matrix; i++) {
		for (int j = i + 1; j < size_matrix; j++) {
			if (sum(matrix[i], idx_l, idx_c, i) >
				sum(matrix[j], idx_l, idx_c, j)) {
				// sortez matricele din vector prin interschimbare
				// complexitate O(size_matrix^2)
				aux1 = matrix[i];
				matrix[i] = matrix[j];
				matrix[j] = aux1;
				// sortez indicii matricelor
				int aux2 = idx_l[i];
				idx_l[i] = idx_l[j];
				idx_l[j] = aux2;
				int aux3 = idx_c[i];
				idx_c[i] = idx_c[j];
				idx_c[j] = aux3;
			}
		}
	}
}

int **multiply(int **m1, int **m2, int n)
{
// functie care inmulteste 2 matrici oarecare
// de dimensiuni n x n
// pe care o folosesc pentru functia power
	int **m3;
	m3 = alloc_matrix(n, n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			m3[i][j] = 0;
			for (int k = 0; k < n; k++) {
				m3[i][j] += (m1[i][k] * m2[k][j]) % MODULO;
				m3[i][j] = (m3[i][j] % MODULO + MODULO) % MODULO;
			}
		}
	}
	/* rezultatul il memorez in prima matrice
	 apoi eliberez matricea m3 auxiliara
	 returnez rezultatul inmultirii */
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			m1[i][j] = m3[i][j];
	free_matrix(m3, n);
	return m1;
}

int **power(int ***matrix, int *idx_l, int mat_crt, int p)
{
	int **id;// creez matricea In(matricea identitate)
	id = alloc_matrix(idx_l[mat_crt],
					  idx_l[mat_crt]);
	for (int i = 0; i < idx_l[mat_crt]; i++) {
		for (int j = 0; j < idx_l[mat_crt]; j++) {
			if (i == j)
				id[i][j] = 1;
			else
				id[i][j] = 0;
		}
	}
	while (p > 0) {
		if (p % 2 == 1) {
			id =
				multiply(id, matrix[mat_crt], idx_l[mat_crt]);
			// daca puterea e impara , inmultesc matricea cu In ca sa o pastrez
		}
		matrix[mat_crt] =
			multiply(matrix[mat_crt], matrix[mat_crt],
					 idx_l[mat_crt]);
				// daca puterea e para , ridic matricea la patrat
		p /= 2;
		// impart puterea la 2 pentru a reduce nr de operatii
	}
	free_matrix(matrix[mat_crt], idx_l[mat_crt]);
	// eliberez matricea veche
	return id;
}

void sum_matrix(int n, int **a, int **b, int **c)
{
	/* functie care realizeaza suma intre 2 matrice
	patratice de dimensiune n */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			c[i][j] = ((a[i][j] + b[i][j]) % MODULO);
			c[i][j] = ((c[i][j] % MODULO) + MODULO) % MODULO;
		}
	}
}

void dif_matrix(int n, int **a, int **b, int **c)
{
	/* functie care realizeaza diferenta intre 2 matrice
	patratice de dimensiune n */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			c[i][j] = ((a[i][j] - b[i][j]) % MODULO);
			c[i][j] = ((c[i][j] % MODULO) + MODULO) % MODULO;
		}
	}
}

void divide(int n, int **A, int **A11, int **A12, int **A21, int **A22)
{
	/* functie care imi desparte matricea in mai multe blocuri de
	matrice(submatrice) */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			A11[i][j] = A[i][j];
			A12[i][j] = A[i][j + n];
			A21[i][j] = A[i + n][j];
			A22[i][j] = A[i + n][j + n];
		}
	}
}

void strassen(int n, int **A, int **B, int **C)
{
	if (n == 1) {
		C[0][0] = (((A[0][0] * B[0][0]) % MODULO) % MODULO + MODULO) % MODULO;
		// pentru a evita probleme de overflow
	} else {
		int newn = n / 2;
		int **A11 = alloc_matrix(newn, newn);
		int **A12 = alloc_matrix(newn, newn);
		int **A21 = alloc_matrix(newn, newn);
		int **A22 = alloc_matrix(newn, newn);
		int **B11 = alloc_matrix(newn, newn);
		int **B12 = alloc_matrix(newn, newn);
		int **B21 = alloc_matrix(newn, newn);
		int **B22 = alloc_matrix(newn, newn);
		int **C11 = alloc_matrix(newn, newn);
		int **C12 = alloc_matrix(newn, newn);
		int **C21 = alloc_matrix(newn, newn);
		int **C22 = alloc_matrix(newn, newn);
		int **aux1 = alloc_matrix(newn, newn);
		int **aux2 = alloc_matrix(newn, newn);
		int **M1 = alloc_matrix(newn, newn);
		int **M2 = alloc_matrix(newn, newn);
		int **M3 = alloc_matrix(newn, newn);
		int **M4 = alloc_matrix(newn, newn);
		int **M5 = alloc_matrix(newn, newn);
		int **M6 = alloc_matrix(newn, newn);
		int **M7 = alloc_matrix(newn, newn);
		// aloc memorie tuturor matricilor pe care le voi utiliza
		divide(newn, A, A11, A12, A21, A22);
		divide(newn, B, B11, B12, B21, B22);
		// impart in blocuri matricile ce intra in inmultire
		sum_matrix(newn, A11, A22, aux1);// A11+A22
		sum_matrix(newn, B11, B22, aux2);// B11+B22
		strassen(newn, aux1, aux2, M1);// (A11+A22)*(B11+B22)=M1
		sum_matrix(newn, A21, A22, aux1);// A21+A22
		strassen(newn, aux1, B11, M2);// (A21+A22)*B11=M2
		dif_matrix(newn, B12, B22, aux1);// B11-B22
		strassen(newn, A11, aux1, M3);// A11*(B11-B22)=M3
		dif_matrix(newn, B21, B11, aux1);// B21-B11
		strassen(newn, A22, aux1, M4);// A22*(B21-B11)=M4
		sum_matrix(newn, A11, A12, aux1);// A11+A12
		strassen(newn, aux1, B22, M5);// A22*(B21-B11)=M5
		dif_matrix(newn, A21, A11, aux1);// A21-A11
		sum_matrix(newn, B11, B12, aux2);// B11+B12
		strassen(newn, aux1, aux2, M6);// (A21-A22)*(B11+B12)=M6
		dif_matrix(newn, A12, A22, aux1);// A12-A22
		sum_matrix(newn, B21, B22, aux2);// B21+B22
		strassen(newn, aux1, aux2, M7);// (A12-A22)*(B21+B22)=M7
		sum_matrix(newn, M1, M4, aux1);// M1+M4
		dif_matrix(newn, aux1, M5, aux2);// (M1+M4)-M5
		sum_matrix(newn, aux2, M7, C11);// (M1+M4)-M5+M7=C11
		sum_matrix(newn, M3, M5, C12);// M3+M5=C12
		sum_matrix(newn, M2, M4, C21);// M2+M4=C21
		dif_matrix(newn, M1, M2, aux1);// M1-M2
		sum_matrix(newn, M3, M6, aux2);// M3+M6
		sum_matrix(newn, aux1, aux2, C22);//(M1-M2)+(M3+M6)=C22
		// actualizez matricea rezultat (unesc blocurile rezultate)
		for (int i = 0; i < newn; i++) {
			for (int j = 0; j < newn; j++) {
				C[i][j] = C11[i][j];
				C[i][j + newn] = C12[i][j];
				C[i + newn][j] = C21[i][j];
				C[i + newn][j + newn] = C22[i][j];
			}
		}
		// eliberarea matricilor alocate
		free_matrix(A11, newn); free_matrix(A12, newn);
		free_matrix(A21, newn); free_matrix(A22, newn);
		free_matrix(B11, newn); free_matrix(B12, newn);
		free_matrix(B21, newn); free_matrix(B22, newn);
		free_matrix(C11, newn); free_matrix(C12, newn);
		free_matrix(C21, newn); free_matrix(C22, newn);
		free_matrix(aux1, newn); free_matrix(aux2, newn);
		free_matrix(M1, newn); free_matrix(M2, newn);
		free_matrix(M3, newn); free_matrix(M4, newn);
		free_matrix(M5, newn); free_matrix(M6, newn);
		free_matrix(M7, newn);
	}
}

void redimens(int ***matrix, int mat_crt, int *idx_l,
			  int *idx_c, int size_matrix)
{
	scanf("%d", &mat_crt);
	int *lin;
	int *col;
	/* fac un vector de linie si un vector de coloane carea imi va
	retine indicii */
	int nrlin, nrcol;
	scanf("%d\n", &nrlin);
	lin = (int *)malloc(nrlin * sizeof(int));
	if (!lin) {
		printf("malloc failed!\n");
		return;
	}
	for (int i = 0; i < nrlin; i++)
		scanf("%d", &lin[i]);
	scanf("%d", &nrcol);
	col = (int *)malloc(nrcol * sizeof(int));
	if (!col) {
		printf("malloc failed!\n");
		return;
	}
	for (int j = 0; j < nrcol; j++)
		scanf("%d", &col[j]);
	if (mat_crt < 0 || mat_crt >= size_matrix) {
		printf("No matrix with the given index\n");
	} else {
		int **red = alloc_matrix(nrlin, nrcol);
		for (int i = 0; i < nrlin; i++) {
			for (int j = 0; j < nrcol; j++)
				red[i][j] = matrix[mat_crt][lin[i]][col[j]];
				// redimensionarea matricii dorite
		}
		// eliberarea memoriei matricei vechi si actualizarea ei
		free_matrix(matrix[mat_crt], idx_l[mat_crt]);
		matrix[mat_crt] = red;
		// Actualizare dimensiuni
		idx_l[mat_crt] = nrlin;
		idx_c[mat_crt] = nrcol;
	}
	free(lin);
	free(col);
}

void release(int ***matrix, int *idx_l, int *idx_c,
			 int *size_matrix)
{
	int current_matrix;
	int **aux1;
	scanf("%d", &current_matrix);
	if (current_matrix < 0 || current_matrix >= *size_matrix) {
		printf("No matrix with the given index\n");
	} else {
		for (int i = current_matrix; i < *size_matrix - 1; i++) {
			/* mut la stanga matricile din vector cu cate o pozitie
			pentru a tine cont de indici si
			pentru a nu lasa spatiu gol */
			aux1 = matrix[i];
			matrix[i] = matrix[i + 1];
			matrix[i + 1] = aux1;
			// de asemenea pentru linii si coloane
			int aux2 = idx_l[i];
			idx_l[i] = idx_l[i + 1];
			idx_l[i + 1] = aux2;
			int aux3 = idx_c[i];
			idx_c[i] = idx_c[i + 1];
			idx_c[i + 1] = aux3;
		}
		free_matrix(matrix[(*size_matrix) - 1], idx_l[(*size_matrix) - 1]);
		*size_matrix = (*size_matrix) - 1;
	}
}

void dimensions(int mat_crt, int size_matrix, int *idx_c, int *idx_l)
{
	scanf("%d", &mat_crt);
	if (mat_crt < size_matrix && mat_crt >= 0)
		printf("%d %d\n", idx_l[mat_crt], idx_c[mat_crt]);
		// afisarea dimensiunilor
	else
		printf("No matrix with the given index\n");
}

void print(int mat_crt, int size_matrix, int *idx_l, int *idx_c, int ***matrix)
{
	scanf("%d", &mat_crt);
	if (mat_crt < size_matrix && mat_crt >= 0) {
		for (int i = 0; i < idx_l[mat_crt]; i++) {
			for (int j = 0; j < idx_c[mat_crt]; j++)
				printf("%d ", matrix[mat_crt][i][j]);
				// afisarea matricei dorite
			printf("\n");
		}
	} else {
		printf("No matrix with the given index\n");
	}
}

void end(int ****matrix, int **idx_l, int **idx_c, int size_matrix)
{
	for (int i = 0; i < size_matrix; i++)
		free_matrix((*matrix)[i], (*idx_l)[i]);
	free((*matrix));
	free((*idx_l));
	free((*idx_c));
	size_matrix = 0;
	/* eliberez toate resursele de memorie
	nr de matrici devine 0 deoarece am eliberat tot*/
}

int main(void)
{
	int n, m, i = 0, p;  // n=dimens linie; m=dimens coloana; p=puterea
	char character;
	int ***matrix = NULL, **a, *idx_l = NULL, *idx_c = NULL;
	int capacity = 0, size_matrix = 0, matrix1, matrix2, m1, m2, mat_crt;
	// initial in vectorul de matrici "matrix" nu se afla nicio matrice
	scanf("%c", &character);
	while (character != 'Q') {
		if (character == 'L') {
			realloc_matrix(&matrix, &idx_c, &idx_l, size_matrix, &capacity, 2);
			scanf("%d%d", &n, &m);
			a = alloc_matrix(n, m); read_matrix(a, n, m);
			idx_l[size_matrix] = n;
			idx_c[size_matrix] = m;
			matrix[size_matrix] = a;
			i++; size_matrix++;
		}
		if (character == 'D')
			dimensions(mat_crt, size_matrix, idx_c, idx_l);
		if (character == 'P')
			print(mat_crt, size_matrix, idx_l, idx_c, matrix);
		if (character == 'M') {
			realloc_matrix(&matrix, &idx_c, &idx_l, size_matrix, &capacity, 2);
			scanf("%d%d", &matrix1, &matrix2);
			multiply_matrix(matrix, matrix1, matrix2, idx_l,
							idx_c, &size_matrix);
		}
		if (character == 'T')
			trans(matrix, size_matrix, mat_crt, idx_l,
				  idx_c);
		if (character == 'R') {
			scanf("%d%d", &mat_crt, &p);
			if (mat_crt < 0 || mat_crt > size_matrix) {
				printf("No matrix with the given index\n");
			} else {
				if (p < 0) {
					printf("Power should be positive\n");
				} else {
					if (idx_l[mat_crt] != idx_c[mat_crt])
						printf("Cannot perform matrix multiplication\n");
					else
						matrix[mat_crt] = power(matrix, idx_l, mat_crt, p);
				}
			}
		}
		if (character == 'O')
			sort(matrix, idx_l, idx_c, size_matrix);
		if (character == 'F') {
			release(matrix, idx_l, idx_c, &size_matrix);
			realloc_matrix(&matrix, &idx_c, &idx_l,
						   size_matrix, &capacity, 1 / 2);
		}
		if (character == 'C')
			redimens(matrix, mat_crt, idx_l, idx_c,
					 size_matrix);
		if (character == 'S') {
			scanf("%d%d", &m1, &m2);
			realloc_matrix(&matrix, &idx_c, &idx_l,
						   size_matrix, &capacity, 2);
			matrix[size_matrix] = alloc_matrix(idx_l[m1], idx_c[m2]);
			// aloc memorie pentru a adauga la final rezultatul inmultirii
			strassen((idx_l)[m1], matrix[m1], matrix[m2], matrix[size_matrix]);
			idx_l[size_matrix] = idx_l[m1];// actualizare vectori de indici
			idx_c[size_matrix] = idx_c[m1]; size_matrix++;
		}
		if (character != 'L' && character != 'D' && character != 'P' &&
			character != 'C' && character != 'M' && character != 'O' &&
			character != 'T' && character != 'R' && character != 'F' &&
			character != 'Q' && character != 'S') {
			printf("Unrecognized command\n");
		}
		scanf(" %c", &character);
	}
	end(&matrix, &idx_l, &idx_c, size_matrix);
	return 0;
}
