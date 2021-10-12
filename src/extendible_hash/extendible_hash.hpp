
#include <bitset>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>


#pragma once

using namespace std;

class extendible_hash{
    private:
        // struct header{
        //     //Global Depth
        //     int D;
        //     // Max number of key-position pairs on a bucket
        //     int bucket_max_elements;
        // };
        
        int D;
        int fb; 
        struct bucket_header{
            /// The local depth.
            int d;

            /// The number of key-position pairs on a bucket.
            int size;
        };
        struct key_position
        {
            /// The hash itself.
            int key;

            /// The position of the key in the data file.
            int position;
        };

        struct bucket{
            
        };

        //header h;
	    fstream index_file;
        
    public:
        /// Create a new index.
        extendible_hash(
            int D,
            int fb,
            string index_filename
        );

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
        bool is_open() const;

        vector<int> get_positions(int key_hash);
        bool insert(Registro registro);
        bool delete_from(int key_hash, int position);

};


