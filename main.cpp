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
        -Implementar una funcion que lea los parametros de entrada desde un archivo "parametros.txt"        🗸
            -Continuar la lectura de parametros cuando tenga una struct para la data                        🗸
        -Refactorizar las variables globales a Structs                                                      🗸
        -Agregar probabilidad de cruza
            -Leer probCruzamiento como parametro                                                            🗸
            -Generar numero aleatorio entre 0 y 1                                                           🗸
            - numAleatorio < probCruzamiento ? Cruzar los padres : los padres pasan directamente            🗸
        -Seleccion por truncamiento
*/
#include <cstdio>
#include <cstdlib>
#include <ctime>

//Declarando struct de control de datos para el algogenetico
struct parametros{
    int tamPoblacion;
    int tamCromosoma;
    int maxGeneraciones;
    int cantidadRepeticionesMejor;
    float probMutacion;
    float tazaCambioMutacion;
    float probCruza;
    int ** generacion;
    int * fitness;
};

//Funcion de lectura de parametros de entrada desde archivo
void lecturaParametrosArchivo(struct parametros* parametros);
void lecturaParametrosConsola(struct parametros* parametros);

// Funciones del algoritmo genetico
void crearGeneracion(struct parametros* parametros);
void generacionInicial(struct parametros* parametros);
void imprimirGeneracion(struct parametros* parametros);
void nuevaGeneracion(struct parametros* parametros);
void cruzarPorSegmentos(struct parametros* parametros);
void cruzarUniformemente(struct parametros* parametros);
void mutacion(struct parametros* parametros);

// Funciones generales
int numeroBinario();
float numDecimal();
void calcularFitness(struct parametros*);
int fitnessTotal(struct parametros*);
int mejorIndividuo(struct parametros*);
int seleccion(struct parametros* parametros);

int main() {
    //Inicializando Struct
    struct parametros parametros = {0,0,0,0,0};
    int opcionEntrada = 0;

    // Semilla de números aleatorios
    srand(time(NULL));

    printf("Menu de bienvenida\n");
    do{
        printf("1.-Lectura de parametros por archivo\n");
        printf("2.-Lectura de parametros por entrada estandar (teclado)\n");
        printf("\n");
        scanf("%d",&opcionEntrada);
        printf("\n");
    }while(opcionEntrada != 1 and opcionEntrada !=2);

    if(opcionEntrada == 1){
        lecturaParametrosArchivo(&parametros);
    }
    else{
        lecturaParametrosConsola(&parametros);
    }

    // Asignación dinámica de memoria
    crearGeneracion(&parametros);

    int contadorGeneraciones = 0, fitnessMejorIndividuoAnterior = 0, fitnessMejorIndividuoActual = 0, contadorMejorSinCambio = 0;

    // Generación inicial
    contadorGeneraciones++;
    printf("Generacion %d \n", contadorGeneraciones);
    generacionInicial(&parametros);
    calcularFitness(&parametros);
    imprimirGeneracion(&parametros);
    fitnessMejorIndividuoAnterior = mejorIndividuo(&parametros);

    // Mejoramiento de generaciones
    while (contadorGeneraciones < parametros.maxGeneraciones) {
        contadorGeneraciones++;
        nuevaGeneracion(&parametros);
        //cruzarPorSegmentos(generacion);
        cruzarUniformemente(&parametros);
        mutacion(&parametros);
        calcularFitness(&parametros);
        fitnessMejorIndividuoActual = parametros.fitness[mejorIndividuo(&parametros)];
        
        //Revisando si el mejor individuo no ha cambiado
        if(fitnessMejorIndividuoActual == fitnessMejorIndividuoAnterior) {
            contadorMejorSinCambio++;
            if(contadorMejorSinCambio >= parametros.cantidadRepeticionesMejor) {
                parametros.probMutacion += parametros.tazaCambioMutacion;
                if(parametros.probMutacion > 1) parametros.probMutacion = 1;
                contadorMejorSinCambio = 0;
            }
        } else {
            contadorMejorSinCambio = 0;
        }

        //Imprimiendo los resultados
        printf("Generacion %d \n", contadorGeneraciones);
        imprimirGeneracion(&parametros);

        // Actualizando el índice anterior
        fitnessMejorIndividuoAnterior = fitnessMejorIndividuoActual;
    }

    // Liberar memoria
    for (int i = 0; i < parametros.tamPoblacion; i++) {
        free(parametros.generacion[i]);
    }
    free(parametros.generacion);
    free(parametros.fitness);

    return 0;
}

