/**
 * @file agendaCorrigida.cpp
 * @brief Sistema AgendaFácil — gerenciamento de atendimentos de uma clínica.
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

const int HORA_ABERTURA = 8;
const int HORA_FECHAMENTO = 18;
const int MINUTO_HORA = 60;
const int DURACAO_MINIMA = 15;
const int DURACAO_MAXIMA = 120;
const int PASSO_DO_INTERVALO = 15;
const size_t TAMANHO_DATA = 10;
const size_t POSICAO_HIFEN_ANO = 4;
const size_t POSICAO_HIFEN_MES = 7;
const char* const STATUS_MARCADO = "MARCADO";

using namespace std;

/**
 * @brief Estrutura que representa um atendimento na clínica.
 */
struct Atendimento {
    string paciente;      /**< Nome do paciente */
    string profissional;  /**< Nome do profissional da saúde */
    string data;          /**< Data do atendimento no formato AAAA-MM-DD */
    int horaInicial;      /**< Hora inicial do atendimento (formato de 24h) */
    int duracao;          /**< Tempo de duração do atendimento em minutos */
    string status;        /**< Status do atendimento (ex: MARCADO) */
};

vector<Atendimento> agenda;

/**
 * @brief Verifica se a hora inicial solicitada está dentro do expediente da clínica (8h às 17h).
 * 
 * @param horaInicial Hora de início pretendida (formato 24h).
 * @return true Se a hora inicial está entre 8h (inclusive) e 18h (exclusive).
 * @return false Se a hora inicial estiver fora do horário de funcionamento.
 * @throw Não lança exceções.
 */
bool horaInicialValida(int horaInicial) {
    return horaInicial >= HORA_ABERTURA && horaInicial < HORA_FECHAMENTO;
}

/**
 * @brief Verifica se a duração do atendimento atende às regras de negócio (15 a 120 min, múltiplo de 15).
 * 
 * @param duracao Duração pretendida em minutos.
 * @return true Se a duração estiver entre 15 e 120 minutos e for múltipla de 15.
 * @return false Se a duração estiver fora do intervalo permitido ou não for múltipla de 15.
 * @throw Não lança exceções.
 */
bool duracaoValida(int duracao) {
    return duracao >= DURACAO_MINIMA &&
           duracao <= DURACAO_MAXIMA &&
           (duracao % PASSO_DO_INTERVALO == 0);
}

/**
 * @brief Verifica se o término do atendimento não ultrapassa o horário de fechamento da clínica (18h).
 * 
 * @param horaInicial Hora de início do atendimento (formato 24h).
 * @param duracao Duração do atendimento em minutos.
 * @return true Se o atendimento terminar até as 18h.
 * @return false Se o atendimento terminar após as 18h.
 * @throw Não lança exceções.
 */
bool dentroDoExpediente(int horaInicial, int duracao) {
    return duracao <= (HORA_FECHAMENTO - horaInicial) * MINUTO_HORA;
}

/**
 * @brief Verifica se o profissional já possui outro atendimento no mesmo horário.
 * 
 * @details Decisão de projeto: utiliza-se conversão contínua em minutos para checagem 
 * de interseção em intervalos [inicio, fim). Dois intervalos [A, B) e [C, D)
 * colidem se e somente se: (A < D) e (C < B).
 * 
 * @param profissional Nome do profissional.
 * @param data Data do atendimento no formato AAAA-MM-DD.
 * @param horaInicial Hora de início do atendimento.
 * @param duracao Tempo de duração da consulta em minutos.
 * @param ignorar Índice de um atendimento a ser ignorado na checagem (usado para remarcação), padrão -1.
 * @return true Se houver colisão nos horários.
 * @return false Se não houver colisão nos horários (horário livre).
 * @throw Não lança exceções.
 */
