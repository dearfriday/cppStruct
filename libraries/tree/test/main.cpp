//
// Created by nova on 10/09/2019.
//


#include <tree/bpptree.hpp>
//
#include <string>
#include <iostream>

using namespace zero;


struct book {


    size_t id = 0;
    std::string name;

    bool operator<(const book &b) const {
        return id < b.id;
    }

    bool operator>(const book &b) const {
        return id > b.id;
    }


    std::string operator()(){
        return std::to_string(id);
    }

};


int main() {
    try {
        std::cout << "xxxxxxxxx\n";
        bpptree<int, 4> books;
        std::vector<int> data = {39, 22, 97, 41, 53, 13, 21, 40, 30, 27, 33, 36, 35, 34, 23, 24, 29, 26, 32, 17, 28, 31};

        for (size_t i = 0; i < data.size(); i++) {
            books.insert(data[i]);
        }
        books.insert(32);



        books.debug();

        books.remove(21);
        books.remove(27);
        books.remove(32);
        books.remove(40);
        for(auto &itr : books){
            std::cout << " "  << itr << ",";
        }
        std::cout << "\n---------------------";




    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "unkown exception ....";
    }


    return 0;
}
