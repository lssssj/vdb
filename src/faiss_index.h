#pragma once

#include <vector>

#include "faiss/Index.h"

namespace vdb {

struct SearchResult {
    std::vector<int64_t> indices_;
    std::vector<float> distances_;
};

class FaissIndex {
public:
    explicit FaissIndex(faiss::Index* index);
    void InsertVectors(const std::vector<float>& data, uint64_t lable);
    SearchResult SearchVectors(const std::vector<float>& query, int k);
private:
    faiss::Index* index_;    
};

} // namespace vdb