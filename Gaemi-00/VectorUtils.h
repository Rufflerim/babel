//
// Created by gaetz on 25/12/2021.
//

#ifndef VECTORUTILS_H
#define VECTORUTILS_H

#include <vector>
#include "Defines.h"

class VectorUtils {
public:
    template<typename T>
    static u32 getIndex(T element, std::vector<T>& vector);

    template<typename T>
    static void moveToBack(u32 index, std::vector<T>& vector);

};

template<typename T>
u32 VectorUtils::getIndex(T element, std::vector<T>& vector) {
    if (vector.size() == 0) return -1;
    auto itr = std::find(begin(vector), end(vector), element);
    if(itr == end(vector)) return -1;
    return itr - begin(vector);
}

template<typename T>
void VectorUtils::moveToBack(u32 index, std::vector<T> &vector) {
    auto it = begin(vector) + index;
    std::rotate(it, it + 1, end(vector));
}



#endif //VECTORUTILS_H
