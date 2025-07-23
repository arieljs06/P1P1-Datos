#pragma once
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

/**
 * @brief Ordena un arreglo de punteros usando Bucket Sort alfabético basado en una clave de texto.
 * 
 * Esta función aplica una estrategia de ordenamiento tipo bucket sort en base al primer carácter
 * alfabético de una clave obtenida por la función `keyFunc`. Se utiliza para ordenar estructuras 
 * como pacientes, turnos u otras entidades que tengan atributos de tipo `std::string` (como nombre, apellido, etc.).
 * 
 * @tparam T Tipo del objeto contenido en el arreglo.
 * @tparam KeyFunc Función que extrae la clave `std::string` desde un puntero a T.
 * @tparam Compare Función de comparación (por defecto es `std::less<std::string>`).
 * 
 * @param arr Arreglo de punteros a objetos que se van a ordenar.
 * @param n Cantidad de elementos en el arreglo.
 * @param keyFunc Función que devuelve la clave alfabética (por ejemplo, `[](T* obj) { return obj->getNombre(); }`).
 * @param comp (Opcional) Función de comparación personalizada para ordenar cadenas.
 * 
 * @note Los buckets están agrupados por inicial A-Z y uno adicional para caracteres no alfabéticos.
 * @warning Asegúrate de que `arr` contenga punteros válidos antes de usar esta función.
 */
template<typename T, typename KeyFunc, typename Compare = std::less<std::string>>
void bucketSortAlpha(T** arr, int n, KeyFunc keyFunc, Compare comp = Compare()) {
    const int NUM_BUCKETS = 27; // 26 letras + 1 para otros
    T*** buckets = new T**[NUM_BUCKETS];
    int* bucketSizes = new int[NUM_BUCKETS]();
    int* bucketCapacities = new int[NUM_BUCKETS]();

    // Inicializar los buckets
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        bucketCapacities[i] = 8;
        buckets[i] = new T*[bucketCapacities[i]];
    }

    // Distribuir elementos en los buckets según la letra inicial
    for (int i = 0; i < n; ++i) {
        std::string clave = keyFunc(arr[i]);
        char inicial = !clave.empty() ? std::toupper(clave[0]) : 0;
        int idx = (inicial >= 'A' && inicial <= 'Z') ? (inicial - 'A') : 26;

        // Redimensionar si el bucket está lleno
        if (bucketSizes[idx] == bucketCapacities[idx]) {
            int newCap = bucketCapacities[idx] * 2;
            T** newArr = new T*[newCap];
            for (int j = 0; j < bucketSizes[idx]; ++j)
                newArr[j] = buckets[idx][j];
            delete[] buckets[idx];
            buckets[idx] = newArr;
            bucketCapacities[idx] = newCap;
        }
        buckets[idx][bucketSizes[idx]++] = arr[i];
    }

    // Ordenar internamente cada bucket y reconstruir el arreglo original
    int idx = 0;
    for (int b = 0; b < NUM_BUCKETS; ++b) {
        std::sort(buckets[b], buckets[b] + bucketSizes[b], [&](T* a, T* b) {
            return comp(keyFunc(a), keyFunc(b));
        });
        for (int j = 0; j < bucketSizes[b]; ++j) {
            arr[idx++] = buckets[b][j];
        }
        delete[] buckets[b];
    }

    delete[] buckets;
    delete[] bucketSizes;
    delete[] bucketCapacities;
}
