/*
 * TADTablero.cpp
 *
 *  Created on: 28 dic. 2022
 *      Author: Jorge Carrasco Facundo
 */



#include "TADTablero.h"


void iniciarTablero(Tablero &t, int filasIniciales, int colIniciales) {
	t.celdasUtiles = 0;
	t.ultimaCeldaFila = filasIniciales-1;
	t.ultimaCeldaCol = colIniciales-1;
	t.filas = MAXFILAS;
	t.col = colIniciales;

	int randnum;

	for (int i = 0; i < MAXFILAS; i++) {
		for (int j = 0; j < colIniciales; j++) {
			if (i < filasIniciales && j < colIniciales) {
				randnum = 1+rand()%(10-1);
				iniciarCelda(t.m[i][j], randnum);
				t.celdasUtiles++;
			} else {
				iniciarCeldaVacia(t.m[i][j]);
			}
		}
	}
}

int devolverFilasColumnas(Tablero t, int &col) {
	col = t.col;
	return t.filas;
}

int devolverUltimaCeldaOcupada(Tablero t, int &col) {
	col = t.ultimaCeldaCol;
	return t.ultimaCeldaFila;
}

int obtenerValorCelda(Tablero t, int i, int j) {
	return obtenerValor(t.m[i][j]);
}

void ponerValorCelda(Tablero &t, int i, int j, int num) {
	iniciarCelda(t.m[i][j], num);
}

void borrar(Tablero &t, int i, int j){
	borrarCelda(t.m[i][j]);
}

bool estaBorrada(Tablero t, int i, int j){
	return estaBorrada(t.m[i][j]);
}

bool sonPareja(Tablero t, int i, int j, int k, int m) {
	bool pareja = false;

	// Variables para saber cuál de los dos números pertenece a la fila menor, y sus coordenadas
	int filaMenor, colMenor, filaMayor, colMayor;

	if(i < k) {
		filaMenor = i;
		colMenor = j;
		filaMayor = k;
		colMayor = m;
	} else if (k > i) {
		filaMenor = k;
		colMenor = m;
		filaMayor = i;
		colMayor = j;
	} else {
		if (j < m) {
			filaMenor = i;
			colMenor = j;
			filaMayor = k;
			colMayor = m;
		} else {
			filaMenor = k;
			colMenor = m;
			filaMayor = i;
			colMayor = j;
		}
	}

	// AMPLIACIÓN PAREJAS SEPARADAS

	bool contiguas = true;

	bool ultimaDeFila = true;
	bool primeraDeFila = true;


	if(abs(filaMayor - filaMenor) == abs(colMayor - colMenor)) {// Comprobamos si están en la misma diagonal
		for(int i = 1; i < filaMayor - filaMenor && contiguas; i++){
			if(colMayor - colMenor > 0) { // Diagonal "principal"
				if(!estaBorrada(t.m[filaMenor+i][colMenor+i])){
					contiguas = false;
				}
			} else if (colMayor - colMenor < 0) { //Diagonal "secundaria"
				if(!estaBorrada(t.m[filaMenor+i][colMenor-i])){
					contiguas = false;
				}
			}
		}
	} else if(colMenor == colMayor) { // Si están en la misma columna
		for(int i = 1; i < filaMayor-filaMenor && contiguas; i++) {
			if(!estaBorrada(t.m[filaMenor+i][colMenor])){
				contiguas = false;
			}
		}
	} else if (filaMenor == filaMayor) { // si están en la misma fila
		for(int i = 1; i < colMayor - colMenor; i++) {
			if(!estaBorrada(t.m[filaMenor][colMenor+i])){
				contiguas = false;
			}
		}
	} else if (filaMayor - filaMenor == 1) { // Si son la última de una fila y la primera de la siguiente
		for(int i = colMenor+1; i < t.col && ultimaDeFila; i++){ // Comprobamos si es última no borrada de la fila
			if(!estaBorrada(t.m[filaMenor][i])){
				ultimaDeFila = false;
			}
		}
		for(int i = 0; i < colMayor && primeraDeFila; i++) { // Comprobamos si es la primera no borrada de la fila
			if(!estaBorrada(t.m[filaMayor][i])){
				primeraDeFila = false;
			}
		}

		if(!primeraDeFila || !ultimaDeFila) {
			contiguas = false;
		}
	} else {
		contiguas = false;
	}

	//FIN AMPLIACIÓN PAREJAS SEPARADAS
	// contiguas nos sirve para las condiciones del proyecto base

	bool valoresValidos;

	if((obtenerValor(t.m[i][j]) == obtenerValor(t.m[k][m]) || // Si tienen el mismo valor
	    obtenerValor(t.m[i][j]) + obtenerValor(t.m[k][m]) == 10) && // Si suman diez
		(i != k || j != m)) { // Y si son celdas diferentes
		valoresValidos = true;
	}

	pareja = (contiguas && valoresValidos);

	return pareja;
}


bool estaFilaBorrada(Tablero t, int i){

	bool borrada = true;

	for (int j = 0; j < t.col && borrada; j++){
		if(!estaBorrada(t.m[i][j])){
			borrada = false;
		}
	}

	return borrada;
}

void borrarFila(Tablero &t, int fila) {
	for(int i = fila; i < t.ultimaCeldaFila ; i++){
		for(int j = 0; j < t.col; j++){
			t.m[i][j] = t.m[i+1][j];
		}
	}

	t.celdasUtiles -= t.col;

	for (int j = 0; j < t.col; j++) {
		borrarCelda(t.m[t.ultimaCeldaFila][j]);
	}
	t.ultimaCeldaFila--;
}

void replicarNoBorradas(Tablero &t) {

	int ultimaCeldaFila = t.ultimaCeldaFila;
	int ultimaCeldaCol = t.ultimaCeldaCol;

	bool filaAMedias = ultimaCeldaCol != t.col-1;

	VectorReplica v;
	int pos = 0;
	int posActual = 0;

	// Copio las celdas no borradas en un vector
	for(int i = 0; i <= ultimaCeldaFila; i++) {
		for(int j = 0; j < t.col; j++) {
			if (i < ultimaCeldaFila) {
				if(!estaBorrada(t.m[i][j]) && !estaVacia(t.m[i][j])) {
								iniciarCelda(v[pos], obtenerValor(t.m[i][j]));
								pos++;
				}
			}	else if(i == ultimaCeldaFila && j <= ultimaCeldaCol) {
				if(!estaBorrada(t.m[i][j]) && !estaVacia(t.m[i][j])) {
				iniciarCelda(v[pos], obtenerValor(t.m[i][j]));
				pos++;
			}
			}
		}
	}

	// Replico primero en la fila a medias
	if(filaAMedias) {
		for(int j = ultimaCeldaCol+1; j < t.col && posActual <= pos; j++) {
			iniciarCelda(t.m[ultimaCeldaFila][j], obtenerValor(v[posActual]));
			posActual++;
			t.celdasUtiles++;
			ultimaCeldaCol = j;
		}
	}

	int i, j;
	// Replico en el resto de filas de la matriz
	for(i = ultimaCeldaFila+1; i < t.filas && posActual < pos; i++){
		for(j = 0; j < t.col && posActual < pos; j++) {
			iniciarCelda(t.m[i][j], obtenerValor(v[posActual]));
			posActual++;
			t.celdasUtiles++;
			ultimaCeldaCol = j;
		}
		ultimaCeldaFila = i;
	}

	// Vuelvo a guardar la posición de la última celda ocupada

	t.ultimaCeldaCol = ultimaCeldaCol;
	t.ultimaCeldaFila = ultimaCeldaFila;

}
