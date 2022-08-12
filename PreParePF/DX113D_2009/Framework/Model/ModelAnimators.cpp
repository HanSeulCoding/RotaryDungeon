#include "Framework.h"

ModelAnimators::ModelAnimators(string file)
    : ModelAnimator(file), drawCount(0)
{
    for (UINT i = 0; i < MAX_INSTANCE; i++)
    {
        instanceData[i].world = XMMatrixIdentity();
        instanceData[i].index = i;
    }

    instanceBuffer = new VertexBuffer(instanceData, sizeof(InstanceData), MAX_INSTANCE);
	frustum = new Frustum();

	SetBox(&minBox, &maxBox);
}

ModelAnimators::~ModelAnimators()
{
    delete instanceBuffer;
	delete frustum;
}

void ModelAnimators::Update()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		FrameBuffer::Frame& frameData = (FrameBuffer::Frame&)frameBuffer->data.frames[i];

		{//CurAnimation		
			ModelClip* clip = clips[frameData.cur.clip];

			frameData.cur.time += DELTA * clip->tickPerSecond * frameData.cur.speed;

			if (frameData.cur.time >= 1.0f)
			{
				frameData.cur.curFrame++;
				frameData.cur.curFrame %= (clip->frameCount - 1);
				frameData.cur.time = 0.0f;

				float ratio = (float)frameData.cur.curFrame / (float)clip->frameCount;

				if (ratio > 0.7f)
				{
					if (InstanceEndEvents[i].count(frameData.cur.clip) > 0)
						InstanceEndEvents[i][frameData.cur.clip](params[i][frameData.cur.clip]);
				}
			}
		}

		{//NextAnimation
			if (frameData.next.clip > -1)
			{
				ModelClip* clip = clips[frameData.next.clip];

				frameData.tweenTime += DELTA / frameData.takeTime;

				if (frameData.tweenTime >= 1.0f)
				{
					frameData.cur = frameData.next;
					frameData.tweenTime = 0.0f;

					frameData.next.clip = -1;
					frameData.next.curFrame = 0;
					frameData.next.time = 0.0f;
				}
				else
				{
					frameData.next.time += DELTA * clip->tickPerSecond * frameData.next.speed;

					if (frameData.next.time >= 1.0f)
					{
						frameData.next.curFrame++;
						frameData.next.curFrame %= (clip->frameCount - 1);
						frameData.next.time = 0.0f;
					}
				}
			}
		}
	}

	UpdateTransforms();
}

void ModelAnimators::Render()
{
	if (texture == nullptr)
		CreateTexture();

	frameBuffer->SetVSBuffer(5);
	DC->VSSetShaderResources(0, 1, &srv);

	instanceBuffer->IASet(1);

	MeshRender(transforms.size());
}

Transform* ModelAnimators::Add()
{
	Transform* transform = new Transform();
	transforms.emplace_back(transform);

	InstanceEndEvents.emplace_back();
	params.emplace_back();

    return transform;
}

void ModelAnimators::PlayClip(UINT instance, UINT clip, float speed, float takeTime)
{
	frameBuffer->data.frames[instance].next.clip = clip;
	frameBuffer->data.frames[instance].next.speed = speed;
	frameBuffer->data.frames[instance].takeTime = takeTime;
}

void ModelAnimators::UpdateTransforms()
{
	frustum->Update();
	drawCount = 0;

	for (UINT i = 0; i < transforms.size(); i++)
	{	
		Vector3 globalMin = XMVector3TransformCoord(minBox.data, transforms[i]->GetWorld());
		Vector3 globalMax = XMVector3TransformCoord(maxBox.data, transforms[i]->GetWorld());

		if (frustum->ContainBox(globalMin, globalMax))
		{
			transforms[i]->UpdateWorld();
			instanceData[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
			instanceData[drawCount].index = i;
			drawCount++;
		}		
	}

	instanceBuffer->Update(instanceData, drawCount);
}
