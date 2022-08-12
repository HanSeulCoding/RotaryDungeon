#pragma once

class Vector3
{
public:
	Vector4 data;

	Vector3() { data = XMVectorZero(); }
	Vector3(Float3 value) { data = XMLoadFloat3(&value); }
	Vector3(float x, float y, float z) { data = XMVectorSet(x, y, z, 0); }
	Vector3(Vector4 value) : data(value) {}

	operator Float3()
	{
		Float3 result;
		XMStoreFloat3(&result, data);
		return result;
	}

	void SetX(float value) { data = XMVectorSetX(data, value); }
	void SetY(float value) { data = XMVectorSetY(data, value); }
	void SetZ(float value) { data = XMVectorSetZ(data, value); }
	void SetW(float value) { data = XMVectorSetW(data, value); }
	float GetX() { return XMVectorGetX(data); }
	float GetY() { return XMVectorGetY(data); }
	float GetZ() { return XMVectorGetZ(data); }
	float GetW() { return XMVectorGetW(data); }

	_declspec(property(get = GetX, put = SetX)) float x;
	_declspec(property(get = GetY, put = SetY)) float y;
	_declspec(property(get = GetZ, put = SetZ)) float z;
	_declspec(property(get = GetW, put = SetW)) float w;

	Vector3 operator+(const Vector3& value) const { return data + value.data; }
	Vector3 operator-(const Vector3& value) const { return data - value.data; }
	Vector3 operator*(const Vector3& value) const { return data * value.data; }
	Vector3 operator/(const Vector3& value) const { return data / value.data; }

	void operator+=(const Vector3& value) { data += value.data; }
	void operator-=(const Vector3& value) { data -= value.data; }
	void operator*=(const Vector3& value) { data *= value.data; }
	void operator/=(const Vector3& value) { data /= value.data; }

	Vector3 operator+(const float& value) const { return data + XMVectorReplicate(value); }
	Vector3 operator-(const float& value) const { return data - XMVectorReplicate(value); }
	Vector3 operator*(const float& value) const { return data * XMVectorReplicate(value); }
	Vector3 operator/(const float& value) const { return data / XMVectorReplicate(value); }

	void operator+=(const float& value) { data += XMVectorReplicate(value); }
	void operator-=(const float& value) { data -= XMVectorReplicate(value); }
	void operator*=(const float& value) { data *= XMVectorReplicate(value); }
	void operator/=(const float& value) { data /= XMVectorReplicate(value); }

	bool operator== (const Vector3& value) const { return XMVector3Equal(data, value.data); }

	float operator[](const UINT& index) const 
	{
		switch (index)
		{
		case 0:
			return XMVectorGetX(data);
		case 1:
			return XMVectorGetY(data);
		case 2:
			return XMVectorGetZ(data);		
		}

		return 0.0f;
	}

	float Length() const { return XMVectorGetX(XMVector3Length(data)); }

	Vector3 Normalized() const { return XMVector3Normalize(data); }
	void Normalize() { data = XMVector3Normalize(data); }

	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2) { return XMVector3Cross(vec1.data, vec2.data); }
	static float Dot(const Vector3& vec1, const Vector3& vec2) { return XMVectorGetX(XMVector3Dot(vec1.data, vec2.data)); }
};