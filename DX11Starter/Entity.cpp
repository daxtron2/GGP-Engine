#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material)
{
	material = _material;
	mesh = _mesh;
	float randX = -5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.0f - -5.0f)));
	float randZ = -5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.0f - -5.0f)));

	transform.SetPosition(randX, 0, 0);
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
	//XMFLOAT3 pos = transform.GetPosition();
	//transform.SetPosition(pos.x, sinf(totalTime), pos.z);
	//transform.Rotate(0, 0, 1 * deltaTime);
	//transform.SetScale(sinf(totalTime) + 1.5f, sinf(totalTime) + 1.5f, 1);
}


void Entity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera)
{
	std::shared_ptr<SimpleVertexShader> vs = material->GetVertexShader();
	vs->SetFloat4("colorTint", material->GetColorTint());
	vs->SetMatrix4x4("world", transform.GetWorldMatrix());
	vs->SetMatrix4x4("view", camera->GetViewMatrix());
	vs->SetMatrix4x4("projection", camera->GetProjectionMatrix());

	std::shared_ptr<SimplePixelShader> ps = material->GetPixelShader();
	ps->SetFloat("specIntensity", material->GetSpecularIntensity());
	ps->SetSamplerState("samplerState", material->GetSamplerState());
	ps->SetShaderResourceView("diffuseTexture", material->GetTextureSRV());

	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();

	vs->CopyAllBufferData();
	ps->CopyAllBufferData();


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
