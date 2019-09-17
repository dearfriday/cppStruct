//
// Created by nova on 10/09/2019.
//


#include <tree/bpptree.hpp>

#include <string>

using namespace zero;


struct book {

    size_t id = 0;
    std::string name;


//    book& operator = (const book &b){
//        id = b.id;
//        name = b.name;
//        return *this;
//    }


    bool operator<(const book &b) const {
        return id < b.id;
    }

    bool operator>(const book &b) const {
        return id > b.id;
    }

};


int main() {
//    std::vector<size_t> test;
//    test.push_back(1);
//    test.push_back(3);
//    test.push_back(5);  //2
//    test.push_back(7);
//    test.push_back(9);
//
//
//
//    size_t key = 9;
//
//    size_t front = 0;
//    size_t end = test.size() - 1;
//    size_t mid = (front + end) / 2;
//
//    auto func = [](const size_t &t1, const size_t &t2) {
//        return t1 < t2;
//    };
//
//    //&& test.size() > mid && mid > 0
//    while (front < end ) {
//        if (func(key, test[mid])) {
//            end = mid > 0 ? mid - 1 : mid;
//        } else if (func(test[mid], key)) {
//            front = mid + 1;
//        } else {
//            std::cout << "eq ? " << mid << std::endl;
//            break;
//        }
//        mid = (front + end) / 2;
//    }
//
//    if (func(test[mid], key)) {
//        mid++;
//    } else {
//        std::cout << "eq ? " << mid << std::endl;
//    }
//
//
//
//
//
//    std::cout << "index " << mid << std::endl;

    try {
        std::cout << "xxxxxxxxx\n";
        bpptree<int, 4> books;
        std::vector<int> data = {39 , 22, 97, 41, 53, 13, 21, 40, 30, 27, 33, 36, 35, 34, 24, 29, 26, 17, 28, 29, 31};
        for(size_t i = 0; i < data.size(); i++){
            books.insert(data[i]);
        }
        books.insert(32);
        std::cout << "\n";
    }catch (const std::exception &e){
        std::cout << e.what() << std::endl;
    }catch (...){
        std::cout << "unkown exception ....";
    }





    return 0;
}
