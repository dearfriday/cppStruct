//
// Created by friday on 2019/10/24.
//

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

namespace zero {


    typedef std::string NodeKey;

    typedef std::string DataKey;
    typedef std::string DataPackage;


    struct databaseDelegate {
        virtual DataPackage getData(const DataKey &nk) = 0;

        virtual void setData(const DataKey &nk, const DataPackage &dp) = 0;
    };


    struct packedDataInterface {
        virtual void decode(const DataPackage &data) = 0;

        virtual DataPackage encode() = 0;
    };


    namespace internal {

        typedef  std::pair<bool, size_t>    find_ret_type;


        /// find key of vector pos.
        /// \tparam T
        /// \tparam Compare
        /// \param key
        /// \param m_values
        /// \param m_compare
        /// \return first insert value index , @bool  true : this value is existed.    false cant find value.
        template<typename T, typename Compare>
        find_ret_type findKeyIndex(const T &key, const std::vector<T> &values, Compare m_compare) {
            size_t front = 0;
            size_t end = values.size() - 1;
            size_t mid = (front + end) / 2;
            bool same = false;

            if (values.size() == 0) {
                return {false, 0};
            }

            while (front < end) {
                if (m_compare(key, values[mid])) {
                    end = mid > 0 ? mid - 1 : mid;
                } else if (m_compare(values[mid], key)) {
                    front = mid + 1;
                } else {
                    same = true;
                    break;
                }
                mid = (front + end) / 2;
            }

            if (m_compare(key, values[mid])) {

            } else if (m_compare(values[mid], key)) {
                mid++;
            } else {
                same = true;
            }
            return {same, mid};
        }

        template<typename T>
        void insertToVector(const T &key, std::vector<T> &values, size_t pos) {
            values.push_back(key);
            for (size_t i = values.size() - 1; i > pos; i--) {
                std::swap(values[i], values[i - 1]);
            }
        }


        template<typename KEY, typename VALUE, size_t L = 1024, typename Compare = std::less<KEY>>
        struct leaf : public packedDataInterface {

            typedef KEY key_type;
            typedef VALUE value_type;
            typedef std::pair<key_type, value_type> kv_pair;

            NodeKey mSelfKey;
            NodeKey mParentKey;
            NodeKey mNextKey;
            std::vector<kv_pair> mValues;
            Compare mCompare;

            leaf() {}

            std::string encode() {
                return "";
            }

            void decode(const DataPackage &data) {

            }

            find_ret_type getIndex(const kv_pair &kv) {
                auto com = [&](const kv_pair &v1, const kv_pair &v2) -> bool {
                    return mCompare(v1.first, v2.first);
                };
                return findKeyIndex(kv, mValues, com);
            }

            bool insertValue(const key_type &key, const value_type &value) {
                find_ret_type index = getIndex(kv_pair(key, value));
                if(!index.first){
                    insertToVector(kv_pair(key, value), mValues, index.second);
                }
                else{
                    mValues[index.second] = kv_pair(key, value);
                }
                return index.first;
            }

            bool isNull() {
                return mSelfKey.empty() && mParentKey.empty() && mNextKey.empty() && !mValues.size();
            }

        };


        template<typename KEY, size_t L = 1024, typename Compare = std::less<KEY>>
        struct node : public packedDataInterface {

            typedef KEY key_type;


            NodeKey mSelfKey;
            NodeKey mParentKey;
            std::vector<key_type> mKeys;              //keys
            std::vector<NodeKey> mChildrenNodes;     //children keys.
            Compare mCompare;

            std::string encode() {
                return "";
            }

            void decode(const DataPackage &data) {

            }


            void insertKey(const key_type &k, const NodeKey &chKey) {
                auto id = findKeyIndex(k, mKeys, mCompare);
                if (!id.first) {
                    insertToVector(k, mKeys, id.second);
                    insertToVector(chKey, mChildrenNodes, id.second + 1);
                }
                else{
                    mChildrenNodes[id.second + 1] = chKey;
                }

            }

            ///
            /// \param key
            /// \return
            NodeKey getChildrenNode(const key_type &key) {
//                assert(mKeys.size() > 0);
                auto ret = findKeyIndex(key, mKeys, mCompare);
                if(!ret.first){ // key is same
                    return mChildrenNodes[ret.second];
                }
                return mChildrenNodes[ret.second + 1];

            }

            bool isNull() {
                return mSelfKey.empty() && mParentKey.empty() && mKeys.empty() && mChildrenNodes.empty();
            }


        };


    }


    template<typename KEY, typename VALUE, size_t LENGTH = 1024, typename Compare = std::less<KEY>>
    struct bplusTree {
        static_assert(LENGTH > 2, "L must > 2");

