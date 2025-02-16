#include "index_factory.h"
#include "faiss_index.h"

#include "faiss/IndexFlat.h"
#include "faiss/IndexIDMap.h"


namespace vdb {
    
IndexFactory globalIndexFactory; 

IndexFactory* getGlobalIndexFactory() {
    return &globalIndexFactory; 
}

void IndexFactory::init(IndexType type, int dim, MetricType metric) {
    faiss::MetricType faiss_metric = (metric == MetricType::L2) ? faiss::METRIC_L2 : faiss::METRIC_INNER_PRODUCT;

    switch (type) {
        case IndexType::FLAT:
            index_map_[type] = new FaissIndex(new faiss::IndexIDMap(new faiss::IndexFlat(dim, faiss_metric)));
            break;
        default:
            break;           
    }
}

void* IndexFactory::getIndex(IndexType type) const { 
    auto it = index_map_.find(type);
    if (it != index_map_.end()) {
        return it->second;
    }
    return nullptr;
}

} // namespace vdb