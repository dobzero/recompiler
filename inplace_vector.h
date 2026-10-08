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
    }

    template<typename ...Args> requires std::is_constructible_v<T>
    T & emplace_back(Args && ... args) {
        auto * type_ptr=&data_bytes[i_size++ * sizeof(T)];

        auto * type_object = new (type_ptr) T(std::forward<Args>(args)...);
        return *type_object;
    }
    const T & operator [](size_t index) const {
        return data()[index];
    }
    operator std::span<const T>() const {
        return std::span(data(), size());
    }
    auto empty() const {
        return size()==0;
    }
    auto size() const {
        return i_size;
    }
    auto capacity() const {
        return data_bytes.size()/sizeof(T);
    }
    auto data()const {
        return reinterpret_cast<const T*>(data_bytes.data());
    }

    auto begin() const {
        return reinterpret_cast<const T*>(data_bytes.data());
    }
    auto end() const {
        return begin()+i_size;
    }

    void clear() {
        if constexpr (std::is_destructible_v<T>) {
            for (T &type_reference : std::span<T>(reinterpret_cast<T*>(data_bytes.data()), i_size))
                type_reference.~T();
        } else {
            memset(data_bytes.data(), 0, i_size);
        }
        i_size=0;
    }

private:
    size_t i_size=0;
    std::array<uint8_t, N*sizeof(T)> data_bytes;
};
}
