#pragma once
#include <algorithm>
#include <vector>

namespace stdu {
    template <class T, class TComp>
    class sorted_vector {
        struct vector_index {
            bool exists;
            int index;
        };

        using       iter = typename std::vector<T>::      iterator;
        using const_iter = typename std::vector<T>::const_iterator;
    
        private:
        std::vector<T> _items {};
        TComp _comparer;
        public:
        sorted_vector(TComp comparer) : _items(std::vector<T>{ }), _comparer(comparer) { }
        sorted_vector(std::initializer_list<T> elems, TComp comparer) : _comparer(comparer) {
            _items = elems;
            std::sort(_items.begin(), _items.end(), _comparer);
        }
        template <class TVec>
        sorted_vector(TVec&& elems, TComp comparer) : _comparer(comparer) {
            _items = std::forward<TVec>(elems);
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

        [[nodiscard]] vector_index index_of(const T& val) const {
            int l = 0;
            int r = _items.size() - 1;
            auto wv = _comparer(val);
            while (l <= r) {
                int m = (l + r) / 2;
                auto wm = _comparer(_items[m]);
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
        
        [[nodiscard]] vector_index find(const T& val, int start) const {
            auto wv = _comparer(val);
            auto ws = _comparer(_items[start]);
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
                auto wm = _comparer(_items[m]);
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
        
        template <class TMatch>
        [[nodiscard]] vector_index find_predicate(TMatch matcher, int start) const {
            auto fcomp = matcher(_items[start]);
            int l;
            int r;
        
            if (fcomp < 0) {
                l = 0;
                r = start;
            } else if (fcomp > 0) {
                l = start + 1;
                r = _items.size() - 1;
            } else {
                return {.exists = true, .index = start};
            }
        
            while (l <= r) {
                int m = (l + r) / 2;
                auto cmp = matcher(_items[m]);
                if (cmp < 0) {
                    r = m - 1;
                } else if (cmp > 0) {
                    l = m + 1;
                } else {
                    return {.exists = true, .index = m};
                }
            }
            return {.exists = false, .index = l};
        }
        
        template <class TVal>
        void insert(TVal&& val) {
            auto index = index_of(val).index;
            _items.insert(_items.begin() + index, std::forward<TVal>(val));
        }

        // returns true if succeeded
        bool remove(const T& val) {
            vector_index item = index_of(val);
            if (item.exists)
            {
                _items.erase(_items.begin() + item.index);
            }
            return item.exists;
        }
        
        bool exists(const T& val) const {
            return index_of(val).exists;
        }
        
        void resort() {
            std::sort(_items.begin(), _items.end(), [&](const T& a, const T& b){ return _comparer(a) < _comparer(b); });
        }
        
        void clear() {
            _items.clear();
        }
        
        void resize(int total, const T& def) {
            _items.resize(total, def);
        }
        
        void reserve(int num) {
            _items.reserve(num);
        }
        
        // ! idk if this works or not
        void extend  (const sorted_vector& extend) {
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

                      iter       begin()       { return _items.begin(); }
        [[nodiscard]] const_iter begin() const { return _items.begin(); }
                      iter       end  ()       { return _items.end  (); }
        [[nodiscard]] const_iter end  () const { return _items.end  (); }

        std::vector<T>& data() { return _items; }
        [[nodiscard]] std::vector<T>& data() const { return _items; }
        [[nodiscard]] int size() const { return _items.size(); }
        [[nodiscard]] const auto& get_compare() const { return _comparer; }

        const T& operator[](int index) const { return _items[index]; }
        T& operator[](int index) { return _items[index]; }
    };
}
