# EvIBM

This program is a scientific iniciation project. It simulates an Individual Based Model created by Marcus Aguiar (Aguiar _et al_, 2009). The project is funded by FAPESP - Fundação de apoio a pesquisa do estado de São Paulo.

HELLO!

Welcome to my developing code for this Evolutionary Model! If you are VERY interested in the code and the model behind it, please read the DETAILED.md file.

This branch is used to test and analise the situation of the aglutination of individuals in the model. If you want to run the program and see the results, follow the instructions below

## How to compile

To compile this code you may have to install some programs before. See [instalation](#install) for more information.

### With make
Compiling like this will create a lot of files in the "position" folder and it will create numerous png files in the "plots" folder, and a final gif.

After cloning the folder, go in it and compile with the command
```bash
$ make
```

### Just compile with gcc

If you don't have anything installed or time for it, just type on the terminal

```bash
$ gcc main.c -o out
$ ./out
```

## Installation <a name="install"></a>

Here's how to install make

## References

DE AGUIAR, Marcus Aloizo Martinez et al. Global patterns of speciation and diversity. **Nature**, v. 460, n. 7253, p. 384-387, 2009.
