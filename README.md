# staxtest

Create a C++ program that implements a linked list. The linked list should be a doubly linked list, allowing movement forward and backward. This program should allow you to add and remove nodes from the list. Each node should contain a reference to application data. The program does not have to provide user interaction. Please include units tests for the program. The program can be submitted by including a link to your solution in Github.

# remarks
Description of task gives free interpretation of class interface, requirements and application
I decided to make it a very lighweight **std::list** (on my machine standard list has ~2500 lines of code) version which
development time fits into few hours maximum (as per recruter suggestion)
**dlist** class has typical list complexity:
* Insertion O(1)
* Deletion O(N)
* Access O(N)  
It also compatible with at least some standard algorithms, implements iterator, range based loop
It has *dummy* exceptions, no logging options (which is nice to have in real life project and switch it on/off with macro without performance reduction)

# unit tests
Unit tests cover only basic test cases and use no library. In real life project I would use some unit tests library like **boost test** or more lightweigh library like **Catch2**
It would be nice to have stress tests also

# build and test
This is cmake project without any but standard library usage. You can use the following commands or your prefered way:  
`mkdir build && cd build`  
`cmake .. && make -j`  
Test:  
`ctest --output-on-failure`  