#include "spacesaving.h"

space_saving::space_saving(){
    used = 0;
    size = 0;
    heap.resize(size);
}

space_saving::space_saving(int s){
    used = 0;
    size = s;
    heap.resize(size);
}

void space_saving::insert(int val, int freq){
    n += freq;
    for(int i = 0; i < used; i += 1){
        if(heap[i].value == val){
            heap[i].freq += freq;
            heapify(i);
            return ;
        }
    }
    if(used < size){
        heap[used] = node(val, freq);
        used += 1;
        if(used == size){
            make_heap();
        }
        return ;
    }
    heap[0].value = val;
    heap[0].over_est = heap[0].freq;
    heap[0].freq += 1;
    heapify(0);
}

void space_saving::query_top(std::vector<node> & res){
    res.assign(heap.begin(), heap.end());
}

void space_saving::heapify(int idx){
    int left_son = (idx << 1) + 1;
    int right_son = (idx << 1) + 2;
    if(right_son < used){
        int min_val = std::min(heap[idx].freq, std::min(heap[left_son].freq, heap[right_son].freq));
        if(min_val == heap[idx].freq){
            return ;
        }else if(min_val == heap[left_son].freq){
            std::swap(heap[idx], heap[left_son]);
            heapify(left_son);
            return ;
        }else{
            std::swap(heap[idx], heap[right_son]);
            heapify(right_son);
            return ;
        }
    }else if(left_son < used){
        int min_val = std::min(heap[idx].freq, heap[left_son].freq);
        if(min_val == heap[idx].value){
            return ;
        }else{
            std::swap(heap[idx], heap[left_son]);
            heapify(left_son);
            return ;
        }
    }else{
        return ;
    }
    return ;
}

void space_saving::make_heap(){
    for(int idx = (size - 1) / 2; idx >= 0; idx -= 1){
        heapify(idx);
    }
}
