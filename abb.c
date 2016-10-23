#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <string.h>

/*Definición del struct nodo para la tabla de hash. */
typedef struct nodo_abb {
	char *clave;
	void *valor;
	struct nodo_abb* izq;
	struct nodo_abb* der;
} nodo_abb_t;

struct abb {
	size_t cantidad;
	nodo_abb_t* raiz;
	f_dest dest;
	f_cmp cmp;
};

struct abb_iter {
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

/* Recibe un puntero a un nodo, otro a una función de comparación, un puntero a
un char clave y un doble puntero a la raiz. En caso de encontrar un nodo con la
clave pasada, se lo devuelve, sino se devuelve NULL. */
nodo_abb_t* buscar_nodo(nodo_abb_t* nodo, abb_comparar_clave_t cmp, const char *clave, nodo_abb_t** raiz) {
	if(!nodo) return NULL;
	int res = cmp(clave, nodo->clave);
	if(res == 0) return nodo;
	if(raiz) {
		*raiz = nodo;
		if(res < 0) return buscar_nodo(nodo->izq, cmp, clave, raiz);
		return buscar_nodo(nodo->der, cmp, clave, raiz);
	}
}

/* Recibe puntero a un nodo y libera todos los nodos que están por debajo de él,
junto con los datos dinámicos almacenados en el árbol. */
void _liberar_datos_y_nodos(nodo_abb_t *nodo) {
	if(!arbol) return;
	_liberar_datos_y_nodos(nodo->izq);
	_liberar_datos_y_nodos(nodo->der);
	if(arbol->f_dest) arbol->f_dest(nodo->valor);
	free(nodo->clave);
	free(nodo);
}

//PRIMITIVAS ÁRBOL BINARIO DE BÚSQUEDA.

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
	abb_t** padre;
	nodo_abb_t* nodo = buscar_nodo(arbol->raiz, arbol, clave, padre);
	if(!nodo) {
		nodo_abb_t* nodo_nuevo = crear_nodo(clave, dato);
		if(!nodo_nuevo) return false;
		if(arbol->cmp(clave, nodo->clave) < 0) *(padre)->izq = nodo_nuevo;
		*(padre)->der = nodo_nuevo;
	}
	else{
		if(arbol->f_dest) arbol->f_dest(nodo->valor);
		nodo->valor = dato;
	}
	return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	nodo_abb_t* nodo = buscar_nodo(arbol->raiz, arbol, clave, NULL);
	if(!nodo) return NULL;
	return nodo->valor;
	
}	

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	nodo_abb_t* nodo = buscar_nodo(arbol->raiz, arbol, clave, NULL);
	return nodo;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
	_destruir_datos_y_nodos(arbol->raiz);
	free(arbol);
}

//PRIMITIVAS DEL ITERADOR EXTERNO DEL ÁRBOL.

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter) return NULL;
	pila_t* pila = malloc(sizeof(pila_t));
	if(!pila) {
		free(iter);
		return NULL;
	}
	iter->pila = pila;
	nodo_abb_t* act = arbol->raiz;
	while(act->izq) {
		pila_apilar(iter->pila, act->clave);
		act = act->izq;
	}
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	if(abb_iter_al_final(iter)) return false;
	nodo_abb_t* ant = pila_desapilar(iter->pila);
	if(ant->der) {
		pila_apilar(iter->pila, ant->der->clave);
		nodo_abb_t* act = ant->der->izq;
		while(act) {
			pila_apilar(iter->pila, act->clave);
			act = act->izq;
		}
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	if(abb_iter_in_al_final(iter)) return NULL;
	return pila_ver_tope(iter->pila);
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	free(iter->pila);
	free(iter);
}

//PRIMITIVA DEL ITERADOR INTERNO DEL ÁRBOL.

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	nodo_abb_t* act = arbol->raiz;
	while(act->izq) act = act->izq;
	while(aux && visitar(act->clave, *, *)) {

		act = aux->sig;
}