void crearGeneracion(struct parametros* parametros){
    //Asignando memoria para las filas de la matriz
    printf("Alocando memoria");
    parametros->generacion = (int**)malloc(parametros->tamPoblacion * sizeof (int*));
    if(parametros->generacion == NULL){
        printf("Error en la alocacion de memoria en filas de la nueva generacion.\n");
        free(parametros->generacion);
        exit(1);
    }

    //Asignando memoria para las columnas de la matriz
    for(int i=0; i<parametros->tamPoblacion; i++){
        parametros->generacion[i] = (int*)malloc(parametros->tamCromosoma * sizeof(int));
        if(parametros->generacion == NULL){
            printf("Error en la alocacion de memoria en columnas de la nueva generacion\n");
            for(int j=0; j<i; j++){
                free(parametros->generacion[j]);
            }
            free(parametros->generacion);
            free(parametros);
            exit(1);
        }
    }
    //Asignando memoria para fitness
    parametros->fitness=(int*)malloc(parametros->tamPoblacion * sizeof(int));
    if(parametros->fitness == NULL){
        printf("Error en la alocacion de memoria para el fitness de la generacion.");
        for(int i=0; i<parametros->tamPoblacion; i++){
            free(parametros->fitness);
        }
        exit(1);
    }
}

void generacionInicial(struct parametros* parametros){
    for(int i=0; i<parametros->tamPoblacion; i++){
        for(int j=0; j<parametros->tamCromosoma; j++){
            parametros->generacion[i][j] = numeroBinario();
        }
    }
}

void nuevaGeneracion(struct parametros* parametros) {
    int individuoSeleccionado;

    // Agregando directamente en la primera posición el mejor individuo de la generación pasada
    for (int i = 0; i < parametros->tamCromosoma; i++) {
        parametros->generacion[0][i] = parametros->generacion[mejorIndividuo(parametros)][i];
    }

    // Proceso de selección del resto de individuos
    for (int i = 1; i < parametros->tamPoblacion; i++) {
        individuoSeleccionado = seleccion(parametros);
        for (int j = 0; j < parametros->tamCromosoma; j++) {
            parametros->generacion[i][j] = parametros->generacion[individuoSeleccionado][j];
        }
    }
}

int seleccion(struct parametros* parametros) {
    int fitTotal = fitnessTotal(parametros);
    int numeroAleatorio = rand() % fitTotal + 1;
    int sumatoria = 0;

    for (int i = 0; i < parametros->tamPoblacion; i++) {
        sumatoria += parametros->fitness[i];
        if (sumatoria >= numeroAleatorio) {
            return i;
        }
    }
}

