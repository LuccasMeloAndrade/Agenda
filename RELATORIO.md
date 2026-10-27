# Relatório do Projeto da Unidade 1 — AgendaFácil

**DIM0501 — Boas Práticas de Programação**  
Prof. Marcus Terra · DIMAp/UFRN · Semestre 2026.1 · Entrega: 25/10/2026

| Integrantes (nome e matrícula) | Linguagem |
| :--- | :--- |
| ALDCEJAM PAULINO MORAIS JUNIOR (20230034141)<br>FLAVIA JAMILY DOS SANTOS MACENA (20240032561)<br>LUCCAS MELO DE ANDRADE (20250030963)<br>VICTOR HUGO PEREIRA DE SOUZA (20250035870) | C++ |

**Repositório entregue:** https://github.com/LuccasMeloAndrade/Agenda.git

---

## 1. Diagnóstico do código-base

| # | Categoria | Anomalia e onde está | Por que é um problema |
| :-: | :--- | :--- | :--- |
| 1 | Adequação e lógica | Na função `add`, três `if` aninhados (`h >= 8`, `h < 18`, `dur > 0`) sem `else`. | A hora ou duração inválidas não geram mensagem nem erro (falha silenciosa). |
| 2 | Adequação e lógica | A função `rem` (remarcar) não valida nova data (`nd`) e nova hora (`nh`), nem avisa se não há atendimento. | Sem a validação, é possível adicionar uma nova data ou nova hora que não são válidas. Além disso, se não houver atendimento, isso não é informado. |
| 3 | Adequação e lógica | As funções `add` e `rem` consideram apenas o horário de início do atendimento (`ag[i].h == h`), sem considerar a duração e sem validar múltiplos de 15 min (entre 15 e 120 min) nem se o término ultrapassa as 18h. | Dois atendimentos podem ser marcados em horários sobrepostos com o mesmo profissional ou terminar após o fechamento da clínica. |
| 4 | Manutenibilidade | Nomes: `A`, `p`, `pr`, `d`, `h`, `st`, `ag`. Funções: `add`, `rem`, `lst`. | São nomes que não comunicam propósito; não é possível em um primeiro momento entender o que representam. |
| 5 | Manutenibilidade | Números e literais soltos no código: `h >= 8`, `h < 18`, `"MARCADO"`. | Se for preciso mudar o horário do expediente ou regras de negócio, é necessário buscar esses valores mágicos espalhados pelo código. |
| 6 | Manutenibilidade | Na função `add`, há muitos `if` aninhados e a função mistura validação, regra de negócio e impressão em tela (`cout`). | O aninhamento dificulta a leitura e manutenção, e a mistura de responsabilidades acopla a lógica de negócio à interface de saída. |
| 7 | Organização e estética | `// TODO: rever isso depois` e função `// void desmarcar` comentada. | O comentário `TODO` vencido não diz o que deve ser revisado e a função `desmarcar` comentada é código morto que polui o arquivo. |
| 8 | Organização e estética | Cabeçalho com histórico manual de alterações e comentários superficiais (`// lista os atendimentos do mes`, `// adiciona na lista`). | O histórico pertence ao controle de versão (Git) e os comentários apenas narram o *quê* o código faz (às vezes de forma incorreta, como dizer que lista do mês quando filtra por dia), e não o *porquê*. |
| 9 | Programação defensiva e segurança | `try { ... } catch (...) { }` com bloco `catch` vazio na função `add`. | Qualquer exceção eventual é engolida silenciosamente, ocultando falhas em tempo de execução. |
| 10 | Programação defensiva e segurança | Nas funções `add`, `rem` e `lst` não é validado um formato padrão de data nem se campos de texto (`p`, `pr`) estão vazios. | A data (`d`) aceita qualquer texto (`"2026-10-5"` e `"2026-10-05"` são tratadas como datas distintas) e é possível agendar atendimentos sem paciente ou profissional. |
| 11 | Desempenho e recursos | Nas assinaturas de `add`, `rem` e `lst` e nos laços de busca, todas as `string` (`p`, `pr`, `d`, `nd`) são passadas por valor. | Realiza cópias desnecessárias de objetos `std::string` em memória a cada chamada de função, desperdiçando recursos. |
| 12 | Manutenibilidade | O vetor de atendimentos `vector<A> ag` é uma variável global mutável. | O estado global pode ser modificado diretamente por qualquer função do arquivo, dificultando testes isolados e o controle de efeitos colaterais. |

