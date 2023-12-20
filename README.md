# cpsgui

# Utilização

Para utilizar a API CPSGUI, tenha em seu projeto o arquivo API.h, que está localizado na pasta build, e o arquivo API.lib, que está localizado dentro da pasta build, nas pastas windows e linux. Utilize o arquivo na pasta correspondente ao sistema operacional que você irá utilizar.

Em seu programa, inclua o header API.h. Lembre de utilizar aspas duplas, ou seja, ``#include "API.h"``.

## Compilação

Para compilar o seu programa, você precisará informar opções adicionais (flags) ao compilador.

Em Windows, adicione a flag -mwindows

Por exemplo, ``gcc main.c API.lib -mwindows -lgdi32``

Em Linux, adicione a flag -lX11

Por exemplo, ``gcc main.c API.lib -lX11``

Se você utiliza uma IDE, procure como especificar opções adicionais de compilação nela.
