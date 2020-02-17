#include <vector>
#include <list>
#include <stdexcept>

template <typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>>
class HashMap {
private:
    const size_t BODY_SIZE = 7e5;
    std::list<std::pair<const KeyType, ValueType>> container;
    std::vector<std::list<typename std::list<std::pair<const KeyType, ValueType>>::iterator>> body;
    Hash hasher;
public:
    HashMap(Hash _hasher=Hash()): body(BODY_SIZE), hasher(_hasher) {
    }
    template <typename SequenseIt>
    HashMap(SequenseIt begin, SequenseIt end, Hash hasher=Hash()): body(BODY_SIZE), hasher(hasher) {
        for (auto it = begin; it != end; ++it) {
            insert(*it);
        }
    }
    HashMap(std::initializer_list<std::pair<const KeyType, ValueType>> il, Hash hasher=Hash()): body (BODY_SIZE), hasher(hasher) {
        for (auto p : il) {
            insert(p);
        }
    }
    void clear() {
        for (std::pair<const KeyType, ValueType> p : container) {
            body[p.first % BODY_SIZE].clear();
        }
        container.clear();
    }

    HashMap& operator=(const HashMap<KeyType, ValueType, Hash>& other) {
        body.resize(BODY_SIZE);
        hasher = other.hasher;
        std::vector<std::pair<const KeyType, ValueType>> tmp;
        for (std::pair<const KeyType, ValueType> p: other) {
            tmp.push_back(p);
        }
        clear();
        for (std::pair<const KeyType, ValueType> p : tmp) {
            insert(p);
        }
        return *this;
    }

    class iterator {
    private:
        typename std::list<std::pair<const KeyType, ValueType>>::iterator it;
    public:
        iterator() {}
        iterator(const typename std::list<std::pair<const KeyType, ValueType>>::iterator parent): it(parent) {}; 
        iterator(const iterator& other): it(other.it) {}
        std::pair<const KeyType, ValueType>& operator*() {
            return *it;
        }
        iterator& operator++() {
            ++it;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp(*this);
            ++it;
            return tmp;
        }
        std::pair<const KeyType, ValueType>* operator->() {
            return &(*it);
        }
        bool operator==(const iterator& other) const {
            return it == other.it;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };
    class const_iterator {
    private:
        typename std::list<std::pair<const KeyType, ValueType>>::const_iterator it;
    public:
        const_iterator() {}
        const_iterator(typename std::list<std::pair<const KeyType, ValueType>>::const_iterator parent): it(parent) {}; 
        const_iterator(const const_iterator& other): it(other.it) {}
        const std::pair<const KeyType, ValueType> operator*() {
            return *it;
        }
        const_iterator& operator++() {
            ++it;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp(*this);
            ++it;
            return tmp;
        }
        const std::pair<const KeyType, ValueType>* operator->() {
            return &(*it);
        }
        bool operator==(const const_iterator& other) const {
            return it == other.it;
        }
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
    };

    void insert(std::pair<const KeyType, ValueType> pr) {
        size_t hash = hasher(pr.first) % BODY_SIZE;
        for (auto contIterator : body[hash]) {
            if (contIterator->first == pr.first) return;
        }
        container.push_back(pr);
        body[hash].push_back(std::prev(container.end()));
    }
    void erase(const KeyType& key) {
        size_t hash = hasher(key) % BODY_SIZE;
        for (auto it = body[hash].begin(); it != body[hash].end(); ++it) {
            if ((*it)->first == key) {
                container.erase(*it);
                body[hash].erase(it);
                break;
            }
        }
    }
    size_t size() const {
        return container.size();
    }
    bool empty() const {
        return container.empty();
    }
    Hash hash_function() const {
        return hasher;
    }
    iterator find(const KeyType& key) {
        size_t hash = hasher(key) % BODY_SIZE;
        for (auto it = body[hash].begin(); it != body[hash].end(); ++it) {
            if ((*it)->first == key) {
                return iterator(*it);
            }
        }
        return end();
    }
    const_iterator find(const KeyType& key) const {
        size_t hash = hasher(key) % BODY_SIZE;
        for (auto it = body[hash].begin(); it != body[hash].end(); ++it) {
            if ((*it)->first == key) {
                return const_iterator(*it);
            }
        }
        return end();
    }
    ValueType& operator[](const KeyType& key) {
        size_t hash = hasher(key) % BODY_SIZE;
        for (auto it = body[hash].begin(); it != body[hash].end(); ++it) {
            if ((*it)->first == key) {
                return (*it)->second;
            }
        }
        ValueType value = ValueType();
        std::pair<const KeyType, ValueType> to_insert(key, value);
        container.push_back(to_insert);
        body[hash].push_back(std::prev(container.end()));
        return container.back().second;
    }
    const ValueType& at(const KeyType& key) const {
        size_t hash = hasher(key) % BODY_SIZE;
        for (auto it = body[hash].begin(); it != body[hash].end(); ++it) {
            if ((*it)->first == key) {
                return (*it)->second;
            }
        }
        throw(std::out_of_range("At() didn't find an element!"));
        return container.back().second;
    }
    HashMap::iterator begin() {
        return iterator(container.begin());
    }
    HashMap::iterator end() {
        return iterator(container.end());
    }    

    HashMap::const_iterator begin() const {
        return const_iterator(container.begin());
    }
    HashMap::const_iterator end() const {
        return const_iterator(container.end());
    }    
};
