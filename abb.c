#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	abb_destruir_dato_t dest;
	abb_comparar_clave_t cmp;
};

struct abb_iter {
	pila_t* pila;
};

/* Recibe un puntero hacia un dato y crea un nodo con ese valor en caso de ser
posible, y devuelve el nodo. Sino devuelve NULL. */
nodo_abb_t* crear_nodo(const char *clave, void* dato){
	nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
	if(!nodo) return NULL;
	nodo->valor = dato;
	nodo->clave = strdup(clave);
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

/* Recibe un puntero a un nodo, otro a una función de comparación, un puntero a
un char clave y un doble puntero extra, donde se almacenará el padre del nodo a buscar.
En caso de encontrar un nodo con la clave pasada, se lo devuelve, sino se devuelve NULL. */
nodo_abb_t* _buscar_nodo(nodo_abb_t* nodo, abb_comparar_clave_t cmp, const char *clave, nodo_abb_t** padre) {
	if(!nodo) {
		return NULL;
	}
	int res = cmp(clave, nodo->clave);
	if(res == 0) {
		return nodo;
	}
	if(padre) {
		*padre = nodo;
	}
	if(res < 0) {
		return _buscar_nodo(nodo->izq, cmp, clave, padre);
	}
	return _buscar_nodo(nodo->der, cmp, clave, padre);
}

/* Recibe puntero a un nodo y libera todos los nodos que están por debajo de él,
junto con los datos dinámicos almacenados en el árbol (si los hay). */
void _liberar_datos_y_nodos(nodo_abb_t *nodo, abb_destruir_dato_t destruir_dato) {
	if(!nodo) return;
	_liberar_datos_y_nodos(nodo->izq, destruir_dato);
	_liberar_datos_y_nodos(nodo->der, destruir_dato);
	if(destruir_dato) destruir_dato(nodo->valor);
	free(nodo->clave);
	free(nodo);
}

//PRIMITIVAS ÁRBOL BINARIO DE BÚSQUEDA.

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t* arbol = malloc(sizeof(abb_t));
	if(!arbol) return NULL;
	arbol->raiz = NULL;
	arbol->cantidad = 0;
	arbol->dest = destruir_dato;
	arbol->cmp = cmp;
	return arbol;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	nodo_abb_t* padre = NULL;
	nodo_abb_t* nodo = _buscar_nodo(arbol->raiz, arbol->cmp, clave, &padre);
	if(nodo) {
		if(arbol->dest) arbol->dest(nodo->valor);
		nodo->valor = dato;
		return true;
	}
	nodo_abb_t* nodo_nuevo = crear_nodo(clave, dato);
	if(!nodo_nuevo) return false;
	if(!arbol->raiz) {
		arbol->raiz = nodo_nuevo;
	} else if (arbol->cmp(clave, padre->clave) < 0) {
		padre->izq = nodo_nuevo;
	} else {
		padre->der = nodo_nuevo;
	}
	arbol->cantidad++;
	return true;
}

/* Busca la mínima clave en un árbol o subárbol que empieza en la raíz dada.
Post: devuelve un puntero al nodo mínimo.
*/
nodo_abb_t *buscar_min(nodo_abb_t *raiz) {
	if (!raiz->izq) {
		return raiz;
	}
	return buscar_min(raiz->izq);
}

/* Recibe la raíz de un árbol, la clave a borrar y una función de comparación.
Busca el nodo a borrar, recursivamente, y luego se ocupa de borrarlo cambiando las referencias
que sean necesarias para que se sigan cumpliendo las condiciones de ABB.
Post: devuelve la raíz del árbol sin el nodo cuya clave se pasó por parámetro, cumpliendo las
condiciones de ABB.
*/
nodo_abb_t *_abb_borrar(nodo_abb_t *raiz, const char *clave, abb_comparar_clave_t cmp) {
	if (!raiz) {
		return raiz;
	}
	int res = cmp(clave, raiz->clave);
	nodo_abb_t *tmp;
	if (res < 0) {
		raiz->izq = _abb_borrar(raiz->izq, clave, cmp);
	} else if (res > 0) {
		raiz->der = _abb_borrar(raiz->der, clave, cmp);
	} else {
		if (!raiz->izq) {
			tmp = raiz->der;
			if (raiz->clave) free(raiz->clave);
			raiz->clave = NULL;
			free(raiz);
			return tmp;
		}
		if (!raiz->der) {
			tmp = raiz->izq;
			if (raiz->clave) free(raiz->clave);
			raiz->clave = NULL;
			free(raiz);
			return tmp;
		}
		tmp = buscar_min(raiz->der);
		if (raiz->clave) free(raiz->clave);
		raiz->clave = tmp->clave;
		raiz->valor = tmp->valor;
		raiz->der = _abb_borrar(raiz->der, tmp->clave, cmp);
	}
	return raiz;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	nodo_abb_t *padre = NULL;
	nodo_abb_t *nodo = _buscar_nodo(arbol->raiz, arbol->cmp, clave, &padre);
	if (!nodo) {
		return NULL;
	}
	void *tmp = nodo->valor;
	arbol->raiz = _abb_borrar(arbol->raiz, clave, arbol->cmp);
	arbol->cantidad--;
	return tmp;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	nodo_abb_t* nodo = _buscar_nodo(arbol->raiz, arbol->cmp, clave, NULL);
	if(!nodo) return NULL;
	return nodo->valor;	
}	

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	return _buscar_nodo(arbol->raiz, arbol->cmp, clave, NULL) != NULL;
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol) {
	_liberar_datos_y_nodos(arbol->raiz, arbol->dest);
	free(arbol);
}

//PRIMITIVAS DEL ITERADOR EXTERNO DEL ÁRBOL.

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter) return NULL;
	pila_t* pila = pila_crear();
	if(!pila) {
		free(iter);
		return NULL;
	}
	iter->pila = pila;
	nodo_abb_t* act = arbol->raiz;
	while(act) {
		pila_apilar(iter->pila, act);
		act = act->izq;
	}
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	if(pila_esta_vacia(iter->pila)) {
		return false;
	}
	nodo_abb_t* ant = pila_desapilar(iter->pila);
	if(ant->der) {
		pila_apilar(iter->pila, ant->der);
		nodo_abb_t* act = ant->der->izq;
		while(act) {
			pila_apilar(iter->pila, act);
			act = act->izq;
		}
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	if(abb_iter_in_al_final(iter)) {
		return NULL;
	}
	return ((nodo_abb_t*)pila_ver_tope(iter->pila))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	pila_destruir(iter->pila);
	free(iter);
}

//PRIMITIVA DEL ITERADOR INTERNO DEL ÁRBOL.

/* Recibe un puntero a una raíz de un árbol, una función visitar y un puntero extra para hacer con
él lo que se prefiera. */
void _iterar_in_order(nodo_abb_t* nodo, bool visitar(const char *, void *, void *), void *extra) {
	if(!nodo) return;
	_iterar_in_order(nodo->izq, visitar, extra);
	visitar(nodo->clave, nodo->valor, extra);
	_iterar_in_order(nodo->der, visitar, extra);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	_iterar_in_order(arbol->raiz, visitar, extra);
}
