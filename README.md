*Este proyecto ha sido creado como parte del currículo de 42 por pablorere.*

# minishell

## Descripción

Este repositorio contiene una base mínima para el proyecto **minishell**. El
programa muestra un prompt interactivo, mantiene el historial de comandos y
gestiona señales básicas como `Ctrl-C`, `Ctrl-D` y `Ctrl-\` de forma similar a
bash en modo interactivo.

## Compilación

```sh
make
```

## Uso

```sh
./minishell
```

## Desarrollo

La base actual se centra en el loop interactivo y en preparar el entorno para
integrar el resto de requisitos del enunciado (redirecciones, pipes, built-ins,
expansión de variables, etc.).
