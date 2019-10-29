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


book createRand() {

    return {(size_t) rand(), std::to_string(rand())};
}



struct database : public zero::genKeyDelegate{


    virtual void insert(const std::string &key, const std::string &value) {

    }


    virtual std::string getValue(const std::string &key){
        return "";
    }
};





int main() {
    try {

        std::shared_ptr<database> db(new database);
        bpptree<int , 4> books;

        books.setDelegate(db);


        int last_insert = 18;
        for (size_t i = 0; i < last_insert; i++) {
            books.insert(i);
//            std::cout << "-------------------\n";
        }
//        books.debug();
        std::cout << "-------------------\n";
        books.insert(last_insert);
        books.debug();
        int k = 0;

    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "unkown exception ....";
    }


    return 0;
}
