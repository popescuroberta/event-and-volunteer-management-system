#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <xlsxwriter.h>

using namespace std;

// Clasele utilizate
class Sarcina {
private:
    string descriere;
    string voluntarAtribuit;
    string status;

public:
    Sarcina(string desc)
        : descriere(desc), status("In desfasurare") {}

    void atribuieVoluntar(const string& voluntar) {
        voluntarAtribuit = voluntar;
    }

    void finalizeaza() {
        status = "Finalizata";
    }

    void afiseazaDetalii() const {
        cout << "Sarcina: " << descriere << "\nVoluntar: " << voluntarAtribuit
                  << "\nStatus: " << status << '\n';
    }

    string getDescriere() const { return descriere; }
};

class Eveniment {
private:
    string nume;
    string data;
    string locatie;
    string descriere;
    vector<string> listaVoluntari;
    vector<string> listaSarcini;

public:
    Eveniment(string n, string d, string l, string desc)
        : nume(n), data(d), locatie(l), descriere(desc) {}

    void adaugaVoluntar(const string& voluntar) {
        listaVoluntari.push_back(voluntar);
    }

    void adaugaSarcina(const string& sarcina) {
        listaSarcini.push_back(sarcina);
    }

    void afiseazaDetalii() const {
        cout << "Eveniment: " << nume << "\nData: " << data
                  << "\nLocație: " << locatie << "\nDescriere: " << descriere << '\n';
    }

    string getNume() const { return nume; }
    string getData() const { return data; }
    string getLocatie() const { return locatie; }
    string getDescriere() const { return descriere; }
    vector<string> getSarcini() const { return listaSarcini; }
};

class Voluntar {
private:
    string nume;
    string email;
    string telefon;
    vector<string> abilitati;
    vector<int> evaluari;

public:
    Voluntar(string n, string e, string t)
        : nume(n), email(e), telefon(t) {}

    void adaugaAbilitate(const string& abilitate) {
        abilitati.push_back(abilitate);
    }

    void adaugaEvaluare(int scor) {
        evaluari.push_back(scor);
    }

    double scorMediu() const {
        if (evaluari.empty()) return 0.0;
        int suma = 0;
        for (int scor : evaluari) suma += scor;
        return static_cast<double>(suma) / evaluari.size();
    }

    void afiseazaDetalii() const {
        cout << "Voluntar: " << nume << "\nEmail: " << email
                  << "\nTelefon: " << telefon << "\nScor mediu: " << scorMediu() << '\n';
    }

    string getNume() const { return nume; }
    string getEmail() const { return email; }
    string getTelefon() const { return telefon; }
    bool areAbilitate(const string& abilitate) const {
        return find(abilitati.begin(), abilitati.end(), abilitate) != abilitati.end();
    }
};