---

## 2. O que foi resolvido e o que ficou como dívida

| # | Situação | Como foi resolvida, ou por que ficou |
| :-: | :--- | :--- |
| 1 | Resolvida | A função `adicionarAtendimento` aplica cláusulas de guarda separadas (`horaInicialValida`, `duracaoValida` e `dentroDoExpediente`) e lança `std::invalid_argument` informando exatamente o valor inválido recebido em vez de falhar em silêncio. |
| 2 | Resolvida | A função `remarcarAtendimento` valida os parâmetros com cláusulas de guarda no início (`std::invalid_argument`) e lança `std::runtime_error` se o atendimento não existir. |
| 3 | Resolvida | `duracaoValida` exige duração entre 15 e 120 min (múltiplo de 15), `dentroDoExpediente` exige término até 18h e `horarioOcupado` verifica interseção de intervalos contínuos em minutos. |
| 4 | Resolvida | A estrutura e todas as variáveis e funções receberam nomes descritivos (`Atendimento`, `adicionarAtendimento`, `remarcarAtendimento`, `buscarAtendimentos`, `horaInicialValida`, `duracaoValida`, `dentroDoExpediente`, `horarioOcupado`, `dataValida`). |
| 5 | Resolvida | Os valores de negócio foram extraídos para constantes nomeadas em `MAIUSCULAS_COM_UNDERSCORE` no início do código (`HORA_ABERTURA`, `HORA_FECHAMENTO`, `DURACAO_MINIMA`, `DURACAO_MAXIMA`, `PASSO_DO_INTERVALO`, etc.). |
| 6 | Resolvida | Os `if` aninhados deram lugar a cláusulas de guarda independentes no mesmo nível, e as funções de negócio deixaram de imprimir no console (responsabilidade única). |
| 7 | Resolvida | O comentário `TODO` vencido e o código morto comentado (`desmarcar`) foram removidos. |
| 8 | Resolvida | O cabeçalho manual e os comentários narrativos de ruído foram removidos e substituídos por documentação de contratos no padrão Doxygen (`@brief`, `@param`, `@return`, `@throw`) e comentário de decisão (`@details`). |
| 9 | Resolvida | O bloco `try/catch (...)` vazio foi removido; agora as funções lançam exceções específicas com mensagens claras e o tratamento é feito por quem chama (`main`). |
| 10 | Dívida assumida (parcialmente resolvida) | Foi implementada a validação de strings vazias e a função `dataValida` para exigir o formato `AAAA-MM-DD` com mês (`01`–`12`) e dia (`01`–`31`), mas datas de calendário inexistentes como `2026-02-31` ainda passam por não haver calendário completo por mês/ano bissexto. |
| 11 | Resolvida | Todas as funções passaram a receber textos por referência constante (`const string&`) e os laços utilizam referências constantes (`const Atendimento&`), eliminando cópias desnecessárias. |
| 12 | Dívida assumida | O vetor `vector<Atendimento> agenda` foi mantido em escopo global para conservar a arquitetura procedural simples em arquivo único, sendo documentado como dívida técnica assumida. |

---

## 3. Decisões de projeto

| Decisão tomada | Alternativa descartada | Por que decidimos assim |
| :--- | :--- | :--- |
| Agrupar os dados do atendimento em uma `struct Atendimento` com campos nomeados guardada em um `vector`. | Usar vetores diferentes para cada dado, ligados pelo índice, ou tuplas acessadas por posição. | Com vetores distintos, cada dado do mesmo atendimento fica em uma posição de listas diferentes, e remover ou alterar um item exige mexer em todas. A `struct` nomeada mantém os registros coesos e legíveis. |
| Lançar exceções específicas (`std::invalid_argument` e `std::runtime_error`) quando uma validação falha, separando lógica de negócio da impressão (`cout`). | Imprimir mensagem dentro das próprias funções e dar `return` em funções `void`. | Quem valida ou agenda não deve imprimir. Ao lançar exceções com o motivo e o valor inválido, a função cumpre responsabilidade única e sinaliza a falha para quem chama decidir como tratar. |
| Representar cada atendimento como um intervalo contínuo em minutos `[inicio, fim)` e detectar conflito por interseção de intervalos (`inicio < fimExistente && inicioExistente < fim`). | Comparar apenas a hora inteira de início do atendimento (`ag[i].h == h`). | Comparar só a hora de início ignora a duração (ex.: um atendimento das 14h de 90 min conflita com outro às 15h). Convertendo para minutos a partir da meia-noite, hora inicial e duração ficam na mesma unidade e qualquer sobreposição é detectada em um único lugar (`horarioOcupado`). |

