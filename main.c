#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define PALABRAS_MAXIMAS 200 // Palabras maximas por usar
#define LIMITE_LETRAS 8      // Letras maximas por palabra
#define FILAS 8              // Numero de filas y columnas
#define COLUMNAS 8

bool cubosRojos[FILAS][COLUMNAS] = {{false}}; // Generar cubos rojos y verdes en false
bool cubosVerdes[FILAS][COLUMNAS] = {{false}};
bool marcandoDiagonalAbajoDerecha = false; // Direcciones
bool marcandoDiagonalArribaDerecha = false;
bool marcandoDiagonalAbajoIzquierda = false;
bool marcandoDiagonalArribaIzquierda = false;
bool marcandoHorizontal = false;
bool marcandoVertical = false;
bool palabrasListas[PALABRAS_MAXIMAS] = {false}; // Palabras descubiertas
bool win = false;                                // Si se ha ganado

int totalCubosRojos = 0; // Contadores
int totalPalabrasListas = 0;
int cantidadPalabras = 0;
int cantidadPalabrasEnUso = 0;
int ultimoI = -1, ultimoJ = -1;           // Coordenadas último cubo clickeado
int espacioDisponible = FILAS * COLUMNAS; // Calcular espacio
int direction, startX, startY, longitudPalabra; // Iniciar variables

char palabraRandom[9];  // Palabra aleatoria
char caracteresClickeados[LIMITE_LETRAS + 1][2];         // Letras clickeadas
char palabrasEnUso[PALABRAS_MAXIMAS][LIMITE_LETRAS + 1]; // Palabras en uso
char letras[FILAS][COLUMNAS];                            // Letras en el tablero
char palabras[PALABRAS_MAXIMAS][LIMITE_LETRAS + 1] = {   // Lista de palabras posibles por descubrir
    "ANIMAL", "PERRO", "GATO", "CASA", "ARBOL", "TIERRA", "FLORES", "BOSQUE", "RIO", "LAGO", "NUBE", "VIENTO", "LLUVIA", "SOL", "LUNA", "ESTRELLA", "FUEGO", "NIEVE", "MAR", "PLAYA", "CAMPO", "SENDERO", "ROCA", "ARENA", "RAYO", "TRUENO", "CUEVA", "SELVA", "VERDE", "AZUL", "ROJO", "BLANCO", "NEGRO", "AMARILLO", "BRISA", "CIELO", "RAMAS", "HOJA", "FRUTA", "CACTUS", "SAUCE", "CEDRO", "MANGO", "PINO", "ROCIO", "OCEANO", "FARO", "BARCO", "ISLAS", "COSTA", "TIBURON", "CORAL", "DELFIN", "PULPO", "TORTUGA", "PECES", "ALAS", "NIDO", "AGUILA", "COLINA", "ACIERTO", "CIERRE", "LUCIDO", "SEGURO", "RUMOR", "ANHELO", "CIEGA", "ESTUDIO", "MATRIZ", "CONTRA", "VICTORIA", "MUNDO", "FLORECER", "INICIO", "AFICION", "FUERZA", "MAYOR", "CRECER", "COMANDO", "TERCER", "PUERTO", "LENGUA", "RECREO", "ESTILO", "LIDER", "PALABRA", "CULTIVO", "PIEDRA", "GOLPE", "TUMBA", "SENAL", "REGLA", "DESAFIO", "BUSCAR", "CONJUNTO", "RETO", "UNICO", "ENTRAR", "NAVEGAR", "ORIGEN", "ALUMBRAR", "RAPIDO", "TIBIO", "RUGIR", "DESIERTO", "IMPACTO", "RASTRO", "VIGORO", "MUESTRA", "CARTERA", "REMOLQUE", "GUITAR", "SINFONIA", "CABAL", "SITUAR", "DIVISA", "ABORDO", "CRISIS", "VERAZ", "CUPIDO", "RUTA", "FOLIO", "BANCAR", "DURAZNO", "ALBINO", "EQUIPO", "TRAER", "DEFENSOR", "CASUAL", "BELLO", "DIVERSO", "CARTEL", "ALUMNO", "COBARDES", "FELIZ", "FIORDO", "LUCHA", "UNIVERSO", "FINO", "SOLO", "GRITA", "FLECHERO", "RUDA", "CAMINAR", "NACION", "ABRI", "VENTA", "DISCO", "CARBON", "ABALORIO", "CABALLO", "FANATICO", "PLANTON", "COCINERO", "ALUMINIO", "CANAL", "PUNTO", "CAMINO", "FIERRO", "SOCIAL", "IMPULSO", "CERRADO", "AMISTAD", "LIGADO", "AGONIA", "CORREDOR", "CARTON", "ESCOLAR", "JUDIO", "JUDIA"};

