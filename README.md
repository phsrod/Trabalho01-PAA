# 📋 Trabalho 01 - Análise Comparativa de Algoritmos de Ordenação

**Disciplina:** Projeto e Análise de Algoritmos  
**Professor:** Rai Araújo de Miranda  
**Período:** 5° - Sistemas de Informação  
**Ano/Semestre:** 2025.4  
**Universidade Federal do Piauí – Campus Senador Helvídio Nunes de Barros, Picos**

---

## 👥 Participantes do Grupo

| Nome | Responsabilidade |
|------|------------------|
| *[Luciano Sousa Barbosa]* | |
| *[Pedro Henrique Silva Rodrigues]* | |
| *[Tiago Lima de Moura]* | |

---

## 🎯 Objetivo Geral
Avaliar comparativamente algoritmos de ordenação, considerando desempenho, complexidade assintótica e sensibilidade à ordenação inicial dos dados.

---

## 🧠 Algoritmos Sorteados

| Grupo | Algoritmo 1 | Algoritmo 2 |
|-------|-------------|-------------|
| A | *[Shell Sort]* | |
| B |  | *[Cycle Sort]* |

---

## 💻 Ambiente de Desenvolvimento

| Item | Especificação |
|------|---------------|
| **Linguagem** | *[C e Python (Gráficos)]* |
| **Sistema Operacional** | *[Windows 11 Home Single Language (Executado no Ubuntu 24.04.3 LTS via WSL2)]* |
| **Hardware** | *[Processador: Core i5-12450H 12º Gen (2.00 GHz) (8 núcleos, 12 threads, 12 MB cache) Memória RAM: 16,0 GB @ 3200 MHz (utilizável: 15,7 GB)]* |
| **IDE/Editor** | *[VSCode]* |

---

## 📁 Estrutura do Projeto

```text
TRABALHO01-PAA/
├── docs/                      # Documentação e enunciados do trabalho
│   ├── Apresentação de Slides
│   ├── Trabalho_Algoritmos_Ordenac...pdf
│   └── Relatório - 1º Trabalho (PAA)
├── results/                   # Resultados das execuções e análises
│   ├── files/                 # Arquivos de dados brutos de saída
│   │   ├── cycleSort/
│   │   └── shellSort/
│   └── graphics/              # Gráficos gerados para análise comparativa
│       ├── cycleSort/
│       └── shellSort/
├── src/                       # Código-fonte do projeto
│   ├── cycleSort.c            # Implementação do algoritmo Cycle Sort
│   ├── shellSort.c            # Implementação do algoritmo Shell Sort
│   └── gerar_graficos.py      # Script Python para geração de visualizações
├── .gitignore                 # Arquivos ignorados pelo Git
├── LICENSE                    # Licença do projeto
└── README.md                  # Documentação principal

```
---

## ✅ Checklist de Progresso

x -> para marcar

### 🔧 Implementação
- [x] Escolher linguagem e configurar ambiente
- [x] Implementar Algoritmo A (Grupo A)
- [x] Implementar Algoritmo B (Grupo B)
- [x] Criar função de geração de vetores (crescente, decrescente, aleatório)
- [x] Implementar medição precisa de tempo de execução
- [x] Modularizar e documentar o código
- [x] Testar cada algoritmo com vetores pequenos (ex: 10 elementos)

### 🧪 Testes e Coleta de Dados
- [x] Definir tamanhos de vetores (ex: 20k, 40k, 60k)
- [x] Criar scripts para execução automatizada dos testes
- [x] Executar cada cenário 5 vezes: (foram 10)
  - [x] Vetor crescente
  - [x] Vetor decrescente
  - [x] Vetor aleatório
- [x] Calcular médias dos tempos de execução
- [x] Organizar resultados em tabelas

### 📊 Análise e Gráficos
- [x] Identificar pior caso prático para cada algoritmo
- [x] Relacionar resultados com a teoria (complexidade)
- [x] Discutir discrepâncias observadas
- [x] Gerar gráficos (tempo × tamanho da entrada) por cenário
- [x] Criar comparação visual direta entre os algoritmos

### 📝 Relatório Técnico
- [x] Introdução
- [x] Descrição detalhada dos algoritmos implementados
- [x] Análise teórica de complexidade
- [x] Metodologia experimental
- [x] Apresentação e discussão dos resultados
- [x] Conclusão
- [x] Referências bibliográficas

### 🎤 Apresentação Oral
- [x] Preparar slides
- [x] Definir roteiro da apresentação (20–40 min)
- [x] Preparar demonstração passo a passo com vetor pequeno
- [x] Distribuir falas entre os participantes
- [x] Realizar ensaio da apresentação

### 📦 Entrega Final
- [x] Revisar código e documentação
- [x] Revisar relatório
- [x] Consolidar todos os arquivos (código, dados, gráficos, relatório)
- [x] Enviar conforme orientações do professor

---

## 📅 Cronograma Sugerido

| Etapa | Prazo | Status |
|-------|--------|---------|
| Implementação dos algoritmos | *[09/01]* | ✅ |
| Execução dos testes | *[09/01]* | ✅ |
| Geração de gráficos | *[12/01]* | ✅ |
| Redação do relatório | *[14/01]* | ✅ |
| Preparação da apresentação | *[13/01]* | ✅ |
| Revisão final | *[13/01]* | ✅ |
| Apresentação | *[13/01]* | ✅ |
| Entrega | *[16/01]* | ✅ |

---

## 📌 Observações Importantes

- Cada teste deve ser executado **no mínimo 5 vezes**.
- Registrar **médias** e possíveis desvios.
- Todos os membros devem participar da **apresentação oral**.
- A demonstração na apresentação pode usar um vetor de **10 elementos** para clareza.

---

## 🔗 Links Úteis

- [Relatório Acadêmico](https://www.overleaf.com/project/695e6381059f34613c4a4a14)
- [Apresentação de Slides](https://docs.google.com/presentation/d/1gNWAoQPXcdVmxetjCAsR06dCQQXqE98gv0TWxfAHZnM/edit?usp=sharing)
- [Repositório](https://github.com/phsrod/Trabalho01-PAA)

---

**📌 Atualizado em:** *[14/01/2025]*
**✅ Progresso atual:** *[100%]*  

---

*Este README será atualizado conforme o andamento do trabalho. Use o checklist para acompanhar o progresso da equipe!*