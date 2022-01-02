//
// Created by gaetz on 01/01/2022.
//

#ifndef DATA_ANIMATIONDATA_H
#define DATA_ANIMATIONDATA_H

#include "IData.h"

namespace data {
    struct AnimationRow {
        u16 row { 0 };
        u16 frameNumber { 0 };
        str name;
    };

    class AnimationData : public IData {
    public:
        u16 frameWidth { 0 };
        u16 frameHeight { 0 };
        f32 frameTime { 0 };
        str textureName { "" };
        std::vector<AnimationRow> rows;

        AnimationData() = default;

        AnimationData(u16 frameWidthP, u16 frameHeightP, f32 frameTimeP, const str& textureNameP,
                      const std::vector<AnimationRow>& rowsP) :
                frameWidth { frameWidthP }, frameHeight { frameHeightP }, frameTime { frameTimeP },
                textureName { textureNameP }, rows { rowsP } {}
    };
}


#endif //DATA_ANIMATIONDATA_H
