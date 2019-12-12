#pragma once
#include "Drawable.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include <optional>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "ConditionalNoexcept.h"
#include <type_traits>
#include <filesystem>
#include "imgui/imgui.h"
#include "DirectXMathHelper.h"
#include "StringHelper.h"
#include "LightCommon.h"

struct MeshParamters
{

};

class ModelException : public BaseException
{
public:
	ModelException(int line, const char* file, std::string note) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	const std::string& GetNote() const noexcept;
private:
	std::string note;
};

class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, std::vector<std::shared_ptr<Bind::Bindable>> bindPtrs);
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node
{
	friend class Model;
public:

public:
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in) noxnd;
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;
	void SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept;
	const DirectX::XMFLOAT4X4& GetAppliedTransform() const noexcept;
	int GetId() const noexcept;
	void ShowTree(Node*& pSelectedNode) const noexcept;

private:
	//only model  can add child to node
	void AddChild(std::unique_ptr<Node> pChild) noxnd;

private:
	std::string name;
	int id;
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	//transform from load file
	DirectX::XMFLOAT4X4 transform;
	//transform from user
	DirectX::XMFLOAT4X4 appliedTransform;
};

class Model
{
public:
	Model(Graphics& gfx, const std::string& pathString, float scale = 1.0f);
	void Draw(Graphics& gfx) const noxnd;
	void ShowWindow(Graphics& gfx,const char* windowName = nullptr) noexcept;
	void SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
	~Model() noexcept;

	void AddPixelShader(Graphics& gfx, std::string PS_Name);
	template<typename C>
	void AddConstantBuffer(Graphics& gfx, C& c, int slot)
	{
		C pmc;
		bindablePtrs.push_back((Bind::PixelConstantBuffer<C>::Resolve(gfx, pmc, slot)));
		meshPtrs.push_back(std::make_unique<Mesh>(gfx, std::move(bindablePtrs)));
	}
	void AddPBRTexture(Graphics& gfx, std::vector<std::string>& filePaths);

private:
	std::unique_ptr<Mesh> ParseMesh(Graphics& gfx,
		const aiMesh& mesh, const aiMaterial* const* pMaterials,
		const std::filesystem::path& path, float scale);
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
};

