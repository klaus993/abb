#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <string.h>

/*Definición del struct nodo para la tabla de hash. */
typedef struct nodo_abb {
	char *clave;
	void *valor;
	struct abb_nodo* izq;
	struct abb_nodo* der;
} nodo_abb_t;

struct abb {
	size_t cantidad;
	abb_nodo_t* raiz;
	f_dest dest;
	f_cmp cmp;
};

struct abb_iter {
	abb_t* abb;
	pila_t* pila;
};

/* Recibe un puntero hacia un dato y crea un nodo con ese valor en caso de ser
posible, y devuelve el nodo. Sino devuelve NULL. */
nodo_abb_t* crear_nodo(const char *clave, void* dato){
	nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
	if(!nodo) return NULL;
	nodo->dato = dato;
	nodo->clave = clave;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t* arbol = malloc(sizeof(abb_t));
	if(!arbol) return NULL;
	arbol->raiz = NULL;
	arbol->cantidad = 0;
	arbol->f_dest = destruir_dato;
	arbol->f_cmp = cmp;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	if(arbol->cmp(clave, arbol->raiz->clave) == 0) {
		arbol->raiz->valor = dato;
		return true;
	}
	else if(arbol->cmp(clave, arbol->raiz->clave) < 0) abb_guardar(arbol->raiz->izq, clave, dato);
	else abb_guardar(arbol->raiz->der, clave, dato);
	if(!arbol->raiz->izq && !arbol->raiz->der) {
		nodo_abb_t* nodo = crear_nodo(clave, dato);
		if(!nodo) return false;
		if(arbol->cmp(clave, arbol->raiz->clave) > 0) arbol->raiz->der = nodo;
		else arbol->raiz->izq = nodo;
		(abb->cantidad)++;
		return true;
	}
}

void *abb_borrar(abb_t *arbol, const char *clave) {
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	if(arbol->cmp(clave, arbol->raiz->clave) == 0)
	if(!arbol->raiz->der && !arbol->raiz->izq)
	abb_obtener(arbol->raiz->der, clave);
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
}

size_t abb_cantidad(abb_t *arbol) {
	return abb->cantidad;
}

void abb_destruir(abb_t *arbol) {
}
