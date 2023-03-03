#pragma once
#include <algorithm>
#include <functional>
#include <vector>

namespace stdu {
    template <typename T>
    class sorted_vector {
        struct vector_index
        {
            bool exists;
            int index;
        };

        using       iter = typename std::vector<T>::      iterator;
        using const_iter = typename std::vector<T>::const_iterator;
    
        private:
        std::vector<T> _items {};
        std::function<int(T)> _comparer;
        public:
        sorted_vector(std::function<int(T)> comparer) : _items(std::vector<T>{ }), _comparer(std::move(comparer)) {}
        sorted_vector(std::initializer_list<T> elems, std::function<int(T)> comparer) : _comparer(std::move(comparer)) {
            _items = elems;
            std::sort(_items.begin(), _items.end(), _comparer);
        }
        
        sorted_vector(const std::vector<T>& elems, std::function<int(T)> comparer) : _comparer(std::move(comparer)) {
            _items = std::move(elems);
            std::sort(_items.begin(), _items.end(), _comparer);
        }

        sorted_vector(const sorted_vector& other) : _items(other._items), _comparer(other._comparer) {}
        sorted_vector(sorted_vector&& other) noexcept : _items(std::move(other._items)), _comparer(std::move(other._comparer)) {}
        sorted_vector& operator=(const sorted_vector& other) {
            if (this == &other)
                return *this;
            _items    = other._items;
            _comparer = other._comparer;
            return *this;
        }
        sorted_vector& operator=(sorted_vector&& other) noexcept {
            if (this == &other)
                return *this;
            _items    = std::move(other._items);
            _comparer = std::move(other._comparer);
            return *this;
        }
        
        ~sorted_vector() = default;

        vector_index index_of(const T& val) const;
        vector_index find    (const T& val, int start) const;
        void insert  (const T& val);
        bool remove  (const T& val); // returns true if succeeded
        bool exists  (const T& val) const;
        void resort  ();
        void clear   ();
        void resize  (int total, const T& def);
        void reserve (int num);
        void extend  (sorted_vector extend);

                      iter       begin()       { return _items.begin(); }
        [[nodiscard]] const_iter begin() const { return _items.begin(); }
                      iter       end  ()       { return _items.end  (); }
        [[nodiscard]] const_iter end  () const { return _items.end  (); }

        std::vector<T>& data() { return _items; }
        [[nodiscard]] std::vector<T>& data() const { return _items; }
        int size() const { return _items.size(); }
        [[nodiscard]] const auto& get_compare() const { return _comparer; }

        const T& operator[](int index) const;
        T& operator[](int index);
    };

    template <typename T>
    typename sorted_vector<T>::vector_index sorted_vector<T>::index_of(const T& val) const
    {
        int l = 0;
        int r = _items.size() - 1;
        int wv = _comparer(val);
        while (l <= r) {
            int m = (l + r) / 2;
            int wm = _comparer(_items[m]);
            if (wv < wm) {
                r = m - 1;
            } else if (wv > wm) {
                l = m + 1;
            } else {
                return {.exists = true, .index = m};
            }
        }
        return {.exists = false, .index = l};
    }

    template <typename T>
    typename sorted_vector<T>::vector_index sorted_vector<T>::find(const T& val, int start) const {
        int wv = _comparer(val);
        int ws = _comparer(_items[start]);
        int l;
        int r;
        
        if (wv < ws) {
            l = 0;
            r = start;
        } else if (wv > ws) {
            l = start + 1;
            r = _items.size() - 1;
        } else {
            return {.exists = true, .index = start};
        }
        
        while (l <= r) {
            int m = (l + r) / 2;
            int wm = _comparer(_items[m]);
            if (wv < wm) {
                r = m - 1;
            } else if (wv > wm) {
                l = m + 1;
            } else {
                return {.exists = true, .index = m};
            }
        }
        return {.exists = false, .index = l};
    }

    template <typename T>
    void sorted_vector<T>::insert(const T& val)
    {
        _items.insert(_items.begin() + index_of(val).index, val);
    }

    template <typename T>
    bool sorted_vector<T>::remove(const T& val)
    {
        vector_index item = index_of(val);
        if (item.exists)
        {
            _items.erase(_items.begin() + item.index);
        }
        return item.exists;
    }

    template <typename T>
    bool sorted_vector<T>::exists(const T& val) const
    {
        return index_of(val).exists;
    }

    template <typename T>
    void sorted_vector<T>::resort() {
        std::sort(_items.begin(), _items.end(), _comparer);
    }

    template <typename T>
    void sorted_vector<T>::clear() {
        _items.clear();
    }

    template <typename T>
    void sorted_vector<T>::resize(int total, const T& def) {
        _items.resize(total, def);
    }

    template <typename T>
    void sorted_vector<T>::reserve(int num) {
        _items.reserve(num);
    }

    template <typename T>
    void sorted_vector<T>::extend(sorted_vector extend)
    {
        iter it1 = this-> end() - 1;
        iter it2 = extend.end() - 1;

        this->resize(size() + extend.size());
        
        iter last = this->end();

        while (last != this->begin() && it1 != this->begin() && it2 != extend.begin()) {
            --last;
            if (_comparer(*it1) > _comparer(*it2)) {
                std::swap(*it1, *last);
                --it1;
            } else {
                *last = *it2;
                --it2;
            }
        }

        if (it2 != extend.begin()) {
            std::copy(this->begin(), it2 + 1, extend.begin());   
        }
    }

    template <typename T>
    const T& sorted_vector<T>::operator[](int index) const {
        return _items[index];
    }

    template <typename T>
    T& sorted_vector<T>::operator[](int index) {
        return _items[index];
    }
}
