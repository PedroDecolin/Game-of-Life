// Portuguese - Português

-=-=-=-=-=-=-=-=-=-=[ Life Game ]=-=-=-=-=-=-=-=-=-=-
Feito por :
    Gregor U. - a52981
    Pedro D. - a52982
    
Trabalho 2 | Sistemas Operativos // IPB

Data da versão final:
    15/JAN/2024
Data da última atualização:
    19/FEV/2024
-=-=-=-=-=-=-=-=-=-=-=-=-=*=-=-=-=-=-=-=-=-=-=-=-=-=-
-=-=-=-=-=-=-=-=[ Instruções Gerais ]=-=-=-=-=-=-=-=-
Esse programa é uma versão alternativa de "Conway's 
Game of Life", mais informaçoes em :
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

O código precisa ser compilado da seguinte forma :
    > gcc <ficheiro>.c -o <ficheiro>.exe lncurses

* Provavelmente será necessário instalar a 
biblioteca do lncurses com o comando :
    > sudo apt-get install libncurses-dev

Para correr o código é necessário apenas digitar :
    > ./<ficheiro>.exe <padrão>

* Necessário adaptar a compilação e a execução caso
esteja a utilizar um sistema operativo diferente de
Linux
-=-=-=-=-=-=-=-=-=-=-=-=-=*=-=-=-=-=-=-=-=-=-=-=-=-=-
-=-=-=-=-=-=-=-=-=-=-=[ Erros ]=-=-=-=-=-=-=-=-=-=-=-
Número de erro : Nome do erro
    > Causa
    < Possível solução

404 : INVALID PATTERN
    > O padrao escolhido pelo utilizador não é aceito
    < O utilizador deve escolher outro padrão
*405 : NO WORKERS
    > Não há processos filhos
    < Incrementar a constante NWORKERS (linha 29)
406 : INVALID CELL STATE
    > Uma célula assumiu um valor impossível
    < Deve-se verificar a integridade dos ficheios
407 : NOT ENOUGH ARGUMENTS
    > Um padrão não foi selecionado
    < Verificar "Instruções Gerais"
408 : TOO MANY ARGUMENTS
    > O programa aceita apenas um padrão
    < Verificar "Instruções Gerais"
*409 : MORE WORKERS THAN CELLS
    > Há mais processos filhos do que células
    < Decrementar a constante NWORKERS (linha 29)
410 : MISSING IMPORTANT FILE
    > Um ficheiro binário não existe
    < Compilar e executar "file-generator.c"

* Esses erros sao possíveis apenas na versao multi
process
-=-=-=-=-=-=-=-=-=-=-=-=-=*=-=-=-=-=-=-=-=-=-=-=-=-=-
-=-=-=-=-=-=-=-=-=-=-=-[ Fim ]-=-=-=-=-=-=-=-=-=-=-=-
Para qualquer sugestão ou problema relacionado ao
código, favor entrar em contato.
-=-=-=-=-=-=-=-=-=-=-=-=-=*=-=-=-=-=-=-=-=-=-=-=-=-=-

// English - Inglês

-=-=-=-=-=-=-=-=-=-=[ Life Game ]=-=-=-=-=-=-=-=-=-=-
Authors :
    Gregor U. - a52981
    Pedro D. - a52982
    
2nd Assignment | Operating Systems // IPB

Final release date:
    15/JAN/2024
Date of the last update:
    19/FEB/2024
-=-=-=-=-=-=-=-=-=-=-=-=-=*=-=-=-=-=-=-=-=-=-=-=-=-=-
-=-=-=-=-=-=-=-[ General Instructions ]=-=-=-=-=-=-=-
This code implements an alternative version of the 
"Conway's Game of Life", more information at :
https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

The code needs to be compiled as seen below :
    > gcc <file>.c -o <file>.exe lncurses

* You will likely need to install the lncurses'
library with the command :
    > sudo apt-get install libncurses-dev

To run the code, just use the command :
    > ./<file>.exe <pattern>

* If you are not using a Linux system, you will need
to adapt the steps above to your operating system of
choice
-=-=-=-=-=-=-=-=-=-=-=-=-=*=-=-=-=-=-=-=-=-=-=-=-=-=-
-=-=-=-=-=-=-=-=-=-=-=[ Errors ]-=-=-=-=-=-=-=-=-=-=-
Error number : Error name
    > Cause
    < Suggested solution

404 : INVALID PATTERN
    > The pattern inputed is not recognized
    < The user should choose another pattern
*405 : NO WORKERS
    > There is no child processes
    < Increment the constant NWORKERS (line 29)
406 : INVALID CELL STATE
    > A cell was assigned an impossible value
    < Check file integrity
407 : NOT ENOUGH ARGUMENTS
    > Missing pattern selection
    < Refer to the "General Instructions" section
408 : TOO MANY ARGUMENTS
    > Two or more patterns selected
    < Refer to the "General Instructions" section
*409 : MORE WORKERS THAN CELLS
    > There are more child processes than cells
    < Decrement the constant NWORKERS (line 29)
410 : MISSING IMPORTANT FILE
    > A binary file is missing
    < Compile and run "file-generator.c"

* Errors marked can only occour at the multi-process
version
-=-=-=-=-=-=-=-=-=-=-=-=-=*=-=-=-=-=-=-=-=-=-=-=-=-=-
-=-=-=-=-=-=-=-=-=-=-=-[ End ]-=-=-=-=-=-=-=-=-=-=-=-
For any suggestions or other problems with the 
implementation, please, message us.
-=-=-=-=-=-=-=-=-=-=-=-=-=*=-=-=-=-=-=-=-=-=-=-=-=-=-