void cruzarPorSegmentos(struct parametros* parametros) {
    int **nuevaGeneracion = (int **)malloc(parametros->tamPoblacion * sizeof(int *));
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        nuevaGeneracion[i] = (int *)malloc(parametros->tamCromosoma * sizeof(int));
    }
    int indice1, indice2, puntoCruza1, puntoCruza2;

    for (int i = 1; i < parametros->tamPoblacion; i++) {
        //Buscando dos padres distintos de la poblacion
        do {
            indice1 = rand() % parametros->tamPoblacion;
            indice2 = rand() % parametros->tamPoblacion;
        } while (indice1 == indice2);
        //Revisando si los padres se cruzan o se pasan directamente
        if(parametros->probCruza >= numDecimal()){
            //Encontrando dos indices distintos para tomar segmentos
            do {
                puntoCruza1 = rand() % (parametros->tamCromosoma - 2);
                puntoCruza2 = puntoCruza1 + (rand () % (parametros->tamCromosoma - puntoCruza1));
            } while (puntoCruza1 == puntoCruza2);
            //Generando descendientes
            for (int j = 0; j < parametros->tamCromosoma; j++) {
                //Logica para separar el cruzamiento de descendiente1 y descendiente 2
                if (i % 2 == 0) {
                    //nuevaGeneracion[i][j] = (j <= puntoCruza1) ? generacion[indice1][j] : generacion[indice2][j];
                    nuevaGeneracion[i][j] = (j <= puntoCruza1) ? parametros->generacion[indice1][j] : (j <= puntoCruza2) ? parametros->generacion[indice2][j] : parametros->generacion[indice1][j];
                } else {
                    //nuevaGeneracion[i][j] = (j <= puntoCruza1) ? generacion[indice2][j] : generacion[indice1][j];
                    nuevaGeneracion[i][j] = (j <= puntoCruza1) ? parametros->generacion[indice2][j] : (j <= puntoCruza2) ? parametros->generacion[indice1][j] : parametros->generacion[indice2][j];
                }
            }
        }
        //Los padres pasan directo a la siguiente generacion
        else{
            //Primer padre
            nuevaGeneracion[i] = parametros->generacion[indice1];
            i++;
            //Segundo Padre
            if( i < parametros->tamPoblacion ){
                nuevaGeneracion[i] = parametros->generacion[indice2];
            }
        }
    }

    for (int i = 1; i < parametros->tamPoblacion; i++) {
        for (int j = 0; j < parametros->tamCromosoma; j++) {
            parametros->generacion[i][j] = nuevaGeneracion[i][j];
        }
    }

    // Liberar memoria de nuevaGeneracion
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        free(nuevaGeneracion[i]);
    }
    free(nuevaGeneracion);
}

void cruzarUniformemente(struct parametros* parametros){
    int **nuevaGeneracion = (int **)malloc(parametros->tamPoblacion * sizeof (int*));
    for (int i=0; i<parametros->tamPoblacion; i++){
        nuevaGeneracion[i] = (int*) malloc(parametros->tamCromosoma * sizeof (int*));
    }
    int indice1 = 0,indice2 = 0,numeroRandom = 0;
    for(int i = 0; i< parametros->tamPoblacion; i++){
        do{
            indice1 = rand() % parametros->tamPoblacion;
            indice2 = rand() % parametros->tamPoblacion;
        }while(indice1 == indice2);
        //Se hace cruzamiento
        if(parametros->probCruza >= numDecimal()){
            for(int j = 0; j < parametros->tamCromosoma; j++){
                numeroRandom = numeroBinario();
                nuevaGeneracion[i][j] = (numeroRandom % 2 == 0) ? parametros->generacion[indice1][j] : parametros->generacion[indice2][j];
            }
        }
        //Pasan los padres directamente
        else{
            nuevaGeneracion[i] = parametros->generacion[indice1];
            i++;
            if( i < parametros->tamPoblacion ){
                nuevaGeneracion[i] = parametros->generacion[indice2];
            }
        }
    }
    //Saltando el primer individuo que es el mejor de la generacion anterior
    for(int i = 1; i < parametros->tamPoblacion; i++){
        for(int j = 0; j < parametros->tamCromosoma; j++){
            parametros->generacion[i][j] = nuevaGeneracion[i][j];
        }
    }

    for (int i = 0; i < parametros->tamPoblacion; i++){
        free(nuevaGeneracion[i]);
    }
    free(nuevaGeneracion);
}

void mutacion(struct parametros* parametros) {
    //Saltando el mejor individuo de la generacion anterior
    for (int i = 1; i < parametros->tamPoblacion; i++) {
        for (int j = 0; j < parametros->tamCromosoma; j++) {
            if ((float)rand() / RAND_MAX <= parametros->probMutacion) {
                parametros->generacion[i][j] = 1 - parametros->generacion[i][j];
            }
        }
    }
}

void imprimirGeneracion(struct parametros* parametros) {
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        printf("%d.-", i + 1);
        for (int j = 0; j < parametros->tamCromosoma; j++) {
            printf("%d", parametros->generacion[i][j]);
        }
        printf("    %d", parametros->fitness[i]);
        printf("\n");
    }
    printf("\n");
    printf("El mejor individuo de la generacion obtuvo:  %d\n", parametros->fitness[mejorIndividuo(parametros)]);
    printf("Probabilidad de mutacion actual:  %f\n", parametros->probMutacion);
}

int numeroBinario() {
    return rand() % 2;
}

