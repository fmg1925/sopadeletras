#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#define PALABRAS_MAXIMAS 169 // Palabras maximas por usar
#define LIMITE_LETRAS 8     // Letras maximas por palabra
#define FILAS 8             // Numero de filas y columnas
#define COLUMNAS 8

bool cubosRojos[FILAS][COLUMNAS] = {{false}}; // Generar en false
bool cubosVerdes[FILAS][COLUMNAS] = {{false}};
bool marcandoDiagonalAbajoDerecha = false; // Direcciones
bool marcandoDiagonalArribaDerecha = false;
bool marcandoDiagonalAbajoIzquierda = false;
bool marcandoDiagonalArribaIzquierda = false;
bool marcandoHorizontal = false;
bool marcandoVertical = false;
int totalCubosRojos = 0; // Contadores
int contador = 0;
int totalPalabrasListas = 0;
char caracteresClickeados[LIMITE_LETRAS + 1][2];     // Letras clickeadas
char palabrasEnUso[PALABRAS_MAXIMAS][LIMITE_LETRAS]; // Palabras en uso
bool palabrasListas[PALABRAS_MAXIMAS] = {false};     // Palabras descubiertas
bool win = false;                                    // Si se ha ganado
char letras[FILAS][COLUMNAS];
char palabras[PALABRAS_MAXIMAS][LIMITE_LETRAS] = { // Lista de palabras posibles por descubrir
"ANIMAL", "PERRO", "GATO", "CASA", "ARBOL", "TIERRA", "FLORES", "BOSQUE", "RIO", "LAGO", "NUBE", "VIENTO", "LLUVIA", "SOL", "LUNA", "ESTRELLA", "FUEGO", "NIEVE", "MAR", "PLAYA", "CAMPO", "SENDERO", "ROCA", "ARENA", "RAYO", "TRUENO", "CUEVA", "SELVA", "VERDE", "AZUL", "ROJO", "BLANCO", "NEGRO", "AMARILLO", "BRISA", "CIELO", "RAMAS", "HOJA", "FRUTA", "CACTUS", "SAUCE", "CEDRO", "MANGO", "PINO", "ROCIO", "OCEANO", "FARO", "BARCO", "ISLAS", "COSTA", "TIBURON", "CORAL", "DELFIN", "PULPO", "TORTUGA", "PECES", "ALAS", "NIDO", "AGUILA", "COLINA", "ACIERTO", "CIERRE", "LUCIDO", "SEGURO", "RUMOR", "ANHELO", "CIEGA", "ESTUDIO", "MATRIZ", "CONTRA", "VICTORIA", "MUNDO", "FLORECER", "INICIO", "AFICION", "FUERZA", "MAYOR", "CRECER", "COMANDO", "TERCER", "PUERTO", "LENGUA", "RECREO", "ESTILO", "LIDER", "PALABRA", "CULTIVO", "PIEDRA", "GOLPE", "TUMBA", "SENAL", "REGLA", "DESAFIO", "BUSCAR", "CONJUNTO", "RETO", "UNICO", "ENTRAR", "NAVEGAR", "ORIGEN", "ALUMBRAR", "RAPIDO", "TIBIO", "RUGIR", "DESIERTO", "IMPACTO", "RASTRO", "VIGORO", "MUESTRA", "CARTERA", "REMOLQUE", "GUITAR", "SINFONIA", "CABAL", "SITUAR", "DIVISA", "ABORDO", "CRISIS", "VERAZ", "CUPIDO", "RUTA", "FOLIO", "BANCAR", "DURAZNO", "ALBINO", "EQUIPO", "TRAER", "DEFENSOR", "CASUAL", "BELLO", "DIVERSO", "CARTEL", "ALUMNO", "COBARDES", "FELIZ", "FIORDO", "LUCHA", "UNIVERSO", "FINO", "SOLO", "GRITA", "FLECHERO", "RUDA", "CAMINAR", "NACION", "ABRI", "VENTA", "DISCO", "CARBON", "ABALORIO", "CABALLO", "FANATICO", "PLANTON", "COCINERO", "ALUMINIO", "CANAL", "PUNTO", "CAMINO", "FIERRO", "SOCIAL", "IMPULSO", "CERRADO", "AMISTAD", "LIGADO", "AGONIA", "CORREDOR", "CARTON", "ESCOLAR", "JUDIO", "JUDIA"};
int ultimoI = -1, ultimoJ = -1; // Último cubo clickeado

int intentosGeneracionTabla = 0; // Intentos de generar tabla