        typedef KEY key_type;
        typedef VALUE value_type;
        typedef internal::node<KEY, LENGTH, Compare> node_type;
        typedef internal::leaf<KEY, VALUE, LENGTH, Compare> leaf_type;
        typedef std::pair<bool, node_type> node_result;
        typedef std::pair<bool, leaf_type> leaf_result;

        enum class NodeLeaf {
            node = 0,
            leaf = 1,
            null
        };

        bplusTree(std::shared_ptr<databaseDelegate> dl = nullptr) {
            mDelegate = dl;
        }

        bool insert(const key_type &key, const value_type &value) {
            return __insert(key, value);
        }

        bool remove(const key_type &key) {
            return __remove(key);
        }


        void debug() {
            std::cout << "root : " << rootKey << std::endl;
            for (auto &itr : mNodes) {
                std::string str = "key: " + itr.first  + " = ";
                for (auto &it : itr.second.mKeys) {
                    str += std::to_string(it) + ",";
                }
                std::string str1;
                for (auto &it : itr.second.mChildrenNodes) {
                    str1 += it + ",";
                }
                std::cout << str << "[" << str1 << "]" << std::endl;
            }
            std::cout << "leaf --------------------\n";
            for(auto &itr : mLeafs){
                std::string str = "key:" + itr.first + " = ";
                for(auto &it : itr.second.mValues){
                    str += "(" + std::to_string(it.first) + "," + it.second + "),";
                }
                std::cout << str << std::endl;
            }
        }


    private:

        bool __remove(const key_type &key){



            return true;
        }

        bool __insert(const key_type &key, const value_type &value) {
            NodeKey insertKey;
            bool insert_ret = false;
            auto root_type = getType(rootKey);

            if (root_type.second == NodeLeaf::null) {    //create root.
                auto ret = createData(mLeafs);
                insert_ret = ret.second.insertValue(key, value);
                rootKey = ret.second.mSelfKey;
                return true;
            } else if (root_type.second == NodeLeaf::leaf) {  //insert to root leaf.
                auto &ret = mLeafs[rootKey];
                insert_ret = ret.insertValue(key, value);
                insertKey = ret.mSelfKey;
            } else { // root node is not leaf,
                auto ret = findInsertPos(key, value);
                insert_ret = ret.second.insertValue(key, value);
                insertKey = ret.second.mSelfKey;
            }

            checkFormat(insertKey);

            return insert_ret;
        }

        /// check node or leaf is format of tree.
        /// \param insertNk insert key.
        void checkFormat(const NodeKey &insertNk) {
            NodeKey checkNode = insertNk;

            while (checkNode.size() > 0) {
                switch (getType(checkNode).second) {
                    case NodeLeaf::node: {
                        checkNode = checkFormatNode(checkNode);
                        break;
                    }
                    case NodeLeaf::leaf: {
                        checkNode = checkFormatLeaf(checkNode);
                        break;
                    }
                    default: {
                        assert(false);
                    }
                }
            }
        }


        NodeKey checkFormatNode(const NodeKey &nk) {
            auto &nd = mNodes[nk];
            assert(!nd.isNull());
            if (nd.mKeys.size() == LENGTH + 1) {
                size_t mid_pos = nd.mKeys.size() / 2;
                auto &nodeLeft = createData(mNodes).second;
                if(nd.mParentKey.empty()){
                    auto &parent = createData(mNodes).second;

                    ///relative key of parent
                    nodeLeft.mParentKey = parent.mSelfKey;
                    nd.mParentKey = parent.mSelfKey;

                    ///relative key children.
                    parent.mChildrenNodes.push_back(nd.mSelfKey);
                    parent.mChildrenNodes.push_back(nodeLeft.mSelfKey);

                    ///values
                    parent.mKeys.push_back(nd.mKeys[mid_pos]);

                    nodeLeft.mKeys.resize(mid_pos);
                    std::copy(nd.mKeys.begin() + mid_pos + 1, nd.mKeys.end(), nodeLeft.mKeys.begin());

                    nodeLeft.mChildrenNodes.resize(mid_pos + 1);
                    std::copy(nd.mChildrenNodes.begin() + mid_pos, nd.mChildrenNodes.end(), nodeLeft.mChildrenNodes.begin());

                    ///pop value.
                    size_t pop_size = mid_pos + 1;
                    while (pop_size-- != 0){
                        nd.mKeys.pop_back();
                        nd.mChildrenNodes.pop_back();
                    }
                    rootKey = parent.mSelfKey;
                    return parent.mSelfKey;
                }
                else{
                    nodeLeft.mKeys.resize(mid_pos);
                    std::copy(nd.mKeys.begin() + mid_pos + 1, nd.mKeys.end(), nodeLeft.mKeys.begin());

                    nodeLeft.mChildrenNodes.resize(mid_pos + 1);
                    std::copy(nd.mChildrenNodes.begin() + mid_pos, nd.mChildrenNodes.end(), nodeLeft.mChildrenNodes.begin());

                    ///pop value.
                    size_t pop_size = mid_pos + 1;
                    while (pop_size-- != 0){
                        nd.mKeys.pop_back();
                        nd.mChildrenNodes.pop_back();
                    }

                    auto &parentNode = mNodes[nd.mParentKey];
                    parentNode.insertKey(nd.mKeys[mid_pos], nodeLeft.mSelfKey);
                    nodeLeft.mParentKey = parentNode.mSelfKey;
                    assert(parentNode.mSelfKey == nodeLeft.mParentKey && nodeLeft.mParentKey == nodeLeft.mParentKey);
                    return parentNode.mSelfKey;
                }
            }

            return NodeKey();
        }

