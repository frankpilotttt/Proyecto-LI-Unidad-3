


# Proyecto 3 - Contador de Bits

**Opción A: Comparación entre C y ASM inline**

## ¿De qué trata el proyecto?

En este proyecto hice un programa en C que sirve para contar cuántos bits están encendidos (en 1) dentro de números de 64 bits.

La idea es hacer la misma operación de tres maneras diferentes y comparar cuánto tarda cada una:

- **C puro:** utiliza un ciclo `while` para revisar los bits uno por uno.
- **ASM inline:** realiza el mismo procedimiento, pero utilizando instrucciones de ensamblador.
- **POPCNT:** utiliza una instrucción del procesador que permite contar directamente los bits activados.

Con esto podemos observar las diferencias de rendimiento entre las implementaciones.

## Herramientas utilizadas

- Windows 10/11.
- Visual Studio Code.
- Lenguaje C.
- GCC (MinGW-w64).
- Ensamblador inline x86-64.

## ¿Cómo funciona?

Primero, el programa genera 100 millones de números de 64 bits que se utilizan para realizar las pruebas.

Después, esos mismos números pasan por las tres implementaciones para contar sus bits.

Al terminar, el programa muestra el tiempo que tardó cada una y compara los resultados para comprobar que todas hayan realizado correctamente la misma operación.

También calcula cuántas veces más rápida fue una implementación respecto a la versión en C.

## Compilación y ejecución

Para compilar el programa desde la terminal de PowerShell de Visual Studio Code, utilizo el siguiente comando:

```powershell
& "C:\mingw64\bin\gcc.exe" -O0 -Wall -Wextra .\main.c -o .\main.exe 2>&1
```

Tuve que utilizar este comando por que con 
```
gcc -O0 -Wall -Wextra -std=gnu11 main.c -o main.exe
```
No pude Compilarlo.

## Instrucciones de ensamblador utilizadas

En la versión de ASM tradicional se utilizan las siguientes instrucciones:

| Instrucción | Para qué se utiliza |
|---|---|
| TEST | Comprueba si el número es cero. |
| AND | Obtiene el último bit del número. |
| ADD | Suma los bits encontrados. |
| SHR | Desplaza los bits hacia la derecha. |
| JNZ | Repite el ciclo mientras el resultado no sea cero. |

En la tercera versión se utiliza `POPCNT`, que cuenta directamente los bits activados.

## Resultados de las pruebas

Los tiempos se agregan después de ejecutar el programa en mi computadora.

| Implementación | Tiempo de ejecución |
|---|---|
| C puro | 4.3820 segundos |
| ASM inline | 2.8890 segundos |
| ASM con POPCNT | 0.1760 segundos |

**Comprobación:** Las tres implementaciones deben obtener el mismo total de bits activados.

## Capturas de pantalla

En esta sección se agregarán algunas capturas del desarrollo y funcionamiento del programa:

- Compilación desde Visual Studio Code.
- Código de las implementaciones en C y ASM.
- Ejecución del programa.
- Resultados y comparación de tiempos.

## Conclusión

Con este proyecto pude trabajar con ensamblador inline y conocer un poco más sobre cómo se ejecutan las instrucciones del procesador.

También pude comparar diferentes formas de realizar una misma operación y observar las diferencias en sus tiempos de ejecución.

Lo interesante fue utilizar POPCNT, ya que permite realizar el conteo de bits mediante una instrucción especializada del procesador, en lugar de recorrerlos uno por uno.




