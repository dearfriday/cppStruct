//
// Created by nova on 10/09/2019.
//


#include <tree/bpptree.hpp>

#include <string>

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

};


int main() {
//    std::vector<size_t> test;
//    test.push_back(1);
//    test.push_back(3);
//    test.push_back(5);
//    test.push_back(7);
//    test.push_back(9);
////    test.push_back(11);
////    test.push_back(13);
//
//    size_t key = 5;
//
//    size_t mid = test.size() > 0 ? test.size() / 2 : 0;
//    size_t add = mid;
//
//    auto func = [](const size_t &t1, const size_t &t2) {
//        return t1 < t2;
//    };
//
//    size_t flag = 0;
//
//    while (add > 0 && test.size()) {
//        if (func(key, test[mid])) {
//            add = add > 0 ? add / 2 : 0;
//            mid = mid - add;
//        } else if (func(test[mid], key)) {
//            add = add > 1 ? add / 2 : 1;
//            mid = mid + add;
//        } else {
//            std::cout << "eq ? " << mid << std::endl;
//            break;
//        }
//
//    }
//    std::cout << "index " << mid << std::endl;


    std::cout << "xxxxxxxxx\n";
    bpptree<book, 12> books;

    books.insert({1, "1"});
    books.insert({4, "4"});
    books.insert({2, "2"});
    books.insert({5, "6"});
    books.insert({3, "3"});


    std::cout << "\n";


    return 0;
}
