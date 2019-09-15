#pragma once
#include "Drawable.h"
#include "BindableCommon.h"
#include "Vertex.h"
#include <optional>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelException : public ChiliException
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
	friend class ModelWithOutTexture;
public:
	Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform_in) noxnd;
	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noxnd;
	void SetAppliedTransform(DirectX::FXMMATRIX transform) noexcept;
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
	Model(Graphics& gfx, const std::string fileName);
	void Draw(Graphics& gfx) const noxnd;
	void ShowWindow(const char* windowName = nullptr) noexcept;
	void SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
	~Model() noexcept;
private:
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	//analize
	// take reference node struct from assimp
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node)noexcept;

private:
	//Link structor
	std::unique_ptr<Node> pRoot;
	//load all mesh  store in 
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
};

class ModelWithOutTexture
{
public:
	ModelWithOutTexture(Graphics& gfx, const std::string fileName);
	void Draw(Graphics& gfx) const noxnd;
	void ShowWindow(const char* windowName = nullptr) noexcept;
	~ModelWithOutTexture() noexcept;
private:
	static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
	//analize
	// take reference node struct from assimp
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node)noexcept;

private:
	//Link structor
	std::unique_ptr<Node> pRoot;
	//load all mesh  store in 
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;

};