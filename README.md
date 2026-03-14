# Magical Paths

```c
#define SUBJECT "Projeto e Análise de Algoritmos"
#define WORK "A Busca por Caminhos Mágicos"
#define PROFESSOR "Leonardo Rocha"
#define STUDENTS ["Caren Ferreira", "Gabriel de Paula", "Rodrigo Zonzin"]
```

[`> Click here to see the documentation <`](./docs/documentation-ptbr.pdf)

&nbsp;

## Running the program

Run the makefile to generate the compilation objects and the final executable `tp`:

```bash
make
```

To run the program you must provide `{input_path}` and `{output_path}` with the flags `-i` and `-o`:

```bash
./bin/tp -i {input_path} -o {output_path}
```

&nbsp;

## Memory leak tests

You must have `valgrind` installed in your machine:

```bash
sudo apt install valgrind
```

And normally run the program using it:

```bash
valgrind ./bin/tp -i {input_path} -o {output_path}
```

It is expected to display:\
`All heap blocks were freed -- no leaks are possible`
