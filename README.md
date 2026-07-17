Library
/
README.md


# Teoria dos Jogos Causais em Grafos Contínuos para Dissuasão Econômica Mínima

Modelo computacional de jogo diferencial em grafo causal contínuo para analisar pressão econômica, propagação sistêmica de dano e intervenções seletivas de baixo custo.

A implementação é determinística, escrita em C90 e utiliza apenas GCC, GNU Make e a biblioteca matemática padrão.

## Objetivo

O sistema representa dependências econômicas como um grafo dirigido e ponderado. Dois agentes interagem sobre esse grafo:

- **Agente A** aplica uma política de pressão econômica.
- **Agente B** seleciona intervenções para reduzir dano, propagação e exposição sistêmica.

O objetivo é calcular uma política de intervenção limitada que estabilize a dinâmica, concentre recursos nos vértices mais relevantes e reduza o retorno marginal da política de pressão.

## Principais características

- Grafo causal dirigido e ponderado.
- Dinâmica contínua com discretização explícita.
- Jogo diferencial entre pressão e mitigação.
- Política de intervenção esparsa.
- Priorização por vulnerabilidade, estado e conectividade causal.
- Implementação determinística em C90.
- Entrada configurável por arquivos de texto.
- Trajetória e métricas gravadas separadamente.
- Complexidade dominante O(K n²) para matriz densa.

## Modelo causal

O sistema utiliza o grafo:

```text
G(t) = (V, E, W(t), S(t))
```

onde:

- `V` é o conjunto de vértices econômicos;
- `E` é o conjunto de relações causais;
- `W(t)` é a matriz de acoplamento causal;
- `S(t)` é a matriz de sensibilidade estratégica;
- `n = |V|` é o número de vértices.

O estado econômico agregado do agente B é:

```text
x(t) ∈ Rⁿ, com x(t) ≥ 0
```

Cada componente pode representar dano, perda de capacidade, degradação logística, fragilidade financeira, dependência tecnológica ou vulnerabilidade sistêmica.

As políticas dos agentes são:

```text
u_A(t) ∈ Rⁿ, com u_A(t) ≥ 0
u_B(t) ∈ Rⁿ, com u_B(t) ≥ 0
```

## Dinâmica do sistema

A dinâmica nominal é:

```text
x_dot(t) = α Wᵀ x(t)
           + diag(1 + v) u_A(t)
           - ρ x(t)
           - γ u_B(t)
           - η Sᵀ u_B(t)
```

Parâmetros:

- `v`: vulnerabilidades estruturais;
- `α`: ganho de propagação causal;
- `ρ`: taxa endógena de recuperação;
- `γ`: ganho direto de mitigação;
- `η`: ganho de amortecimento distribuído.

A atualização numérica é:

```text
x_(k+1) = projection_nonnegative(
            x_k + Δt f(x_k, u_A,k, u_B,k)
          )
```

A projeção mantém todos os estados no domínio não negativo.

## Jogo diferencial

O custo do agente B é:

```text
J_B = integral de 0 a T de
      [xᵀQx + u_BᵀRu_B + λ_E u_Aᵀu_B + λ_C Φ(G,x)] dt
      + x(T)ᵀ Q_T x(T)
```

Esse funcional combina:

- dano acumulado;
- custo das intervenções;
- penalização de escalada;
- concentração causal e exposição estrutural;
- dano terminal.

O retorno do agente A é:

```text
J_A = integral de 0 a T de
      [r_A(x,u_A) - c_A(u_A) - d_A(u_B)] dt
```

A condição operacional de dissuasão é:

```text
E[J_A | u_B*] ≤ E[J_A | u_A = 0]
```

sujeita à restrição:

```text
||u_B*||₂ < u_ret
```

## Política de intervenção causal mínima

Para cada vértice `i`, calcula-se o escore estratégico:

```text
σ_i = u_A,i (1 + x_i)
      [1
       + Σ_j |W_ij| (1 + S_ij)
       + 0.5 Σ_j |W_ji|]
```

A política atua somente nos vértices com maior escore, limitando o suporte de `u_B` a aproximadamente `sqrt(n)` componentes.

Para cada componente selecionado:

```text
u_B,i* = sat_[0,u_max](
           β σ_i / (λ_U + λ_E u_A,i + ε)
         )
```

A regra aproxima um controle preditivo esparso com penalização de energia e escalada.

## Métricas

Dano acumulado:

```text
D_T = integral de 0 a T de ||x(t)||₂² dt
```

Esforço acumulado de controle:

```text
U_T = integral de 0 a T de ||u_B(t)||₂² dt
```

Acoplamento entre pressão e resposta:

```text
E_T = integral de 0 a T de u_A(t)ᵀ u_B(t) dt
```

Dano terminal:

```text
D_f = ||x(T)||₂²
```

Índice de dissuasão residual:

```text
I_D = 1 / [1 + D_f + 1ᵀu_A(T)]
```

Valores maiores de `I_D` indicam menor dano terminal e menor pressão econômica remanescente.

## Resultado de referência

Configuração utilizada:

- horizonte temporal `T = 40`;
- passo temporal `Δt = 0.01`;
- `4000` iterações.

| Métrica | Resultado |
|:---|---:|
| `D_T` | `27.101402` |
| `U_T` | `11.614500` |
| `E_T` | `8.449700` |
| `D_f` | `0.205755` |
| `I_D` | `0.454799` |

Todos os estados econômicos terminais permaneceram abaixo de aproximadamente `0.21`.

O resultado caracteriza estabilização dinâmica com intervenção seletiva e limitada. Ele não constitui, isoladamente, prova de equilíbrio de Nash nem demonstração formal de dissuasão ótima.

## Estrutura do projeto

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

## Requisitos

- Linux
- GCC
- GNU Make
- biblioteca matemática padrão

## Compilação

```bash
make clean
make
```

A compilação utiliza:

```text
gcc -std=c89 -pedantic -Wall -Wextra -O2
```

## Execução

```bash
./causal_game data/graph.txt data/config.txt \
  > trajectory.csv \
  2> metrics.txt
```

Arquivos gerados:

- `trajectory.csv`: trajetória temporal dos estados;
- `metrics.txt`: métricas agregadas da simulação.

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

Para uma representação densa do grafo:

- atualização dinâmica por passo: `O(n²)`;
- ordenação dos escores: `O(n log n)`;
- simulação completa com `K = T / Δt` passos: `O(K n²)`;
- memória: `O(n²)`.

A atualização matricial domina o custo total.

## Escopo e limitações

Este repositório implementa um modelo experimental para análise computacional de estabilidade, propagação causal e intervenção seletiva em sistemas econômicos conectados.

O modelo é adequado para:

- experimentos com grafos causais sintéticos;
- avaliação de políticas esparsas;
- análise de sensibilidade;
- comparação entre estratégias de pressão e mitigação;
- estudo de estabilidade sob diferentes topologias e parâmetros.

As saídas devem ser interpretadas como resultados de simulação, não como previsão econômica automática nem recomendação operacional.

