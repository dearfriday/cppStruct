//
// Created by nova on 10/09/2019.
//

#pragma once


#include <vector>
#include <set>
#include <iostream>
#include <iterator>
#include <stack>
namespace zero {


    template<typename KEY, size_t M = 1024, typename Compare = std::less<KEY>>
    struct bpptree {


        static_assert(M >= 3, "bpptree M must > 3");
        typedef     std::pair<KEY, bool>    InsertResult;
        typedef     KEY                     key_type;

        typedef     key_type&               reference;
        typedef     const key_type          const_reference;

        struct iterator{
            iterator(bpptree *root):m_root(root),m_index(0){
                m_node = getBeginPoint();
            }

            iterator &operator++(int){
                assert(m_node != nullptr);
                if(m_node->m_leaf.size() == 0){
                    if(m_index + 1 == m_node->m_values.size()){
                        if(m_node->m_parent != nullptr){
                            m_node = m_node->m_parent;
                            m_index = indexOfParent.top();
                            indexOfParent.pop();

                            while(m_node->m_values.size() == m_index && m_node->m_parent != nullptr){
                                m_node = m_node->m_parent;
                                m_index = indexOfParent.top();
                                indexOfParent.pop();
                            }
                            if(m_node->m_values.size() <= m_index){
                                m_node = nullptr;
                                m_index = 0;
                            }
                        }
                        else{
                            std::cout << "end ....\n";
                            m_node = nullptr;
                            m_index = 0;
                        }
                    }
                    else{
                        m_index++;
                    }
                }
                else {
                   if(m_index + 1 < m_node->m_leaf.size()){
                       m_index++;
                       indexOfParent.push(m_index);
                       m_node = &m_node->m_leaf[m_index];
                       m_index = 0;
                       while(m_node->m_leaf.size()){
                           indexOfParent.push(m_index);
                           m_node = &m_node->m_leaf[m_index];
                           m_index = 0;
                       }
                    }
                    else{
                        std::cout << "? \n";
                    }
                }
                return *this;
            }

            key_type &operator ->(){
                assert(m_node != nullptr && m_node->m_values.size() - 1 >= m_index);
                return m_node->m_values[m_index];
            }

            key_type &operator *(){
                assert(m_node != nullptr && m_node->m_values.size() - 1 >= m_index);
                return m_node->m_values[m_index];
            }

            bool operator == (const iterator &itr){
                return m_node == itr.m_node &&  m_index == itr.m_index;
            }

            bool operator != (const iterator &itr){
                return !this->operator==(itr);
            }

        private:

            /// @brief get min node by root node.
            /// \return
            bpptree *getBeginPoint(){
                if(m_root == nullptr){
                    return nullptr;
                }
                bpptree *node = m_root;
                while (node->m_leaf.size() != 0){
                    node = &node->m_leaf[0];
                    indexOfParent.push(0);
                }
                m_index = 0;
                return node;
            }

            bpptree             *m_root;
            bpptree             *m_node;
            size_t              m_index;
            std::stack<size_t>  indexOfParent;
        };


        iterator begin() {
            return  iterator(this);
        }

        iterator end() {
            return iterator(nullptr);
        }


        void insert(const key_type &key) {
            bpptree *insert_node = findIndexNode(key);
            insert_node->insertValueToNode(key);
        }

        bool remove(const key_type &key) {

            return true;
        }


        bpptree():m_parent(nullptr){}

        ~bpptree() {
            m_parent = nullptr;
        }

    private:


        bpptree(bpptree *parent)
        : m_parent(parent)
        {

        }





        /// @brief
        /// \return
        bool empty() {
            return m_values.size() == 0;
        }

        /// insert value and leaf to parent
        /// \param key      insert  value
        /// \param first    left    value
        /// \param second   right   value
        /// \return parent
        bpptree *insertValueLeafToNode(key_type &&key, bpptree<key_type, M, Compare> &&first,
                                       bpptree<key_type, M, Compare> &&second) {
            assert(this->m_parent);
            bpptree *insert_node = this->m_parent;
            auto insert_index = insert_node->findInsertIndex(key);
            if (!insert_index.second) {
                insert_node->m_values.push_back(key);
                for (size_t i = insert_node->m_values.size() - 1; i > insert_index.first; i--) {
                    std::swap(insert_node->m_values[i], insert_node->m_values[i - 1]);
                }

                assert(insert_node->m_leaf.size() >= insert_index.first);
                std::swap(insert_node->m_leaf[insert_index.first], first);
                insert_node->m_leaf.push_back(second);
                for (size_t i = insert_node->m_leaf.size() - 1; i > insert_index.first + 1; i--) {
                    std::swap(insert_node->m_leaf[i], insert_node->m_leaf[i - 1]);
                }
            }
            return insert_node;
        }


