/*
 * @Descripttion: 
 * @version: v0.0.1
 * @Author: friday
 * @Date: 2019-10-25 10:47:37
 * @LastEditors: friday
 * @LastEditTime: 2019-10-25 11:35:15
 */
//
// Created by friday on 2019/10/24.
//



#include <tree/DataBase.hpp>


using namespace zero;


struct testdb : public zero::genKeyDelegate{


    virtual void insert(const std::string &key, const std::string &value) {
        std::cout << "insert key " << key << " value: " << value << std::endl;
    }


    virtual std::string getValue(const std::string &key){
        return "";
    }
};







int main(int argc, char *argv[]){
    std::cout << "hello world!.\n";

    std::shared_ptr<testdb> db(new testdb);

    storageData<size_t> root("unkown");
    root.setDelegate(db);



    for(size_t i = 0; i < 32 ; i++){
        root.insert(i);
    }




    return 0;
}