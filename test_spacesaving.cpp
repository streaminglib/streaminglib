#include "spacesaving.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
    space_saving ss(500);
    vector<int> data(10240);
    for(int i = 0; i < 10240; i += 1){
    	data[i] = i + 1;
	}
	random_shuffle(data.begin(), data.end());
	for(int i = 0; i < 10240; i += 1){
		ss.insert(data[i], 10240 / data[i]);
	}
    vector<node> res(500);
    ss.query_top(res);
    // works better on skewed data
    for(int i = 0; i < 500; i += 1){
    	cout << res[i].value << endl;
	}
    return 0;
}
