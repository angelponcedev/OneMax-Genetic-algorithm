/*
To Do  List:
		-Generar una Poblacion                                                                              🗸
		-Evaluar Fitness de cada individuo                                                                  🗸
		-Seleccion por ruleta                                                                               🗸
		    -El mejor individuo de la poblacion pasa directamente a la siguiente generacion                 🗸
            -La seleccion de ruleta se hace por sumatoria del fitness y se le asigna a cada inidviduo       🗸
            su porcentaje de contribucion al fitness total
            -Seleccion aleatorio entre 1 y 100 y se escoge al ultimo individuo necesario para llegar a ese  🗸
            porcentaje (Empezando del mas grande al mas pequeno)
		-Numero aleatorio entre 0 y 1                                                                       🗸
		-Seleccionar aleatoriamente dos individuos, generar un numero aleatorio dentro del tamCromosoma,    🗸
		la primera mitad del descendiente es del individuo1
		la otra mitad es del individuo 2, hacer lo contrario para el segundo descendiente
		-iterar por cada bit del cromosoma de los individuos de la generacion
			-Generar un numero random entre 0 y 1, si cae dentro del rango de mutabilidad. Cambiar el bit.  🗸
		-Incluir a los dos cromosomas que hayan resultado del proceso anterior.                             🗸
		-Hacerlo hasta que llegue al total de la poblacion                                                  🗸
        (Repetir hasta que se llegue a soluciones adecuadas);                                               🗸
        -Implementar una funcion que lea los parametros de entrada desde un archivo "parametros.txt"
*/
#include <cstdio>
#include <cstdlib>
#include <ctime>

// Declarando variables globales
int tamPoblacion = 0, tamCromosoma = 0, maxGeneraciones = 0, cantidadRepeticionesMejor = 0;
float probMutacion = 0, tazaDeCambioMutacion = 0;

//Funcion de lectura de paramtros de entrada desde archivo
void lecturaParametrosArchivo();

// Funciones del algoritmo genetico
void generacionInicial(int **generacion);
void imprimirGeneracion(int **generacion, int *fitness, float probMutacion);
void nuevaGeneracion(int **generacion, int *fitness);
void cruzarPorSegmentos(int **generacion);
void cruzarUniformemente(int **generacion);
void mutacion(int **generacion);

// Funciones generales
int numeroBinario();
void calcularFitness(int **generacion, int *fitness);
int fitnessTotal(int *fitness);
int mejorIndividuo(int *fitness);
int seleccion(int **generacion, int *fitness);

int main() {
    int opcionEntrada = 0;

    // Semilla de números aleatorios
    srand(time(NULL));

    printf("Menu de bienvenida\n");
    do{
        printf("1.-Lectura de parametros por archivo\n");
        printf("2.-Lectura de paramteros por entrada estandar (teclado)\n");
        scanf("%d",&opcionEntrada);
    }while(opcionEntrada != 1 and opcionEntrada !=2);

    if(opcionEntrada == 1){
        lecturaParametrosArchivo();
    }
    else{
        // Solicitar valores al usuario
        do{
            printf("Ingrese el tamaño de la población (Minimo 2): ");
            scanf("%d", &tamPoblacion);
        }while(tamPoblacion<2);

        do {
            printf("Ingrese el tamaño de los cromosomas (Minimo 3): ");
            scanf("%d", &tamCromosoma);
        } while(tamCromosoma < 3);

        do{
            printf("Ingrese el número máximo de generaciones (Minimo 1): ");
            scanf("%d", &maxGeneraciones);
        }while(maxGeneraciones<1);

        do {
            printf("Ingrese la probabilidad de mutación (Entre 0 y 1): ");
            scanf("%f", &probMutacion);
        } while(probMutacion < 0 or probMutacion > 1);

        do{
            printf("Ingrese cantidad de veces que se puede repetir el mejor individuo para aumentar la probabilidad de mutacion (Minimo 1): ");
            scanf("%d", &cantidadRepeticionesMejor);
        }while(cantidadRepeticionesMejor<1);

        do{
            printf("Ingrese la cantidad tipo float a sumar sobre la probabilidad de mutacion original despues de %d repeticiones del mejor individuo (Entre 0 y 1): ", cantidadRepeticionesMejor);
            scanf("%f", &tazaDeCambioMutacion);
        }while(tazaDeCambioMutacion < 0 or tazaDeCambioMutacion > 1);
    }

    // Asignación dinámica de memoria
    int **generacion = (int **)malloc(tamPoblacion * sizeof(int *));
    for (int i = 0; i < tamPoblacion; i++) {
        generacion[i] = (int *)malloc(tamCromosoma * sizeof(int));
    }
    int *fitness = (int *)malloc(tamPoblacion * sizeof(int));

    int contadorGeneraciones = 0, fitnessMejorIndividuoAnterior = 0, fitnessMejorIndividuoActual = 0, contadorMejorSinCambio = 0;

    // Generación inicial
    contadorGeneraciones++;
    printf("Generacion %d \n", contadorGeneraciones);
    generacionInicial(generacion);
    calcularFitness(generacion, fitness);
    imprimirGeneracion(generacion, fitness, probMutacion);
    fitnessMejorIndividuoAnterior  = mejorIndividuo(fitness);

    // Mejoramiento de generaciones
    while (contadorGeneraciones < maxGeneraciones) {
        contadorGeneraciones++;
        nuevaGeneracion(generacion, fitness);
        //cruzarPorSegmentos(generacion);
        cruzarUniformemente(generacion);
        mutacion(generacion);
        calcularFitness(generacion, fitness);
        fitnessMejorIndividuoActual = fitness[mejorIndividuo(fitness)];
        
        //Revisando si el mejor individuo no ha cambiado
        if(fitnessMejorIndividuoActual == fitnessMejorIndividuoAnterior) {
            contadorMejorSinCambio++;
            if(contadorMejorSinCambio >= cantidadRepeticionesMejor) {
                probMutacion += tazaDeCambioMutacion;
                if(probMutacion > 1) probMutacion = 1;
                contadorMejorSinCambio = 0;
            }
        } else {
            contadorMejorSinCambio = 0;
        }

        //Imprimiendo los resultados
        printf("Generacion %d \n", contadorGeneraciones);
        imprimirGeneracion(generacion, fitness, probMutacion);

        // Actualizando el índice anterior
        fitnessMejorIndividuoAnterior = fitnessMejorIndividuoActual;
    }

    // Liberar memoria
    for (int i = 0; i < tamPoblacion; i++) {
        free(generacion[i]);
    }
    free(generacion);
    free(fitness);

    return 0;
}

