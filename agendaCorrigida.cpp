
#include <iostream>
#include <string>
#include <vector>

const int HORA_ABERTURA = 8;
const int HORA_FECHAMENTO = 18;
const int MINUTO_HORA = 60;

using namespace std;

struct Atendimento {
    string paciente;
    string profissional;
    string data;
    int hora;
    int duracao;
    string status;
};

vector<Atendimento> agenda;

bool horarioValido(int hora, int duracao) {
    return hora >= HORA_ABERTURA && hora < HORA_FECHAMENTO &&
           duracao > 0 && duracao <= (HORA_FECHAMENTO - hora) * MINUTO_HORA;
}

bool horarioOcupado(string profissional, string data,
                    int hora, int duracao, int ignorar = -1) {
    int inicio = hora * MINUTO_HORA;
    int fim = inicio + duracao;

    for (int i = 0; i < agenda.size(); i++) {
        if (i == ignorar) {
            continue;
        }

        if (agenda[i].profissional == profissional &&
            agenda[i].data == data) {

            int inicioExistente = agenda[i].hora * MINUTO_HORA;
            int fimExistente = inicioExistente + agenda[i].duracao;

            if (inicio < fimExistente && inicioExistente < fim) {
                return true;
            }
        }
    }

    return false;
}

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

void adicionar(string paciente, string profissional,
               string data, int hora, int duracao) {

    if (paciente.empty() || profissional.empty()) {
        cout << "Paciente ou profissional esta vazio\n";
        return;
    }           
    if (!dataValida(data)) {
        cout << "Data invalida.\n";
        return;
    }
    if (!horarioValido(hora, duracao)) {
        cout << "Horario ou duracao invalida.\n";
        return;
    }

    if (horarioOcupado(profissional, data, hora, duracao)) {
        cout << "Horario ocupado.\n";
        return;
    }

    agenda.push_back({
        paciente, profissional, data, hora, duracao, "MARCADO"
    });

    cout << "Atendimento marcado para " << paciente << ".\n";
}

void remarcar(string paciente, string dataAtual,
              int horaAtual, string novaData, int novaHora) {
    for (int i = 0; i < agenda.size(); i++) {
        if (agenda[i].paciente == paciente &&
            agenda[i].data == dataAtual &&
            agenda[i].hora == horaAtual) {

            if (!dataValida(novaData)) {
                cout << "Nova data invalida.\n";
                return;
            }

            if (!horarioValido(novaHora, agenda[i].duracao)) {
                cout << "Novo horario invalido.\n";
                return;
            }

            if (horarioOcupado(agenda[i].profissional, novaData,
                               novaHora, agenda[i].duracao, i)) {
                cout << "Horario ocupado.\n";
                return;
            }

            agenda[i].data = novaData;
            agenda[i].hora = novaHora;

            cout << "Atendimento remarcado.\n";
            return;
        }
    }

    cout << "Atendimento nao encontrado.\n";
}

void listar(string data) {
    bool encontrado = false;

    for (int i = 0; i < agenda.size(); i++) {
        if (agenda[i].data == data) {
            cout << agenda[i].hora << "h - "
                 << agenda[i].paciente << " - "
                 << agenda[i].profissional << " - "
                 << agenda[i].status << "\n";

            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "Nenhum atendimento encontrado.\n";
    }
}

int main() {
    adicionar("Ana", "Dr. Silva", "2026-10-05", 9, 30);
    adicionar("Bruno", "Dr. Silva", "2026-10-05", 9, 30);
    adicionar("Carla", "Dra. Souza", "2026-10-05", 14, 60);
    //teste
    adicionar("Carla", "Dra. Souza", "2026-u0-05", 14, 60);
    adicionar("Carla", "Dra. Souza", "20d6-u0-05", 14, 60);
    adicionar("Carla", "Dra. Souza", "2026-u0-0", 14, 60);
    adicionar("", "Dra. Souza", "2026-70-60", 14, 60);
    //
    remarcar("Ana", "2026-10-05", 9, "2026-10-06", 10);

    listar("2026-10-05");
    return 0;
}
