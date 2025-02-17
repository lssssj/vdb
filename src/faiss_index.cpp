#include "faiss_index.h"
#include "faiss/MetricType.h"

namespace vdb {

FaissIndex::FaissIndex(faiss::Index* index) : index_(index) {

}

void FaissIndex::InsertVectors(const std::vector<float>& data, uint64_t lable) {
    faiss::idx_t id = static_cast<faiss::idx_t>(lable);
    index_->add_with_ids(1, data.data(), &id);
}

SearchResult FaissIndex::SearchVectors(const std::vector<float>& query, int k) {
    int dim = index_->d;
    int num_queries = query.size();
    
    std::vector<faiss::idx_t> indices(num_queries * k);
    std::vector<float> distance(num_queries * k);
    index_->search(num_queries, query.data(), k, distance.data(), indices.data());
    
    return {indices, distance};
}

} // namespace vdb
