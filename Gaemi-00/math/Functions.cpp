//
// Created by gaetz on 12/06/2021.
//

#include "Functions.h"

bool gmath::nearZero(f32 val, f32 epsilon) {
    if (fabs(val) <= epsilon) {
        return true;
    } else {
        return false;
    }
}