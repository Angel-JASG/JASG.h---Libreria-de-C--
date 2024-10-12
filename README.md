# JASG.h | Libreria de C++
 ¡Hola! 😊 Bienvenido a mi repositorio. Esta es una biblioteca de C++ que creé por diversión mientras aprendía a programar. La he utilizado en algunos proyectos y espero que te sirva también.
## Introducción

La biblioteca JASG es una colección de utilidades en C++ diseñada para simplificar tareas comunes, como la gestión de variables, la visualización de arte en consola y la reproducción de sonidos. Esta biblioteca es ligera y no depende de librerías externas, lo que la hace fácil de integrar en tus proyectos.

## Características

- **Gestión de Variables**: Permite almacenar y recuperar diferentes tipos de variables (enteros, cadenas y booleanos).
- **Funciones de Arte**: Facilita la visualización de texto y arte en la consola con una función simplificada para imprimir múltiples líneas.
- **Funciones de Sonido**: Proporciona funciones para emitir tonos (beeps) en sistemas Windows y un esquema para agregar sonido que se puede expandir.
- **Evaluación de Expresiones**: Permite la evaluación de expresiones matemáticas simples.

## Instalación

Para utilizar la biblioteca JASG, simplemente incluye el archivo `JASG.h` en tu proyecto:

```cpp
#include "JASG.h"
```
## Uso Gestión de Variables

```cpp
// Registro de variables
SV("miEntero", new IntVariable(10));
SV("miCadena", new StringVariable("Hola"));
SV("miBooleano", new BoolVariable(true));

// Recuperación de valores
cout << getVariableValue("miEntero") << endl; // Imprime: 10
```
## Función Art

La función Art permite imprimir múltiples líneas de texto de manera sencilla.

```cpp
Art(5,
    "-------------------------------",
    "Hola",
    "-------------------------------",
    " ", // Línea en blanco
    " "
);
```
## Evaluación de Expresiones

Puedes evaluar expresiones matemáticas de la siguiente manera:

```cpp
double resultado = eval("3 + 5 * 2 - 1");
cout << "Resultado: " << resultado << endl; // Imprime: Resultado: 12
```
## Sonidos

Para emitir un tono, puedes usar la función beepTone:

```cpp
beepTone(5); // Emite un tono de grado 5
```
## Contribuciones

Las contribuciones son bienvenidas. Si deseas mejorar la biblioteca, no dudes en hacer un fork y enviar un pull request.
Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo LICENSE para más detalles.

### Notas

- Puedes ajustar cualquier sección según sea necesario.
- Asegúrate de que el archivo `JASG.h` esté en el mismo directorio o especifica la ruta correcta si es necesario.
- Puedes incluir ejemplos más específicos o más funcionalidades dependiendo de la evolución de la biblioteca.

