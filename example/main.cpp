//
// Created by boy on 18-8-22.
//

#include <map>
#include <vector>
#include <iostream>





int main(int argc, char **argv){
    std::map<int32_t, int32_t> vv;
    vv[1] = 1;
    vv[2] = 2;
    vv.operator[](55) = 1;
    vv.operator[](54) = 5;
    for(auto itr = vv.begin(); itr != vv.end(); itr++){
        std::cout <<  "value " << itr->second << std::endl;
        std::cout << " key : " << itr->first << std::endl;
    }
    return 0;
}