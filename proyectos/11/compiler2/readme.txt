COMPILADOR DE LENGUAJE DE PROGRAMACIÓN JACK
- Autor: Hamilton Tobón Mosquera

1- Software requerido: 
   - gcc (GNU Compiler Colection): En específico g++.
      https://gcc.gnu.org/
      
   - flex (The fast Lexical Analyzer): Analizador léxico.
      https://github.com/westes/flex
      
   - bison (GNU Bison): Analizador sintáctico.
      https://www.gnu.org/software/bison/

   - La ruta a cada uno de los ejecutables de los programas anteriores debe estar incluida en la variable de ambiente PATH,
     para poder compilar correctamente el programa.

2- Compilación
   1. Diríjase al directorio src/
   2. Ejecute el comando make
      $ make
      El ejecutable del programa será generado en el mismo directorio src/ con el nombre "compiler" ("compiler.exe" si usa Windows).

3- Ejecución
   1. Ejecute el comando ./compiler.exe seguido del directorio (o ficher) que tiene los ficheros con extensión .jack
      $ ./compiler.exe ruta/al/directorio/o/fichero
   2. En el mismo directorio que introdujo como parámetro del comando anterior, será generado un fichero .vm por cada fichero .jack encontrado.
