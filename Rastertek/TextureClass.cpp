#include "TextureClass.h"

TextureClass::TextureClass()
{
	texture = nullptr;
	textureView = nullptr;
}

TextureClass::TextureClass(const TextureClass&)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	result = CreateDDSTextureFromFile(device, filename, &texture, &textureView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	if(textureView)
	{
		textureView->Release();
		textureView = nullptr;
	}

	if (texture)
	{
		texture->Release();
		texture = nullptr;
	}
}

ID3D11Resource* TextureClass::GetTexture()
{
	return texture;
}

ID3D11ShaderResourceView* TextureClass::GetTextureView()
{
	return textureView;
}
