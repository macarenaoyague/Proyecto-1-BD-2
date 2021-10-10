#include <bits/stdc++.h>
#include <fstream>
using namespace std;

int index_level;
//nombre genérico de los archivos indice, añadir el númmero al leer
string index_name;
//nombre data
string data_name;
//máxima cantidad de índices

//factor de bloque
const int fb = 4;

const int page_size = 100;
const int keys_per_page = page_size/(sizeof(char[6])+sizeof(int));

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

struct PageIndex {
    /* key, value */
    pair<char[6], int> entries [keys_per_page];
};

struct PageData {
    Registro registros[fb];
    int cont = 0;
    int overflow_next = -1;
};

//falta la parte de agregar a indices, splits
void add_register(int pos, Registro registro, string file_name) {

    PageData page = PageData();

    fstream data_file;
    data_file.open(file_name+".dat", ios::in | ios::out | ios::binary);
    int seekpos;

    while(true) {
        page = read_page<PageData>(data_name, pos);
        /* espacio libre en page */
        if (page.cont < fb) break;
        //no espacio libre en page y no overflow page existente
        if (page.overflow_next == -1) {
            page = PageData(); 
            break; 
        }
        pos = page.overflow_next;
    }

    page.registros[page.cont++] = registro;
    seekpos = (pos)*page_size;
    data_file.seekg(seekpos, ios::beg);
    data_file.write((char*)&page, page_size);
    data_file.close();
}

template<typename T>
T read_page(string file_name, int pos) {
    /*
        Me posiciono en la ubicacion fisica de la
        page y leo su contenido.
    */
    ifstream data_file;
    data_file.open(file_name+".dat", ios::binary);
    // 0-based index
    int seekpos = (pos)*page_size;
    data_file.seekg(seekpos, ios::beg);
    T page = T();
    data_file.read((char*) &page, page_size);
    data_file.close();
    return page;
}

int BB(PageIndex page, string key) {

    /*
        Regresa un par con dos elementos:
            - El primero es el registro encontrado
            - El segundo es la posición lógica de dicho registro
    */
    
    auto& page_keys = page.entries;

    int l = 0; 
    int r = keys_per_page-1;
    int m;

    char current_key[6];
    int next_page_index;

    while (l <= r) {

        m = (l + r) / 2;

        strncpy(current_key, page_keys[m].first, 6);
        next_page_index = page.entries[m].second;

        if (string(current_key) < key)
            l = ++m;

        else if (string(current_key) > key) 
            r = --m;

        else if (string(current_key) == key) 
            return next_page_index;
    }

    return next_page_index;
}

void add(Registro registro) {
    int level_cur = 0;
    PageIndex page = PageIndex();
    int pos = 0;
    /* 
        Buscar en los niveles < al último 
        para hallar la siguiente página 
        con key menor a.
    */
    while(++level_cur <= index_level) {
        page = read_page<PageIndex>(index_name.append(to_string(level_cur)), pos);
        pos = BB(page, registro.codigo);
    }
    add_register(pos, registro, data_name);
}

vector<Registro> read_all_records(int logical_position) {
    /*
        This will return all records of the page and the 
        linked overflow pages.
    */
    vector<Registro> registros;
    PageData page = PageData();
    do {
       page = read_page<PageData>("data", page.overflow_next);
       if (page.cont == 0) //pagina vacia 
            return registros;
       registros.insert(registros.end(), begin(page.registros), begin(page.registros) + page.cont);
    } while(page.overflow_next != -1);

    return registros;
}

vector<Registro> search(string key) {
    /* 
        Buscar en los niveles < al último 
        para hallar la siguiente página 
        con key menor a
    */

    int level_cur = 0;
    PageIndex page = PageIndex();
    int pos = 0;

    while(++level_cur <= index_level) {
        page = read_page<PageIndex>(index_name.append(to_string(level_cur)), pos);
        pos = BB(page, key);
    }
    
    /* Leer la data */
    vector<Registro> possible_matchs = read_all_records(pos);
    vector<Registro> matchs;

    /* Busqueda secuencial */
    for (auto e : possible_matchs)
        if (e.codigo == key) 
            matchs.push_back(e);

    return matchs;
}

vector<Registro> rangeSearch(string begin_key, string end_key) {
    vector<Registro> registros;
    PageData page = PageData();
    do {
       page = read_page<PageData>("data", page.overflow_next);
       if (page.cont == 0) //pagina vacia 
            return registros;
       registros.insert(registros.end(), begin(page.registros), begin(page.registros) + page.cont);
    } while(page.overflow_next != -1);

    return registros;
}

int main () {
    index_level = 1;
    data_name = "data";
    index_name = "index";

    return 0;
}