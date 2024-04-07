#include <iostream>
#include "dlist.hpp"
#include <list>
#include <numeric>

using namespace std;

int main(int, char**){
    dlist<int> t;
    t.insert(1);
    assert(t == dlist<int>{1});
    t.insert(2);
    assert(t == dlist<int>({1,2}));
    t.insert(3);
    assert(t == dlist<int>({1,2,3}));
    t.insert(4);
    assert(t == dlist<int>({1,2,3,4}));

    t.erase(++t.begin());
    assert(t == dlist<int>({1,3,4}));
    t.insert(5);
    assert(t == dlist<int>({1,3,4,5}));
    assert(t != dlist<int>({1,3,4,6}));
    assert(t != dlist<int>({1,3,4}));
    assert(t != dlist<int>());
    assert(t.size() == 4);

    t.insert(t.begin(), -1);
    assert(t == dlist<int>({-1,1,3,4,5}));
    t.insert(++t.begin(), 0);
    cout << t.size() << endl;
    assert(t == dlist<int>({-1,0,1,3,4,5}));

    t.clear();
    assert(t == dlist<int>());
    assert(!t.size());

    t.insert(7);
    t.insert(7);
    assert(t == dlist<int>({7,7}));

    for (auto v : t)
        assert(v == 7);
    
    for (auto it = t.begin(); it != t.end(); ++it)
        assert(*it == 7);
    for (auto it = t.begin(); it != t.end(); it++)
        assert(*it == 7);
    
    t.insert(8);
    assert(t == dlist<int>({7,7,8}));
    auto it = t.begin();
    std::advance(it, 2);
    assert(*it == 8);

    assert(t.begin()++ == t.begin());

    t.erase(t.begin());
    t.erase(t.begin());
    assert(*it == 8);

    t.insert(9);
    t.erase(it++);
    assert(*it == 9);
    assert(t == dlist<int>({9}));

    t.insert(10);
    // checking dlist works with some std algorithms
    assert( std::reduce(t.begin(), t.end(), 0) == 19 );
    assert( *std::find_if(t.begin(), t.end(), [](auto v){ return v > 9; }) == 10 );

    t.erase(t.begin());
    bool thrown = false;
    try {
        // checking expired iterator
        assert(*it == 9);
    } catch (...) {
        thrown = true;
    }
    assert(thrown);
}