        NodeKey checkFormatLeaf(const NodeKey &nk) {
            auto &nd = mLeafs[nk];
            assert(!nd.isNull());
            if (nd.mValues.size() == LENGTH + 1) {
                size_t mid_pos = nd.mValues.size() / 2;

                auto &leaf = createData(mLeafs).second;
                if (nd.mParentKey.empty()) {
                    auto &node = createData(mNodes).second;

                    //relative key of parent
                    nd.mParentKey = node.mSelfKey;
                    leaf.mParentKey = node.mSelfKey;

                    //relative key children.
                    node.mChildrenNodes.push_back(nd.mSelfKey);
                    node.mChildrenNodes.push_back(leaf.mSelfKey);

                    //value
                    node.mKeys.push_back(nd.mValues[mid_pos].first);

                    leaf.mValues.resize(mid_pos + 1);
                    std::copy(nd.mValues.begin() + mid_pos, nd.mValues.end(), leaf.mValues.begin());

                    size_t pop_size = mid_pos + 1;
                    while (pop_size-- != 0) {
                        nd.mValues.pop_back();
                    }

                    rootKey = node.mSelfKey;
                    return node.mSelfKey;
                } else {
                    leaf.mValues.resize(mid_pos + 1);
                    std::copy(nd.mValues.begin() + mid_pos, nd.mValues.end(), leaf.mValues.begin());

                    size_t pop_size = mid_pos + 1;
                    while (pop_size-- != 0) {
                        nd.mValues.pop_back();
                    }

                    auto &parentNode = mNodes[nd.mParentKey];
                    parentNode.insertKey(nd.mValues[mid_pos].first, leaf.mSelfKey);
                    leaf.mParentKey = nd.mParentKey;
                    assert( parentNode.mSelfKey == leaf.mParentKey && parentNode.mSelfKey == leaf.mParentKey);
                    return parentNode.mSelfKey;
                }
            }

            return NodeKey();
        }


        std::pair<bool, leaf_type &> findInsertPos(const key_type &key, const value_type &value) {
            NodeKey findKey = rootKey;
            while (true) {
                auto find_node_type = getType(findKey);
                assert(find_node_type.first);

                if (find_node_type.second == NodeLeaf::node) {
                    auto ret = getData(findKey, mNodes);
                    assert(ret.first);
                    findKey = ret.second->getChildrenNode(key);
                    continue;
                } else if (find_node_type.second == NodeLeaf::leaf) {
                    auto ret = getData(findKey, mLeafs);
                    assert(ret.first);
                    findKey = ret.second->mSelfKey;
                    break;
                } else {
                    assert(false);
                }
            }

            return {false, mLeafs[findKey]};
        }

        std::pair<bool, NodeLeaf> getType(const NodeKey &nk) {
            if (nk.size() == 0) {
                return {false, NodeLeaf::null};
            }

            if (getData(nk, mLeafs).first) {
                return {true, NodeLeaf::leaf};
            }


            if (getData(nk, mNodes).first) {
                return {true, NodeLeaf::node};
            }

            return {false, NodeLeaf::null};
        }


        NodeKey generateKey() {
            return std::to_string(mGenerateKey++);
        }

        ////////////////////
        template<typename T>
        std::pair<bool, T *> getData(const NodeKey &nk, std::map<NodeKey, T> &db) {
            if (db.count(nk)) {
                return {true, &db[nk]};
            }
            if (mDelegate) {
                auto data = mDelegate->getData(nk);
                if (data.size()) {
                    T t;
                    t.decode(data);
                    assert(db.count(nk));
                    db[nk] = t;
                    return {true, &db[nk]};
                }
            }
            return {false, nullptr};
        }

        template<typename T>
        std::pair<bool, T &> createData(std::map<NodeKey, T> &db) {
            T t;
            t.mSelfKey = generateKey();
            db[t.mSelfKey] = t;
            return {true, db[t.mSelfKey]};
        }


        NodeKey rootKey;
        uint64_t mGenerateKey = 0;
        std::shared_ptr<databaseDelegate> mDelegate;

        std::map<NodeKey, node_type> mNodes;
        std::map<NodeKey, leaf_type> mLeafs;

    };

}