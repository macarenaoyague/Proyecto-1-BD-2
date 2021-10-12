#include <assert.h>
#include "extendible_hash.hpp"

using namespace std;

extendible_hash::extendible_hash(int D, int fb, string index_filename){
    this->D = D;
    this->fb = fb;
    this->index_file.open(index_filename, ios::in | ios::binary);
	//index_file.read((char*)&h, sizeof(h));
}

extendible_hash::~extendible_hash(){
    this->index_file.close();
}


bool extendible_hash::insert(Registro registro){
    /* ver si index esta vacio
        - si esta vacio:
            Crear 0 y 1
        Hallar valor H de la key del registro
        Valor H a hash -> binario
        Buscar en el indice LSB, HI
        Buscar el bucket -> ver si esta lleno, si no es asi, agregar registro al Bucket. Si es asi, hacer el resize.
        Resize: 
            si d < D
                Agregar bit a la izquierda de HI(agregar 0 y 1)
                Cambiar HI por 0+HI
                Escribir en el indice el 1+HI
                Crear nodo en el data File asociado a 1+HI
                Volver a insertar los registros que estaban en el bucket y el nuevo registro
                Actualizar el contador de los nodos -> actualizar cantidad_de_nodos_en_bucket
    */
}