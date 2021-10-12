#include <assert.h>
#include "extendible_hash.hpp"

using namespace std;


extendible_hash::extendible_hash(const filesystem::path& index_path):index_file(index_path,
		ios::binary |
		ios::in |
		ios::out
	)
{

}