---

## 4. Mudanças de comportamento

| Entrada ou situação | O que acontecia antes | O que acontece agora |
| :--- | :--- | :--- |
| Hora fora do expediente (ex.: 6h, 22h). | Não era adicionado o atendimento e nem era exibida mensagem informando erro (falha silenciosa). | `horaInicialValida` reprova e lança `std::invalid_argument` informando `"Horário inicial inválido: <hora>h."`. |
| Duração de atendimento zero, negativa, fora do intervalo `[15, 120]` ou não múltipla de 15. | Durações `<= 0` eram ignoradas em silêncio; durações fora do padrão (ex.: 10 min ou 500 min) eram aceitas. | `duracaoValida` reprova e lança `std::invalid_argument` informando `"Duração inválida: <duracao> min."`. |
| Atendimento que ultrapassa o horário de funcionamento (18h). | Era aceito, pois só a hora de início do atendimento era validada, sem descontar a duração até as 18h. | `dentroDoExpediente` reprova e lança `std::invalid_argument` informando `"Atendimento ultrapassa o horário de fechamento (18h): início às <hora>h com duração de <duracao> min."`. |
| Sobreposição de horários de atendimento com o mesmo profissional. | Era aceita caso os horários de início fossem diferentes, pois não contabilizava a duração. | Lança `std::runtime_error` com `"Horario ocupado"`. |
| Paciente ou profissional vazios (`""`). | Era aceito e na listagem os campos apareciam em branco. | Lança `std::invalid_argument` com `"Nome do paciente está vazio."` ou `"Nome do profissional está vazio."`. |
| Data fora do formato `AAAA-MM-DD` (em agendar, remarcar ou buscar). | Era aceita qualquer string sem padrão definido. | Lança `std::invalid_argument` com `"Data inválida: <valor>"`. Só são aceitas datas no formato `AAAA-MM-DD`. |
| Remarcar para nova data ou novo horário inválidos, ou atendimento inexistente. | Datas/horas inválidas eram aceitas (só verificava conflito de hora exata) e paciente inexistente falhava em silêncio. | Lança `std::invalid_argument` (`"Nova data inválida: <novaData>"` ou `"Novo horário inválido: <novaHora>h."`) ou `std::runtime_error` (`"Atendimento nao encontrado para o paciente <paciente>"`). |

---

## 5. Como executar e gerar a documentação

| Item | Resposta |
| :--- | :--- |
| **Comando para compilar e executar** | `g++ -std=c++17 -Wall agendaCorrigida.cpp -o agenda && ./agenda` |
| **Comando para gerar a documentação de API** | `doxygen Doxyfile` |
| **Onde a documentação gerada ficou no repositório** | Na pasta `docs/` (página principal em `docs/html/index.html`). |
| **Saída do programa de demonstração** | `Atendimento marcado para Ana`<br>`Horario ocupado`<br>`Atendimento marcado para Carla`<br>`Remarcado`<br>`14 Carla Dra. Souza MARCADO` |

---

## 6. Uso de ferramentas de IA

| Pergunta | Resposta |
| :--- | :--- |
| **Foram usadas ferramentas de IA neste trabalho?** | ( ) Não &nbsp;&nbsp;&nbsp; (X) Sim |
| **Em quais etapas, e para quê?** | Revisão final de conformidade com os requisitos do edital, validação das configurações de geração da documentação no arquivo `Doxyfile`, auxílio na estruturação da documentação nos arquivos `README.md` e `RELATORIO.md` e revisão ortográfica e gramatical do código-fonte (`agendaCorrigida.cpp`) e dos arquivos de documentação (`README.md` e `RELATORIO.md`).  |

---

## Conferência antes de enviar

- [x] O programa de demonstração compila e roda, e imprime a mesma saída do código-base.
- [x] Toda anomalia da seção 1 aparece na seção 2, resolvida ou declarada como dívida.
- [x] O README permite que outra pessoa execute o projeto sem perguntar nada.
- [x] Os textos em itálico deste modelo foram apagados.
