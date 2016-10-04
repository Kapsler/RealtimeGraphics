#pragma once

#include <d3d11.h>
#include "DDSTextureLoader.h"
using namespace DirectX;

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11Resource* GetTexture();
	ID3D11ShaderResourceView* GetTextureView();

private:
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;
};