
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
        struct header{
            //Global Depth
            int D;
            // Max number of key-position pairs on a bucket
            int bucket_max_elements;
        };
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
        header h;
	    fstream index_file;

        void write_empty_bucket(int d);

        /// Get the size of bucket_header, it's pairs and the pointer.
        int bucket_size() const;

        /// Get the d least significant bits from the hash.
        int d_bit(int d, int key_hash) const;

        /// Get the D least significant bits from the hash.
        int D_bit(int key_hash) const;

        /// Returns 0(d_bit(d,key)) 1(d_bit(d,key))
        pair<int, int> get_new_d_bits(int d, int key_hash) const;

        /// Get the position of the directory of a hash.
        int get_directory(int key_hash) const;

        /// Get the position of the first bucket.
        int get_first_bucket() const;

        /// Get the pointer of a bucket from a hash.
        int get_bucket(int key_hash);

        /// Updates the header.
        void set_bucket_header(int pos, bucket_header h);

        /// Updates the header's pointer
        void set_pointer(int pos, int b_p);

        /// Gets a header from a bucket.
        bucket_header get_bucket_header(int pos);

        /// Gets a pointer from a bucket.
        int get_pointer(int pos);

        /// Add a pair to a bucket with enough space.
        void add_pair(int pos, key_position kp);

    public:
    	/// Use existing index.
        extendible_hash(const filesystem::path& index_path);

        /// Create a new index.
        extendible_hash(
            const filesystem::path& index_path,
            int D,
            int bucket_max_elements
        );

        bool is_open() const;

        vector<int> get_positions(int key_hash);
        bool insert(int key_hash, int position);
        bool delete_from(int key_hash, int position);
};
