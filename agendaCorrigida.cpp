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
 * @brief Verifica se o horário e a duração solicitadas estão dentro do expediente da clínica
 * 
 * @param horaInicial Hora de ínicio pretendido
 * @param duracao Duração pretendida em minutos
 * @return false Se a duração não estiver entre 15 e 120 minutos ou não for multipla de 15
 * @return true Se o horário está dentro do expediente (8h às 18h) e a duração é válida
 * @return false Se o horário não atender ao quadro de funcionamento ou duração seja inválida
 */

bool horarioValido(int horaInicial, int duracao) {
    if(duracao < DURACAO_MINIMA || duracao > DURACAO_MAXIMA || (duracao % PASSO_DO_INTERVALO != 0)){
        return false;
    }
    if (horaInicial < HORA_ABERTURA || horaInicial >= HORA_FECHAMENTO) {
        return false;
    }
        
    return duracao <= (HORA_FECHAMENTO - horaInicial) * MINUTO_HORA;
}

/**
 * @brief Verifica se o profissional já possui outro atendimento no mesmo horario
 * 
 * @details Decisão de projeto: utiliza-se conversão contínua em minutos para checagem 
 * de interseção em intervalos [inicio, fim). Dois intervalos [A, B) e [C, D)
 * colidem se e somente se: (A < D) e (C < B).
 * 
 * @param profissional Nome do profissional
 * @param data Data do atendimento
 * @param horaInicial Hora de inicio do atendimento
 * @param duracao Tempo de duração da consulta em minutos
 * @param ignorar Índice de um atendimento a ser ignorado na checagem (usado para remarcação), começa em -1
 * @return true Se houver colisão nos horários
 * @return false Se não houver colisão nos horários, horário livre
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
 * @brief Verifica se a data fornecida está no formato AAAA-MM-DD e possui valor logico
 * 
 * @param data Data em formato de string
 * @return true Se o formato e os valores enviados são válidos
 * @return false Se o formato ou limite de mes e dias estajam incorretos
 */

bool dataValida(const string& data) {
    if (data.size() != 10 || data[4] != '-' || data[7] != '-') {
        return false;
    }
    for (size_t i = 0; i < data.size(); i++) {
        if (i == 4 || i == 7) continue;
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
 * @brief Adiciona um atendimento na agenda caso atenda todos os pré-requisitos do negócio
 * 
 * @param paciente Nome do paciente
 * @param profissional Nome do profissional
 * @param data Data no formato AAAA-MM-DD
 * @param horaInicial Hora de ínicio do atendimento
 * @param duracao Tempo de duração em minutos
 * @throw invalid_argument Se os campos forem vazios, a data for inválida ou o horário violar o expediente
 * @throw runtime_error Se houver sobreposição na agenda do profissional
 */
void adicionarAtendimento(const string& paciente, const string& profissional, const string& data,
     int horaInicial, int duracao) {
    if (paciente.empty() || profissional.empty()) {
        throw invalid_argument("Paciente ou profissional está vazio.");
    }    

    if (!dataValida(data)) {
        throw invalid_argument("Data inválida.");
    }

    if (!horarioValido(horaInicial, duracao)) {
        throw invalid_argument("Horário ou duração inválida.");
    }

    if (horarioOcupado(profissional, data, horaInicial, duracao)) {
        throw runtime_error("Horario ocupado");
    }

    agenda.push_back({
        paciente, profissional, data, horaInicial, duracao, "MARCADO"
    });
}

/**
 * @brief Remarca um atendimento já existente para uma nova data e hora
 * 
 * @param paciente Nome do paciente
 * @param dataAtual Data agendada que será remarcada
 * @param horaAtual Hora agendada que será remarcada
 * @param novaData Nova data desejada para o atendimento
 * @param novaHora Nova hora desejada para o atendimento
 * @throw invalid_argument Se a nova data ou horário violarem as regras cadastrais
 * @throw runtime_error Se o atendimento original não existir ou se o novo horário estiver ocupado
 */
void remarcarAtendimento(const string& paciente, const string& dataAtual,
        int horaAtual, const string& novaData, int novaHora) {
    for (size_t i = 0; i < agenda.size(); i++) {
        if (agenda[i].paciente == paciente &&
            agenda[i].data == dataAtual &&
            agenda[i].horaInicial == horaAtual) {

            if (!dataValida(novaData)) {
                throw invalid_argument("Nova data invalida.");
            }

            if (!horarioValido(novaHora, agenda[i].duracao)) {
                throw invalid_argument("Novo horario invalido.");
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
 * @brief Lista todos os atendimentos marcados naquela data
 * 
 * @param data Data que listará os atendimentos previstos
 * @return vector<Atendimento> Lista de atendimentos agendados
 */
vector<Atendimento> buscarAtendimentos(const string& data) {
    vector<Atendimento> filtrados;
    for (const Atendimento& item : agenda) {
        if (item.data == data) {
            filtrados.push_back(item);
        }
    }
    
    return filtrados;
}

int main() {

    //Caminho feliz
    try{
        adicionarAtendimento("Ana", "Dr. Silva", "2026-10-05", 9, 30);
        cout << "Atendimento marcado para Ana" << endl;
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    
    try{
        adicionarAtendimento("Bruno", "Dr. Silva", "2026-10-05", 9, 30);
        cout << "Atendimento marcado para Bruno" << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what() << endl;
    }
    
    try {
        adicionarAtendimento("Carla", "Dra. Souza", "2026-10-05", 14, 60);
        cout << "Atendimento marcado para Carla" << endl;
    } 
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    
    try{
        remarcarAtendimento("Ana", "2026-10-05", 9, "2026-10-06", 10);
        cout << "Remarcado" << endl;
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    
    //Listagem atendimentos data 05/10/2026
    vector<Atendimento> lista = buscarAtendimentos("2026-10-05");
    for (const Atendimento& at : lista) {
        cout << at.horaInicial << " " << at.paciente << " " 
             << at.profissional << " " << at.status << endl;
    }

    //Testes de robustez com entradas inválidas
    try{
        adicionarAtendimento("Carla", "Dra. Souza", "2026-u0-05", 14, 60);
    }
    catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }

    try{
        adicionarAtendimento("Carla", "Dra. Souza", "20d6-u0-05", 14, 60);
    }
    catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }

    try{
        adicionarAtendimento("Carla", "Dra. Souza", "2026-u0-0", 14, 60);
    }
    catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }

    try{
        adicionarAtendimento("", "Dra. Souza", "2026-70-60", 14, 60);
    }
    catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }
    
    return 0;
}