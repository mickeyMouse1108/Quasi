#pragma once
#include <functional>
#include <vector>

namespace stdu {
    template <typename T>
    class sorted_vector
    {
        struct vector_index
        {
            bool exists;
            int index;
        };

        using       iter = typename std::vector<T>::      iterator;
        using const_iter = typename std::vector<T>::const_iterator;
    
        private:
        std::vector<T> _items;
        std::function<int(T)> _comparer;
        public:
        sorted_vector(std::function<int(T)> comparer) : _comparer(std::move(comparer)) {}
        ~sorted_vector() = default;

        vector_index index_of(const T& val) const;
        vector_index find    (const T& val, int start) const;
        void insert  (const T& val);
        bool remove  (const T& val); // returns true if succeeded
        bool exists  (const T& val) const;
        // void extend  (sorted_vector<T> vals);

        iter       begin()       { return _items.begin(); }
        const_iter begin() const { return _items.begin(); }
        iter       end  ()       { return _items.end  (); }
        const_iter end  () const { return _items.end  (); }
    
        std::vector<T>& data() const { return _items; }
        int             size() const { return _items.size(); }

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

    // template <typename T>
    // void sorted_vector<T>::extend(sorted_vector<T> vals)
    // {
    //     // create iterators to the beginning of the two input vectors
    //     iter this_iter = this->begin();
    //     iter this_end  = this->end();
    //     iter vals_iter = vals .begin();
    //
    //     // loop until one of the iterators reaches the end of its vector
    //     while (this_iter != this_end && vals_iter != vals.end()) {
    //         // add the smallest of the two current elements to the result vector
    //         if (*this_iter < *vals_iter) {
    //             this->_items.push_back(*this_iter);
    //             ++this_iter;
    //         } else {
    //             this->_items.push_back(*vals_iter);
    //             ++vals_iter;
    //         }
    //     }
    //
    //     // add the remaining elements from the non-empty vector to the result
    //     if (this_iter != this->end()) {
    //         this->_items.insert(this->end(), this_iter, this_end  );
    //     } else {
    //         this->_items.insert(this->end(), vals_iter, vals.end());
    //     }
    // }

    template <typename T>
    const T& sorted_vector<T>::operator[](int index) const {
        return _items[index];
    }

    template <typename T>
    T& sorted_vector<T>::operator[](int index) {
        return _items[index];
    }
}
