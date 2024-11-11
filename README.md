# CabecarCompiler

Para ejecutar el Programa con el Generador de Código corra en el cmd el archivo que está en la carpeta Debug que se llama ParserApp (se llama igual a la etapa de parsing pero incluye todas las partes, incluidas las modificaciones para el análisis semántico y generación de código) y pásele la dirección donde se encuentra el archivo a probar junto con el nombre y la extensión wgw

Una vez ubicado en el directorio correcto, es decir el de la carpeta Compiler, luego la carpeta Debug, un ejemplo de como podría verse este comando sería así:

ParserApp.exe < dirección donde se encuentra el archivo de prueba > input.wgw

Esto ejecutará el Parser con el archivo de prueba llamado input.wgw y mostrará todo el resultado en el cmd.
Al final se muestra el arbol sintáctico, pero antes de este aparecen todos los pasos que fue realizando el Parser para generar el árbol, es decir, se mostrarán las predicciones que espera cuando analiza no-terminales y los match que va haciendo con cada token que encuentra al analizar un terminal. Ahí mismo se mostrarán los errores que aparezcan y se intentará recuperar para seguir con el proceso de compilación. 
Tamién se mostraran los errores semánticos que aparezcan durante el análisis (estos se mostraran antes del arbol sintáctico).

Una vez que termine de mostrarse el arbol sintáctico, sabemos que se generó el código ensamblador, que se habrá generado en un archivo con el mismo nombre del que se ejecutó (ejemplo: input.wgw -> input.wgw.asm), y se encontrará en la misma dirección del archivo ejecutado. Ahí podrá revisar el código ensamblador generado