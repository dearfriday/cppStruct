//
// Created by nova on 10/09/2019.
//

#pragma once


#include <vector>
#include <set>
#include <iostream>
namespace zero{


    template <typename KEY, size_t M = 1024, typename Compare = std::less<KEY>>
    struct bpptree{
        bpptree():m_parent(nullptr){

        }

        typedef std::pair<KEY, bool>   InsertResult;
        typedef KEY                    key_type;

        void insert(const key_type &key){
            size_t insert_index = 0;

            auto &insert_node = find_index_node(key);
            insert_node.insert_value_to_node(key);

        }




    private:

        void insert_value_to_node(const key_type &key){
            auto  insert_index = find_insert_index(key);
            if(!insert_index.second){
                m_values.push_back(key);
                for(size_t i = m_values.size() - 1; i > insert_index.first; i--){
                    std::swap(m_values[i], m_values[i - 1]);
                }
            }
            else{
                m_values[insert_index.first] = key;
            }

        }

        std::pair<size_t, bool> find_insert_index(const key_type &key){
            size_t mid = m_values.size() > 0 ? m_values.size() / 2 : 0;
            size_t add = m_values.size();
            size_t flag = 0;
            while (add > 0 && m_values.size()){
                if (m_compare(key, m_values[mid])) {
                    add = add > 0 ? add / 2 : 0;
                    mid = mid - add;
                } else if (m_compare(m_values[mid], key)) {
                    add = add > 1 ? add / 2 : 1;
                    mid = mid + add;
                } else {
//                    std::cout << "eq ? " << mid << std::endl;
//                    break;
                    return {mid, true};
                }
            }
            return {mid, false};
        }

        bpptree &find_index_node(const key_type &key){
            bpptree *insert_node = this;
            bool find = false;
            while (!find){
                size_t  size = insert_node->m_values.size();
                if(size == 0){
                    return  *insert_node;
                }
                if(!insert_node->m_compare(insert_node->m_values.front(), key) ){
                    if(insert_node->m_leaf.size() > 0){
                        insert_node = &insert_node->m_leaf.front();
                    }
                    else{
                        find = true;
                    }
                }
                else if(!insert_node->m_compare(key, insert_node->m_values.back())){
                    if(insert_node->m_leaf.size() > 0){
                        insert_node = &insert_node->m_leaf.back();
                    } else{
                        find = true;
                    }
                } else{
                    find = true;
                }
            }
            return *insert_node;
        }



        Compare                                             m_compare;
        std::vector<key_type>                               m_values;
        std::vector<bpptree<key_type, M, Compare>>          m_leaf;
        std::shared_ptr<bpptree>                            m_parent;
    };



}
