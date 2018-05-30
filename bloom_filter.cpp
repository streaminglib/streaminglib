#include <vector>
#include <string>
#include <algorithm>
#include "MurmurHash3.h"
using namespace std;

typedef vector<vector<bool> > hash_table_t;


class Hash_table
{
public:
	Hash_table(int filters, int hashes, int seed);
	int get_filters() const;
	int get_hashes() const;
private:
	hash_table_t ht;
}

class Bloom_filter
{
public:
	Bloom_filter(int filter_len, int hashes, int seed=131);
	void insert_element(string elem);
	bool query_element(string elem) const;
	double get_occupancy() const;
	int get_filter_len() const;
	int get_hashes() const;
	int get_seed() const;
private:
	int _get_hashval(string elem) const;
	hash_table_t hash_table;
	int filter_len;
	int hashes;
	int seed;
};

Bloom_filter::Bloom_filter(int filter_len, int hashes)
{
	hash_table.resize(hashes);
	for(int i = 0; i < hashes; i++)
		hash_table[i].resize(filter_len);
	this->filter_len = filter_len;
	this->hashes = hashes;
	this->seed = seed;
}

void Bloom_filter::insert_element(string key)
{
	int len = key.size();
	const void *data = key.c_str();
	char out[16];
	int hashval = 0;
	for(int i = 0; i < hashes; i++)
	{
		MurmurHash_x64_128(data, len, seed+i, out);
		hashval = *(int *)out;
		hash_table[i][hashval%filter_len] = True;
	}
}

bool Bloom_filter::query_element(string key)
{
	int len = key.size();
	const void *data = key.c_str();
	char out[16];
	int hashval = 0;
	for(int i = 0; i < hashes; i++)
	{
		MurmurHash_x64_128(data, len, seed+i, out);
		hashval = *(int *)out;
		if(hash_table[i][hashval%filter_len] == false)
			return false;
	}
	return true;
}

int Bloom_filter::get_filter_len() const
{
	return filter_len;
}

int Bloom_filter::get_hashes() const
{
	return hashes;
}

int Bloom_filter::get_seed() const
{
	return seed;
}

void Bloom_filter::get_occupancy(double[] out) const
{
	for(int i = 0; i < hashes; i++)
	{
		out[i] = 0;
		for(int j = 0; j < filter_len; j++)
		{
			if(hash_table[i][j])
				out[i] ++;
		}
		out[i] /= filter_len;
		
	}
}


int main()
{
	string filename = "./test_file";
	int query_num = 32;
	int filter_size = 128;
	int hash_num = 5;
	
	bf = Bloom_filter(filter_size, hash_num);
	bf.load_data_from_file(filename);
	bool exist = bf.query(query_num);
	cout << exist << endl;
	
	return 0;
}