void generacionInicial(int **generacion) {
    for (int i = 0; i < tamPoblacion; i++) {
        for (int j = 0; j < tamCromosoma; j++) {
            generacion[i][j] = numeroBinario();
        }
    }
}

void nuevaGeneracion(int **generacion, int *fitness) {
    int **nuevaGeneracion = (int **)malloc(tamPoblacion * sizeof(int *));
    for (int i = 0; i < tamPoblacion; i++) {
        nuevaGeneracion[i] = (int *)malloc(tamCromosoma * sizeof(int));
    }
    int individuoSeleccionado;

    // Agregando directamente en la primera posición el mejor individuo de la generación pasada
    for (int i = 0; i < tamCromosoma; i++) {
        nuevaGeneracion[0][i] = generacion[mejorIndividuo(fitness)][i];
    }

    // Proceso de selección del resto de individuos
    for (int i = 1; i < tamPoblacion; i++) {
        individuoSeleccionado = seleccion(generacion, fitness);
        for (int j = 0; j < tamCromosoma; j++) {
            nuevaGeneracion[i][j] = generacion[individuoSeleccionado][j];
        }
    }

    // Intercambiando los nuevos individuos con los de la generación anterior
    for (int i = 0; i < tamPoblacion; i++) {
        for (int j = 0; j < tamCromosoma; j++) {
            generacion[i][j] = nuevaGeneracion[i][j];
        }
    }

    // Liberar memoria de nuevaGeneracion
    for (int i = 0; i < tamPoblacion; i++) {
        free(nuevaGeneracion[i]);
    }
    free(nuevaGeneracion);
}

void cruzarPorSegmentos(int **generacion) {
    int **nuevaGeneracion = (int **)malloc(tamPoblacion * sizeof(int *));
    for (int i = 0; i < tamPoblacion; i++) {
        nuevaGeneracion[i] = (int *)malloc(tamCromosoma * sizeof(int));
    }
    int indice1, indice2, puntoCruza1, puntoCruza2;

    for (int i = 1; i < tamPoblacion; i++) {
        //Buscando dos padres distintos de la poblacion
        do {
            indice1 = rand() % tamPoblacion;
            indice2 = rand() % tamPoblacion;
        } while (indice1 == indice2);
        //Encontrando dos indices distintos para tomar segmentos
        do {
            puntoCruza1 = rand() % (tamCromosoma - 2);
            puntoCruza2 = puntoCruza1 + (rand () % (tamCromosoma - puntoCruza1));
        } while (puntoCruza1 == puntoCruza2);
        //Generando descendientes
        for (int j = 0; j < tamCromosoma; j++) {
            //Logica para separar el cruzamiento de descendiente1 y descendiente 2
            if (i % 2 == 0) {
                //nuevaGeneracion[i][j] = (j <= puntoCruza1) ? generacion[indice1][j] : generacion[indice2][j];
                nuevaGeneracion[i][j] = (j <= puntoCruza1) ? generacion[indice1][j] : (j <= puntoCruza2) ? generacion[indice2][j] : generacion[indice1][j];
            } else {
                //nuevaGeneracion[i][j] = (j <= puntoCruza1) ? generacion[indice2][j] : generacion[indice1][j];
                nuevaGeneracion[i][j] = (j <= puntoCruza1) ? generacion[indice2][j] : (j <= puntoCruza2) ? generacion[indice1][j] : generacion[indice2][j];
            }
        }
    }

    for (int i = 1; i < tamPoblacion; i++) {
        for (int j = 0; j < tamCromosoma; j++) {
            generacion[i][j] = nuevaGeneracion[i][j];
        }
    }

    // Liberar memoria de nuevaGeneracion
    for (int i = 0; i < tamPoblacion; i++) {
        free(nuevaGeneracion[i]);
    }
    free(nuevaGeneracion);
}

