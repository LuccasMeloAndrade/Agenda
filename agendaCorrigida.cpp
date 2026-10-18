
#include <iostream>
#include <string>
#include <vector>
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
    return hora >= 8 && hora < 18 &&
           duracao > 0 && hora * 60 + duracao <= 18 * 60;
}

bool horarioOcupado(string profissional, string data,
                    int hora, int duracao, int ignorar = -1) {
    int inicio = hora * 60;
    int fim = inicio + duracao;

    for (int i = 0; i < agenda.size(); i++) {
        if (i == ignorar) {
            continue;
        }

        if (agenda[i].profissional == profissional &&
            agenda[i].data == data) {

            int inicioExistente = agenda[i].hora * 60;
            int fimExistente = inicioExistente + agenda[i].duracao;

            if (inicio < fimExistente && inicioExistente < fim) {
                return true;
            }
        }
    }

    return false;
}

void adicionar(string paciente, string profissional,
               string data, int hora, int duracao) {
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

    remarcar("Ana", "2026-10-05", 9, "2026-10-06", 10);

    listar("2026-10-05");

    return 0;
}
