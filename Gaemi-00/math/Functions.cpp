//
// Created by gaetz on 12/06/2021.
//

#include "Functions.h"
#include "Vec2i.h"

bool gmath::nearZero(f32 val, f32 epsilon) {
    if (fabs(val) <= epsilon) {
        return true;
    } else {
        return false;
    }
}
gmath::Vec2i gmath::convert1Dto2DCoords(i32 index, i32 rowSize) {
    return gmath::Vec2i { index % rowSize, index / rowSize };
}
