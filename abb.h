#ifndef ABB_H
#define ABB_H

/* ESTTRUCTURA ARBOL BINARIO DE BUSQUEDA */

typedef struct abb abb_t;

//Tipo de función para comparar claves.
typedef int (*abb_comparar_clave_t) (const char *, const char *);

//Tipo de función para destruir dato.
typedef void (*abb_destruir_dato_t) (void *);

/* Crea el abb. 
Post: Si pudo crearlo, devuelve un puntero al abb, sino, devuelve un puntero a
NULL. */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Pre: El abb fue creado.
Post: Se almacenó el par (clave, dato). Si la clave ya se encuentra en la
estructura, reemplaza el valor con el nuevo. De no poder guardarlo devuelve false, de lo contrario, devuelve true. */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Pre: El abb fue creado.
Post: El elemento fue borrado de la estructura, se devolvió el valor,
en el caso de que estuviera guardado. Si el dato no estaba, devuelve NULL. */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Pre: El abb fue creado.
Post: Devuelve el valor de un elemento del abb, si la clave no se encuentra
devuelve NULL. */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Pre: La estructura abb fue inicializada.
Post: Si el elemento está en el abb, devuelve true, de lo contrario, false. */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Pre: La estructura hash fue inicializada
Post: devuelve la cantidad de elementos del abb. */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
destruir para cada dato en caso de existir dicha función.
Pre: El abb fue creado.
Post: La estructura abb fue destruida. */
void abb_destruir(abb_t *arbol);

/* ITERADOR INTERNO */

// Itera sobre el abb de manera in order y lo modifica mediante la función visitar 
// recibida por parámetro. Recibe un puntero extra para hacer con el lo que se 
// prefiera.
// Pre: el abb fue creado. Visitar es una función capaz de modificar los
// elementos del abb.
// Post: se modificaron todos los elementos del abb según lo que hace
// la función visitar.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

/* ITERADOR EXTERNO */

typedef struct abb_iter abb_iter_t;

// Crea iterador.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza iterador.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Comprueba si terminó la iteración.
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye iterador.
void abb_iter_in_destruir(abb_iter_t* iter);

#endif