void desmarcarTodo()
{
    for (int i = 0; i < FILAS; i++) // Limpiar cubos rojos de toda la tabla
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            cubosRojos[i][j] = false;
        }
    }
    marcandoDiagonalAbajoIzquierda = false; // Resetear direcciones
    marcandoDiagonalArribaIzquierda = false;
    marcandoDiagonalAbajoDerecha = false;
    marcandoDiagonalArribaDerecha = false;
    marcandoHorizontal = false;
    marcandoVertical = false;
    totalCubosRojos = 0; // Reiniciar contadores y coordenadas de último click
    ultimoI = -1;
    ultimoJ = -1;
}

int contarPalabras() // Contar palabras en la lista
{
    for (int i = 0; i < PALABRAS_MAXIMAS; i++)
    {
        if (palabras[i][0] == '\0')
            break;
        cantidadPalabras++;
    }
    return cantidadPalabras;
}

int contarPalabrasEnUso() // Contar palabras en uso que no están vacías
{
    for (int i = 0; i < PALABRAS_MAXIMAS; i++)
    {
        if (palabrasEnUso[i][0] == '\0')
            break;
        cantidadPalabrasEnUso++;
    }
    return cantidadPalabrasEnUso;
}

int comprobarPalabraRepetida(const char *palabra) // Comprobar palabras repetidas
{
    if(cantidadPalabrasEnUso == 0) return 0;
    for (int i = 0; i < cantidadPalabrasEnUso; i++)
    {
        if (strcmp(palabra, palabrasEnUso[i]) == 0) // Si la palabra está repetida
        {
            return 1;
        }
    }
    return 0; // Si la palabra no está repetida
}

