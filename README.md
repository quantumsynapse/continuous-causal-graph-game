# Teoria dos Jogos Causais em Grafos Contínuos para Dissuasão Econômica Mínima

## Formulação

Considere um grafo causal dirigido e ponderado

\[
G(t) = \bigl(V,E,W(t),S(t)\bigr),
\]

com \(|V|=n\), matriz de acoplamento causal \(W(t)\in\mathbb{R}^{n\times n}\) e matriz de sensibilidade estratégica \(S(t)\in\mathbb{R}^{n\times n}\). O estado econômico agregado do agente \(B\) é representado por

\[
x(t)\in\mathbb{R}_{\ge 0}^{n},
\]

onde cada componente codifica dano, perda de capacidade, degradação logística, fragilidade financeira, dependência tecnológica ou vulnerabilidade sistêmica associada a um vértice do grafo.

O agente \(A\) aplica uma política de pressão

\[
u_A(t)\in\mathbb{R}_{\ge 0}^{n},
\]

e o agente \(B\) seleciona uma política de intervenção

\[
u_B(t)\in\mathbb{R}_{\ge 0}^{n}.
\]

A dinâmica nominal é

\[
\dot{x}(t)=
\alpha W^{\mathsf T}x(t)
+
\operatorname{diag}(1+v)u_A(t)
-
\rho x(t)
-
\gamma u_B(t)
-
\eta S^{\mathsf T}u_B(t),
\]

com \(v\in\mathbb{R}_{\ge0}^{n}\) representando vulnerabilidades estruturais, \(\alpha>0\) o ganho de propagação causal, \(\rho>0\) a taxa endógena de recuperação, \(\gamma>0\) o ganho direto de mitigação e \(\eta>0\) o ganho de amortecimento distribuído.

A discretização explícita utilizada é

\[
x_{k+1}
=
\Pi_{\mathbb{R}_{\ge0}^{n}}
\left[
 x_k+\Delta t\,f(x_k,u_{A,k},u_{B,k})
\right],
\]

onde \(\Pi\) é a projeção no ortante não negativo.

## Jogo diferencial

O funcional do agente \(B\) é

\[
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
\right)dt
+
x(T)^{\mathsf T}Q_Tx(T).
\]

O primeiro termo penaliza dano acumulado. O segundo restringe magnitude e custo das intervenções. O terceiro penaliza acoplamento direto entre pressão e resposta, tratado como proxy de escalada. O quarto penaliza concentração causal e exposição estrutural.

O agente \(A\) maximiza o retorno econômico de sua política sob custo próprio:

\[
J_A
=
\int_0^T
\left(
 r_A(x,u_A)
-
 c_A(u_A)
-
 d_A(u_B)
\right)dt.
\]

A condição de dissuasão é obtida quando

\[
\mathbb{E}[J_A\mid u_B^{\star}]
\le
\mathbb{E}[J_A\mid u_A=0],
\]

sem que

\[
\lVert u_B^{\star}\rVert_2
\]

atinja níveis associados a retaliação econômica de grande magnitude.

## Política de intervenção causal mínima

A implementação utiliza uma aproximação esparsa para a política de \(B\). Para cada vértice \(i\), define-se o escore

\[
\sigma_i
=
 u_{A,i}
(1+x_i)
\left[
1
+
\sum_j |W_{ij}|(1+S_{ij})
+
\frac{1}{2}\sum_j |W_{ji}|
\right].
\]

A intervenção é aplicada somente aos vértices com maior \(\sigma_i\), limitando o suporte de \(u_B\) a aproximadamente \(\sqrt{n}\) componentes. Para cada componente selecionado,

\[
u_{B,i}^{\star}
=
\operatorname{sat}_{[0,u_{\max}]}
\left(
\frac{\beta\sigma_i}
{\lambda_U+\lambda_Eu_{A,i}+\varepsilon}
\right).
\]

A regra aproxima uma política de controle preditivo esparso com penalização de energia e escalada. O mecanismo atua sobre cortes causais de alta centralidade estratégica e busca reduzir o retorno marginal da política de \(A\) com o menor suporte de intervenção possível.

## Métricas

A saída padrão contém a trajetória temporal dos estados. A saída de erro contém

\[
D_T=\int_0^T\lVert x(t)\rVert_2^2dt,
\]

\[
U_T=\int_0^T\lVert u_B(t)\rVert_2^2dt,
\]

\[
E_T=\int_0^Tu_A(t)^{\mathsf T}u_B(t)dt,
\]

\[
D_f=\lVert x(T)\rVert_2^2,
\]

\[
I_D=
\frac{1}
{1+D_f+\mathbf{1}^{\mathsf T}u_A(T)}.
\]

\(I_D\) é um índice adimensional de dissuasão residual. Valores maiores indicam menor dano terminal e menor pressão remanescente.

## Estrutura dos dados

O arquivo de grafo possui o formato

```text
n_nodes n_edges
from to weight sensitivity
...
```

O arquivo de parâmetros possui pares chave-valor

```text
dt value
horizon value
alpha value
beta value
gamma value
lambda_u value
lambda_e value
lambda_c value
pressure_gain value
recovery_gain value
control_limit value
sparsity_threshold value
max_iterations value
```

## Compilação

```sh
make
```

## Execução

```sh
./causal_game data/graph.txt data/config.txt > trajectory.csv 2> metrics.txt
```

## Ambiente

Sistema operacional: Linux

Compilador: GCC

Padrão da linguagem: ISO C90

Dependências externas: nenhuma

Complexidade temporal por passo:

\[
O(n^2+n\log n)
\]

na formulação conceitual e

\[
O(n^2)
\]

na implementação atual por varredura determinística.

Complexidade espacial:

\[
O(n^2+|E|).
\]
