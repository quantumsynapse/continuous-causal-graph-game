# Teoria dos Jogos Causais em Grafos Contínuos para Dissuasão Econômica Mínima

## Formulação

Considere um grafo causal dirigido e ponderado

```math
G(t)=\bigl(V,E,W(t),S(t)\bigr),
```

com $|V|=n$, matriz de acoplamento causal $W(t)\in\mathbb{R}^{n\times n}$ e matriz de sensibilidade estratégica $S(t)\in\mathbb{R}^{n\times n}$.

O estado econômico agregado do agente $B$ é representado por

```math
x(t)\in\mathbb{R}_{\geq 0}^{n},
```

onde cada componente codifica dano, perda de capacidade, degradação logística, fragilidade financeira, dependência tecnológica ou vulnerabilidade sistêmica associada a um vértice do grafo.

O agente $A$ aplica uma política de pressão

```math
u_A(t)\in\mathbb{R}_{\geq 0}^{n},
```

enquanto o agente $B$ seleciona uma política de intervenção

```math
u_B(t)\in\mathbb{R}_{\geq 0}^{n}.
```

A dinâmica nominal é

```math
\dot{x}(t)
=
\alpha W^{\top}x(t)
+
\mathrm{diag}(1+v)u_A(t)
-
\rho x(t)
-
\gamma u_B(t)
-
\eta S^{\top}u_B(t).
```

O vetor $v\in\mathbb{R}_{\geq 0}^{n}$ representa vulnerabilidades estruturais. O parâmetro $\alpha>0$ define o ganho de propagação causal, $\rho>0$ a taxa endógena de recuperação, $\gamma>0$ o ganho direto de mitigação e $\eta>0$ o ganho de amortecimento distribuído.

A discretização explícita utilizada é

```math
x_{k+1}
=
\Pi_{\mathbb{R}_{\geq 0}^{n}}
[
x_k
+
\Delta t\,
f(x_k,u_{A,k},u_{B,k})
)].
```

O operador $\Pi_{\mathbb{R}_{\geq 0}^{n}}$ representa a projeção no ortante não negativo.

## Jogo diferencial

O funcional de custo do agente $B$ é

```math
J_B
=
\int_0^T
(
x^{\top}Qx
+
u_B^{\top}Ru_B
+
\lambda_Eu_A^{\top}u_B
+
\lambda_C\Phi(G,x)
)
\,dt
+
x(T)^{\top}Q_Tx(T).
```

O primeiro termo penaliza o dano acumulado. O segundo restringe a magnitude e o custo das intervenções. O terceiro penaliza o acoplamento direto entre pressão e resposta, utilizado como aproximação da escalada. O quarto penaliza concentração causal e exposição estrutural.

O agente $A$ maximiza o retorno econômico de sua política sob custo próprio:

```math
J_A
=
\int_0^T
(
r_A(x,u_A)
-
c_A(u_A)
-
d_A(u_B)
)
\,dt.
```

A condição de dissuasão é

```math
\mathbb{E}[J_A\mid u_B^{\star})]
\leq
\mathbb{E}[J_A\mid u_A=0)],
```

sob a restrição

```math
\left\lVert u_B^{\star})\rVert_2
<
u_{\mathrm{ret}},
```

onde $u_{\mathrm{ret}}$ representa o limiar associado a uma resposta econômica de grande magnitude.

## Política de intervenção causal mínima

A implementação utiliza uma aproximação esparsa para a política do agente $B$. Para cada vértice $i$, define-se o escore

```math
\sigma_i
=
u_{A,i}(1+x_i)
[
1
+
\sum_j
|W_{ij})|
(1+S_{ij})
+
\frac{1}{2}
\sum_j
|W_{ji})|
)].
```

A intervenção é aplicada somente aos vértices com maior valor de $\sigma_i$, limitando o suporte de $u_B$ a aproximadamente $\sqrt{n}$ componentes.

Para cada componente selecionado,

```math
u_{B,i}^{\star}
=
\mathrm{sat}_{[0,u_{\max}]}
(
\frac{
\beta\sigma_i
}{
\lambda_U
+
\lambda_Eu_{A,i}
+
\varepsilon
}
).
```

A regra aproxima uma política de controle preditivo esparso com penalização de energia e escalada. O mecanismo atua sobre vértices de elevada centralidade estratégica e busca reduzir o retorno marginal da política de $A$ com o menor suporte de intervenção possível.

## Métricas

A saída padrão contém a trajetória temporal dos estados. A saída de erro contém as métricas agregadas.

Dano acumulado:

```math
D_T
=
\int_0^T
\left\lVert x(t))\rVert_2^2
\,dt.
```

Esforço acumulado de controle:

```math
U_T
=
\int_0^T
\left\lVert u_B(t))\rVert_2^2
\,dt.
```

Acoplamento entre pressão e resposta:

```math
E_T
=
\int_0^T
u_A(t)^{\top}u_B(t)
\,dt.
```

Dano terminal:

```math
D_f
=
\left\lVert x(T))\rVert_2^2.
```

Índice de dissuasão residual:

```math
I_D
=
\frac{
1
}{
1
+
D_f
+
\mathbf{1}^{\top}u_A(T)
}.
```

O índice $I_D$ é adimensional. Valores maiores indicam menor dano terminal e menor pressão econômica remanescente.

## Resultado da configuração de referência

Para uma simulação com $T=40$, passo temporal $\Delta t=0.01$ e $4000$ iterações, foram obtidos:

| Métrica | Resultado |
|:---|---:|
| $D_T$ | $27.101402$ |
| $U_T$ | $11.614500$ |
| $E_T$ | $8.449700$ |
| $D_f$ | $0.205755$ |
| $I_D$ | $0.454799$ |

A configuração de referência manteve todos os estados econômicos terminais abaixo de aproximadamente $0.21$.

O resultado caracteriza estabilização dinâmica com intervenção seletiva e limitada. Não constitui, isoladamente, prova de equilíbrio de Nash ou demonstração formal de dissuasão ótima.

## Estrutura do sistema

```text
.
├── Makefile
├── README.md
├── data
│   ├── config.txt
│   └── graph.txt
├── include
│   └── model.h
└── src
    ├── dynamics.c
    ├── game.c
    ├── graph.c
    ├── io.c
    └── main.c
```

## Compilação

O sistema requer Linux, GCC e a biblioteca matemática padrão.

```bash
make clean
make
```

A compilação utiliza C90:

```text
gcc -std=c89 -pedantic -Wall -Wextra -O2
```

## Execução

```bash
./causal_game data/graph.txt data/config.txt > trajectory.csv 2> metrics.txt
```

A trajetória temporal é gravada em `trajectory.csv`. As métricas agregadas são gravadas em `metrics.txt`.

## Verificação

```bash
cat metrics.txt
head trajectory.csv
tail trajectory.csv
```

## Limpeza

```bash
make clean
rm -f trajectory.csv metrics.txt
```

## Complexidade computacional

Para um grafo representado por matriz densa, a atualização da dinâmica possui complexidade

```math
\mathcal{O}(n^2)
```

por passo temporal.

A ordenação dos escores estratégicos possui complexidade

```math
\mathcal{O}(n\log n).
```

Para $K=T/\Delta t$ passos, a complexidade total é

```math
\mathcal{O}(Kn^2),
```

com memória

```math
\mathcal{O}(n^2).
```