bool horarioOcupado(const string& profissional, const string& data,
                    int horaInicial, int duracao, int ignorar = -1) {
    int inicio = horaInicial * MINUTO_HORA;
    int fim = inicio + duracao;

    for (size_t i = 0; i < agenda.size(); i++) {
        if (static_cast<int>(i) == ignorar) {
            continue;
        }

        if (agenda[i].profissional == profissional &&
            agenda[i].data == data) {

            int inicioExistente = agenda[i].horaInicial * MINUTO_HORA;
            int fimExistente = inicioExistente + agenda[i].duracao;

            if (inicio < fimExistente && inicioExistente < fim) {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Verifica se a data fornecida está no formato AAAA-MM-DD e possui valores numéricos válidos de mês e dia.
 * 
 * @param data Data em formato de string.
 * @return true Se o formato AAAA-MM-DD e os limites básicos de mês (01-12) e dia (01-31) forem válidos.
 * @return false Se o formato ou os limites de mês e dia estiverem incorretos.
 * @throw Não lança exceções.
 */
bool dataValida(const string& data) {
    if (data.size() != TAMANHO_DATA || data[POSICAO_HIFEN_ANO] != '-' || data[POSICAO_HIFEN_MES] != '-') {
        return false;
    }
    for (size_t i = 0; i < data.size(); i++) {
        if (i == POSICAO_HIFEN_ANO || i == POSICAO_HIFEN_MES) continue;
        if (data[i] < '0' || data[i] > '9') {
            return false;
        }
    }
    string mes = data.substr(5, 2);
    string dia = data.substr(8, 2);

    if (mes < "01" || mes > "12") return false;
    if (dia < "01" || dia > "31") return false;

    return true;
}

/**
 * @brief Adiciona um atendimento na agenda caso atenda a todos os pré-requisitos do negócio.
 * 
 * @param paciente Nome do paciente.
 * @param profissional Nome do profissional.
 * @param data Data no formato AAAA-MM-DD.
 * @param horaInicial Hora de início do atendimento.
 * @param duracao Tempo de duração em minutos.
 * @throw invalid_argument Se paciente/profissional forem vazios, a data for inválida ou o horário/duração violarem as regras.
 * @throw runtime_error Se houver sobreposição de horário na agenda do profissional.
 */
void adicionarAtendimento(const string& paciente, const string& profissional, const string& data,
                          int horaInicial, int duracao) {
    if (paciente.empty()) {
        throw invalid_argument("Nome do paciente está vazio.");
    }

    if (profissional.empty()) {
        throw invalid_argument("Nome do profissional está vazio.");
    }

    if (!dataValida(data)) {
        throw invalid_argument("Data inválida: " + data);
    }

    if (!horaInicialValida(horaInicial)) {
        throw invalid_argument("Horário inicial inválido: " + to_string(horaInicial) + "h.");
    }

    if (!duracaoValida(duracao)) {
        throw invalid_argument("Duração inválida: " + to_string(duracao) + " min.");
    }

    if (!dentroDoExpediente(horaInicial, duracao)) {
        throw invalid_argument("Atendimento ultrapassa o horário de fechamento (18h): início às " +
                               to_string(horaInicial) + "h com duração de " + to_string(duracao) + " min.");
    }

    if (horarioOcupado(profissional, data, horaInicial, duracao)) {
        throw runtime_error("Horario ocupado");
    }

    agenda.push_back({
        paciente, profissional, data, horaInicial, duracao, STATUS_MARCADO
    });
}

/**
 * @brief Remarca um atendimento já existente para uma nova data e hora.
 * 
 * @param paciente Nome do paciente.
 * @param dataAtual Data agendada que será remarcada.
 * @param horaAtual Hora agendada que será remarcada.
 * @param novaData Nova data desejada para o atendimento (formato AAAA-MM-DD).
 * @param novaHora Nova hora desejada para o atendimento (8h às 17h).
 * @throw invalid_argument Se paciente for vazio ou se as datas/horários violarem as regras de negócio.
 * @throw runtime_error Se o atendimento original não existir ou se o novo horário estiver ocupado.
 */
void remarcarAtendimento(const string& paciente, const string& dataAtual,
                         int horaAtual, const string& novaData, int novaHora) {
    if (paciente.empty()) {
        throw invalid_argument("Nome do paciente está vazio.");
    }

    if (!dataValida(dataAtual)) {
        throw invalid_argument("Data atual inválida: " + dataAtual);
    }

    if (!horaInicialValida(horaAtual)) {
        throw invalid_argument("Horário atual inválido: " + to_string(horaAtual) + "h.");
    }

    if (!dataValida(novaData)) {
        throw invalid_argument("Nova data inválida: " + novaData);
    }

    if (!horaInicialValida(novaHora)) {
        throw invalid_argument("Novo horário inválido: " + to_string(novaHora) + "h.");
    }

    for (size_t i = 0; i < agenda.size(); i++) {
        if (agenda[i].paciente == paciente &&
            agenda[i].data == dataAtual &&
            agenda[i].horaInicial == horaAtual) {

            if (!dentroDoExpediente(novaHora, agenda[i].duracao)) {
                throw invalid_argument("Novo horário ultrapassa o fechamento (18h): início às " +
                                       to_string(novaHora) + "h com duração de " + to_string(agenda[i].duracao) + " min.");
            }

            if (horarioOcupado(agenda[i].profissional, novaData,
                               novaHora, agenda[i].duracao, static_cast<int>(i))) {
                throw runtime_error("Horario ocupado");
            }

            agenda[i].data = novaData;
            agenda[i].horaInicial = novaHora;

            return;
        }
    }

    throw runtime_error("Atendimento nao encontrado para o paciente " + paciente);
}

/**
 * @brief Busca todos os atendimentos marcados em uma determinada data.
 * 
 * @param data Data a ser consultada no formato AAAA-MM-DD.
 * @return vector<Atendimento> Lista de atendimentos agendados para a data informada.
 * @throw invalid_argument Se a data fornecida não estiver no formato válido AAAA-MM-DD.
 */
vector<Atendimento> buscarAtendimentos(const string& data) {
    if (!dataValida(data)) {
        throw invalid_argument("Data inválida: " + data);
    }

    vector<Atendimento> filtrados;
    for (const Atendimento& item : agenda) {
        if (item.data == data) {
            filtrados.push_back(item);
        }
    }
    
    return filtrados;
}

int main() {
    try {
        adicionarAtendimento("Ana", "Dr. Silva", "2026-10-05", 9, 30);
        cout << "Atendimento marcado para Ana" << endl;
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    
    try {
        adicionarAtendimento("Bruno", "Dr. Silva", "2026-10-05", 9, 30);
        cout << "Atendimento marcado para Bruno" << endl;
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    
    try {
        adicionarAtendimento("Carla", "Dra. Souza", "2026-10-05", 14, 60);
        cout << "Atendimento marcado para Carla" << endl;
    } 
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    
    try {
        remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-06", 10);
        cout << "Remarcado" << endl;
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    
    vector<Atendimento> lista = buscarAtendimentos("2026-10-05");
    for (const Atendimento& at : lista) {
        cout << at.horaInicial << " " << at.paciente << " " 
             << at.profissional << " " << at.status << endl;
    }
    
    return 0;
}