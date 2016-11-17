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
	char *clave_copiada = malloc(sizeof(char) * strlen(clave) + 1);
	strcpy(clave_copiada, clave);
	nodo->clave = clave_copiada;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

/* Recibe un puntero a un nodo, otro a una función de comparación, un puntero a
un char clave y un doble puntero al padre. En caso de encontrar un nodo con la
clave pasada, se lo devuelve, sino se devuelve NULL. */
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
junto con los datos dinámicos almacenados en el árbol. */
void _liberar_datos_y_nodos(nodo_abb_t *nodo, abb_destruir_dato_t destruir_dato) {
	if(!nodo) return;
	_liberar_datos_y_nodos(nodo->izq, destruir_dato);
	if(destruir_dato) destruir_dato(nodo->valor);
	free(nodo->clave);
	free(nodo);
	_liberar_datos_y_nodos(nodo->der, destruir_dato);
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

nodo_abb_t *buscar_max(nodo_abb_t *raiz) {
	if (!raiz->der) {
		return raiz;
	}
	return buscar_max(raiz->der);
}

void intercambiar(nodo_abb_t *nodo) {
	nodo_abb_t *max = buscar_max(nodo->izq);
	if (!max->izq && !max->der) {
		return;
	}
	nodo->clave = max->clave;
	nodo->valor = max->valor;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	nodo_abb_t* padre = arbol->raiz;
	nodo_abb_t* nodo = _buscar_nodo(arbol->raiz, arbol->cmp, clave, &padre);
	if(!nodo) {
		return NULL;
	}
	if(nodo == arbol->raiz && !nodo->izq && !nodo->der) {
		arbol->raiz = NULL;
	} else if(!nodo->izq && !nodo->der) {
		if(nodo == padre->izq) {
			padre->izq = NULL;
		} else {
			padre->der = NULL;
		}
	} else if ((!nodo->izq || !nodo->der) && nodo != arbol->raiz) {
		if(nodo == padre->izq) {
			if(nodo->der) {
				padre->izq = nodo->der;
			} else {
				padre->izq = nodo->izq;
			}
		} else {
			if(nodo->der) {
				padre->der = nodo->der;
			} else {
				padre->der = nodo->izq;
			}
		}
	} else {
		nodo_abb_t* act = nodo->izq;
		nodo_abb_t* ant;
		while(act->der) {
			ant = act;
			act = act->der;
		}
		char* auxclave;
		void* auxvalor;
		auxclave = nodo->clave;
		nodo->clave = act->clave;
		act->clave = auxclave;
		auxvalor = nodo->valor;
		nodo->valor = act->valor;
		act->valor = auxvalor;
		if(act->izq) {
			ant->der = act->izq;
		} else {
			ant->der = NULL;
		}
		nodo = act;
	}
	arbol->cantidad--;
	void* dato = nodo->valor;
	free(nodo->clave);
	free(nodo);
	return dato;
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
	free(iter->pila);
	free(iter);
}

//PRIMITIVA DEL ITERADOR INTERNO DEL ÁRBOL.

/* Recibe un puntero a una raíz de un árbol, una función visitar para modificar 
los subárboles ubicados por debajo de la raíz y un puntero extra para hacer con
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
