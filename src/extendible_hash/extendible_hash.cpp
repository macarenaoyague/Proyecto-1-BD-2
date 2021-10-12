#include <assert.h>
#include "extendible_hash.hpp"

extendible_hash::extendible_hash(const std::filesystem::path& index_path):
	index_file(index_path,
		std::ios::binary |
		std::ios::in |
		std::ios::out
	)
{

}

