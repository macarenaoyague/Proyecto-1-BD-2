#include <iostream>
//#include <bits/stdc++.h>
#include <vector>
#include <fstream>
#include <tuple>
#include "parsing.h"

using namespace std;

string file_name;
string aux_name;
string header_name;
const string MAX_VALUE(60, 'Z');


// struct Population {
    
//     char country [6];
//     char nombre [20];
//     char carrera [20];
//     int ciclo;
//     int next = 0;

//     void setData() {
//         cout << "country:\n";
//         cin >> country;
//         cout << "Nombre:\n";
//         cin >> nombre;
//         cout << "Carrera:\n";
//         cin >> carrera;
//         cout << "Ciclo:\n";
//         cin >> ciclo;
//         cout << "Next:\n";
//         cin >> next;
//     }

//     void show() {
//         cout << '\n';
//         cout << country << '\n';
//         cout << nombre << '\n';
//         cout << carrera << '\n';
//         cout << ciclo << '\n';
//         cout << next << '\n';
//         cout << '\n';
//     }

// };

void set_header()
{
    fstream header_file;
    header_file.open(header_name, ios::in | ios::out | ios::binary);
    int pos = 1;
    header_file.write((char*) &pos, sizeof(int));
    // char aux_first_key[6] = {'P', '-', '9', '9', '9'};
    char aux_first_key[60];
    fill(aux_first_key, aux_first_key+60, 'Z');
    header_file.write((char*) &aux_first_key, 60);
    header_file.close();
}

pair<int, string> read_header()
{
    fstream header_file;
    header_file.open(header_name, ios::in | ios::binary);

    int pos_aux_first_key;
    char aux_first_key[6];
    header_file.read((char*) &pos_aux_first_key, sizeof(int));
    header_file.read((char*) &aux_first_key, sizeof(aux_first_key));
    
    header_file.close();

    return {pos_aux_first_key, string(aux_first_key)};
}

void update_header(string aux_first_key, int pos)
{
    fstream header_file;
    header_file.open(header_name, ios::in | ios::out | ios::binary);
    char aux_first_key_array[60];
    copy(aux_first_key.begin(), aux_first_key.end(), aux_first_key_array);
    header_file.write((char*) &pos, sizeof(int));
    header_file.write((char*) &aux_first_key_array, 60);
    header_file.close();
}

Population read_register(int m, string file){
    ifstream data_file;
    data_file.open(file, ios::binary);
    // 1-based index
    int seekpos = (m-1)*(sizeof(Population));
    data_file.seekg(seekpos, ios::beg);
    Population population = Population();
    data_file.read((char*) &population, sizeof(Population));
    data_file.close();
    return population;
}

int file_size(string file){
    ifstream data_file;
    data_file.open(file, ifstream::ate | ifstream::binary);
    int ret = data_file.tellg()/sizeof(Population);
    data_file.close();
    return ret;
}

void write_register(Population population, string file) {
    auto [pos_aux_first_key, aux_first_key] = read_header();
    ofstream  data_file;
    data_file.open(file, ios::app | ios::binary);
    data_file.write((char*)& population, sizeof(Population));
    data_file.close();
    //Update header with new record if necessary
    auto str_code = string(population.country);
    if (str_code < aux_first_key)
    {
        int size = file_size(aux_name);
        update_header(str_code, size);
    }
}

void update_register(Population Population, int pos, string file) {
    fstream data_file;
    data_file.open(file, ios::in | ios::out | ios::binary);
    int seekpos = (pos)*(sizeof(Population));
    data_file.seekg(seekpos, ios::beg);
    data_file.write((char*)& Population, sizeof(Population));
    data_file.close();
}

tuple<Population, bool, int> sequential_exploration(int m, string key) {
    /*
    Esta funcion regresa el Population con el ultimo country
    que es menor o igual que la key buscada dentro de aux. Sigue la 
    secuencia de punteros en cada Population.

    Regreso:
        - candidate::Population
        - stop_search::bool
        - candidate_logical_position::int

    Por ejemplo, si key=20:
    
    Caso 1:
        4, 5, 30

        Detener Binary Search, retorna lo que retorna esta funcion (5)
        bool : 1

    Caso 2:
        4, 5, 20

        Detener Binary Search, retorna lo que retorna esta funcion (20)
        bool : 1

    Caso 3:
        4, 5, 11

        Continuar Binary Search, no retorna
        bool : 0
*/

    auto [pos_aux_first_key, aux_first_key] = read_header();

    if (aux_first_key == MAX_VALUE)
    {
        Population not_found = Population();
        copy(aux_first_key.begin(), aux_first_key.end(), not_found.country);
        not_found.next = -2;
        return {not_found, false, -1};
    }

    Population Population_prev = Population();
    Population population = Population();
    bool stop_search = false;
    int m_prev = m;

    while (m < 0) {
        Population_prev = population;
        population = read_register(-m, aux_name);
        //Population.show();
        if (population.country == key) {
           return {population, true, m}; //{Population_encontrado, stop, posicion_logica}
        }
        if (population.country > key) {
            return {Population_prev, true, m_prev};
        }
        m_prev = m;
        m = population.next;
    }

    Population_prev = population;
    population = read_register(m, file_name);

    if (population.country > key)
        return {Population_prev, true, m_prev};
    if (population.country == key)
        return {population, true, m};
    return {Population_prev, false, m_prev};
}

