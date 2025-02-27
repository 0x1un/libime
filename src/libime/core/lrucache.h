/*
 * Copyright (C) 2017~2017 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#ifndef _FCITX_LIBIME_CORE_LRU_H_
#define _FCITX_LIBIME_CORE_LRU_H_

#include <boost/unordered_map.hpp>
#include <list>

namespace libime {

// A simple LRU cache.
template <typename K, typename V>
class LRUCache {
    typedef K key_type;
    typedef V value_type;
    // we use boost's unordered_map is for the heterogeneous lookup
    // functionality.
    typedef boost::unordered_map<K,
                                 std::pair<V, typename std::list<K>::iterator>>
        dict_type;
    dict_type dict_;
    std::list<K> order_;
    // Maximum size of the cache.
    size_t sz_;

public:
    LRUCache(size_t sz = 80) : sz_(sz) {}

    size_t size() const { return dict_.size(); }

    size_t capacity() const { return sz_; }

    bool empty() const { return dict_.empty(); }

    bool contains(const key_type &key) {
        return dict_.find(key) != dict_.end();
    }

    template <typename... Args>
    value_type *insert(const key_type &key, Args &&... args) {
        auto iter = dict_.find(key);
        if (iter == dict_.end()) {
            if (size() >= sz_) {
                evict();
            }

            order_.push_front(key);
            auto r = dict_.emplace(
                key, std::make_pair(value_type(std::forward<Args>(args)...),
                                    order_.begin()));
            return &r.first->second.first;
        }
        return nullptr;
    }

    void erase(const key_type &key) {
        auto i = dict_.find(key);
        if (i == dict_.end()) {
            return;
        }
        order_.erase(i->second.second);
        dict_.erase(i);
    }

    // find will refresh the item, so it is not const.
    value_type *find(const key_type &key) {
        // lookup value in the cache
        auto i = dict_.find(key);
        return find_helper(i);
    }

    template <class CompatibleKey, class CompatibleHash,
              class CompatiblePredicate>
    value_type *find(CompatibleKey const &k, CompatibleHash const &h,
                     CompatiblePredicate const &p) {
        return find_helper(dict_.find(k, h, p));
    }

    void clear() {
        dict_.clear();
        order_.clear();
    }

private:
    void evict() {
        // evict item from the end of most recently used list
        auto i = std::prev(order_.end());
        dict_.erase(*i);
        order_.erase(i);
    }

    value_type *find_helper(typename dict_type::iterator i) {
        if (i == dict_.end()) {
            // value not in cache
            return nullptr;
        }

        // return the value, but first update its place in the most
        // recently used list
        auto j = i->second.second;
        if (j != order_.begin()) {
            order_.splice(order_.begin(), order_, j, std::next(j));
            j = order_.begin();
            i->second.second = j;
        }
        return &i->second.first;
    }
};
} // namespace libime

#endif // _FCITX_LIBIME_CORE_LRU_H_
