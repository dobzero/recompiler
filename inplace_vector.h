#pragma once
#include <cstdint>
#include <cstring>
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
        return reinterpret_cast<T*>(iv_container.data())[index];
    }
    auto size() const {
        return iv_size;
    }

    void clear() {
        if constexpr (std::is_destructible_v<T>) {
            for (T &type_reference : std::span<T>(reinterpret_cast<T*>(iv_container.data()), iv_size))
                type_reference.~T();
        } else {
            memset(iv_container.data(), 0, iv_size);
        }
        iv_size=0;
    }

private:
    size_t iv_size=0;
    std::array<uint8_t, N*sizeof(T)> iv_container;
};
}
