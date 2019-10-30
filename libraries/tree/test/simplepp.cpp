//
// Created by friday on 2019/10/30.
//
#include <string>
#include <iostream>




template <typename T, typename M>
void test(T t, M m){

   std::cout << typeid(t).name() << std::endl;
   std::cout << typeid(m).name() << std::endl;
   std::cout << m() << std::endl;



}



struct student{
    std::string name;

    std::string &operator()(){
        return name;
    }
};

int main(){
    student st;
    st.name = "c++";
    test(&student::name, st);
    return 0;
}