# 1948-compiler
beecrowd | 1948

## Compilador
Por Emilio Wuerges, UFFS BR Brazil

Depois do ataque dos alienígenas que acabou com quase toda a tecnologia avançada da humanidade, você foi designado a participar da força tarefa para reconstruir.

Depois de muita tentativa e erro, foi descoberto que os computadores que tinham apenas 2 registradores fonte e um destino eram imunes as radiações alienígenas. O problema agora é que nenhum programa escrito até então funcionava nestes computadores, e muito menos um compilador adequado.

Você está de posse de um dos últimos computadores de verdade que ainda funcionam, e sua tarefa será escrever o compilador. Mas como tempo é dinheiro, e tempo de CPU é muito mais dinheiro, antes de compilar o programa inteiro, você deve apenas checar se é possível compilar o programa desejado.

##Entrada

Cada entrada consiste de um programa. O programa só contem 2 tipos de instruções. Uma para declarar o valor de uma variável e outra para executar uma soma.

As instruções de declaração são no formato:

A := B

Onde A é um nome de variável válido e B é um inteiro positivo.

As instruções de soma são no formato:

A := B + C

Onde A é um nome de variável válido e B ou C são ou um nome de variável válido ou um inteiro positivo.

Os tokens deste programa são sempre separados por espaço e as instruções são separadas por uma quebra de linha.

São nomes de variáveis válidos todas as combinações de até 8 letras minúsculas.

Os programas tem, no máximo, 2000 instruções.

Variáveis recebem uma atribuição, seja através de uma soma ou de uma declaração, no máximo, 1 vez.
##Saída

A saída consistirá sempre de somente 1 linha:

OK

No caso de ser possível compilar.

Compilation error

No caso de não ser possível.

| Exemplo de Entrada | Exemplo de Saída |
| --- | --- |
|a := 0 <br> b := 1 <br> c := a + b <br> d := a + b | OK |

| Exemplo de Entrada | Exemplo de Saída |
| --- | --- |
|a := 0 <br> b := 1 <br> c := a + b <br> d := a + b <br> e := a + c <br> f := a + c | Compilation Error |
