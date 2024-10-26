# CabecarCompiler

Para ejecutar el Programa con el Analizis Semántico corra en el cmd el archivo que está en la carpeta Debug que se llama ParserApp (se llama igual a la etapa anterior pero incluye todas las partes incluidas las modificaciones para el análisis semántico) y pásele la dirección donde se encuentra el archivo a probar junto con el nombre y la extención wgw

Una vez ubicado en el directorio correcto, es decir el de la carpeta Compiler, luego la carpeta Debug, un ejemplo de como podría verse este comando sería así:

ParserApp.exe < dirección donde se encuentra el archivo de prueba > Prueba_Gordita.wgw > testPG.txt

Esto ejecutará el Parser con el archivo de prueba llamado Prueba_Gordita.wgw y el resultado lo mostrará en un archivo que en este caso se definió como testPG.txt
En este archivo generado, al final se muestra el arbol sintáctico, pero antes de este aparecen todos los pasos que fue realizando el Parser para generar el árbol, es decir, se mostrarán las predicciones que espera cuando analiza no-terminales y los match que va haciendo con cada token que encuentra al analizar un terminal. Ahí mismo se mostrarán los errores que aparezcan y se intentará recuperar para seguir con el proceso de compilación. Tamién se mostraran los errores semánticos que aparezcan durante el análisis (estos se mostraran antes del arbol sintáctico).
Además de la prueba gordita hay otros archivos de prueba como el "inputCorrecto-1-wgw" que es un programa correcto y más corto para revisar más fácilmente, se recomienda también ejecutar dicha prueba.