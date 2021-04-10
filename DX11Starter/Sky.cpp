#include "Sky.h"

Sky::Sky(std::shared_ptr<Mesh> skyMesh, 
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState,
	std::shared_ptr<SimplePixelShader> pixelShader,
	std::shared_ptr<SimpleVertexShader> vertexShader,
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyTexture,
	Microsoft::WRL::ComPtr<ID3D11Device> device)
{
	skyboxMesh = skyMesh;
	samplerOptions = samplerState;
	skyPixelShader = pixelShader;
	skyVertexShader = vertexShader;
	cubeMapTexture = skyTexture;

	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	device->CreateRasterizerState(&rasterizerDesc, &rasterizerOptions);

	D3D11_DEPTH_STENCIL_DESC stencilDesc = {};
	stencilDesc.DepthEnable = true;
	stencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&stencilDesc, &depthStencilOptions);
}

void Sky::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera)
{
	context->RSSetState(rasterizerOptions.Get());
	context->OMSetDepthStencilState(depthStencilOptions.Get(), 0);

	skyVertexShader->SetShader();
	skyVertexShader->SetMatrix4x4("view", camera->GetViewMatrix());
	skyVertexShader->SetMatrix4x4("projection", camera->GetProjectionMatrix());

	skyPixelShader->SetShader();
	skyPixelShader->SetSamplerState("samplerOptions", samplerOptions.Get());
	skyPixelShader->SetShaderResourceView("cubeMap", cubeMapTexture.Get());

	skyVertexShader->CopyAllBufferData();
	skyPixelShader->CopyAllBufferData();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, skyboxMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(skyboxMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(skyboxMesh->GetIndexCount(), 0, 0);

	context->RSSetState(nullptr);
	context->OMSetDepthStencilState(nullptr, 0);
}
