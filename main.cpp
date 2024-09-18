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
        -Seleccion por truncamiento                                                                         🗸
        -Hacer menu para los cruzamientos                                                                   🗸
        -Operador de 3 mutaciones                                                                           🗸
        -Seleccion por torneo
*/
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
//Declarando struct de control de datos para el algogenetico
struct parametros {
    int tamPoblacion;
    int tamCromosoma;
    int maxGeneraciones;
    int cantidadRepeticionesMejor;
    float probMutacion;
    float tazaCambioMutacion;
    float probCruza;
    int porcientoTruncamiento;
    int opcionCruzamiento;
    int **generacion;
    int *fitness;
    int **individuosSeleccionados;
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
void cruzarTruncamiento(struct parametros* parametros);
void cruzarTorneo(struct parametros* parametros);
void cruzarMascaraAleatoria(struct parametros* parametros);
void cruzarInversion(struct parametros* parametros);
void seleccionEstocastica(struct parametros* parametros);
void mutacion(struct parametros* parametros);

// Funciones generales
int numeroBinario();
float numDecimal();
void ordenar_por_fitness(struct parametros * params);
int compara_fitness(const void * a, const void * b);
void calcularFitness(struct parametros* parametros);
int fitnessTotal(struct parametros* parametros);
int mejorIndividuo(struct parametros* parametros);
int seleccion(struct parametros* parametros);

int main() {
    //Inicializando Struct
    struct parametros parametros = {0,0,0,0,0.01,0.01,0.7,50,0};
    int opcionEntrada = 0, opcionCruzamiento = 0;

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
        opcionCruzamiento = parametros.opcionCruzamiento;
    }
    else{
        lecturaParametrosConsola(&parametros);
    }
    //Inicializando memoria para las selecciones
    parametros.individuosSeleccionados = (int**) malloc(parametros.tamPoblacion*sizeof(int*));
    for(int i = 0; i<parametros.tamPoblacion; i++){
        parametros.individuosSeleccionados[i] = (int*)malloc(parametros.tamCromosoma*sizeof (int));
    }

