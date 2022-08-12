#include "Framework.h"

bool Collider::isVisible = false;

Collider::Collider()
{
    material = new Material(L"Collider");
    SetColor(0, 1, 0);
}

Collider::~Collider()
{
    delete material;
    delete mesh;
}

bool Collider::Collision(Collider* collider)
{
    collider->UpdateWorld();
    this->UpdateWorld();

    switch (collider->type)
    {
    case Collider::BOX:
        return BoxCollision(static_cast<BoxCollider*>(collider));
        break;
    case Collider::SPHERE:
        return SphereCollision(static_cast<SphereCollider*>(collider));
        break;
    case Collider::CAPSULE:
        return CapsuleCollision(static_cast<CapsuleCollider*>(collider));
        break;    
    }

    return false;
}

void Collider::Render()
{
  

    SetWorldBuffer();

    if (!isVisible) return;
    mesh->IASet(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    material->Set();
    DC->DrawIndexed(indices.size(), 0, 0);
}
