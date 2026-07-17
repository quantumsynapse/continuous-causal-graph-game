# Teoria dos Jogos Causais em Grafos Contínuos para Dissuasão Econômica Mínima

## Formulação

Considere um grafo causal dirigido e ponderado

$$
G(t)=\bigl(V,E,W(t),S(t)\bigr),
$$

com $|V|=n$, matriz de acoplamento causal $W(t)\in\mathbb{R}^{n\times n}$ e matriz de sensibilidade estratégica $S(t)\in\mathbb{R}^{n\times n}$.

O estado econômico agregado do agente $B$ é representado por

$$
x(t)\in\mathbb{R}_{\geq 0}^{n},
$$

onde cada componente codifica dano, perda de capacidade, degradação logística, fragilidade financeira, dependência tecnológica ou vulnerabilidade sistêmica associada a um vértice do grafo.

O agente $A$ aplica uma política de pressão

$$
u_A(t)\in\mathbb{R}_{\geq 0}^{n},
$$

enquanto o agente $B$ seleciona uma política de intervenção

$$
u_B(t)\in\mathbb{R}_{\geq 0}^{n}.
$$

A dinâmica nominal é

$$
\dot{x}(t)
=
\alpha W^{\mathsf T}x(t)
+
\operatorname{diag}(1+v)u_A(t)
-
\rho x(t)
-
\gamma u_B(t)
-
\eta S^{\mathsf T}u_B(t),
$$

com $v\in\mathbb{R}_{\geq 0}^{n}$ representando vulnerabilidades estruturais, $\alpha>0$ o ganho de propagação causal, $\rho>0$ a taxa endógena de recuperação, $\gamma>0$ o ganho direto de mitigação e $\eta>0$ o ganho de amortecimento distribuído.

A discretização explícita utilizada é

$$
x_{k+1}
=
\Pi_{\mathbb{R}_{\geq 0}^{n}}
\left[
x_k
+
\Delta t\,
f\left(x_k,u_{A,k},u_{B,k}\right)
\right],
$$

onde $\Pi_{\mathbb{R}_{\geq 0}^{n}}$ representa a projeção no ortante não negativo.

## Jogo diferencial

O funcional de custo do agente $B$ é

$$
J_B
=
\int_0^T
\left(
x^{\mathsf T}Qx
+
u_B^{\mathsf T}Ru_B
+
\lambda_E u_A^{\mathsf T}u_B
+
\lambda_C\Phi(G,x)
\right)
\,dt
+
x(T)^{\mathsf T}Q_Tx(T).
$$

O primeiro termo penaliza o dano acumulado. O segundo restringe a magnitude e o custo das intervenções. O terceiro penaliza o acoplamento direto entre pressão e resposta, utilizado como proxy de escalada. O quarto penaliza concentração causal e exposição estrutural.

O agente $A$ maximiza o retorno econômico de sua política sob custo próprio:

$$
J_A
=
\int_0^T
\left(
r_A(x,u_A)
-
c_A(u_A)
-
d_A(u_B)
\right)
\,dt.
$$

A condição de dissuasão é obtida quando

$$
\mathbb{E}\left[J_A\mid u_B^{\star}\right]
\leq
\mathbb{E}\left[J_A\mid u_A=0\right],
$$

sem que

$$
\left\lVert u_B^{\star}\right\rVert_2
$$

atinja níveis associados a uma retaliação econômica de grande magnitude.

## Política de intervenção causal mínima

A implementação utiliza uma aproximação esparsa para a política do agente $B$. Para cada vértice $i$, define-se o escore

$$
\sigma_i
=
u_{A,i}(1+x_i)
\left[
1
+
\sum_j
\left|W_{ij}\right|
\left(1+S_{ij}\right)
+
\frac{1}{2}
\sum_j
\left|W_{ji}\right|
\right].
$$

A intervenção é aplicada somente aos vértices com maior valor de $\sigma_i$, limitando o suporte de $u_B$ a aproximadamente $\sqrt{n}$ componentes.

Para cada componente selecionado,

$$
u_{B,i}^{\star}
=
\operatorname{sat}_{[0,u_{\max}]}
\left(
\frac{
\beta\sigma_i
}{
\lambda_U
+
\lambda_Eu_{A,i}
+
\varepsilon
}
\right).
$$

A regra aproxima uma política de controle preditivo esparso com penalização de energia e escalada. O mecanismo atua sobre cortes causais de alta centralidade estratégica e busca reduzir o retorno marginal da política de $A$ com o menor suporte de intervenção possível.

## Métricas

A saída padrão contém a trajetória temporal dos estados. A saída de erro contém as métricas agregadas.

Dano acumulado:

$$
D_T
=
\int_0^T
\left\lVert x(t)\right\rVert_2^2
\,dt.
$$

Esforço acumulado de controle:

$$
U_T
=
\int_0^T
\left\lVert u_B(t)\right\rVert_2^2
\,dt.
$$

Acoplamento entre pressão e resposta:

$$
E_T
=
\int_0^T
u_A(t)^{\mathsf T}u_B(t)
\,dt.
$$

Dano terminal:

$$
D_f
=
\left\lVert x(T)\right\rVert_2^2.
$$

Índice de dissuasão residual:

$$
I_D
=
\frac{
1
}{
1
+
D_f
+
\mathbf{1}^{\mathsf T}u_A(T)
}.
$$

O índice $I_D$ é adimensional. Valores maiores indicam menor dano terminal e menor pressão econômica remanescente.

## Resultado da configuração de referência

Para uma simulação com $T=40$, passo temporal $\Delta t=0{,}01$ e $4\,000$ iterações, foram obtidos:

| Métrica | Resultado |
|---|---:|
| $D_T$ | $27{,}101402$ |
| $U_T$ | $11{,}614500$ |
| $E_T$ | $8{,}449700$ |
| $D_f$ | $0{,}205755$ |
| $I_D$ | $0{,}454799$ |

A configuração de referência manteve todos os estados econômicos terminais abaixo de aproximadamente $0{,}21$.

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