    printf("\n\n\nMenu de Cruzamiento\n");
    while(opcionCruzamiento != 1 and opcionCruzamiento !=2 and opcionCruzamiento !=3 and opcionCruzamiento != 4 and opcionCruzamiento!=5 and opcionCruzamiento!=6 and opcionCruzamiento!=7){
        printf("1.-Seleccion por Ruleta cruzamiento por Segmentos\n");
        printf("2.-Seleccion por Ruleta cruzamiento Uniforme\n");
        printf("3.-Seleccion por Truncamiento cruzamiento por Segmentos\n");
        printf("4.-Seleccion por Torneo cruzamiento por Segmentos\n");
        printf("5.-Seleccion Estocastica cruzamiento por Segmentos\n");
        printf("6.-Seleccion Estocastica cruzamiento por Mascara Aleatoria\n");
        printf("7.-Seleccion Estocastica cruzamiento por Inversion");
        printf("\n");
        scanf("%d",&opcionCruzamiento);
        printf("\n");
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
        switch (opcionCruzamiento) {
            case 1:{
                nuevaGeneracion(&parametros);
                cruzarPorSegmentos(&parametros);
                break;
            }
            case 2:{
                nuevaGeneracion(&parametros);
                cruzarUniformemente(&parametros);
                break;
            }
            case 3:{
                cruzarTruncamiento(&parametros);
                break;
            }
            case 4:{
                cruzarTorneo(&parametros);
                break;
            }
            case 5:{
                seleccionEstocastica(&parametros);
                cruzarPorSegmentos(&parametros);
                break;
            }
            case 6:{
                seleccionEstocastica(&parametros);
                cruzarMascaraAleatoria(&parametros);
                break;
            }
            case 7:{
                seleccionEstocastica(&parametros);
                cruzarInversion(&parametros);
                break;
            }
            default:{
                printf("Ocurrio un error en la seleccion de Cruza.\n");
                exit(1);
            }
        }

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

void seleccionEstocastica(struct parametros* parametros) {
    int fitTotal = fitnessTotal(parametros);
    float segmento = (float)fitTotal / parametros->tamPoblacion;
    float sumatoriaSegmentos = 0.0;
    int sumatoria = 0;
    int j = 0;

    // Crear una nueva generación temporal
    int** nuevaGeneracion = (int**)malloc(parametros->tamPoblacion * sizeof(int*));
    if (nuevaGeneracion == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para nuevaGeneracion\n");
        exit(1);
    }

    for (int i = 0; i < parametros->tamPoblacion; i++) {
        nuevaGeneracion[i] = (int*)malloc(parametros->tamCromosoma * sizeof(int));
        if (nuevaGeneracion[i] == NULL) {
            fprintf(stderr, "Error: No se pudo asignar memoria para nuevaGeneracion[%d]\n", i);
            // Liberar la memoria ya asignada
            for (int k = 0; k < i; k++) {
                free(nuevaGeneracion[k]);
            }
            free(nuevaGeneracion);
            exit(1);
        }
    }

    // Asegurar que el mejor individuo pase a la siguiente generación
    int mejorIndice = mejorIndividuo(parametros);
    memcpy(nuevaGeneracion[0], parametros->generacion[mejorIndice], parametros->tamCromosoma * sizeof(int));

    // Selección estocástica
    for (int i = 1; i < parametros->tamPoblacion; i++) {
        sumatoriaSegmentos += segmento;
        sumatoria = 0;
        j = 0;

        while (sumatoria < sumatoriaSegmentos && j < parametros->tamPoblacion) {
            sumatoria += parametros->fitness[j];
            if (sumatoria >= sumatoriaSegmentos) {
                memcpy(nuevaGeneracion[i], parametros->generacion[j], parametros->tamCromosoma * sizeof(int));
                break;
            }
            j++;
        }
        // Si no se seleccionó ningún individuo, seleccionar el último
        if (j == parametros->tamPoblacion) {
            memcpy(nuevaGeneracion[i], parametros->generacion[parametros->tamPoblacion - 1], parametros->tamCromosoma * sizeof(int));
        }
    }

    // Liberar la generación anterior
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        free(parametros->generacion[i]);
    }
    free(parametros->generacion);

    // Asignar la nueva generación
    parametros->generacion = nuevaGeneracion;
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

            //Encontrando dos indices distintos para tomar segmentos
            do {
                puntoCruza1 = rand() % (parametros->tamCromosoma - 2);
                puntoCruza2 = puntoCruza1 + (rand () % (parametros->tamCromosoma - puntoCruza1));
            } while (puntoCruza1 == puntoCruza2);
            //Generando descendientes
            for (int j = 0; j < parametros->tamCromosoma; j++) {
                //Revisando si los padres se cruzan o se pasan directamente
                if(parametros->probCruza >= numDecimal()) {
                    //Logica para separar el cruzamiento de descendiente1 y descendiente 2
                    if (i % 2 == 0) {
                        //nuevaGeneracion[i][j] = (j <= puntoCruza1) ? generacion[indice1][j] : generacion[indice2][j];
                        nuevaGeneracion[i][j] = (j <= puntoCruza1) ? parametros->generacion[indice1][j] : (j <=
                                                                                                           puntoCruza2)
                                                                                                          ? parametros->generacion[indice2][j]
                                                                                                          : parametros->generacion[indice1][j];
                    } else {
                        //nuevaGeneracion[i][j] = (j <= puntoCruza1) ? generacion[indice2][j] : generacion[indice1][j];
                        nuevaGeneracion[i][j] = (j <= puntoCruza1) ? parametros->generacion[indice2][j] : (j <=
                                                                                                           puntoCruza2)
                                                                                                          ? parametros->generacion[indice1][j]
                                                                                                          : parametros->generacion[indice2][j];
                    }
                }
                    //Los padres pasan directo a la siguiente generacion
                else{
                    //Primer padre
                    nuevaGeneracion[i][j] = parametros->generacion[indice1][j];
                    //Segundo Padre
                    if (i+1 < parametros->tamPoblacion) {
                        nuevaGeneracion[i+1][j] = parametros->generacion[indice2][j];
                    }
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

void cruzarInversion(struct parametros* parametros) {
    // Crear nueva generación
    int **nuevaGeneracion = (int **)malloc(parametros->tamPoblacion * sizeof(int*));
    if (nuevaGeneracion == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para nuevaGeneracion\n");
        exit(1);
    }

    for (int i = 0; i < parametros->tamPoblacion; i++) {
        nuevaGeneracion[i] = (int*)malloc(parametros->tamCromosoma * sizeof(int));
        if (nuevaGeneracion[i] == NULL) {
            fprintf(stderr, "Error: No se pudo asignar memoria para nuevaGeneracion[%d]\n", i);
            // Liberar la memoria ya asignada
            for (int k = 0; k < i; k++) {
                free(nuevaGeneracion[k]);
            }
            free(nuevaGeneracion);
            exit(1);
        }
    }

    // Copiar el mejor individuo a la nueva generación
    int mejorIndice = mejorIndividuo(parametros);
    memcpy(nuevaGeneracion[0], parametros->generacion[mejorIndice], parametros->tamCromosoma * sizeof(int));

    // Generar el resto de la población
    for (int i = 1; i < parametros->tamPoblacion; i++) {
        int indice1 = rand() % parametros->tamPoblacion;
        int fitnessIndividuo = parametros->fitness[indice1];
        int fitnessInversion = 0;
        int inversion[parametros->tamCromosoma];

        // Generar la inversión del individuo y calcular su fitness
        for (int j = 0; j < parametros->tamCromosoma; j++) {
            inversion[j] = 1 - parametros->generacion[indice1][j];
            fitnessInversion += inversion[j];
        }

        int fitnessTotal = fitnessIndividuo + fitnessInversion;
        float porcientoIndividuo = (float)fitnessIndividuo / fitnessTotal;

        for (int j = 0; j < parametros->tamCromosoma; j++) {
            if (numDecimal() < parametros->probCruza) {
                nuevaGeneracion[i][j] = (numDecimal() <= porcientoIndividuo) ?
                                        parametros->generacion[indice1][j] : inversion[j];
            } else {
                nuevaGeneracion[i][j] = parametros->generacion[indice1][j];
            }
        }
    }

    // Liberar la memoria de la generación anterior
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        free(parametros->generacion[i]);
    }
    free(parametros->generacion);

    // Asignar la nueva generación
    parametros->generacion = nuevaGeneracion;
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

        for(int j = 0; j < parametros->tamCromosoma; j++){
            //Se hace cruzamiento
            if(parametros->probCruza >= numDecimal()){
            numeroRandom = numeroBinario();
            nuevaGeneracion[i][j] = (numeroRandom % 2 == 0) ? parametros->generacion[indice1][j] : parametros->generacion[indice2][j];
            }
            //Pasan los padres directamente
            else{
                nuevaGeneracion[i][j] = parametros->generacion[indice1][j];
                if( i+1 < parametros->tamPoblacion ){
                    nuevaGeneracion[i+1][j] = parametros->generacion[indice2][j];
                }
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

void cruzarMascaraAleatoria(struct parametros* parametros){
    int **nuevaGeneracion = (int **)malloc(parametros->tamPoblacion * sizeof (int*));
    for (int i=0; i<parametros->tamPoblacion; i++){
        nuevaGeneracion[i] = (int*) malloc(parametros->tamCromosoma * sizeof (int*));
    }
    int indice1 = 0,indice2 = 0;

    int mascaraAleatoria[parametros->tamCromosoma];
    for(int i = 0; i< parametros->tamPoblacion; i++){
        do{
            indice1 = rand() % parametros->tamPoblacion;
            indice2 = rand() % parametros->tamPoblacion;
        }while(indice1 == indice2);
        //Generando mascara
        for(int j=0; j<parametros->tamCromosoma; j++){
            mascaraAleatoria[j] = numeroBinario();
        }

        for(int j = 0; j < parametros->tamCromosoma; j++){
            //Se hace cruzamiento
            if(parametros->probCruza >= numDecimal()){
                nuevaGeneracion[i][j] = (mascaraAleatoria[j]) ? parametros->generacion[indice1][j] : parametros->generacion[indice2][j];
            }
                //Pasan los padres directamente
            else{
                nuevaGeneracion[i][j] = parametros->generacion[indice1][j];
                if( i+1 < parametros->tamPoblacion ){
                    nuevaGeneracion[i+1][j] = parametros->generacion[indice2][j];
                }
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

void cruzarTruncamiento(struct parametros* parametros){
    //Calculando la cantidad de individuos de la poblacion que se van a usar para la cruza
    int totalIndividuosCruza = (parametros->porcientoTruncamiento * parametros->tamPoblacion) / 100;
    int indice1, indice2;

    //nueva Generacion
    int** nuevaGeneracion = (int**) malloc(parametros->tamPoblacion * sizeof(int*));
    for(int i = 0; i < parametros->tamPoblacion; i++){
        nuevaGeneracion[i] = (int*) malloc (parametros->tamCromosoma * sizeof(int));
    }

    //Ordenando la generacion por fitness
    ordenar_por_fitness(parametros);


    //Haciendo el cruzamiento
    for(int i = 0; i < parametros->tamPoblacion; i++){
        for(int j = 0; j < parametros->tamCromosoma; j++){
            //Pasando el mejor individuo a la siguiente generacion directamente
            if(i == 0){
                nuevaGeneracion[i][j] = parametros -> generacion[mejorIndividuo(parametros)][j];
            }
            else {
                //Seleccionar dos padres dentro del grupo de individuos ordenado
                do {
                    indice1 = rand() % totalIndividuosCruza;
                    indice2 = rand() % totalIndividuosCruza;
                } while (indice1 == indice2);
                //Cruzamiento
                if (i % 2) {
                    (j <= parametros->tamCromosoma / 2) ? nuevaGeneracion[i][j] = parametros->generacion[indice1][j]
                                                        : nuevaGeneracion[i][j] = parametros->generacion[indice2][j];
                } else {
                    (j <= parametros->tamCromosoma / 2) ? nuevaGeneracion[i][j] = parametros->generacion[indice2][j]
                                                        : nuevaGeneracion[i][j] = parametros->generacion[indice1][j];
                }
            }
        }
    }

    // Liberar la generación antigua
    for(int i = 0; i < parametros->tamPoblacion; i++) {
        free(parametros->generacion[i]);
    }
    free(parametros->generacion);

    //Cambiando la generacion anterior con la nueva
    parametros->generacion = nuevaGeneracion;
}

void cruzarTorneo(struct parametros* parametros) {
    int indice1, indice2;

    // Reservar memoria para los individuos seleccionados
    int** individuosSeleccionados = (int**) malloc(parametros->tamPoblacion * sizeof(int*));
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        individuosSeleccionados[i] = (int*) malloc(parametros->tamCromosoma * sizeof(int));
    }

    // Seleccionar el mejor individuo y ponerlo en el primer lugar
    int mejor = mejorIndividuo(parametros);
    for (int j = 0; j < parametros->tamCromosoma; j++) {
        individuosSeleccionados[0][j] = parametros->generacion[mejor][j];
    }

    // Seleccionar el resto de los individuos
    for (int i = 1; i < parametros->tamPoblacion; i++) {
        do {
            indice1 = rand() % parametros->tamPoblacion;
            indice2 = rand() % parametros->tamPoblacion;
        } while (indice1 == indice2);

        // Escoger el mejor padre de los dos
        (parametros->fitness[indice1] >= parametros->fitness[indice2]) ?
        memcpy(individuosSeleccionados[i], parametros->generacion[indice1], parametros->tamCromosoma * sizeof(int)) :
        memcpy(individuosSeleccionados[i], parametros->generacion[indice2], parametros->tamCromosoma * sizeof(int));
    }

    // Reservar memoria para la nueva generación
    int** nuevaGeneracion = (int**) malloc(parametros->tamPoblacion * sizeof(int*));
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        nuevaGeneracion[i] = (int*) malloc(parametros->tamCromosoma * sizeof(int));
    }

    // Copiar el mejor individuo directamente a la nueva generación
    for (int j = 0; j < parametros->tamCromosoma; j++) {
        nuevaGeneracion[0][j] = individuosSeleccionados[0][j];
    }

    // Cruzar el resto de los individuos
    for (int i = 1; i < parametros->tamPoblacion; i++) {
        do {
            indice1 = rand() % parametros->tamPoblacion;
            indice2 = rand() % parametros->tamPoblacion;
        } while (indice1 == indice2);

        for (int j = 0; j < parametros->tamCromosoma; j++) {
            if (i % 2) {
                nuevaGeneracion[i][j] = (j <= parametros->tamCromosoma / 2) ?
                                        individuosSeleccionados[indice1][j] : individuosSeleccionados[indice2][j];
            } else {
                nuevaGeneracion[i][j] = (j <= parametros->tamCromosoma / 2) ?
                                        individuosSeleccionados[indice2][j] : individuosSeleccionados[indice1][j];
            }
        }
    }

    // Liberar memoria de la generación anterior y los individuos seleccionados
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        free(parametros->generacion[i]);
        free(individuosSeleccionados[i]);
    }
    free(parametros->generacion);
    free(individuosSeleccionados);

    // Asignar la nueva generación
    parametros->generacion = nuevaGeneracion;
}


void mutacion(struct parametros* parametros) {
    int cantidadMutaciones, indiceAleatorio;
    for (int i = 1; i < parametros->tamPoblacion; i++) {
        if ((float)rand() / RAND_MAX <= parametros->probMutacion) {
            cantidadMutaciones = (rand() / (RAND_MAX / 3 + 1)) + 1;
            for (int k = 0; k < cantidadMutaciones; k++) {
                indiceAleatorio = rand() % parametros->tamCromosoma;
                parametros->generacion[i][indiceAleatorio] = 1 - parametros->generacion[i][indiceAleatorio];
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

// Función para ordenar la generación por fitness
void ordenar_por_fitness(struct parametros* parametros) {
    int indicesFitness[parametros->tamPoblacion];

    // Inicializar indices desde 0 hasta tamPoblacion-1
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        indicesFitness[i] = i;
    }

    int temp;
    int indiceTemp;

    // Bubble sort para ordenar fitness e indicesFitness
    for (int i = 0; i < parametros->tamPoblacion - 1; i++) {
        for (int j = 0; j < parametros->tamPoblacion - i - 1; j++) {
            if (parametros->fitness[j] < parametros->fitness[j + 1]) {
                // Intercambiar valores de fitness
                temp = parametros->fitness[j];
                parametros->fitness[j] = parametros->fitness[j + 1];
                parametros->fitness[j + 1] = temp;

                // Intercambiar los índices correspondientes
                indiceTemp = indicesFitness[j];
                indicesFitness[j] = indicesFitness[j + 1];
                indicesFitness[j + 1] = indiceTemp;
            }
        }
    }

    // Crear nueva generación basada en el orden de indicesFitness
    int** nuevaGeneracion = (int**) malloc(parametros->tamPoblacion * sizeof(int*));
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        nuevaGeneracion[i] = (int*) malloc(parametros->tamCromosoma * sizeof(int));
    }

    // Rellenar la nueva generación con el orden correcto
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        for (int j = 0; j < parametros->tamCromosoma; j++) {
            nuevaGeneracion[i][j] = parametros->generacion[indicesFitness[i]][j];
        }
    }

    // Liberar la memoria de la generación anterior
    for (int i = 0; i < parametros->tamPoblacion; i++) {
        free(parametros->generacion[i]);
    }
    free(parametros->generacion);

    // Asignar nueva generación a la variable original
    parametros->generacion = nuevaGeneracion;
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
                case 8:{
                    parametros->porcientoTruncamiento = atoi(buffer);
                    printf("\nporcientoTruncamiento: %d",parametros->porcientoTruncamiento);
                    break;
                }
                case 9:{
                    parametros->opcionCruzamiento = atoi(buffer);
                    printf("\nopcionCruza: %d",parametros->opcionCruzamiento);
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

    do{
        printf("Ingrese la cantidad tipo int para porcentaje de poblacion usada para cruze por truncamiento (Entre 0 y 100): ");
        scanf("%d", &parametros->porcientoTruncamiento);
    }while(parametros->porcientoTruncamiento < 0 or parametros->porcientoTruncamiento > 100);
}