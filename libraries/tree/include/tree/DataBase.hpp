//
// Created by friday on 2019/10/24.
//

#pragma once

#include <tree/bpptree.hpp>

namespace zero{


    template<typename KEY, typename Compare = std::less<KEY>>
    class storageData {
    public:
        storageData(const std::string &rootKey):m_rootKey(rootKey){

        }

        void setDelegate(std::shared_ptr<genKeyDelegate> delegate){
            m_root.setDelegate(delegate);
        }

        void insert(const KEY &key){
            m_root.insert(key);
        }


    private:
        bpptree<KEY, 5, Compare>     m_root;
        std::string                     m_rootKey;
    };

}