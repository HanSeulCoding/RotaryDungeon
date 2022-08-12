#pragma once

class ModelAnimator : public ModelReader
{
protected:
	FrameBuffer* frameBuffer;
	ClipTransform* clipTransform;
	ClipTransform* nodeTransform;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
	int key;
	map<int,ModelClip*> clips;

	map<int, CallBack> EndEvents;
public:
	ModelAnimator(string file);
	virtual ~ModelAnimator();

	virtual void Update();
	virtual void Render();	

	void ReadClip(int key, string file, UINT clipNum = 0);
	void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 0.2f);

	void SetEndEvent(int clip, CallBack Event) { EndEvents[clip] = Event; }

	Matrix GetTransformByNode(int nodeIndex);
protected:
	void CreateTexture();
	void CreateClipTransform(UINT index);
};