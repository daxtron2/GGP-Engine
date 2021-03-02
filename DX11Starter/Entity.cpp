#include "Entity.h"
#include "BufferStructs.h"

Entity::Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material)
{
	material = _material;
	mesh = _mesh;
	float randX = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));
	float randZ = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - -1.0f)));

	transform.SetPosition(randX, 0, randZ);
}

Entity::~Entity()
{
}

std::shared_ptr<Mesh> Entity::GetMesh()
{
	return mesh;
}

Transform* Entity::GetTransform()
{
	return &transform;
}
void Entity::Update(float deltaTime, float totalTime) 
{
	XMFLOAT3 pos = transform.GetPosition();
	transform.SetPosition(pos.x, sinf(totalTime), pos.z);
	transform.Rotate(0, 0, 1 * deltaTime);
	transform.SetScale(sinf(totalTime) + 1.5f, sinf(totalTime) + 1.5f, 1);
}


void Entity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer, Camera* camera)
{
	VertexShaderExternalData vsData{};
	vsData.colorTint = material->GetColorTint();
	vsData.worldMatrix = transform.GetWorldMatrix();
	vsData.projectionMatrix = camera->GetProjectionMatrix();
	vsData.viewMatrix = camera->GetViewMatrix();

	context->VSSetShader(material->GetVertexShader().Get(), 0, 0);
	context->PSSetShader(material->GetPixelShader().Get(), 0, 0);

	D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
	context->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
	context->Unmap(vsConstantBuffer.Get(), 0);

	context->VSSetConstantBuffers
	(0,// Which slot (register) to bind the buffer to?
		1,// How many are we activating?  Can do multiple at once
		vsConstantBuffer.GetAddressOf());// Array of buffers (or the address of one)

	// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		//  - for this demo, this step *could* simply be done once during Init(),
		//    but I'm doing it here because it's often done multiple times per frame
		//    in a larger application/game
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(mesh->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);


	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}
