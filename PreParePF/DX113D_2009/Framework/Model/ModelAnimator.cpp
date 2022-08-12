#include "Framework.h"

ModelAnimator::ModelAnimator(string file)
	: ModelReader(file), texture(nullptr), srv(nullptr)
{
	frameBuffer = new FrameBuffer();
	typeBuffer->data.type = 1;
}

ModelAnimator::~ModelAnimator()
{
	delete frameBuffer;
	delete[] clipTransform;

	texture->Release();
	srv->Release();

	delete clips[key];
}

void ModelAnimator::Update()
{
	FrameBuffer::Frame& frameData = (FrameBuffer::Frame&)frameBuffer->data.frames[0];

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
				if (EndEvents.count(frameData.cur.clip) > 0)
					EndEvents[frameData.cur.clip]();
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

void ModelAnimator::Render()
{
	if (texture == nullptr)
		CreateTexture();

	frameBuffer->SetVSBuffer(5);
	DC->VSSetShaderResources(0, 1, &srv);

	MeshRender();
}

void ModelAnimator::ReadClip(int key,string file, UINT clipNum)
{
	file = "ModelData/Clips/" + file + to_string(clipNum) + ".clip";
	this->key = key;
	BinaryReader* r = new BinaryReader(file);
	
	ModelClip* clip = new ModelClip();
	clip->name = r->String();
	clip->duration = r->Float();
	clip->tickPerSecond = r->Float();
	clip->frameCount = r->UInt();

	UINT keyFrameCount = r->UInt();
	for (UINT i = 0; i < keyFrameCount; i++)
	{
		KeyFrame* keyFrame = new KeyFrame();
		keyFrame->boneName = r->String();

		UINT size = r->UInt();
		if (size > 0)
		{
			keyFrame->transforms.resize(size);

			void* ptr = (void*)keyFrame->transforms.data();
			r->Byte(&ptr, sizeof(KeyTransform) * size);
		}
		clip->keyFrames[keyFrame->boneName] = keyFrame;
	}
	clips[key] = clip;
}

void ModelAnimator::PlayClip(UINT clip, float speed, float takeTime)
{
	frameBuffer->data.frames[0].next.clip = clip;
	frameBuffer->data.frames[0].next.speed= speed;
	frameBuffer->data.frames[0].takeTime = takeTime;
}

Matrix ModelAnimator::GetTransformByNode(int nodeIndex)
{
	if (nodeTransform == nullptr)
		return XMMatrixIdentity();

	FrameBuffer::Clip& curClip = frameBuffer->data.frames[0].cur;

	Matrix cur = nodeTransform[curClip.clip].transform[curClip.curFrame][nodeIndex];
	Matrix next = nodeTransform[curClip.clip].transform[curClip.curFrame + 1][nodeIndex];

	Matrix curAnim = LERP(cur, next, curClip.time);

	FrameBuffer::Clip& nextClip = frameBuffer->data.frames[0].cur;

	if (nextClip.clip == -1)
		return curAnim;

	cur = nodeTransform[nextClip.clip].transform[nextClip.curFrame][nodeIndex];
	next = nodeTransform[nextClip.clip].transform[nextClip.curFrame + 1][nodeIndex];

	Matrix nextAnim = LERP(cur, next, nextClip.time);

	return LERP(curAnim, nextAnim, frameBuffer->data.frames[0].tweenTime);
}

void ModelAnimator::CreateTexture()
{
	UINT clipCount = clips.size();

	clipTransform = new ClipTransform[clipCount];
	nodeTransform = new ClipTransform[clipCount];

	for (UINT i = 0; i < clipCount; i++)
		CreateClipTransform(i);

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = MAX_BONE * 4;
	desc.Height = MAX_FRAME_KEY;
	desc.ArraySize = clipCount;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	UINT pageSize = MAX_BONE * sizeof(Matrix) * MAX_FRAME_KEY;

	void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

	for (UINT c = 0; c < clipCount; c++)
	{
		UINT start = c * pageSize;

		for (UINT y = 0; y < MAX_FRAME_KEY; y++)
		{
			void* temp = (BYTE*)p + MAX_BONE * y * sizeof(Matrix) + start;

			VirtualAlloc(temp, MAX_BONE * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
			memcpy(temp, clipTransform[c].transform[y], MAX_BONE * sizeof(Matrix));
		}
	}

	D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

	for (UINT c = 0; c < clipCount; c++)
	{
		void* temp = (BYTE*)p + c * pageSize;

		subResource[c].pSysMem = temp;
		subResource[c].SysMemPitch = MAX_BONE * sizeof(Matrix);
		subResource[c].SysMemSlicePitch = pageSize;
	}

	V(DEVICE->CreateTexture2D(&desc, subResource, &texture));

	delete[] subResource;
	VirtualFree(p, 0, MEM_RELEASE);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = clipCount;

	V(DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv));
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	ModelClip* clip = clips[index];

	for (UINT f = 0; f < clip->frameCount; f++)
	{
		UINT nodeIndex = 0;
		for (NodeData* node : nodes)
		{
			Matrix animation;
			KeyFrame* frame = clip->GetKeyFrame(node->name);
			if (frame != nullptr)
			{
				KeyTransform& transform = frame->transforms[f];

				Matrix S = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
				Matrix R = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.rotation));
				Matrix T = XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);

				animation = S * R * T;
			}
			else
			{
				animation = XMMatrixIdentity();
			}

			Matrix parent;
			int parentIndex = node->parent;

			if (parentIndex < 0)
				parent = XMMatrixIdentity();
			else
				parent = nodeTransform[index].transform[f][parentIndex];

			nodeTransform[index].transform[f][nodeIndex] = animation * parent;

			if (boneMap.count(node->name) > 0)
			{
				int boneIndex = boneMap[node->name];
				Matrix transform = XMLoadFloat4x4(&bones[boneIndex]->offset);
				transform *= nodeTransform[index].transform[f][nodeIndex];
				clipTransform[index].transform[f][boneIndex] = transform;
			}

			nodeIndex++;
		}
	}
}
