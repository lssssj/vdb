#pragma once

#include <vector>

#include "faiss/Index.h"

namespace vdb {

struct SearchResult {
    std::vector<int64_t> indices;
    std::vector<float> distances;
};

class FaissIndex {
public:
    FaissIndex(faiss::Index* index);
    void insert_vectors(const std::vector<float>& data, uint64_t lable);
    SearchResult search_vectors(const std::vector<float>& query, int k);
private:
    faiss::Index* index_;    
};

} // namespace vdb