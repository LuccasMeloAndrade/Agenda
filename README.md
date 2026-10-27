# AgendaFácil

Sistema de agendamento de atendimentos para uma clínica médica, desenvolvido em **C++ (C++17)** como projeto da Unidade 1 da disciplina **DIM0501 — Boas Práticas de Programação** (DIMAp/UFRN, 2026.1, Prof. Marcus Terra).

---

## Sobre o Sistema

O **AgendaFácil** permite:
- **Agendar atendimentos** (`adicionarAtendimento`), validando horário de expediente (8h às 18h), duração (15 a 120 minutos, em múltiplos de 15), formato de data (`AAAA-MM-DD`) e conflitos de intervalo por profissional;
- **Remarcar atendimentos** (`remarcarAtendimento`), garantindo que o novo horário respeite o expediente e não apresente sobreposição;
- **Buscar atendimentos por data** (`buscarAtendimentos`), retornando os registros agendados para o dia consultado.

O projeto teve como objetivo refatorar o código-base do sistema, aplicando princípios de programação defensiva, eliminação de valores mágicos, documentação de contratos via **Doxygen** e tratamento de exceções específicas (`std::invalid_argument` e `std::runtime_error`).

---

## Estrutura do Repositório

- `agendaCorrigida.cpp` — Código-fonte refatorado, documentado e com programação defensiva.
- `agendaDefeito.cpp` — Código-base original fornecido na especificação para fins de comparação.
- `Doxyfile` — Arquivo de configuração do Doxygen para geração da documentação de API.
- `docs/` — Documentação de API gerada automaticamente pelo Doxygen (`docs/html/index.html`).
- `RELATORIO.md` — Relatório técnico contendo o diagnóstico do código-base, dívidas técnicas, decisões de projeto e mudanças de comportamento.

---

## Pré-requisitos

- Compilador C++ com suporte a **C++17** (`g++` recomendado)
- **Doxygen** (opcional, apenas para regerar a documentação de API)

---

## Como Compilar e Executar

No terminal, a partir da raiz do repositório, execute:

```bash
g++ -std=c++17 -Wall agendaCorrigida.cpp -o agenda && ./agenda
```

### Saída esperada do programa de demonstração

```text
Atendimento marcado para Ana
Horario ocupado
Atendimento marcado para Carla
Remarcado
14 Carla Dra. Souza MARCADO
```

---

## Como Gerar a Documentação de API

Para gerar (ou atualizar) a documentação de API na pasta `docs/`, execute na raiz do projeto:

```bash
doxygen Doxyfile
```

A documentação gerada ficará disponível em:
- `docs/html/index.html` (abra este arquivo no navegador para visualizar).

---

## Equipe (Integrantes e Matrículas)

- **Aldcejam Paulino Morais Junior** — `20230034141`
- **Flávia Jamily dos Santos Macena** — `20240032561`
- **Luccas Melo de Andrade** — `20250030963`
- **Victor Hugo Pereira de Souza** — `20250035870`
