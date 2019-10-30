//
// Created by friday on 2019/10/28.
//



#include <treeplus/bplusTree.hpp>
#include <iostream>
int main(){

    zero::bplusTree<int, std::string, 4> bp;
//    size_t end = 6;
//    for(size_t i = 0; i < end; i++){
//        bp.insert(i, std::to_string(i));
//    }
//    bp.insert(end, std::to_string(end));

    std::vector<int> vv = {5, 8, 10, 15, 16, 17, 18, 19, 20, 21, 6, 9, 7, 22};
    for(size_t i = 0; i < vv.size() - 1; i++){
        bp.insert(vv[i], std::to_string(vv[i]));
    }
    bp.debug();
    bp.insert(vv.back(), std::to_string(vv.back()));

    bp.remove(22);
    bp.remove(15);
    bp.debug();
    bp.remove(7);
    bp.debug();


    std::cout << "1111111-----" << std::endl;

    return 0;
}