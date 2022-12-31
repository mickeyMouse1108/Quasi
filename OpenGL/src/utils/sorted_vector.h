#pragma once
#include <functional>
#include <vector>

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
    sorted_vector(const std::function<int(T)>& comparer) : _comparer(comparer) {}
    ~sorted_vector() {}

    vector_index index_of(T val) const;
    void insert  (T val);
    bool remove  (T val); // returns true if succeeded
    bool exists  (T val) const;
    void extend  (sorted_vector<T> vals);

    iter       begin()       { return _items.begin(); }
    const_iter begin() const { return _items.begin(); }
    iter       end  ()       { return _items.end  (); }
    const_iter end  () const { return _items.end  (); }
    
    std::vector<T>& data() const { return _items; }
    int             size() const { return _items.size(); }

    T& operator[](int index) const;
};

template <typename T>
typename sorted_vector<T>::vector_index sorted_vector<T>::index_of(T val) const
{
    int l = 0;
    int r = _items.size() - 1;
    while (l <= r)
    {
        int m = (l + r) / 2;
        int wv = _comparer(val);
        int wm = _comparer(_items[m]);
        if (wv < wm)
        {
            r = m - 1;
        } else if (wv > wm)
        {
            l = m + 1;
        } else
        {
            return {.exists = true, .index = m};
        }
    }
    return {.exists = false, .index = l};
}

template <typename T>
void sorted_vector<T>::insert(T val)
{
    _items.insert(_items.begin() + index_of(val).index, val);
}

template <typename T>
bool sorted_vector<T>::remove(T val)
{
    vector_index item = index_of(val);
    if (item.exists)
    {
        _items.erase(_items.begin() + item.index);
    }
    return item.exists;
}

template <typename T>
bool sorted_vector<T>::exists(T val) const
{
    return index_of(val).exists;
}

template <typename T>
void sorted_vector<T>::extend(sorted_vector<T> vals)
{
    // create iterators to the beginning of the two input vectors
    iter this_iter = this->begin();
    iter this_end  = this->end();
    iter vals_iter = vals .begin();

    // loop until one of the iterators reaches the end of its vector
    while (this_iter != this_end && vals_iter != vals.end()) {
        // add the smallest of the two current elements to the result vector
        if (*this_iter < *vals_iter) {
            this->_items.push_back(*this_iter);
            ++this_iter;
        } else {
            this->_items.push_back(*vals_iter);
            ++vals_iter;
        }
    }

    // add the remaining elements from the non-empty vector to the result
    if (this_iter != this->end()) {
        this->_items.insert(this->end(), this_iter, this_end  );
    } else {
        this->_items.insert(this->end(), vals_iter, vals.end());
    }
}

template <typename T>
T& sorted_vector<T>::operator[](int index) const
{
    return _items[index];
}
