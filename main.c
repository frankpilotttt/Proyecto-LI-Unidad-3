// ============================================================
// PROYECTO MODULO 3: ACELERADOR DE FUNCIONES
// OPCION A: CONTADOR DE BITS (POPULATION COUNT)
//
// Comparacion de C puro vs ASM inline vs POPCNT
// Compilador: GCC / MinGW-w64
// Arquitectura: x86-64
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define N 100000000ULL

// ============================================================
// GENERADOR DE NUMEROS ALEATORIOS DE 64 BITS
// ============================================================

static uint64_t estado = 123456789ULL;

uint64_t generar_numero(void) {

    estado ^= estado << 13;
    estado ^= estado >> 7;
    estado ^= estado << 17;

    return estado;
}

// ============================================================
// VERSION 1: C 
// Cuenta los bits activados mediante un bucle.
// ============================================================

int contar_bits_c(unsigned long long n) {

    int count = 0;

    while (n) {
        count += n & 1;
        n >>= 1;
    }

    return count;
}

// ============================================================
// VERSION 2: ASM INLINE TRADICIONAL
// Instrucciones: TEST, MOV, AND, ADD, SHR y JNZ.
// ============================================================

int contar_bits_asm(unsigned long long n) {

    unsigned int count;
    unsigned long long value = n;

    asm volatile (

        "xorl %[count], %[count]       \n\t"

        "testq %[value], %[value]      \n\t"
        "jz 2f                         \n\t"

        "1:                            \n\t"
        "movq %[value], %%rax          \n\t"
        "andq $1, %%rax                \n\t"
        "addl %%eax, %[count]          \n\t"
        "shrq $1, %[value]             \n\t"
        "jnz 1b                        \n\t"

        "2:                            \n\t"

        : [count] "=&r" (count),
          [value] "+&r" (value)

        :

        : "rax", "cc"
    );

    return (int)count;
}

// ============================================================
// VERSION 3: ASM INLINE OPTIMIZADO CON POPCNT
// Cuenta los bits en 1 mediante una instruccion especializada.
// ============================================================

int contar_bits_popcnt(unsigned long long n) {

    unsigned long long result;

    asm volatile (
        "popcntq %1, %0"

        : "=r" (result)

        : "r" (n)

        : "cc"
    );

    return (int)result;
}

// ============================================================
// FUNCION PARA MOSTRAR LA MEJORA DE VELOCIDAD
// ============================================================

void mostrar_mejora(double tiempo_c, double tiempo_asm) {

    if (tiempo_asm > 0.0) {

        printf("Mejora: %.2fx\n",
               tiempo_c / tiempo_asm);

    } else {

        printf("Tiempo demasiado pequeno para comparar.\n");
    }
}

// ============================================================
// FUNCION PRINCIPAL
// ============================================================

int main(void) {

    printf("============================================\n");
    printf("       PROYECTO 3 - CONTADOR DE BITS\n");
    printf("============================================\n");

    printf("Cantidad de numeros: %llu\n", N);

    // Reservar memoria para los numeros.
    unsigned long long *arr =
        malloc((size_t)N * sizeof(unsigned long long));

    if (arr == NULL) {

        printf("Error al reservar memoria.\n");
        printf("Se necesitan aproximadamente 800 MB.\n");

        return 1;
    }

    // Generar los numeros de prueba.
    printf("\nGenerando numeros...\n");

    for (size_t i = 0; i < (size_t)N; i++) {
        arr[i] = generar_numero();
    }

    printf("Generacion terminada.\n");

    // Comprobar soporte del procesador.
    __builtin_cpu_init();

    int tiene_popcnt =
        __builtin_cpu_supports("popcnt");

    // ========================================================
    // MEDICION DE C PURO
    // ========================================================

    long long total_c = 0;

    clock_t inicio = clock();

    for (size_t i = 0; i < (size_t)N; i++) {
        total_c += contar_bits_c(arr[i]);
    }

    clock_t fin = clock();

    double tiempo_c =
        (double)(fin - inicio) / CLOCKS_PER_SEC;

    // ========================================================
    // MEDICION DE ASM TRADICIONAL
    // ========================================================

    long long total_asm = 0;

    inicio = clock();

    for (size_t i = 0; i < (size_t)N; i++) {
        total_asm += contar_bits_asm(arr[i]);
    }

    fin = clock();

    double tiempo_asm =
        (double)(fin - inicio) / CLOCKS_PER_SEC;

    // ========================================================
    // MEDICION DE POPCNT
    // ========================================================

    long long total_popcnt = 0;
    double tiempo_popcnt = 0.0;

    if (tiene_popcnt) {

        inicio = clock();

        for (size_t i = 0; i < (size_t)N; i++) {
            total_popcnt += contar_bits_popcnt(arr[i]);
        }

        fin = clock();

        tiempo_popcnt =
            (double)(fin - inicio) / CLOCKS_PER_SEC;
    }

    // ========================================================
    // PRESENTACION DE RESULTADOS
    // ========================================================

    printf("\n============================================\n");
    printf("             RESULTADOS\n");
    printf("============================================\n");

    printf("\nTotal en C:       %lld\n", total_c);
    printf("Total en ASM:     %lld\n", total_asm);

    if (tiene_popcnt) {
        printf("Total en POPCNT:  %lld\n", total_popcnt);
    }

    // Verificar que los resultados coincidan.
    printf("\n--------------------------------------------\n");

    if (total_c == total_asm &&
        (!tiene_popcnt || total_c == total_popcnt)) {

        printf("CORRECTO: Todos los resultados coinciden.\n");

    } else {

        printf("ERROR: Los resultados son diferentes.\n");

    }

    // Mostrar tiempos.
    printf("\n--------------------------------------------\n");

    printf("Tiempo en C:      %.4f segundos\n", tiempo_c);
    printf("Tiempo en ASM:    %.4f segundos\n", tiempo_asm);

    printf("\nMejora ASM: ");
    mostrar_mejora(tiempo_c, tiempo_asm);

    if (tiene_popcnt) {

        printf("\nTiempo POPCNT:    %.4f segundos\n",
               tiempo_popcnt);

        printf("Mejora POPCNT: ");

        mostrar_mejora(tiempo_c, tiempo_popcnt);

    } else {

        printf("\nEl procesador no soporta POPCNT.\n");
    }

    printf("\n============================================\n");

    // Liberar memoria.
    free(arr);

    return 0;
}