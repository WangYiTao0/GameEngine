#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include "Mesh.h"
#include "Node.h"

class Model
{
public:
	Model(Graphics& gfx, const std::string& pathString, float scale = 1.0f, std::vector<std::string> filePaths = { " " });
	void Draw(Graphics& gfx) const noxnd;
	void ShowWindow(Graphics& gfx, const char* windowName = nullptr) noexcept;
	void SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
	~Model() noexcept;

	void AddShader(Graphics& gfx, std::string VS_Name, std::string PS_Name);
	template<typename C>
	void AddConstantBuffer(Graphics& gfx, C& c, int slot)
	{
		C pmc;
		bindablePtrs.push_back((Bind::PixelConstantBuffer<C>::Resolve(gfx, pmc, slot)));
		meshPtrs.push_back(std::make_unique<Mesh>(gfx, std::move(bindablePtrs)));
	}
	void AddPBRTexture(std::vector<std::string>& filePaths);

private:
	std::unique_ptr<Mesh> ParseMesh(Graphics& gfx,
		const aiMesh& mesh, const aiMaterial* const* pMaterials,
		const std::filesystem::path& path, float scale);
	//std::unique_ptr<Mesh> ParseMaterial(Graphics& gfx)
	//analize
	// take reference node struct from assimp
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node)noexcept;

private:
	//Link structor
	std::unique_ptr<Node> pRoot;
	//load all mesh  store in 
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
	std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;
	Dvtx::VertexBuffer vbuf;

	std::vector<std::string> textures;
	std::vector<std::shared_ptr<Bind::Bindable>> DepthPtrs;
};