void generarTabla()
{
    if (cantidadPalabras == 0)
        contarPalabras();      // Solo contar una vez
    srand(time(NULL) ^ rand());         // Seed aleatoria
    desmarcarTodo();    // Desmarcar todo en caso de volver a jugar
    cantidadPalabrasEnUso = 0; // Reiniciar valores y vaciar contadores
    totalPalabrasListas = 0;
    totalCubosRojos = 0;
    espacioDisponible = FILAS * COLUMNAS;
    direction = 0;
    startX = 0;
    startY = 0;
    longitudPalabra = 0;

    win = false;            // No se ha ganado
    bool hayEspacio = true; // Asumir que hay espacio

    memset(cubosRojos, 0, sizeof(cubosRojos)); // Llenar cubos rojos, verdes y palabras listas con ceros
    memset(cubosVerdes, 0, sizeof(cubosVerdes));
    memset(palabrasListas, 0, sizeof(palabrasListas));

    for (int i = 0; i < PALABRAS_MAXIMAS; i++)
    {
        for (int j = 0; j < LIMITE_LETRAS; j++)
        {
            palabrasEnUso[i][j] = '\0'; // Vaciar todos los carácteres de todas las palabras en uso
            memset(caracteresClickeados[j], '\0', 2); // Vaciar carácteres clickeados
            memset(palabraRandom, '\0', LIMITE_LETRAS + 1); // Vaciar palabra aleatoria
        }
    }
    
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            letras[i][j] = ' '; // Iniciar tabla con caracteres en blanco
        }
    }

    while (espacioDisponible > 20) // Mientras haya más de 20 espacios
    {
        direction = (rand() % 6);                     // Valor aleatorio entre 0 y 5 para las direcciones
        do
        {
            snprintf(palabraRandom, LIMITE_LETRAS + 1, "%s", palabras[rand() % cantidadPalabras]); // Asignar palabra aleatoria a palabraRandom
        } while (comprobarPalabraRepetida(palabraRandom)); // Repite hasta encontrar una palabra no usada
        longitudPalabra = (int)strlen(palabraRandom); // Calcular longitud palabra
        switch (direction)                            // Por cada dirección
        {
        case 0: // Horizontal
            startX = rand() % FILAS;
            startY = rand() % (COLUMNAS - longitudPalabra + 1);
            hayEspacio = true;
            for (int i = 0; i < longitudPalabra; i++)
            {
                if (startY + i >= COLUMNAS) // Si el valor aleatorio más la iteración se excede
                {
                    hayEspacio = false;
                    break;
                }

                if (letras[startX][startY + i] != ' ' && letras[startX][startY + i] != palabraRandom[i]) // Si colisiona con otra letra
                {
                    hayEspacio = false;
                    break;
                }
            }

            if (hayEspacio)
            {
                for (int i = 0; i < longitudPalabra; i++) // Asignar letras de la palabra random en las coordenadas
                {
                    letras[startX][startY + i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[cantidadPalabrasEnUso], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                cantidadPalabrasEnUso++; // Aumentar contador palabras en uso
                espacioDisponible -= longitudPalabra; // Restar espacio disponible con la longitud de la palabra random
            }
            break;
        case 1: // Vertical
            startX = rand() % (FILAS - longitudPalabra + 1);
            startY = rand() % COLUMNAS;
            hayEspacio = true;
            for (int i = 0; i < longitudPalabra; i++)
            {
                if (startY + i >= FILAS) // Si el valor aleatorio más la iteración se excede
                {
                    hayEspacio = false;
                    break;
                }

                if (letras[startX][startY + i] != ' ' && letras[startX][startY + i] != palabraRandom[i]) // Si colisiona con otra letra
                {
                    hayEspacio = false;
                    break;
                }
            }

            if (hayEspacio)
            {
                for (int i = 0; i < longitudPalabra; i++) // Asignar letras de la palabra random en las coordenadas
                {
                    letras[startX][startY + i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[cantidadPalabrasEnUso], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                cantidadPalabrasEnUso++; // Aumentar contador palabras en uso
                espacioDisponible -= longitudPalabra; // Restar espacio disponible con la longitud de la palabra random
            }
            break;
        case 2: // Diagonal
            startX = rand() % (FILAS - longitudPalabra + 1);
            startY = rand() % (COLUMNAS - longitudPalabra + 1);

            hayEspacio = true;
            for (int i = 0; i < longitudPalabra; i++)
            {
                if (startX + i >= FILAS && startY + i >= COLUMNAS) // Si el valor aleatorio más la iteración se excede
                {
                    hayEspacio = false;
                    break;
                }

                if (letras[startX + i][startY + i] != ' ' && letras[startX + i][startY + i] != palabraRandom[i]) // Si colisiona con otra letra
                {
                    hayEspacio = false;
                    break;
                }
            }

            if (hayEspacio)
            {
                for (int i = 0; i < longitudPalabra; i++) // Asignar letras de la palabra random en las coordenadas
                {
                    letras[startX + i][startY + i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[cantidadPalabrasEnUso], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                cantidadPalabrasEnUso++; // Aumentar contador palabras en uso
                espacioDisponible -= longitudPalabra; // Restar espacio disponible con la longitud de la palabra random
            }
            break;
        case 3: // Horizontal Invertido
            startX = rand() % FILAS;
            startY = rand() % (COLUMNAS - longitudPalabra + 1) + (longitudPalabra - 1);
            hayEspacio = true;

            for (int i = 0; i < longitudPalabra; i++)
            {
                if (startY - i < 0) // Si el valor aleatorio más la iteración se excede (negativo)
                {
                    hayEspacio = false;
                    break;
                }

                if (letras[startX][startY - i] != ' ' && letras[startX][startY - i] != palabraRandom[i]) // Si colisiona con otra letra
                {
                    hayEspacio = false;
                    break;
                }
            }

            if (hayEspacio)
            {
                // Insertar la palabra de derecha a izquierda
                for (int i = 0; i < longitudPalabra; i++) // Asignar letras de la palabra random en las coordenadas
                {
                    letras[startX][startY - i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[cantidadPalabrasEnUso], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                cantidadPalabrasEnUso++; // Aumentar contador palabras en uso
                espacioDisponible -= longitudPalabra; // Restar espacio disponible con la longitud de la palabra random
            }
            break;
        case 4: // Vertical Invertido
            startX = rand() % (FILAS - longitudPalabra + 1) + (longitudPalabra - 1);
            startY = rand() % COLUMNAS;
            hayEspacio = true;

            for (int i = 0; i < longitudPalabra; i++)
            {
                if (startX - i < 0) // Si el valor aleatorio más la iteración se excede (negativo)
                {
                    hayEspacio = false;
                    break;
                }

                if (letras[startX - i][startY] != ' ' && letras[startX - i][startY] != palabraRandom[i]) // Si colisiona con otra letra
                {
                    hayEspacio = false;
                    break;
                }
            }

            if (hayEspacio)
            {
                for (int i = 0; i < longitudPalabra; i++) // Asignar letras de la palabra random en las coordenadas
                {
                    letras[startX - i][startY] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[cantidadPalabrasEnUso], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                cantidadPalabrasEnUso++; // Aumentar contador palabras en uso
                espacioDisponible -= longitudPalabra; // Restar espacio disponible con la longitud de la palabra random
            }
            break;
        case 5: // Diagonal Invertido
            startX = rand() % (FILAS - longitudPalabra + 1) + (longitudPalabra - 1);
            startY = rand() % (COLUMNAS - longitudPalabra + 1) + (longitudPalabra - 1);
            hayEspacio = true;

            for (int i = 0; i < longitudPalabra; i++)
            {
                if (startY - i < 0) // Si el valor aleatorio más la iteración se excede (negativo)
                {
                    hayEspacio = false;
                    break;
                }

                if (letras[startX - i][startY - i] != ' ' && letras[startX - i][startY - i] != palabraRandom[i]) // Si colisiona con otra letra
                {
                    hayEspacio = false;
                    break;
                }
            }

            if (hayEspacio)
            {
                for (int i = 0; i < longitudPalabra; i++) // Asignar letras de la palabra random en las coordenadas
                {
                    letras[startX - i][startY - i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[cantidadPalabrasEnUso], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                cantidadPalabrasEnUso++; // Aumentar contador palabras en uso
                espacioDisponible -= longitudPalabra; // Restar espacio disponible con la longitud de la palabra random
            }
            break;
        }
    }

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (letras[i][j] == ' ')
            {
                letras[i][j] = (char)GetRandomValue('A', 'Z'); // Rellenar con letras aleatorias
            }
        }
    }
}

int main()
{
    const int screenHeight = 600; // Alto y ancho de pantalla
    const int screenWidth = 600;
    const int cubeSizeX = screenWidth / 12; // Tamaño de los cubos
    const int cubeSizeY = screenHeight / 12;
    const int paddingLeft = cubeSizeX * 2; // Margen izquierdo
    const int spacing = screenWidth / 500; // Espaciado entre cubos

    void desmarcarTodo(); // Definir funciones
    void verificarVecinos(int i, int j);
    bool comprobarPalabras();


    InitWindow(screenWidth, screenHeight, "Sopa de letras"); // Iniciar ventana
    SetTargetFPS(60);                                        // Fijar a 60 fps

    generarTabla(); // Generar tabla
    while (!WindowShouldClose()) // Mientras esté en ejecución
    {
        BeginDrawing();            // Dibujar
        ClearBackground(RAYWHITE); // Limpiar fondo

        Vector2 mousePosition = GetMousePosition(); // Conseguir posición del mouse

        for (int i = 0; i < FILAS; i++) // Por cada fila
        {
            for (int j = 0; j < COLUMNAS; j++) // Por cada columna
            {
                int cubeX = paddingLeft + (cubeSizeX + spacing) * j; // Calcular coordenadas de los cubos
                int cubeY = 10 + (cubeSizeY + spacing) * i;

                char letrastr[2] = {letras[i][j], '\0'}; // Terminar letra con carácter nulo
                int fontSize = 20;
                int textWidth = MeasureText(letrastr, fontSize); // Calcular posición de las letras
                int textX = cubeX + (cubeSizeX - textWidth) / 2;
                int textY = cubeY + (cubeSizeY - fontSize) / 2;

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) // Al clickear
                {
                    if (mousePosition.x >= cubeX && mousePosition.x <= cubeX + cubeSizeX && // Conseguir coordenadas de mouse
                        mousePosition.y >= cubeY && mousePosition.y <= cubeY + cubeSizeY)
                    {
                        if (!cubosRojos[i][j]) // Si el cubo no está seleccionado
                        {
                            cubosRojos[i][j] = true; // Seleccionar cubo
                            totalCubosRojos++;
                            strcpy(caracteresClickeados[totalCubosRojos - 1], (char[]){letras[i][j], '\0'}); // Almacenar la letra clickeada
                            ultimoI = i; // Guardar coordenadas del último click
                            ultimoJ = j;
                            if (totalCubosRojos >= 2)
                            {
                                verificarVecinos(ultimoI, ultimoJ); // Verificar colisiones en torno a la dirección
                            }
                        }
                        for (int i = 0; i < cantidadPalabrasEnUso; i++) // Por cada palabra en uso
                        {
                            if (comprobarPalabras(palabrasEnUso[i])) // Comprobar si lo seleccionado es una palabra
                            {
                                palabrasListas[i] = true; // Marcar como palabra lista
                                for (int h = 0; h < FILAS; h++)
                                {
                                    for (int j = 0; j < COLUMNAS; j++)
                                    {
                                        if (cubosRojos[h][j]) // Convertir cubos rojos en verdes
                                        {
                                            cubosVerdes[h][j] = true;
                                        }
                                    }
                                    memset(caracteresClickeados[h], '\0', 2); // Vaciar caracteres clickeados
                                }
                                desmarcarTodo();
                            }
                        }
                    }
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) // Si se presiona el click derecho se remueve el marcado
                {
                    if (mousePosition.x >= cubeX && mousePosition.x <= cubeX + cubeSizeX &&
                        mousePosition.y >= cubeY && mousePosition.y <= cubeY + cubeSizeY)
                    {
                        if (cubosRojos[i][j] && letras[i][j] == caracteresClickeados[totalCubosRojos - 1][0])
                        {
                            cubosRojos[i][j] = false;
                            totalCubosRojos--;
                            ultimoI = i;
                            ultimoJ = j;
                        }
                    }
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) // Botón del medio para desmarcar
                {
                    if (!win) // Si no se ha ganado desmarcar
                    {
                        desmarcarTodo();
                    }
                    else // Si no reiniciar todo
                    {
                        desmarcarTodo();
                        generarTabla();
                    }
                }
                if (IsKeyPressed(KEY_R)) // R para reiniciar
                {
                    desmarcarTodo();
                    generarTabla();
                }
                if (totalCubosRojos <= 0) // Si no hay cubos rojos
                {
                    ultimoI = -1; // Reiniciar coordenadas de último cubo
                    ultimoJ = -1;
                }
                if (totalCubosRojos > LIMITE_LETRAS) // Si se marca más de lo posible
                {
                    desmarcarTodo();
                }

                if (cubosRojos[i][j]) // Si hay un cubo rojo en la coordenada
                {
                    DrawRectangle(cubeX, cubeY, cubeSizeX, cubeSizeY, RED); // Dibujar cubo rojo
                }
                else if (cubosVerdes[i][j]) // Si hay un cubo verde en la coordenada
                {
                    DrawRectangle(cubeX, cubeY, cubeSizeX, cubeSizeY, GREEN); // Dibujar cubo verde
                }
                else // Si no
                {
                    DrawRectangle(cubeX, cubeY, cubeSizeX, cubeSizeY, GRAY); // Dibujar cubo gris
                }
                DrawText(letrastr, textX, textY, fontSize, WHITE); // Dibujar letra del cubo
            }
        }
        for (int i = 0; i < cantidadPalabrasEnUso; i++)
        {
            int yPos = screenHeight - screenHeight / 4 + (i / 5) * 30; // Aumentar Y cada 5 palabras
            Color colorPalabra = palabrasListas[i] ? GREEN : BLACK;
            DrawText(palabrasEnUso[i], screenWidth / 25 + (i % 5) * 120, yPos, 20, colorPalabra);
        }
        for (int i = 0; i < totalCubosRojos; i++)
        {
            DrawText(caracteresClickeados[i], screenWidth - screenWidth / 12, screenHeight - screenHeight / 1.05 + 40 * i, 20, BLACK); // Dibujar letra de cubos clickeados
        }
        if (win) // Al ganar
        {
            int padding = MeasureText("¡GANASTE!", 40); // Calcular longitud del texto
            DrawText("¡GANASTE!", screenWidth / 2 - padding / 2, screenHeight / 1.4 - padding, 40, BLACK);
        }
        DrawText("R\nPARA\nREINICIAR\n\nRUEDA DEL\nMOUSE\nPARA\nDESMARCAR", 5, 10, 15, BLACK); // Mostrar información de los botones
        EndDrawing(); // Terminar de dibujar
    }

    CloseWindow(); // Cerrar ventana

    return 0; // Cerrar sin errores
}

void verificarVecinos(int i, int j) // Comprobar colisiones según click anterior
{
    // Abajo Derecha
    if (i + 1 < FILAS && j + 1 < COLUMNAS && cubosRojos[i + 1][j + 1] && !marcandoHorizontal && !marcandoVertical && !marcandoDiagonalAbajoIzquierda && !marcandoDiagonalArribaDerecha && !marcandoDiagonalArribaIzquierda)
    {
        marcandoDiagonalAbajoDerecha = true;
        return;
    }

    // Abajo Izquierda
    if (i + 1 < FILAS && j - 1 >= 0 && cubosRojos[i + 1][j - 1] && !marcandoHorizontal && !marcandoVertical && !marcandoDiagonalAbajoDerecha && !marcandoDiagonalArribaDerecha && !marcandoDiagonalArribaIzquierda)
    {
        marcandoDiagonalAbajoIzquierda = true;
        return;
    }

    // Arriba Derecha
    if (i - 1 >= 0 && j + 1 < COLUMNAS && cubosRojos[i - 1][j + 1] && !marcandoHorizontal && !marcandoVertical && !marcandoDiagonalAbajoIzquierda && !marcandoDiagonalAbajoDerecha && !marcandoDiagonalArribaIzquierda)
    {
        marcandoDiagonalArribaDerecha = true;
        return;
    }

    // Arriba Izquierda
    if (i - 1 >= 0 && j - 1 >= 0 && cubosRojos[i - 1][j - 1] && !marcandoHorizontal && !marcandoVertical && !marcandoDiagonalAbajoIzquierda && !marcandoDiagonalArribaDerecha && !marcandoDiagonalAbajoDerecha)
    {
        marcandoDiagonalArribaIzquierda = true;
        return;
    }

    // Abajo
    if (i + 1 < FILAS && cubosRojos[i + 1][j] && !marcandoHorizontal && !marcandoDiagonalAbajoIzquierda && !marcandoDiagonalAbajoDerecha && !marcandoDiagonalArribaDerecha && !marcandoDiagonalArribaIzquierda)
    {
        marcandoVertical = true;
        return;
    }

    // Arriba
    if (i - 1 >= 0 && cubosRojos[i - 1][j] && !marcandoHorizontal && !marcandoDiagonalAbajoIzquierda && !marcandoDiagonalAbajoDerecha && !marcandoDiagonalArribaDerecha && !marcandoDiagonalArribaIzquierda)
    {
        marcandoVertical = true;
        return;
    }

    // Derecha
    if (j + 1 < COLUMNAS && cubosRojos[i][j + 1] && !marcandoVertical && !marcandoDiagonalAbajoIzquierda && !marcandoDiagonalAbajoDerecha && !marcandoDiagonalArribaDerecha && !marcandoDiagonalArribaIzquierda)
    {
        marcandoHorizontal = true;
        return;
    }

    // Izquierda
    if (j - 1 >= 0 && cubosRojos[i][j - 1] && !marcandoVertical && !marcandoDiagonalAbajoIzquierda && !marcandoDiagonalAbajoDerecha && !marcandoDiagonalArribaDerecha && !marcandoDiagonalArribaIzquierda)
    {
        marcandoHorizontal = true;
        return;
    }

    desmarcarTodo(); // Si no es una posición valida, desmarcar todo
}

bool comprobarPalabras(char *palabrasUsada) // Conseguir palabra no usada
{
    char palabra[LIMITE_LETRAS + 1]; // Variable para usar en comparación con los caracteres clickeados

    int i;
    for (i = 0; i < LIMITE_LETRAS && i < totalCubosRojos; i++) // Por cada cubo rojo
    {
        palabra[i] = caracteresClickeados[i][0]; // Asignar letras clickeadas en orden a la palabra
    }

    palabra[i] = '\0'; // Terminar palabra en carácter nulo

    if (palabrasUsada != NULL) // Si hay palabra usada
    {
        if (palabrasUsada[0] != palabra[0]) // Si el primer carácter no coincide (evita errores en el cálculo)
            return false;
        if (strstr(palabra, palabrasUsada) != NULL) // Si las letras clickeadas coinciden con una palabra
        {
            totalPalabrasListas++; // Aumentar palabras listas
            if (totalPalabrasListas == cantidadPalabrasEnUso) // Se gana si el total de palabras verdes es igual a las del tablero
            {
                win = true;
            }
            return true; // La palabra era correcta
        }
    }
    return false; // La palabra era incorrecta
}
