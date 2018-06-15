#include "skim_sketch.h"

#include <iostream>

skim_sketch::skim_sketch(): h1(32, s2, s1), h2(32, s2, s1){
    join_size = 0;
}


skim_sketch::skim_sketch(std::vector<int> f1, std::vector<int> f2, int domain_size): h1(32, s2, s1), h2(32, s2, s1){
    std::cout << "in skim_sketch()" << std::endl;
    join_size = 0;

    flow_length1 = f1.size();
    flow_length2 = f2.size();

    e1.resize(domain_size);
    e2.resize(domain_size);

    get_hash_table(f1, h1);
    get_hash_table(f2, h2);
    join_size = est_skim_join_size();
}


int skim_sketch::est_skim_join_size(){
    std::cout << "in est_skim_join_size()" << std::endl;
    skim_dense(h1, flow_length1, e1);
    skim_dense(h2, flow_length2, e2);
    int j_dd = inner_product(e1, e2, e1.size());
    int j_ds = est_sub_join_size(e1, h2);
    int j_sd = est_sub_join_size(e2, h1);

    std::vector<int> j_sss(s1);
    for(int p = 0; p < s1; p += 1){
        j_sss[p] = 0;
        for(int q = 0; q < s2; q += 1){
            j_sss[p] += h1.get(q, p) * h2.get(q, p);
        }
    }
    int j_ss = median(j_sss, s1);
    return j_dd + j_ds + j_sd + j_ss;
}

void skim_sketch::skim_dense(Hashtable & h, int & flow_length, std::vector<int> & e){
    std::cout << "in skim_dense()" << std::endl;
    int threhold = flow_length / s1 * 0.1;
    for(int i = 0; i < e.size(); i += 1){
        e[i] = 0;
    }
    for(int i = 0; i < e.size(); i += 1){
        std::vector<int> f(s1);
        for(int j = 0; j < s1; j += 1){
            f[j] = h.get(hash_template(j, i), j) * (((hash_template(i, j) & 1) << 1) - 1);
        }
        int est_u = median(f, s1);
        if(est_u >= 2 * threhold){
            e[i] = est_u;
        }
    }

    for(int i = 0; i < e.size(); i += 1){
        if(e[i] > 0){
            nvec idx(s1);
            vector<int> delta(s1);
            for(int j = 0; j < s1; j += 1){
                idx[j] = hash_template(j, i);
                delta[j] = (-1) * e[i] * (((hash_template(i, j) & 1) << 1) - 1);
            }
            h.inc(idx, delta, -1);
        }
    }
}

int skim_sketch::est_sub_join_size(std::vector<int> & e, Hashtable & h){
    std::cout << "in est_sub_join_size()" << std::endl;
    std::vector<int> j(s1);
    for(int p = 0; p < s1; p += 1){
        j[p] = 0;
        for(int i = 0; i < e.size(); i += 1){
            if(e[i] > 0){
                int q = hash_template(p, i);
                j[p] += h.get(q, p) * e[i] * (((hash_template(i, p) & 1) << 1) - 1);
            }
        }
    }
    int res = median(j, s1);
    return res;
}

int hash_template(int seed, int n) {
	uint64_t *res = (uint64_t* )malloc(sizeof(uint64_t) * 2);
	MurmurHash3_x64_128(&n, 1, seed, res);

	int ans = (*res) & (s2 - 1);
	free(res);
	return ans;
}

void get_hash_table(std::vector<int> & f, Hashtable & h){
    std::cout << "in get_hash_table()" << std::endl;
    for(int i = 0; i < f.size(); i += 1){
        nvec idx(s1);
        vector<int> delta(s1);
        for(int j = 0; j < s1; j += 1){
            idx[j] = hash_template(j, f[i]);
            delta[j] = ((hash_template(f[i], j) & 1) << 1) - 1;
        }
        h.inc(idx, delta, -1);
    }
}

int inner_product(vector<int> &p, vector<int> &q, int length){
    int res = 0;
    for(int i = 0; i < length; i += 1){
        res += p[i] * q[i];
    }
    return res;
}

int median(vector<int> p, int length){
    vector<int> pv(p);
    nth_element(pv.begin(), pv.begin() + pv.size() / 2, pv.end());
    return pv[length / 2];
}
