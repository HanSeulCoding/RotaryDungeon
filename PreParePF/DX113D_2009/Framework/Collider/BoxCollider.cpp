#include "Framework.h"

BoxCollider::BoxCollider(Vector3 minBox, Vector3 maxBox)
    : minBox(minBox), maxBox(maxBox)
{
    type = BOX;
    CreateMesh();
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
    UpdateWorld();

    if (ray.dir.Length() == 0.0f)
        return false;

    Contact temp;
    temp.distance = FLT_MAX;

    UINT face[] = {
        0, 1, 2, 3,//F
        4, 5, 6, 7,//B
        0, 1, 5, 4,//L
        1, 5, 6, 2,//R
        2, 3, 7, 6,//U
        0, 3, 7, 4,//D
    };

    for (UINT i = 0; i < 6; i++)
    {
        Vector3 p[4];

        p[0] = vertices[face[i * 4 + 0]].position;
        p[1] = vertices[face[i * 4 + 1]].position;
        p[2] = vertices[face[i * 4 + 2]].position;
        p[3] = vertices[face[i * 4 + 3]].position;

        p[0] = XMVector3TransformCoord(p[0].data, world);
        p[1] = XMVector3TransformCoord(p[1].data, world);
        p[2] = XMVector3TransformCoord(p[2].data, world);
        p[3] = XMVector3TransformCoord(p[3].data, world);


        float dist;
        if (Intersects(ray.pos.data, ray.dir.data, p[0].data, p[1].data, p[2].data, dist))
        {
            if (dist < temp.distance)
            {
                temp.distance = dist;
                temp.hitPoint = ray.pos + ray.dir * dist;
            }
        }

        if (Intersects(ray.pos.data, ray.dir.data, p[0].data, p[3].data, p[2].data, dist))
        {
            if (dist < temp.distance)
            {
                temp.distance = dist;
                temp.hitPoint = ray.pos + ray.dir * dist;
            }
        }
    }

    if (temp.distance == FLT_MAX)
        return false;

    if (contact != nullptr)
    {
        contact->distance = temp.distance;
        contact->hitPoint = temp.hitPoint;
    }

    return true;
}

bool BoxCollider::BoxCollision(BoxCollider* collider)
{
    Obb box1 = GetObb();
    Obb box2 = collider->GetObb();

    Vector3 D = box2.position - box1.position;

    for (UINT i = 0; i < 3; i++)
    {
        if (SeperateAxis(D, box1.axis[i], box1, box2)) return false;
        if (SeperateAxis(D, box2.axis[i], box1, box2)) return false;
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if (box1.axis[i] == box2.axis[i]) return true;
        }
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            Vector3 cross = Vector3::Cross(box1.axis[i], box2.axis[j]);
            if (SeperateAxis(D, cross, box1, box2)) return false;
        }
    }

    return true;
}

/*
bool BoxCollider::SphereCollision(SphereCollider* collider)
{
    Matrix T = XMMatrixTranslation(globalPosition.x, globalPosition.y, globalPosition.z);
    Matrix R = XMMatrixRotationQuaternion(globalRotation.data);

    Matrix invWorld = XMMatrixInverse(nullptr, R * T);

    Vector3 spherePos = XMVector3TransformCoord(collider->GlobalPosition().data, invWorld);

    Vector3 tempMin = minBox * globalScale;
    Vector3 tempMax = maxBox * globalScale;

    Vector3 temp;
    temp.x = max(tempMin.x, min(spherePos.x, tempMax.x));
    temp.y = max(tempMin.y, min(spherePos.y, tempMax.y));
    temp.z = max(tempMin.z, min(spherePos.z, tempMax.z));

    temp -= spherePos;

    return temp.Length() <= collider->Radius();
}
*/

bool BoxCollider::SphereCollision(SphereCollider* collider)
{
    Obb box = GetObb();

    Vector3 pos = box.position;

    for (UINT i = 0; i < 3; i++)
    {
        float length = Vector3::Dot(box.axis[i], collider->GlobalPosition() - box.position);

        float mult = (length < 0.0f) ? -1.0f : 1.0f;

        length = min(abs(length), box.halfSize[i]);
        pos += box.axis[i] * length * mult;
    }

    float distance = (collider->GlobalPosition() - pos).Length();

    return distance <= collider->Radius();
}

bool BoxCollider::CapsuleCollision(CapsuleCollider* collider)
{
    return collider->BoxCollision(this);
}

bool BoxCollider::SphereCollision(Vector3 center, float radius)
{
    Matrix T = XMMatrixTranslation(globalPosition.x, globalPosition.y, globalPosition.z);
    Matrix R = XMMatrixRotationQuaternion(globalRotation.data);

    Matrix invWorld = XMMatrixInverse(nullptr, R * T);

    Vector3 spherePos = XMVector3TransformCoord(center.data, invWorld);

    Vector3 tempMin = minBox * globalScale;
    Vector3 tempMax = maxBox * globalScale;

    Vector3 temp;
    temp.x = max(tempMin.x, min(spherePos.x, tempMax.x));
    temp.y = max(tempMin.y, min(spherePos.y, tempMax.y));
    temp.z = max(tempMin.z, min(spherePos.z, tempMax.z));

    temp -= spherePos;

    return temp.Length() <= radius;
}

Vector3 BoxCollider::GlobalMinBox()
{
    return XMVector3TransformCoord(minBox.data, world);
}

Vector3 BoxCollider::GlobalMaxBox()
{
    return XMVector3TransformCoord(maxBox.data, world);
}

Obb BoxCollider::GetObb()
{
    obb.position = globalPosition;

    obb.axis[0] = Right();
    obb.axis[1] = Up();
    obb.axis[2] = Forward();

    obb.halfSize = (maxBox - minBox) * 0.5f * globalScale;

    return obb;
}

void BoxCollider::CreateMesh()
{
    vertices.emplace_back(minBox.x, minBox.y, minBox.z);
    vertices.emplace_back(minBox.x, maxBox.y, minBox.z);
    vertices.emplace_back(maxBox.x, maxBox.y, minBox.z);
    vertices.emplace_back(maxBox.x, minBox.y, minBox.z);

    vertices.emplace_back(minBox.x, minBox.y, maxBox.z);
    vertices.emplace_back(minBox.x, maxBox.y, maxBox.z);
    vertices.emplace_back(maxBox.x, maxBox.y, maxBox.z);
    vertices.emplace_back(maxBox.x, minBox.y, maxBox.z);

    indices = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    mesh = new Mesh(vertices.data(), sizeof(Vertex), vertices.size(),
        indices.data(), indices.size());
}

bool BoxCollider::SeperateAxis(Vector3 D, Vector3 axis, Obb box1, Obb box2)
{
    float distance = abs(Vector3::Dot(D, axis));

    float a = 0.0f;
    float b = 0.0f;

    for (UINT i = 0; i < 3; i++)
    {
        a += abs(Vector3::Dot(box1.axis[i] * box1.halfSize[i], axis));
        b += abs(Vector3::Dot(box2.axis[i] * box2.halfSize[i], axis));
    }

    return distance > a + b;
}