void generarTabla()
{
    srand(time(NULL)); // Seed aleatoria
    contador = 0;      // Vaciar contadores
    totalPalabrasListas = 0;
    totalCubosRojos = 0;
    intentosGeneracionTabla = 0;

    int espacioDisponible = FILAS * COLUMNAS;       // Calcular espacio
    int direction, startX, startY, longitudPalabra; // Iniciar variables

    win = false;            // No se ha ganado
    bool hayEspacio = true; // Asumir que hay espacio

    memset(cubosRojos, 0, sizeof(cubosRojos)); // Definir en false
    memset(cubosVerdes, 0, sizeof(cubosVerdes));
    memset(palabrasListas, 0, sizeof(palabrasListas));

    size_t cantidadPalabras = sizeof(palabras) / sizeof(palabras[0]); // Calcular cantidad de palabras

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            letras[i][j] = ' '; // Iniciar tabla con caracteres en blanco
        }
    }

    char palabraRandom[9];   // Asignar para palabra aleatoria
    palabraRandom[0] = '\0'; // Asegurar terminación nula
    do
    {
        // Copiar la palabra seleccionada a la variable de salida
        snprintf(palabraRandom, LIMITE_LETRAS + 1, "%s", palabras[rand() % cantidadPalabras]);

        longitudPalabra = (int)strlen(palabraRandom); // Calcular longitud palabra

        direction = (rand() % 6); // Valor entre 0 y 5 para las direcciones
        switch (direction)        // Por cada dirección
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
                for (int i = 0; i < longitudPalabra; i++)
                {
                    letras[startX][startY + i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[contador], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                contador++;
                espacioDisponible -= longitudPalabra;
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
                for (int i = 0; i < longitudPalabra; i++)
                {
                    letras[startX][startY + i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[contador], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                contador++;
                espacioDisponible -= longitudPalabra;
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
                for (int i = 0; i < longitudPalabra; i++)
                {
                    letras[startX + i][startY + i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[contador], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                contador++;
                espacioDisponible -= longitudPalabra;
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
                for (int i = 0; i < longitudPalabra; i++)
                {
                    letras[startX][startY - i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[contador], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                contador++;
                espacioDisponible -= longitudPalabra;
            }

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
                for (int i = 0; i < longitudPalabra; i++)
                {
                    letras[startX - i][startY] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[contador], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                contador++;
                espacioDisponible -= longitudPalabra;
            }

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
                for (int i = 0; i < longitudPalabra; i++)
                {
                    letras[startX - i][startY - i] = palabraRandom[i];
                }

                strncpy(palabrasEnUso[contador], palabraRandom, strlen(palabraRandom) + 1); // Copiar palabra aleatoria a palabras en uso

                contador++;
                espacioDisponible -= longitudPalabra;
            }
        }
    } while (espacioDisponible > 20); // Mientras haya más de 20 espacios
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
    bool comprobarPalabras();

    void verificarVecinos(int i, int j);

    InitWindow(screenWidth, screenHeight, "Sopa de letras"); // Iniciar ventana
    SetTargetFPS(60);                                        // Fijar a 60 fps

    generarTabla();
    while (!WindowShouldClose()) // Mientras esté en ejecución
    {
        BeginDrawing();            // Dibujar
        ClearBackground(RAYWHITE); // Limpiar fondo

        Vector2 mousePosition = GetMousePosition();

        for (int i = 0; i < FILAS; i++) // Por cada fila
        {
            for (int j = 0; j < COLUMNAS; j++) // Por cada columna
            {
                int cubeX = paddingLeft + (cubeSizeX + spacing) * j; // Calcular coordenadas de los cubos
                int cubeY = 10 + (cubeSizeY + spacing) * i;

                char letrastr[2] = {letras[i][j], '\0'};
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
                            cubosRojos[i][j] = true;
                            totalCubosRojos++;
                            strcpy(caracteresClickeados[totalCubosRojos - 1], (char[]){letras[i][j], '\0'});
                            ultimoI = i;
                            ultimoJ = j;
                            if (totalCubosRojos >= 2)
                            {
                                verificarVecinos(ultimoI, ultimoJ); // Verificar colisiones en torno a la dirección
                            }
                        }
                        for (int i = 0; i < contador; i++) // Por cada palabra en uso
                        {
                            if (comprobarPalabras(palabrasEnUso[i])) // Comprobar si lo seleccionado es una palabra
                            {
                                palabrasListas[i] = true;
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
        for (int i = 0; i < contador; i++)
        {
            int yPos = screenHeight - screenHeight / 4 + (i / 5) * 30; // Aumenta Y cada 5 palabras
            Color colorPalabra = palabrasListas[i] ? GREEN : BLACK;
            DrawText(palabrasEnUso[i], screenWidth / 25 + (i % 5) * 120, yPos, 20, colorPalabra);
        }
        for (int i = 0; i < totalCubosRojos; i++)
        {
            DrawText(caracteresClickeados[i], screenWidth - screenWidth / 12, screenHeight - screenHeight / 1.05 + 40 * i, 20, BLACK); // Dibujar letra de cubos clickeados
        }
        if (win) // Al ganar
        {
            int padding = MeasureText("GANASTE!", 40);
            int padding2 = MeasureText("Presiona rueda del mouse para volver a jugar", 20);
            DrawText("GANASTE!", screenWidth / 2 - padding / 2, screenHeight / 1.4 - padding, 40, BLACK);
            DrawText("Presiona rueda del mouse para volver a jugar", screenWidth / 4 - padding2, screenHeight / 4 - padding2, 20, BLACK);
        }
        DrawText("R\nPARA\nREINICIAR", 10, 10, 15, BLACK);
        EndDrawing(); // Terminar de dibujar
    }

    CloseWindow(); // Cerrar ventana

    return 0;
}

void desmarcarTodo()
{
    for (int i = 0; i < FILAS; i++) // Limpiar cubos rojos
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
    totalCubosRojos = 0;
}

void verificarVecinos(int i, int j)
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

    desmarcarTodo();
}

bool comprobarPalabras(char *palabrasUsada) // Conseguir palabra no usada
{
    char palabra[LIMITE_LETRAS + 1];

    int i;
    for (i = 0; i < LIMITE_LETRAS && i < totalCubosRojos; i++)
    {
        palabra[i] = caracteresClickeados[i][0];
    }

    palabra[i] = '\0';

    if (palabrasUsada != NULL)
    {
        if (palabrasUsada[0] != palabra[0])
            return false;
        if (strstr(palabra, palabrasUsada) != NULL)
        {
            printf("%d", totalPalabrasListas);
            totalPalabrasListas++;
            if (totalPalabrasListas == contador) // Se gana si el total de palabras verdes es igual a las del tablero
            {
                win = true;
            }
            return true;
        }
    }
    return false;
}
