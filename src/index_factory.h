#pragma once

#include <map>

namespace vdb {

class IndexFactory {
public:
    enum class IndexType {
        FLAT,
        UNKNOWN = -1 
    };

    enum class MetricType {
        L2,
        IP
    };
    void Init(IndexType type, int dim, MetricType metric = MetricType::L2);
    void* GetIndex(IndexType type) const;
private:
    std::map<IndexType, void*> index_map_;    
};

IndexFactory* GetGlobalIndexFactory();

} // namespace vdb