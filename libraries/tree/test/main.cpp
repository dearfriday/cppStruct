#include <string>
#include <iostream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>

using namespace boost::multi_index;
struct test{
    int i = 0;
    test(int k){
        i = k;
    }
    friend  bool operator < (const test &a,  const test &b) {
        return a.i < b.i;
    }

    friend  bool operator > (const test &a, const test &b){
        return a.i > b.i;
    }
};

struct by_id;
typedef boost::multi_index_container<
        test,
        indexed_by<
                ordered_unique<tag<by_id>,
                        composite_key<test, member<test, int, &test::i>>,
                        composite_key_compare<std::less<test>>
                >
        >

> test_index;




int main() {
    test_index tt;
    tt.insert(test(4));
    tt.insert(test(1));
    tt.insert(test(5));
    tt.insert(test(9));
    tt.insert(test(8));

    test_index::index_specifier_type_list   vv;



//    const auto low = tt.lower_bound(test(6));
//    for(auto itr= low; itr != tt.end(); itr++){
//        std::cout << itr->i << std::endl;
//    }
    return 0;
}