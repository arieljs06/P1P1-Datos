#pragma once
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>

template<typename T, typename KeyFunc, typename Compare = std::less<std::string>>
void bucketSortAlpha(T** arr, int n, KeyFunc keyFunc, Compare comp = Compare()) {
    const int NUM_BUCKETS = 27;
    T*** buckets = new T**[NUM_BUCKETS];
    int* bucketSizes = new int[NUM_BUCKETS]();
    int* bucketCapacities = new int[NUM_BUCKETS]();

    // Inicializar buckets
    for (int i = 0; i < NUM_BUCKETS; ++i) {
        bucketCapacities[i] = 8;
        buckets[i] = new T*[bucketCapacities[i]];
    }

    // Distribuir elementos en los buckets
    for (int i = 0; i < n; ++i) {
        std::string clave = keyFunc(arr[i]);
        char inicial = !clave.empty() ? std::toupper(clave[0]) : 0;
        int idx = (inicial >= 'A' && inicial <= 'Z') ? (inicial - 'A') : 26;

        // Redimensionar si es necesario
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

    // Ordenar y reconstruir el arreglo original
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