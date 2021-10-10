#include <iostream>
//#include <bits/stdc++.h>
#include <vector>
#include <fstream>
#include <tuple>
using namespace std;

string file_name;
string aux_name;

struct Registro {
    
    char codigo [6];
    char nombre [21];
    char carrera [16];
    int ciclo;
    int next = 0;

    void setData() {
        cout << "Codigo:\n";
        cin >> codigo;
        cout << "Nombre:\n";
        cin >> nombre;
        cout << "Carrera:\n";
        cin >> carrera;
        cout << "Ciclo:\n";
        cin >> ciclo;
        cout << "Next:\n";
        cin >> next;
    }

    void show() {
        cout << '\n';
        cout << codigo << '\n';
        cout << nombre << '\n';
        cout << carrera << '\n';
        cout << ciclo << '\n';
        cout << next << '\n';
        cout << '\n';
    }

};

void replace(Registro registro, int pos, string file) {
    fstream data_file;
    data_file.open(file, ios::in | ios::out | ios::binary);
    int seekpos = (pos)*(sizeof(Registro));
    data_file.seekg(seekpos, ios::beg);
    data_file.write((char*)& registro, sizeof(Registro));
    data_file.close();
}

int file_size(string file){
    ifstream data_file;
    data_file.open(file, ifstream::ate | ifstream::binary);
    int ret = data_file.tellg()/sizeof(Registro);
    data_file.close();
    return ret;
}

Registro read_register(int m, string file){
    ifstream data_file;
    data_file.open(file, ios::binary);
    // 1-based index
    int seekpos = (m-1)*(sizeof(Registro));
    data_file.seekg(seekpos, ios::beg);
    Registro registro = Registro();
    data_file.read((char*) &registro, sizeof(Registro));
    data_file.close();
    return registro;
}

tuple<Registro, bool, int> sequential_exploration(int m, string key) {
    /*
    Esta funcion regresa el registro con el ultimo codigo
    que es menor o igual que la key buscada dentro de aux. Sigue la 
    secuencia de punteros en cada registro.

    Regreso:
        - candidate::Registro
        - stop_search::bool
        - candidate_logical_position::int

    Por ejemplo, si key=20
    
    Caso 1:
    4
    5
    30

    bb para, retorna lo que retorna esta funcion (5)
    bool : 1

    Caso 2:
    4
    5
    20
    bb para, retorna lo que retorna esta funcion (20)
    bool : 1

    Caso 3:
    4
    5
    11
    bb sigue, no retorna
    bool : 0
*/
    Registro prev = Registro();
    //prev.next = -1; //solo para que entre al while
    Registro registro = Registro();
    bool stop_search = false;
    int m_prev = m;

    while (m < 0) {
        prev = registro;
        registro = read_register(-m, aux_name);
        //registro.show();
        if (registro.codigo == key) {
           return {registro, true, m}; //{registro_encontrado, stop, posicion_logica}
        }
        if (registro.codigo > key) {
            return {prev, true, m_prev};
        }
        m_prev = m;
        m = registro.next;
    }

    prev = registro;
    registro = read_register(m, file_name);

    if (registro.codigo > key)
        return {prev, true, m_prev};
    if (registro.codigo == key)
        return {registro, true, m};
    return {prev, false, m_prev};
}

pair<Registro, int> BB(string key) {
    /*
        Regresa un par con dos elementos:
            - El primero es el registro encontrado
            - El segundo es la posición lógica de dicho registro
    */
    
    Registro registro = Registro();
    Registro prev = Registro();

    int l = 1; 
    int r = file_size(file_name);
    int m = (l + r) / 2, m_prev = m;
    
    while (l <= r) {
        prev = registro;
        m_prev = m;
        m = (l + r) / 2;
        registro = read_register(m, file_name);
        //registro.show();
        if (registro.next < 0) {
            auto [seq_reg, is_greater_equal, index] = sequential_exploration(registro.next, key);
            if (is_greater_equal) 
                return {seq_reg, index};
        }
        if (registro.codigo < key)
            l = ++m;
        else if (registro.codigo > key) 
            r = --m;
        else if (registro.codigo == key){
            return {registro, m};
        }
    }

    if (registro.codigo > key) return {prev, m_prev};
    return {registro, m};
}

void write_record(Registro registro, string file) {
    ofstream  data_file;
    data_file.open(file, ios::app | ios::binary);
    data_file.write((char*)& registro, sizeof(Registro));
    data_file.close();
}

void add(Registro registro) {
    // search buscar el índice del archivo anterior
    
    auto [prev_reg, prev_m] = BB(registro.codigo);
    
    /* BB devolvió el elemento previo al registro a insertar
        el next del resultado de BB debe pasar a ser el next
        del registro a insertar  */

    registro.next = prev_reg.next;
    write_record(registro, aux_name);
    
    //sacar el tamaño del file aux
    prev_reg.next = -file_size(aux_name);
    
    //escribir el prev nuevamente
    cout << "\n prev_m: " << prev_m <<" \n";

    if (prev_m >= 0) 
        //escribirlo en la posicion prev_m en el data
        replace(prev_reg, prev_m-2, file_name);
    
    else 
        //escribirlo en el aux
        replace(prev_reg, -prev_m-1, aux_name);
    
}

void insertAll(vector<Registro> registros) {
    for (auto r : registros) 
        add(r);
}

// testear con los indexados en aux
Registro search (string key) {
    
    Registro registro = BB(key).first;

    if (registro.codigo == key) return registro;

    return Registro();
}

vector<Registro> rangeSearch(string begin, string end) {
    vector<Registro> registros;
    //TODO
    return registros;
}

void scan_all() {
    ifstream file;
    file.open(file_name, ios::in | ios::binary);
    Registro *registro;
    if (file.is_open()) {
        while (!file.eof()) {
            file.read((char*) &registro, sizeof(Registro));
            registro->show();
        }
        file.close();
    }
}

int main() {

   file_name = "data.dat";
   aux_name = "aux.dat";



    Registro reg = search("P-365");

    reg.show();
return 0;

//   return 0;

    /*
    Registro record;
    for (int i = 0; i < 7; i++) {
        record.setData();
        write_record(record, file_name);
    } 
    */
    

   //return 0;
    
    /*
    Registro r;
    for (int i = 1; i <= 7; i++) {
        r = read_register(i, file_name);
        r.show();
    } */

    
    // P-150 Macarena CS 5 
    /*
    Registro add_r = Registro();
    add_r.setData();
    add(add_r);*/
    

    //BB("P-155").first.show();


    /*
    // P-154 Jose CS 5 
    Registro add_r = Registro();
    add_r.setData();
    add(add_r);
    */

    //BB("P-151").first.show();

    for (int i = 1; i <= 7; i++) {
        Registro r2 = read_register(i, file_name);
        r2.show();
    }

    for (int i = 1; i <= 5; i++) {
        Registro r2 = read_register(i, aux_name);
        r2.show();
    }

    return 0;

    
    // P-154 Jose CS 5 
    /*
    Registro add_r = Registro();
    add_r.setData();
    add(add_r);
    */
    Registro r = search("P-154");

    r.show();

    return 0;
}
