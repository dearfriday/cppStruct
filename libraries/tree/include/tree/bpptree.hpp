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


    struct genKeyDelegate {
        virtual void insert(const std::string &key, const std::string &value) = 0;
        virtual std::string getValue(const std::string &key) = 0;
    };


    struct nodePosition{
        size_t deep = 0;
        size_t index = 0;

        std::string operator ()(){
            return std::to_string(deep) + "-" + std::to_string(index);
        }

        void set(size_t d, size_t i){
            deep = d;
            index = i;
        }
    };


    template<typename KEY, size_t M = 1024, typename Compare = std::less<KEY>>
    struct bpptree {

        static_assert(M >= 3, "bpptree k must > 3");
        typedef std::pair<KEY, bool> InsertResult;
        typedef KEY key_type;

        typedef key_type &reference;
        typedef const key_type const_reference;


        bpptree() : m_parent(nullptr), m_delegate(nullptr) {}

        ~bpptree() {
            m_parent = nullptr;
        }

        struct iterator {
            iterator(bpptree *root) : m_root(root), m_index(0) {
                m_node = getBeginPoint();
            }

            iterator &operator++(int) {
                assert(m_node != nullptr);
                if (m_node->m_leaf.size() == 0) {
                    if (m_index + 1 == m_node->m_values.size()) {
                        if (m_node->m_parent != nullptr) {
                            m_node = m_node->m_parent;
                            m_index = indexOfParent.top();
                            indexOfParent.pop();

                            while (m_node->m_values.size() == m_index && m_node->m_parent != nullptr) {
                                m_node = m_node->m_parent;
                                m_index = indexOfParent.top();
                                indexOfParent.pop();
                            }
                            if (m_node->m_values.size() <= m_index) {
                                m_node = nullptr;
                                m_index = 0;
                            }
                        } else {
                            m_node = nullptr;
                            m_index = 0;
                        }
                    } else {
                        m_index++;
                    }
                } else {
                    if (m_index + 1 < m_node->m_leaf.size()) {
                        m_index++;
                        indexOfParent.push(m_index);
                        m_node = &m_node->m_leaf[m_index];
                        m_index = 0;
                        while (m_node->m_leaf.size()) {
                            indexOfParent.push(m_index);
                            m_node = &m_node->m_leaf[m_index];
                            m_index = 0;
                        }
                    } else {
                        //unkown this condition
                        assert(false);
                    }
                }
                return *this;
            }

            iterator &operator++() {
                return this->operator++(0);
            }


            key_type &operator->() {
                assert(m_node != nullptr && m_node->m_values.size() - 1 >= m_index);
                return m_node->m_values[m_index];
            }

            key_type &operator*() {
                assert(m_node != nullptr && m_node->m_values.size() - 1 >= m_index);
                return m_node->m_values[m_index];
            }

            bool operator==(const iterator &itr) {
                return m_node == itr.m_node && m_index == itr.m_index;
            }

            bool operator!=(const iterator &itr) {
                return !this->operator==(itr);
            }

        private:

            /// @brief get min node by root node.
            /// \return
            bpptree *getBeginPoint() {
                if (m_root == nullptr) {
                    return nullptr;
                }
                bpptree *node = m_root;
                while (node->m_leaf.size() != 0) {
                    node = &node->m_leaf[0];
                    indexOfParent.push(0);
                }
                m_index = 0;
                return node;
            }

            bpptree *m_root;
            bpptree *m_node;
            size_t m_index;
            std::stack<size_t> indexOfParent;
        };

        iterator begin() {
            return iterator(this);
        }

        iterator end() {
            return iterator(nullptr);
        }


        void insert(const key_type &key) {
            bpptree *insert_node = findIndexNode(key);
            insertValueToNode(insert_node, key);
        }


        /// @brief remove key from tree.
        /// \param key
        /// \return  true:      if key existed , remove it.   false:     cant find key.
        bool remove(const key_type &key) {
            bpptree *insert_node = findIndexNode(key);
            assert(insert_node != nullptr);
            auto insert_index = insert_node->findInsertIndex(key);

            ///cant find key.
            if (!insert_index.second) {
                return false;
            }
            size_t remove_index = insert_index.first;

            if (insert_node->m_leaf.size() == 0) {
                for (size_t i = remove_index; i < insert_node->m_values.size() - 1; i++) {
                    std::swap(insert_node->m_values[i], insert_node->m_values[i + 1]);
                }
                insert_node->m_values.pop_back();
                bpptree *node = checkNodeFormat(insert_node);
                while (node != nullptr && node->m_parent != nullptr) {
                    node = checkNodeFormat(node);
                }

                return true;
            } else {
                assert(insert_node->m_leaf.size() >= remove_index + 1);

                auto &itr_back = insert_node->m_leaf[remove_index + 1];
                std::swap(itr_back.m_values[0], insert_node->m_values[remove_index]);
                for (size_t i = 0; i < itr_back.m_values.size() - 1; i++) {
                    std::swap(itr_back.m_values[i], itr_back.m_values[i + 1]);
                }
                itr_back.m_values.pop_back();

                bpptree *node = &itr_back;
                while (node->m_parent != nullptr) {
                    node = checkNodeFormat(node);
                    node = node->m_parent;
                }
            }
            return true;
        }


        void debug() {
            std::string ret;
            print(0, ret);
            std::cout << ret << std::endl;
        }

        void setDelegate(std::shared_ptr<genKeyDelegate> del) {
            assert(m_parent == nullptr);
            m_delegate = del;
        }


    private:

        void print(int depth, std::string &ret) {
            if (m_leaf.size() > 0) {
                m_leaf[0].print(depth + 1, ret);
            }
            for (size_t i = 0; i < m_values.size(); i++) {

                ret += std::to_string((long) this) + " " +
                       (this->m_parent != nullptr ? std::to_string((long) this->m_parent) : "");
                for (size_t j = 0; j < depth; j++) {
                    ret += "\t";
                }
//                ret += std::to_string(m_values[i]) + " "+ m_position() + "\n";
                ret += std::to_string(m_values[i]) + "\n";
                if (m_leaf.size() > i) {
                    m_leaf[i + 1].print(depth + 1, ret);
                }
            }
        }


        void checkParent() {
            for (auto &itr : m_leaf) {
                checkleaf(&itr);
            }
        }

        void checkleaf(bpptree *node) {
            for (auto &itr : node->m_leaf) {
                checkleaf(&itr);
                for (auto &it : itr.m_leaf) {
                    if (it.m_parent != &itr) {
                        it.m_parent = &itr;
                        it.m_position.set(itr.m_position.deep + 1, itr.m_position.index);
                    }
                }
            }
        }


        bpptree(bpptree *parent)
                : m_parent(parent) {

        }


        void pushBack(key_type key) {
            m_values.push_back(key);
            for (size_t i = 0; i < m_values.size() - 1; i++) {
                std::swap(m_values[i], m_values[i + 1]);
            }
        }

        void catchValueFromRight() {
            assert(m_parent != nullptr);
            size_t indexOf = getIndexInParent();

            bpptree *right_node = &m_parent->m_leaf[indexOf + 1];
            m_values.push_back(m_parent->m_values[0]);

            std::swap(right_node->m_values[0], m_parent->m_values[0]);
            for (size_t i = 0; i < right_node->m_values.size() - 1; i++) {
                std::swap(right_node->m_values[i], right_node->m_values[i + 1]);
            }
            right_node->m_values.pop_back();
        }

        void catchValueFromLeft() {
            assert(m_parent != nullptr);
            size_t indexOf = getIndexInParent();

            bpptree *left_node = &m_parent->m_leaf[indexOf - 1];
            pushBack(m_parent->m_values[indexOf - 1]);

            std::swap(m_parent->m_values[indexOf - 1], left_node->m_values.back());
            left_node->m_values.pop_back();
        }

        bpptree *checkNodeFormat(bpptree *node) {
            if (node->m_values.size() < M / 2) {
                if (node->m_parent) {
                    size_t indexOf = node->getIndexInParent();
                    assert(node->m_parent->m_leaf.size() >= indexOf);
                    if (indexOf == 0) {
                        bpptree *right_node = &node->m_parent->m_leaf[indexOf + 1];
                        if (right_node->m_values.size() > M / 2) {
                            this->catchValueFromRight();
                        } else {
                            //TODO
                            assert(false);
                        }

                    } else if (indexOf + 1 == node->m_parent->m_leaf.size()) {
                        bpptree *left_node = &node->m_parent->m_leaf[indexOf - 1];
                        if (left_node->m_values.size() > M / 2) {
                            node->catchValueFromLeft();
                        } else {
                            left_node->m_values.push_back(node->m_parent->m_values[indexOf - 1]);
                            left_node->m_values.insert(left_node->m_values.end(), node->m_values.begin(),
                                                       node->m_values.end());
                            for (auto &&itr : node->m_leaf) {
                                itr.m_parent = left_node;
                                left_node->m_leaf.emplace_back(itr);
                            }

                            bpptree *ret = node->m_parent;
                            node->m_parent->m_values.pop_back();
                            node->m_parent->m_leaf.pop_back();

                            if (ret && ret->m_values.size() == 0) {
                                new(this) bpptree();
                                *this = *left_node;
                                for (auto &itr : this->m_leaf) {
                                    itr.m_parent = this;
                                }
                                this->m_parent = nullptr;
                                ret = nullptr;
                            }
                            return ret;
                        }
                    } else {
                        if (node->m_parent->m_leaf[indexOf - 1].m_values.size() > M / 2) {
                            node->catchValueFromLeft();
                        } else if (node->m_parent->m_leaf[indexOf + 1].m_values.size() > M / 2) {
                            node->catchValueFromRight();
                        } else {
                            bpptree *left_node = &node->m_parent->m_leaf[indexOf - 1];
                            left_node->m_values.push_back(node->m_parent->m_values[indexOf - 1]);
                            left_node->m_values.insert(left_node->m_values.end(), m_values.begin(), m_values.end());

                            for (size_t i = indexOf - 1; i < node->m_parent->m_values.size() - 1; i++) {
                                std::swap(node->m_parent->m_values[i], node->m_parent->m_values[i + 1]);
                            }
                            node->m_parent->m_values.pop_back();

                            for (size_t i = indexOf; i < node->m_parent->m_leaf.size() - 1; i++) {
                                std::swap(node->m_parent->m_leaf[i], node->m_parent->m_leaf[i + 1]);
                            }
                            bpptree *ret = node->m_parent;
                            node->m_parent->m_leaf.pop_back();
                            return ret;
                        }

                    }

                    return node->m_parent;
                }
            }
            return nullptr;
        }


        size_t getIndexInParent() {
            assert(m_parent != nullptr);
            size_t indexOf = UINT64_MAX;
            for (size_t i = 0; i < m_parent->m_leaf.size(); i++) {
                if (this == &m_parent->m_leaf[i]) {
                    indexOf = i;
                    break;
                }
            }
            assert(indexOf != UINT64_MAX);
            return indexOf;
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
        bpptree *insertValueLeafToNode(bpptree *insert_node, key_type &&key, bpptree<key_type, M, Compare> &&first,
                                       bpptree<key_type, M, Compare> &&second) {
//            assert(this->m_parent);
            auto insert_index = insert_node->findInsertIndex(key);
            if (!insert_index.second) {
                insert_node->m_values.push_back(key);
                for (size_t i = insert_node->m_values.size() - 1; i > insert_index.first; i--) {
                    std::swap(insert_node->m_values[i], insert_node->m_values[i - 1]);
                }

                assert(insert_node->m_leaf.size() > insert_index.first);
                std::swap(insert_node->m_leaf[insert_index.first], first);
                assert(second.m_parent == insert_node);

                second.m_position.index = insert_node->m_leaf.size();
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
        void insertValueToNode(bpptree *insert_parent, const key_type &key) {
            auto insert_index = insert_parent->findInsertIndex(key);
            if (!insert_index.second) {
                insert_parent->m_values.push_back(key);
                for (size_t i = insert_parent->m_values.size() - 1; i > insert_index.first; i--) {
                    std::swap(insert_parent->m_values[i], insert_parent->m_values[i - 1]);
                }
            } else {
                insert_parent->m_values[insert_index.first] = key;
                return;
            }

            while (insert_parent != nullptr && !insert_parent->empty()) {
                if (insert_parent->m_values.size() == M + 1) {
                    size_t mid_length = insert_parent->m_values.size() / 2 + 1;

                    bpptree *parent = insert_parent->m_parent != nullptr ? insert_parent->m_parent : insert_parent;
                    bpptree<key_type, M, Compare> front_tree(parent);
                    bpptree<key_type, M, Compare> back_tree(parent);

                    //set position
                    if(parent){
                        front_tree.m_position.set(parent->m_position.deep + 1, parent->m_position.index + 1);
                        front_tree.m_position.set(parent->m_position.deep + 1, parent->m_position.index + 1);
                    }



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
                        insert_parent = insertValueLeafToNode(insert_parent->m_parent, std::move(mid_value),
                                                              std::move(front_tree),
                                                              std::move(back_tree));
                        for (auto &itr : insert_parent->m_leaf) {
                            assert(itr.m_parent == insert_parent);
                            for (auto &it : itr.m_leaf) {
                                if (it.m_parent != &itr) {
                                    it.m_parent = &itr;
                                }
                            }
                        }

                    } else {
                        insert_parent->m_values.clear();
                        insert_parent->m_leaf.clear();

                        insert_parent->m_values.push_back(mid_value);
                        insert_parent->m_leaf.push_back(front_tree);
                        insert_parent->m_leaf.push_back(back_tree);

                        for (auto &itr : insert_parent->m_leaf) {
                            for (auto &it : itr.m_leaf) {
                                it.m_parent = &itr;
                            }
                        }
                        insert_parent = insert_parent->m_parent;
                    }
//                    checkParent();

                } else {
                    return;
                }

            }
        }

        /// @brief  find index by key.
        /// \param key
        /// \return  @param first insert value index , @param second  true : this value is existed.    false cant find value.
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


        Compare m_compare;
        std::vector<key_type> m_values;
        std::vector<bpptree<key_type, M, Compare>> m_leaf;
        bpptree *m_parent;

        std::vector<std::string> m_keyOfLeaf;
        std::string m_keyOfParent;
        std::string m_keyOfSelf;
        std::shared_ptr<genKeyDelegate> m_delegate;

        nodePosition       m_position;


//        static const std::string   rootDefaultKey = "rootKeyBppTree";
    };


}