// Functia pentru salvarea datelor în Excel
void salveazaInExcel(const vector<Eveniment>& evenimente, const vector<Voluntar>& voluntari) {
    lxw_workbook* workbook = workbook_new("EvenimenteSiVoluntari.xlsx");
    lxw_worksheet* worksheetEvenimente = workbook_add_worksheet(workbook, "Evenimente");
    lxw_worksheet* worksheetVoluntari = workbook_add_worksheet(workbook, "Voluntari");

    // Scrie datele despre evenimente
    worksheet_write_string(worksheetEvenimente, 0, 0, "Nume Eveniment", NULL);
    worksheet_write_string(worksheetEvenimente, 0, 1, "Data", NULL);
    worksheet_write_string(worksheetEvenimente, 0, 2, "Locație", NULL);
    worksheet_write_string(worksheetEvenimente, 0, 3, "Descriere", NULL);
    worksheet_write_string(worksheetEvenimente, 0, 4, "Voluntari", NULL);  // Coloană pentru voluntari

    int row = 1;
    for (const auto& ev : evenimente) {
        worksheet_write_string(worksheetEvenimente, row, 0, ev.getNume().c_str(), NULL);
        worksheet_write_string(worksheetEvenimente, row, 1, ev.getData().c_str(), NULL);
        worksheet_write_string(worksheetEvenimente, row, 2, ev.getLocatie().c_str(), NULL);
        worksheet_write_string(worksheetEvenimente, row, 3, ev.getDescriere().c_str(), NULL);

        // Scrie voluntarii care au participat la eveniment
        stringstream voluntariList;
        for (const auto& sarcina : ev.getSarcini()) {
            for (const auto& vol : voluntari) {
                if (vol.areAbilitate(sarcina)) { // Asum că abilitatea voluntarului se potrivește cu sarcina
                    if (!voluntariList.str().empty()) voluntariList << ", ";
                    voluntariList << vol.getNume();
                }
            }
        }
        worksheet_write_string(worksheetEvenimente, row, 4, voluntariList.str().c_str(), NULL);

        row++;
    }

    // Scrie datele despre voluntari
    worksheet_write_string(worksheetVoluntari, 0, 0, "Nume Voluntar", NULL);
    worksheet_write_string(worksheetVoluntari, 0, 1, "Email", NULL);
    worksheet_write_string(worksheetVoluntari, 0, 2, "Telefon", NULL);
    worksheet_write_string(worksheetVoluntari, 0, 3, "Scor Mediu", NULL);
    worksheet_write_string(worksheetVoluntari, 0, 4, "Evenimente", NULL);  // Coloană pentru evenimente

    row = 1;
    for (const auto& vol : voluntari) {
        worksheet_write_string(worksheetVoluntari, row, 0, vol.getNume().c_str(), NULL);
        worksheet_write_string(worksheetVoluntari, row, 1, vol.getEmail().c_str(), NULL);
        worksheet_write_string(worksheetVoluntari, row, 2, vol.getTelefon().c_str(), NULL);

        stringstream scor;
        scor << vol.scorMediu();
        worksheet_write_string(worksheetVoluntari, row, 3, scor.str().c_str(), NULL);

        // Scrie evenimentele la care a participat voluntarul
        stringstream evenimenteList;
        for (const auto& ev : evenimente) {
            for (const auto& sarcina : ev.getSarcini()) {
                if (vol.areAbilitate(sarcina)) {
                    if (!evenimenteList.str().empty()) evenimenteList << ", ";
                    evenimenteList << ev.getNume();
                }
            }
        }
        worksheet_write_string(worksheetVoluntari, row, 4, evenimenteList.str().c_str(), NULL);

        row++;
    }

    // Închide workbook-ul
    workbook_close(workbook);
    cout << "Datele au fost salvate în fișierul Excel: EvenimenteSiVoluntari.xlsx\n";
}


// Functii principale
void creeazaEveniment(vector<Eveniment>& evenimente) {
    string nume, data, locatie, descriere;
    cout << "Introduceți numele evenimentului: ";
    getline(cin, nume);
    cout << "Introduceți data evenimentului: ";
    getline(cin, data);
    cout << "Introduceți locația: ";
    getline(cin, locatie);
    cout << "Introduceți descrierea: ";
    getline(cin, descriere);

    evenimente.emplace_back(nume, data, locatie, descriere);
    cout << "Eveniment creat cu succes!\n";
}


void adaugaVoluntar(vector<Voluntar>& voluntari) {
    string nume, email, telefon;
    cout << "Introduceți numele voluntarului: ";
    getline(cin, nume);
    cout << "Introduceți email-ul voluntarului: ";
    getline(cin, email);
    cout << "Introduceți telefonul voluntarului: ";
    getline(cin, telefon);

    voluntari.emplace_back(nume, email, telefon);
    cout << "Voluntar adăugat cu succes!\n";
}

void atribuieSarcina(vector<Eveniment>& evenimente, vector<Voluntar>& voluntari) {
    if (evenimente.empty() || voluntari.empty()) {
        cout << "Nu există evenimente sau voluntari disponibili!\n";
        return;
    }

    cout << "Selectați evenimentul:\n";
    for (size_t i = 0; i < evenimente.size(); ++i) {
        cout << i + 1 << ". " << evenimente[i].getNume() << "\n";
    }

    int indexEveniment;
    cin >> indexEveniment;
    cin.ignore();

    if (indexEveniment < 1 || indexEveniment > evenimente.size()) {
        cout << "Eveniment invalid!\n";
        return;
    }

    Eveniment& ev = evenimente[indexEveniment - 1];

    cout << "Selectați voluntarul:\n";
    for (size_t i = 0; i < voluntari.size(); ++i) {
        cout << i + 1 << ". " << voluntari[i].getNume() << "\n";
    }

    int indexVoluntar;
    cin >> indexVoluntar;
    cin.ignore();

    if (indexVoluntar < 1 || indexVoluntar > voluntari.size()) {
        cout << "Voluntar invalid!\n";
        return;
    }

    Voluntar& vol = voluntari[indexVoluntar - 1];

    string descriereSarcina;
    cout << "Introduceți descrierea sarcinii: ";
    getline(cin, descriereSarcina);

    Sarcina sarcina(descriereSarcina);
    sarcina.atribuieVoluntar(vol.getNume());

    ev.adaugaSarcina(sarcina.getDescriere());
    cout << "Sarcina atribuită cu succes!\n";
}

