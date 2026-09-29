#pragma once
#include <array>
#include <span>

namespace recompiler {
template<typename T, size_t N>
class inplace_vector {
public:
    inplace_vector() = default;
    ~inplace_vector() {
        clear();
    }

    template<typename ...Args> requires std::is_constructible_v<T>
    T & emplace_back(Args && ... args) {
        auto * type_ptr=&iv_container[iv_size++];

        auto * type_object = new (type_ptr) T(std::forward<Args>(args)...);
        return *type_object;
    }
    T & operator [](size_t index) {
        return iv_container[index];
    }
    auto size() const {
        return iv_size;
    }

    void clear() {
        if constexpr (std::is_destructible_v<T>) {
            for (std::span iv_content(iv_container.data(), iv_size);
                T &type_reference : iv_content)
                type_reference.~T();
        } else {
            for (size_t i=0;i<iv_size;i++)
                iv_container[i]={};
        }
        iv_size=0;
    }

private:
    size_t iv_size=0;
    std::array<T, N> iv_container;
};
}
