#pragma once
#include <memory>

// this is very simple just for sake of simplicity
// in real life project we will have few exceptions with proper description or even stack trace generated
#define ASSERT_THROW(c) \
    if (!(c)) throw std::exception();

// if we follow std approach we should add allocator parameter here but I'm omiting it for sake of simplicity
template<typename T>
class dlist {
    struct node {
        // shared_ptr may reduce performance (need to check this)
        // however it provides a nice weak_ptr compatibility which is used by iterator class
        // shared_ptr provides nice optimization, it doesn't use atomic functions in single threaded program
        // but if performance is really impacted this needs to be redesigned
        using ptr_type = std::shared_ptr<node>;
        T data;
        ptr_type prev;
        ptr_type next;
    
    node(T value) : data(value) {}
    };

    class iterator {
        friend class dlist<T>;
        using ptr_type = std::weak_ptr<node>;
        ptr_type current;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;
        iterator() {}
        iterator(ptr_type node) : current(node) {}
        
        reference operator*() const {
            ASSERT_THROW(!current.expired());
            return current.lock()->data;
        }
        pointer operator->() const {
            ASSERT_THROW(!current.expired());
            return &current.lock()->data;
        }
        
        inline iterator& operator++() {
            ASSERT_THROW(!current.expired());
            current = current.lock()->next;
            return *this;
        }
        inline iterator operator++(int) {
            iterator ret(current);
            ++(*this);
            return ret;
        }
        
        inline iterator& operator--() {
            ASSERT_THROW(!current.expired());
            current = current.lock()->prev;
            return *this;
        }
        inline iterator operator--(int) {
            iterator ret(current);
            --(*this);
            return ret;
        }
        
        inline bool operator==(const iterator& other) const {
            // this method allows expired iterators to be compared as
            // currently there is no difference between operator pointing to deleted item
            // and end() iterator. This doesn't match normal std behavior and leads to possible errors
            // however for sake of simplicity I decided not to implement it
            // in a real life project we would need to:
            // check that iterators belong to the same container and throw if compared with
            // expired iterator
            return !current.owner_before(other.current) && !other.current.owner_before(current);
        }
        inline bool operator!=(const iterator& other) const {
            return !(other == *this);
        }
    };

    using ptr_data_type = node;
    using ptr_type = std::shared_ptr<ptr_data_type>;
    using iterator_type = iterator;
    using const_iterator_type = const iterator_type;

    ptr_type head;
    ptr_type tail;
public:
    dlist() = default;
    dlist(std::initializer_list<T> items) {
        for (auto& v : items) {
            insert(v);
        }
    }

    ~dlist() { clear(); }

    inline iterator_type begin() const { return iterator_type(head); }
    inline const_iterator_type end() const { return {}; }

    bool operator==(const dlist<T>& other) const {
        auto it1 = begin();
        auto it2 = other.begin();
        while (it1 != end() && it2 != other.end()) {
            if (*it1 != *it2)
                return false;
                ++it1;
                ++it2;
        }

        return it1 == end() && it2 == end();
    }

    void clear() {
        auto it = begin();
        while (it != end()) {
            it = erase(it);
        }
    }
    
    template<typename... Args>
    inline const_iterator_type insert( const_iterator_type pos, Args&&... args ) {
        return insert(pos, T{std::forward<Args>(args)...});
    }
    template<typename... Args>
    inline const_iterator_type insert( Args&&... args ) {
        return insert(end(), T{std::forward<Args>(args)...});
    }
    const_iterator_type insert(const_iterator_type pos, T&& value) {
        ptr_type new_node(new ptr_data_type(value));
        if (!head) {
            head = tail = new_node;
        } else {
            if (pos == end()) {
                tail->next = new_node;
                new_node->prev = tail;
                tail = new_node;                
            } else {
                auto next_node = pos.current.lock();
                new_node->prev = next_node->prev;
                new_node->next = next_node;
                if (next_node->prev)
                    next_node->prev->next = new_node;
                next_node->prev = new_node;
                if (next_node == head)
                    head = new_node;
            }
        }

        return {new_node};
    }

    const_iterator_type erase(const_iterator_type it) {
        if (auto tmp = it.current.lock()) {
            auto prev = tmp->prev;
            auto next = tmp->next;
            if (prev)
                prev->next = next;
            if (next)
                next->prev = prev;
            
            if (tmp == head)
                head = next;
            else if (tmp == tail)
                tail = next;

            return {next};
        } // if tmp is zero we probably have end()

        return end();
    }

    size_t size() const {
        size_t size = 0;
        for (auto it = begin(); it != end(); ++it)
            ++size;
        return size;
    }
};