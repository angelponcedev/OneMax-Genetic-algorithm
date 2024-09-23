Este programa implementa un algoritmo genético para resolver el problema de 'One Max',
que busca maximizar el número de 1s en una cadena de bits. El algoritmo utiliza técnicas
de seleccion, cruzamiento y mutacion para evolucionar una población de soluciones.
El programa permite ajustar varios parámetros para personalizar el runtime del algoritmo.

La funcionalidad principal incluye la generación de nuevos individuos, la evaluación de su fitness,
y la creación de la siguiente generación basada en los individuos más aptos.

--- OPCIONES DEL PROGRAMA ---
El programa dispone de operadores de seleccion y operadores de cruza, debera escoger un numero del
listado para que sea valida su lectura en el archivo de parametros.

Opciones disponibles para la seleccion y Cruza:

    MENU SELECCION
        1.-Seleccion por Ruleta
        2.-Seleccion por Truncamiento
        3.-Seleccion por Torneo
        4.-Seleccion Estocastica

    MENU CRUZA
        1.-Cruza por Segmentos
        2.-Cruza Uniforme
        3.-Cruza por Mascara Aleatoria
        4.-Cruza por Inversion [Operador original]

--- ESTRUCTURA DEL ARCHIVO DE PARAMETROS ---
Es importante seguir la estructura de los datos proporcionada para que puedan leerse correctamente,
De lo contrario se asignaran valores por defecto a los parametros de ejecucion.

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
Opcion de seleccion (entero entre 1 y 4)
Opcion de cruzamiento (entero entre 1 y 4)

Se incluye un archivo ejemplo 'parametros.txt' como referencia de un archivo que puede leer el programa.