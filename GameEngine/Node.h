#pragma once
#include "Mesh.h"

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
