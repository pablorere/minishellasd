*Este proyecto ha sido creado como parte del currículo de 42 por pablorere.*

# minishell

## Descripción

minishell es una implementación de un shell UNIX simplificado, desarrollado como proyecto del currículo de 42. El objetivo del proyecto es comprender cómo funcionan los shells como bash: cómo analizan la entrada del usuario, gestionan procesos, redirigen la entrada/salida y expanden variables de entorno.

### Características implementadas

- Prompt interactivo con historial de comandos (readline)
- Búsqueda y ejecución de ejecutables mediante la variable `PATH` o rutas relativas/absolutas
- Gestión de comillas simples (`'`) y dobles (`"`)
- Expansión de variables de entorno (`$VAR`, `$?`)
- Redirecciones: `<` (entrada), `>` (salida), `>>` (salida en modo append), `<<` (heredoc)
- Pipes (`|`) para encadenar comandos
- Señales: `ctrl-C` (nueva entrada), `ctrl-D` (salir del shell), `ctrl-\` (no hace nada)
- Una única variable global para la recepción de señales
- Built-ins: `echo` (con `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit`

## Instrucciones

### Requisitos

- Sistema operativo: Linux / macOS
- Compilador: `cc` (gcc o clang)
- Librería: `readline` (instalar con `sudo apt install libreadline-dev` en Ubuntu/Debian o `brew install readline` en macOS)

### Compilación

```bash
make
```

Esto compilará la librería `libft` y el ejecutable `minishell`.

### Ejecución

```bash
./minishell
```

Aparecerá el prompt `minishell$`. Se pueden introducir comandos como en bash.

### Limpieza

```bash
make clean   # Elimina archivos objeto
make fclean  # Elimina archivos objeto y el ejecutable
make re      # Recompila desde cero
```

## Recursos

### Referencias

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — documentación oficial de bash, utilizada como referencia de comportamiento.
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/rltop.html) — documentación de la librería readline.
- [The Open Group Base Specifications (POSIX)](https://pubs.opengroup.org/onlinepubs/9699919799/) — estándar POSIX para llamadas al sistema y comportamiento del shell.
- [Writing Your Own Shell](https://www.cs.cornell.edu/courses/cs414/2004su/homework/shell/shell.html) — tutorial sobre implementación de shells.
- [Advanced Programming in the UNIX Environment (Stevens)](https://www.apuebook.com/) — referencia clásica sobre programación en UNIX.

### Uso de IA

Se ha utilizado IA (GitHub Copilot) para las siguientes tareas:
- Generación inicial de la estructura del proyecto y los ficheros fuente.
- Implementación de la lógica del lexer, parser, expander y executor.
- Revisión y corrección de errores de compilación.
- Documentación del proyecto (este README).