pair<Population, int> search_util(string key) {
    /*
        Regresa un par con dos elementos:
            - El primero es el Population encontrado
            - El segundo es la posición lógica de dicho Population
    */
    
    Population population = Population();
    Population prev = Population();

    int l = 1; 
    int r = file_size(file_name);
    int m = (l + r) / 2, m_prev = m;

    auto [pos_aux_first_key, aux_first_key] = read_header();
    auto first_data_key = string(read_register(1, file_name).country);

    if (key < first_data_key)
    {
        auto [seq_reg, is_greater_equal, index] = sequential_exploration(-pos_aux_first_key, key); 
        //seq_reg.show();
        //cout << index << endl;

        auto str_code = string(seq_reg.country);
        if (str_code <= key)
        {
            return make_pair(seq_reg, index);
        }
        if (str_code == MAX_VALUE)
        {
            auto Population_dummy = Population();
            Population_dummy.next = 1;

            return make_pair(Population_dummy, 1);
        }
        return make_pair(Population(), 0);
    }
    
    while (l <= r) {
        prev = population;
        m_prev = m;
        m = (l + r) / 2;
        population = read_register(m, file_name);
        //Population.show();
        if (population.next < 0) {
            auto [seq_reg, is_greater_equal, index] = sequential_exploration(population.next, key);
            if (is_greater_equal) 
                return {seq_reg, index};
        }
        if (population.country < key)
            l = ++m;
        else if (population.country > key) 
            r = --m;
        else if (population.country == key){
            return {population, m};
        }
    }

    if (population.country > key) return {prev, m_prev};
    return {population, m};
}

void add(Population Population) {
    // search buscar el índice del archivo anterior
    auto [result_register, prev_m] = search_util(Population.country);

    if (string(result_register.country) == string(Population.country))
        return;

    /* search_util devolvió el elemento previo al Population a insertar
        el next del resultado de search_util debe pasar a ser el next
        del Population a insertar  */

    Population.next = result_register.next;
    write_register(Population, aux_name);
    
    //sacar el tamaño del file aux
    result_register.next = -file_size(aux_name);

    //escribir el prev nuevamente
    //cout << "\n prev_m: " << prev_m <<" \n";

    if (prev_m >= 0) 
        //escribirlo en la posicion prev_m en el data
        update_register(result_register, prev_m-2, file_name);
    
    else 
        //escribirlo en el aux
        update_register(result_register, -prev_m-1, aux_name);
}

void insertAll(vector<Population> Populations) {
    for (auto r : Populations) 
        add(r);
}

// testear con los indexados en aux
Population search (string key) {

    auto [pos_aux_first_key, aux_first_key] = read_header();

    auto population = search_util(key).first;

    if (population.country == key) return population;

    return Population();
}

vector<Population> rangeSearch(string begin, string end) {
    vector<Population> Populations;
    //TODO
    return Populations;
}

void scan_all() {
    ifstream file;
    file.open(file_name, ios::in | ios::binary);
    Population *Population;
    if (file.is_open()) {
        while (!file.eof()) {
            file.read((char*) &Population, sizeof(Population));
            Population->show();
        }
        file.close();
    }
}

void restructure()
{

}

void clear_file(string file_name)
{
    ofstream ofs;
    ofs.open(file_name, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
}


std::vector<Population> useSeqFile(int useCase) {
    file_name = "data.dat";
    aux_name = "aux.dat";
    header_name = "header.dat";
    int size_ = 0 ;
    std::vector <Population> retorno;
    switch (useCase)
    {
    case 5:
        size_ = file_size(file_name);
        for (int i = 1; i <= size_; i++) {
            Population r2 = read_register(i, file_name);
            retorno.push_back(r2);
        }        
        break;
    default:
        break;
    }
    return retorno;
}

