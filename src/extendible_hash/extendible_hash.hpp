
#include <bitset>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>


#pragma once

class extendible_hash{
    private:
        struct header{
            //Global Depth
            size_t D;
            // Max number of key-position pairs on a bucket
            size_t bucket_max_elements;
        };
        struct bucket_header{
            /// The local depth.
            size_t d;

            /// The number of key-position pairs on a bucket.
            size_t size;
        };
        struct key_position
        {
            /// The hash itself.
            size_t key;

            /// The position of the key in the data file.
            size_t position;
        };
        header h;
	    std::fstream index_file;

        void write_empty_bucket(size_t d);

        /// Get the size of bucket_header, it's pairs and the pointer.
        size_t bucket_size() const;

        /// Get the d least significant bits from the hash.
        size_t d_bit(size_t d, size_t key_hash) const;

        /// Get the D least significant bits from the hash.
        size_t D_bit(size_t key_hash) const;

        /// Returns 0(d_bit(d,key)) 1(d_bit(d,key))
        std::pair<size_t, size_t> get_new_d_bits(size_t d, size_t key_hash) const;

        /// Get the position of the directory of a hash.
        size_t get_directory(size_t key_hash) const;

        /// Get the position of the first bucket.
        size_t get_first_bucket() const;

        /// Get the pointer of a bucket from a hash.
        size_t get_bucket(size_t key_hash);

        /// Updates the header.
        void set_bucket_header(size_t pos, bucket_header h);

        /// Updates the header's pointer
        void set_pointer(size_t pos, size_t b_p);

        /// Gets a header from a bucket.
        bucket_header get_bucket_header(size_t pos);

        /// Gets a pointer from a bucket.
        size_t get_pointer(size_t pos);

        /// Add a pair to a bucket with enough space.
        void add_pair(size_t pos, key_position kp);

    public:
    	/// Use existing index.
        extendible_hash(const std::filesystem::path& index_path);

        /// Create a new index.
        extendible_hash(
            const std::filesystem::path& index_path,
            size_t D,
            size_t bucket_max_elements
        );

        bool is_open() const;

        std::vector<size_t> get_positions(size_t key_hash);
        bool insert(size_t key_hash, size_t position);
        bool delete_from(size_t key_hash, size_t position);
};
