compiladores2
=============

Tarea de la Materia Compiladores FPUNA 2014

compilar
========

gcc *.c -o syntax.bin

testing
=======

Por cuestiones de tiempo solo pude testear fuente1.txt

Sin embargo en la linea 16 hay un
alert(msg)

revisando la gramatica me doy cuenta que

ALERT -> alert parametros
PARAMETROS -> valor , parametros | valor | <vacio>

Es decir no hay parentesis en la gramatica luego del alert

Si se quita dichos parentesis, el INPUT es satisfactorio para el programa.
