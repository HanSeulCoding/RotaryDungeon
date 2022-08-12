#pragma once

class Outline : public Sprite
{
private:
	class OutlineBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			float value = 0;
			float range = 0;
			Float2 size = { 0, 0 };
		}data;

		OutlineBuffer() : ConstBuffer(&data, sizeof(Data))
		{
		}
	};	

	

public:
	OutlineBuffer* buffer;
	Outline();
	~Outline();

	OutlineBuffer* GetOutlineBuffer() { return buffer; }
	void Render() override;
};