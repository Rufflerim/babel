//
// Created by gaetz on 27/09/2022.
//

#ifndef RENDER_RENDERREQUEST_H
#define RENDER_RENDERREQUEST_H

class Material;
class Mesh;
class Texture;
class Transform;

struct RenderRequest {
    Material* material;
    Mesh* mesh;
    Texture* texture;
    Transform* transform;
};

#endif //RENDER_RENDERREQUEST_H
