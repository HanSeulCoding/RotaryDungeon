#include "Framework.h"

Models::Models(string file) : ModelReader(file), drawCount(0)
{
    for (InstanceData& data : instanceData)
        data.world = XMMatrixIdentity();

    instanceBuffer = new VertexBuffer(instanceData, sizeof(InstanceData), MAX_INSTANCE);
}

Models::~Models()
{
    for (Transform* transform : transforms)
        delete transform;

    delete instanceBuffer;
}

void Models::Update()
{
    for (UINT i = 0; i < transforms.size(); i++)
    {
        transforms[i]->UpdateWorld();
        instanceData[i].world = XMMatrixTranspose(transforms[i]->GetWorld());
    }

    instanceBuffer->Update(instanceData, drawCount);
}

void Models::Render()
{
    instanceBuffer->IASet(1);

    MeshRender(drawCount);
}

Transform* Models::Add()
{
    Transform* transform = new Transform();
    transforms.emplace_back(transform);

    drawCount = transforms.size();

    return transform;
}
