Este programa implementa un algoritmo genético para resolver el problema de 'One Max',
que busca maximizar el número de 1s en una cadena de bits. El algoritmo utiliza técnicas
de selección por ruleta, cruzamiento por truncamiento y mutación para evolucionar una
población de soluciones. El programa permite ajustar varios parámetros, como el tamaño de la población,
el tamaño de los cromosomas, el número máximo de generaciones y la probabilidad de mutación.

La funcionalidad principal incluye la generación de nuevos individuos, la evaluación de su fitness,
y la creación de la siguiente generación basada en los individuos más aptos.

--- OPCIONES DEL PROGRAMA ---
El programa dispone de 7 combinaciones de seleccion y operadores de cruza, debera escoger un numero del
listado para que sea valida su lectura en el archivo de parametros.

Opciones disponibles para la seleccion y Cruza:

    1.-Seleccion por Ruleta cruzamiento por Segmentos
    2.-Seleccion por Ruleta cruzamiento Uniforme
    3.-Seleccion por Truncamiento cruzamiento por Segmentos
    4.-Seleccion por Torneo cruzamiento por Segmentos
    5.-Seleccion Estocastica cruzamiento por Segmentos
    6.-Seleccion Estocastica cruzamiento por Mascara Aleatoria
    7.-Seleccion Estocastica cruzamiento por Inversion

--- ESTRUCTURA DEL ARCHIVO DE PARAMETROS ---
ES MUY IMPORTANTE QUE LOS VALORES ESTEN DENTRO DE LOS RANGOS ESPECIFICADOS PARA EL CORRECTO FUNCIONAMIENTO
DEL PROGRAMA, ACTUALMENTE NO CUENTA CON VALIDACIONES PARA LA LECTURA DE DATOS POR ARCHIVO.

El archivo de parámetros debe estar en formato de texto plano y debe incluir los siguientes valores,
cada uno en una línea separada:

Tamaño de la población (entero mayor a 0)
Tamaño de los cromosomas (entero mayor a 3)
Número máximo de generaciones (entero mayor a 0)
Cantidad de veces que se puede repetir el mejor individuo para aumentar la mutacion (entero mayor a 0)
Probabilidad de mutación base (flotante entre 0 y 1)
Taza de  aumento de la mutación base (flotante entre 0 y 1)
Probabilidad de cruzamiento (flotante entre 0 y 1)
Porciento de poblacion utilizada para truncamiento (entero entre 1 y 100)
Opcion de cruzamiento (entero entre 1 y 7)

Se incluye un archivo ejemplo 'parametros.txt' como referencia de un archivo que puede leer el programa.