void cruzarUniformemente(int **generacion){
    int **nuevaGeneracion = (int **)malloc(tamPoblacion * sizeof (int*));
    for (int i=0; i<tamPoblacion; i++){
        nuevaGeneracion[i] = (int*) malloc(tamCromosoma * sizeof (int*));
    }
    int indice1 = 0,indice2 = 0,numeroRandom = 0;
    for(int i = 0; i< tamPoblacion; i++){
        do{
            indice1 = rand() % tamPoblacion;
            indice2 = rand() % tamPoblacion;
        }while(indice1 == indice2);
        for(int j = 0; j < tamCromosoma; j++){
            numeroRandom = numeroBinario();
            nuevaGeneracion[i][j] = (numeroRandom % 2 == 0) ? generacion[indice1][j] : generacion[indice2][j];
        }
    }
    //Saltando el primer individuo que es el mejor de la generacion anterior
    for(int i = 1; i < tamPoblacion; i++){
        for(int j = 0; j < tamCromosoma; j++){
            generacion[i][j] = nuevaGeneracion[i][j];
        }
    }

    for (int i = 0; i < tamPoblacion; i++){
        free(nuevaGeneracion[i]);
    }
    free(nuevaGeneracion);
}

void mutacion(int **generacion) {
    //Saltando el mejor individuo de la generacion anterior
    for (int i = 1; i < tamPoblacion; i++) {
        for (int j = 0; j < tamCromosoma; j++) {
            if ((float)rand() / RAND_MAX <= probMutacion) {
                generacion[i][j] = 1 - generacion[i][j];
            }
        }
    }
}

void imprimirGeneracion(int **generacion, int *fitness, float probMutacion) {
    for (int i = 0; i < tamPoblacion; i++) {
        printf("%d.-", i + 1);
        for (int j = 0; j < tamCromosoma; j++) {
            printf("%d", generacion[i][j]);
        }
        printf("    %d", fitness[i]);
        printf("\n");
    }
    printf("\n");
    printf("El mejor individuo de la generacion obtuvo:  %d\n", fitness[mejorIndividuo(fitness)]);
    printf("Probabilidad de mutacion actual:  %f\n", probMutacion);
}

int numeroBinario() {
    return rand() % 2;
}

void calcularFitness(int **generacion, int *fitness) {
    for (int i = 0; i < tamPoblacion; i++) {
        fitness[i] = 0;
        for (int j = 0; j < tamCromosoma; j++) {
            fitness[i] += generacion[i][j];
        }
    }
}

int fitnessTotal(int *fitness) {
    int total = 0;
    for (int i = 0; i < tamPoblacion; i++) {
        total += fitness[i];
    }
    return total;
}

int mejorIndividuo(int *fitness) {
    int mejor = fitness[0], indice = 0;
    for (int i = 0; i < tamPoblacion; i++) {
        if (mejor < fitness[i]) {
            mejor = fitness[i];
            indice = i;
        }
    }
    return indice;
}

int seleccion(int **generacion, int *fitness) {
    int fitTotal = fitnessTotal(fitness);
    int numeroAleatorio = rand() % fitTotal + 1;
    int sumatoria = 0;

    for (int i = 0; i < tamPoblacion; i++) {
        sumatoria += fitness[i];
        if (sumatoria >= numeroAleatorio) {
            return i;
        }
    }
    return tamPoblacion - 1;
}

void lecturaParametrosArchivo(){
    char archivo[50];
    printf("\nIngrese el nombre del archivo a leer:");
    scanf("%s",archivo);

    FILE *fptr;
    fptr = fopen(archivo,"r");
    char* endptr;
    //Verificando que el archivo exista
    if (fptr == nullptr){
        printf("%s no existe en el directorio de ejecucion del codigo, revise el directorio de ejecucion",archivo);
    }

    else{
        char buffer[100];
        int i = 0, num1 = 0, num2 = 0, num3 = 0;
        printf("Contenido del archivo: \n");
        while(fgets(buffer,100,fptr)){
            i++;
            switch (i) {
                case 1:{
                    num1 = atoi(buffer);
                    printf("\n%d",num1);
                    break;
                }
                case 2:{
                    num2 = atoi(buffer);
                    printf("\n%d",num2);
                    break;
                }
                case 3:{
                    num3 = atoi(buffer);
                    printf("\n%d",num3);
                    break;
                }
                default:{
                    printf("\nLectura Finalizada\n");
                    break;
                    //return;
                }
            }
        };

        //Ya se pueden usar los datos del archivo como variables
        printf("\nSuma de los tres numeros: %d", num1 + num2 + num3 );
        return;
    }

    fclose(fptr);
}