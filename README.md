**Sopa de letras**

Proyecto de Sopa de Letras hecho en C usando Raylib.\
Hecho en **3 días** (Aproximadamente 24 horas de trabajo).

**Funcionalidades:**

Palabras en **6 direcciones**: Horizontal, Vertical, Diagonal, Horizontal Invertido, Vertical Invertido y Diagonal Invertido.\
Aleatoriedad en cada jugada.\
Proceso altamente eficiente: ~1% en uso de CPU y ~30MB en uso de RAM.\
Detector de colisiones para evitar clicks incorrectos en cada dirección.\
Se muestra a tiempo real los carácteres clickeados.\
Detector exacto de palabras descubiertas.\
Las palabras no pueden repetirse.\
Opción para volver a jugar.

**Cómo compilar:**

gcc main.c resource.o -o main -lraylib -lgdi32 -lwinmm -mwindows