// Functia pentru generarea raportului pentru fiecare eveniment
void genereazaRaportEveniment(const Eveniment& eveniment, const vector<Voluntar>& voluntari) {
    // Crează un fișier Excel pentru raport
    lxw_workbook* workbook = workbook_new((eveniment.getNume() + "_Raport.xlsx").c_str());
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, "Raport Eveniment");

    // Scrie datele evenimentului
    worksheet_write_string(worksheet, 0, 0, "Eveniment", NULL);
    worksheet_write_string(worksheet, 1, 0, eveniment.getNume().c_str(), NULL);

    worksheet_write_string(worksheet, 0, 1, "Data", NULL);
    worksheet_write_string(worksheet, 1, 1, eveniment.getData().c_str(), NULL);

    worksheet_write_string(worksheet, 0, 2, "Locație", NULL);
    worksheet_write_string(worksheet, 1, 2, eveniment.getLocatie().c_str(), NULL);

    worksheet_write_string(worksheet, 0, 3, "Descriere", NULL);
    worksheet_write_string(worksheet, 1, 3, eveniment.getDescriere().c_str(), NULL);

    // Scrie voluntarii și sarcinile
    worksheet_write_string(worksheet, 3, 0, "Voluntar", NULL);
    worksheet_write_string(worksheet, 3, 1, "Sarcina", NULL);
    worksheet_write_string(worksheet, 3, 2, "Status", NULL);

    int row = 4;
    for (const auto& vol : voluntari) {
        for (const auto& sarcinaDescriere : eveniment.getSarcini()) {
            // Căutăm dacă voluntarul are abilitatea necesară pentru sarcina respectivă
            if (vol.areAbilitate(sarcinaDescriere)) {
                worksheet_write_string(worksheet, row, 0, vol.getNume().c_str(), NULL);
                worksheet_write_string(worksheet, row, 1, sarcinaDescriere.c_str(), NULL);
                worksheet_write_string(worksheet, row, 2, "Finalizata", NULL); // Asumăm că sarcina e finalizată
                row++;
            }
        }
    }

    // Salvează fișierul Excel
    workbook_close(workbook);
    cout << "Raportul evenimentului " << eveniment.getNume() << " a fost salvat în " << eveniment.getNume() << "_Raport.xlsx\n";
}

// Integrarea funcției în fluxul principal
void raporteazaEveniment(const vector<Eveniment>& evenimente, const vector<Voluntar>& voluntari) {
    if (evenimente.empty()) {
        cout << "Nu există evenimente înregistrate!\n";
        return;
    }

    cout << "Selectați evenimentul pentru raport:\n";
    for (size_t i = 0; i < evenimente.size(); ++i) {
        cout << i + 1 << ". " << evenimente[i].getNume() << "\n";
    }

    int indexEveniment;
    cin >> indexEveniment;
    cin.ignore();

    if (indexEveniment < 1 || indexEveniment > evenimente.size()) {
        cout << "Eveniment invalid!\n";
        return;
    }

    // Apelarea funcției pentru generarea raportului
    genereazaRaportEveniment(evenimente[indexEveniment - 1], voluntari);
}

void afiseazaMeniu() {
    cout << "\n--- Manager Evenimente și Voluntari ---\n";
    cout << "1. Creează un eveniment\n";
    cout << "2. Adaugă un voluntar\n";
    cout << "3. Atribuie sarcini voluntarilor\n";
    cout << "4. Afișează evenimentele\n";
    cout << "5. Afișează voluntarii\n";
    cout << "6. Salvează datele în Excel\n";
    cout << "7. Generează raport pentru un eveniment\n"; // Noua opțiune
    cout << "0. Ieșire\n";
    cout << "Alege o opțiune: ";
}

int main() {
    vector<Eveniment> evenimente;
    vector<Voluntar> voluntari;

    while (true) {
        afiseazaMeniu();

        int optiune;
        cin >> optiune;
        cin.ignore();  // pentru a citi corect liniile de text

        switch (optiune) {
            case 1:
                creeazaEveniment(evenimente);
                break;
            case 2:
                adaugaVoluntar(voluntari);
                break;
            case 3:
                atribuieSarcina(evenimente, voluntari);
                break;
            case 4:
                for (const auto& ev : evenimente) {
                    ev.afiseazaDetalii();
                }
                break;
            case 5:
                for (const auto& vol : voluntari) {
                    vol.afiseazaDetalii();
                }
                break;
            case 6:
                salveazaInExcel(evenimente, voluntari);
                break;
            case 7:
                raporteazaEveniment(evenimente, voluntari); // Apelare funcție pentru raport
                break;
            case 0:
                cout << "Ieșire din aplicație...\n";
                return 0;
            default:
                cout << "Opțiune invalidă!\n";
        }
    }

    return 0;
}