float numDecimal(){
    return (float)rand() / (float)RAND_MAX;
}

void calcularFitness(struct parametros* parametros) {
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        parametros->fitness[i] = 0;
        for (int j = 0; j < parametros->tamCromosoma; j++) {
            parametros->fitness[i] += parametros->generacion[i][j];
        }
    }
}

int fitnessTotal(struct parametros* parametros) {
    int total = 0;
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        total += parametros->fitness[i];
    }
    return total;
}

int mejorIndividuo(struct parametros* parametros) {
    int mejor = parametros->fitness[0], indice = 0;
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        if (mejor < parametros->fitness[i]) {
            mejor = parametros->fitness[i];
            indice = i;
        }
    }
    return indice;
}

void lecturaParametrosArchivo(struct parametros* parametros){
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
        char buffer[20];
        printf("Contenido del archivo: \n");
        int i = 0;
        while(fgets(buffer,100,fptr)){
            i++;
            switch (i) {
                case 1:{
                    parametros->tamPoblacion = atoi(buffer);
                    printf("\ntamPoblacion: %d",parametros->tamPoblacion);
                    break;
                }
                case 2:{
                    parametros->tamCromosoma = atoi(buffer);
                    printf("\ntamCromosoma: %d",parametros->tamCromosoma);
                    break;
                }
                case 3:{
                    parametros->maxGeneraciones = atoi(buffer);
                    printf("\nmaxGeneraciones: %d",parametros->maxGeneraciones);
                    break;
                }
                case 4:{
                    parametros->cantidadRepeticionesMejor = atoi(buffer);
                    printf("\nrepeticionesMejor: %d",parametros->cantidadRepeticionesMejor);
                    break;
                }
                case 5:{
                    parametros->probMutacion = atof(buffer);
                    printf("\nprobMutacion: %f",parametros->probMutacion);
                    break;
                }
                case 6:{
                    parametros->tazaCambioMutacion = atof(buffer);
                    printf("\ntazaCambioMutacion: %f",parametros->tazaCambioMutacion);
                    break;
                }
                case 7:{
                    parametros->probCruza = atof(buffer);
                    printf("\nprobCruza: %f",parametros->probCruza);
                    break;
                }
                default:{
                    printf("\nLectura Finalizada\n");
                    break;
                }
            }
        };
        printf("\n");
        return;
    }
    fclose(fptr);
}

void lecturaParametrosConsola(struct parametros* parametros){
    // Solicitar valores al usuario
    do{
        printf("Ingrese el tamaño de la población (Minimo 2): ");
        scanf("%d", &parametros->tamPoblacion);
    }while(parametros->tamPoblacion<2);

    do {
        printf("Ingrese el tamaño de los cromosomas (Minimo 3): ");
        scanf("%d", &parametros->tamCromosoma);
    } while(parametros->tamCromosoma < 3);

    do{
        printf("Ingrese el número máximo de generaciones (Minimo 1): ");
        scanf("%d", &parametros->maxGeneraciones);
    }while(parametros->maxGeneraciones<1);

    do {
        printf("Ingrese la probabilidad de mutación (Entre 0 y 1): ");
        scanf("%f", &parametros->probMutacion);
    } while(parametros->probMutacion < 0 or parametros->probMutacion > 1);

    do{
        printf("Ingrese cantidad de veces que se puede repetir el mejor individuo para aumentar la probabilidad de mutacion (Minimo 1): ");
        scanf("%d", &parametros->cantidadRepeticionesMejor);
    }while(parametros->cantidadRepeticionesMejor<1);

    do{
        printf("Ingrese la cantidad tipo float a sumar sobre la probabilidad de mutacion original despues de %d repeticiones del mejor individuo (Entre 0 y 1): ", parametros->cantidadRepeticionesMejor);
        scanf("%f", &parametros->tazaCambioMutacion);
    }while(parametros->tazaCambioMutacion < 0 or parametros->tazaCambioMutacion > 1);

    do{
        printf("Ingrese la cantidad tipo float para probabilidad de cruza (Entre 0 y 1): ");
        scanf("%f", &parametros->probCruza);
    }while(parametros->probCruza < 0 or parametros->probCruza > 1);
}