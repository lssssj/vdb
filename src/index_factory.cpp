#include "index_factory.h"
#include "faiss_index.h"

#include "faiss/IndexFlat.h"
#include "faiss/IndexIDMap.h"


namespace vdb {
    
IndexFactory global_index_factory; 

IndexFactory* GetGlobalIndexFactory() {
    return &global_index_factory; 
}

void IndexFactory::Init(IndexType type, int dim, MetricType metric) {
    faiss::MetricType faiss_metric = (metric == MetricType::L2) ? faiss::METRIC_L2 : faiss::METRIC_INNER_PRODUCT;

    switch (type) {
        case IndexType::FLAT:
            index_map_[type] = new FaissIndex(new faiss::IndexIDMap(new faiss::IndexFlat(dim, faiss_metric)));
            break;
        default:
            break;           
    }
}

void* IndexFactory::GetIndex(IndexType type) const { 
    auto it = index_map_.find(type);
    if (it != index_map_.end()) {
        return it->second;
    }
    return nullptr;
}

} // namespace vdb