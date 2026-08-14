#pragma once
#include <array>
#include <cstddef>

namespace recompiler {
template<typename T, size_t N>
class inplace_vector {
public:
    inplace_vector() = default;

    template<typename ...Args> requires std::is_constructible_v<T>
    T & emplace_back(Args && ... args) {
        auto * type_ptr=&a_container_data_[vi_next_insert++];

        auto * type_object = new (type_ptr) T(std::forward<Args>(args)...);
        return *type_object;
    }
    T & operator [](size_t index) {
        return a_container_data_[index];
    }
    auto size() const {
        return vi_next_insert;
    }

    void clear() {
        vi_next_insert=0;
        // should be avoided
        //a_container_data_.clear();
    }

private:
    size_t vi_next_insert=0;
    std::array<T, N> a_container_data_;
};
}