        /// @brief  insert value to node , if node value.size == M + 1, try compose all values to parent.
        /// \param key      value
        /// \return
        void insertValueToNode(const key_type &key) {
            auto insert_index = findInsertIndex(key);
            if (!insert_index.second) {
                m_values.push_back(key);
                for (size_t i = m_values.size() - 1; i > insert_index.first; i--) {
                    std::swap(m_values[i], m_values[i - 1]);
                }
            } else {
                m_values[insert_index.first] = key;
                return;
            }

            bpptree *insert_parent = this;
            while (insert_parent != nullptr && !insert_parent->empty()) {
                if (insert_parent->m_values.size() == M + 1) {
                    size_t mid_length = insert_parent->m_values.size() / 2 + 1;

                    bpptree *parent = insert_parent->m_parent != nullptr ? insert_parent->m_parent : insert_parent;
                    bpptree<key_type, M, Compare> front_tree(parent);
                    bpptree<key_type, M, Compare> back_tree(parent);
                    key_type mid_value = insert_parent->m_values[mid_length - 1];

                    auto begin = insert_parent->m_values.begin();
                    auto mid = begin + mid_length - 1;

                    front_tree.m_values.resize(mid_length - 1);
                    std::copy(begin, mid, front_tree.m_values.begin());
                    back_tree.m_values.resize(insert_parent->m_values.size() - mid_length);
                    std::copy(mid + 1, insert_parent->m_values.end(), back_tree.m_values.begin());

                    if (insert_parent->m_leaf.size()) {
                        if (insert_parent->m_leaf.size() <= mid_length) {
                            front_tree.m_leaf.resize(insert_parent->m_leaf.size());
                            std::copy(insert_parent->m_leaf.begin(), insert_parent->m_leaf.end(),
                                      front_tree.m_leaf.begin());
                        } else if (insert_parent->m_leaf.size() > mid_length) {
                            front_tree.m_leaf.resize(mid_length);
                            back_tree.m_leaf.resize(insert_parent->m_leaf.size() - mid_length);
                            std::copy(insert_parent->m_leaf.begin(), insert_parent->m_leaf.begin() + mid_length,
                                      front_tree.m_leaf.begin());
                            std::copy(insert_parent->m_leaf.begin() + mid_length, insert_parent->m_leaf.end(),
                                      back_tree.m_leaf.begin());
                        }
                    }


                    if (insert_parent->m_parent != nullptr) {
                        insert_parent = insert_parent->insertValueLeafToNode(std::move(mid_value),
                                                                             std::move(front_tree),
                                                                             std::move(back_tree));
                    } else {
                        insert_parent->m_values.clear();
                        insert_parent->m_leaf.clear();

                        insert_parent->m_values.push_back(mid_value);
                        insert_parent->m_leaf.push_back(front_tree);
                        insert_parent->m_leaf.push_back(back_tree);


                        for (auto &itr : insert_parent->m_leaf) {
                            size_t index = 0;
                            for (auto &it : itr.m_leaf) {
                                it.m_parent = &itr;
                            }
                        }
                        insert_parent = insert_parent->m_parent;
                    }
                } else {
                    return;
                }

            }
        }

        /// @brief  find index by key.
        /// \param key
        /// \return  @param first insert value index , @param second this value is existed.
        std::pair<size_t, bool> findInsertIndex(const key_type &key) {
            size_t front = 0;
            size_t end = m_values.size() - 1;
            size_t mid = (front + end) / 2;
            bool same = false;

            if (m_values.size() == 0) {
                return {0, false};
            }

            while (front < end) {
                if (m_compare(key, m_values[mid])) {
                    end = mid > 0 ? mid - 1 : mid;
                } else if (m_compare(m_values[mid], key)) {
                    front = mid + 1;
                } else {
                    same = true;
                    break;
                }
                mid = (front + end) / 2;
            }

            if (m_compare(key, m_values[mid])) {

            } else if (m_compare(m_values[mid], key)) {
                mid++;
            } else {
                same = true;
            }
            return {mid, same};
        }

        /// @brief      find node of inserted.
        /// \param key  value.
        /// \return     node point
        bpptree *findIndexNode(const key_type &key) {
            bpptree *insert_node = this;
            while (insert_node->m_leaf.size()) {
                auto itr = insert_node->findInsertIndex(key);
                if (itr.second) {     //same
                    return insert_node;
                }
                assert(insert_node->m_leaf.size() >= itr.first);
                insert_node = &insert_node->m_leaf[itr.first];
            }
            return insert_node;
        }


        Compare                                     m_compare;
        std::vector<key_type>                       m_values;
        std::vector<bpptree<key_type, M, Compare>>  m_leaf;
        bpptree                                     *m_parent;
    };


}