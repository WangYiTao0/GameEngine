#include "Model.h"
#include "imgui/imgui.h"
#include "ModelException.h"
#include "DirectXMathHelper.h"
#include "LightCommon.h"

namespace dx = DirectX;

class ModelWindow // pImpl idiom, only defined in this .cpp
{
public:
	void Show(Graphics& gfx, const char* windowName, const Node& root) noexcept
	{
		// window name defaults to "Model"
		windowName = windowName ? windowName : "Model";
		// need an ints to track node indices and selected node
		int nodeIndexTracker = 0;
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree(pSelectedNode);

			ImGui::NextColumn();
			if (pSelectedNode != nullptr)
			{
				const auto id = pSelectedNode->GetId();
				auto i = transforms.find(id);
				if (i == transforms.end())
				{
					const auto& applied = pSelectedNode->GetAppliedTransform();
					const auto angles = DXMH::ExtractEulerAngles(applied);
					const auto translation = DXMH::ExtractTranslation(applied);
					TransformParameters tp;
					tp.roll = angles.z;
					tp.pitch = angles.x;
					tp.yaw = angles.y;
					tp.x = translation.x;
					tp.y = translation.y;
					tp.z = translation.z;
					auto pMatConst = pSelectedNode->GetMaterialConstants();
					auto buf = pMatConst != nullptr ? std::optional<Dcb::Buffer>{ *pMatConst } : std::optional<Dcb::Buffer>{};
					std::tie(i, std::ignore) = transforms.insert({ id,{ tp,false,std::move(buf),false } });
				}
				// link imgui ctrl to our cached transform params
				{
					auto& transform = i->second.tranformParams;
					// dirty check
					auto& dirty = i->second.transformParamsDirty;
					const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
					// widgets
					ImGui::Text("Orientation");
					dcheck(ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f));
					dcheck(ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f));
					dcheck(ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f));
					ImGui::Text("Position");
					dcheck(ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f));
					dcheck(ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f));
					dcheck(ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f));
				}
				// link imgui ctrl to our cached material params
				if (i->second.materialCbuf)
				{
					auto& mat = *i->second.materialCbuf;
					// dirty check
					auto& dirty = i->second.materialCbufDirty;
					const auto dcheck = [&dirty](bool changed) {dirty = dirty || changed; };
					// widgets
					ImGui::Text("Material");
					if (auto v = mat["normalMapEnabled"]; v.Exists())
					{
						dcheck(ImGui::Checkbox("Norm Map", &v));
					}
					if (auto v = mat["specularMapEnabled"]; v.Exists())
					{
						dcheck(ImGui::Checkbox("Spec Map", &v));
					}
					if (auto v = mat["hasGlossMap"]; v.Exists())
					{
						dcheck(ImGui::Checkbox("Gloss Map", &v));
					}
					if (auto v = mat["materialColor"]; v.Exists())
					{
						dcheck(ImGui::ColorPicker3("Diff Color", reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v))));
					}
					if (auto v = mat["specularPower"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat("Spec Power", &v, 0.0f, 100.0f, "%.1f", 1.5f));
					}
					if (auto v = mat["specularColor"]; v.Exists())
					{
						dcheck(ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&static_cast<dx::XMFLOAT3&>(v))));
					}
					if (auto v = mat["specularMapWeight"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat("Spec Weight", &v, 0.0f, 4.0f));
					}
					if (auto v = mat["specularIntensity"]; v.Exists())
					{
						dcheck(ImGui::SliderFloat("Spec Intens", &v, 0.0f, 1.0f));
					}
				}
			}
		}
		ImGui::End();
	}
	void ApplyParameters() noxnd
	{
		if (TransformDirty())
		{
			pSelectedNode->SetAppliedTransform(GetTransform());
			ResetTransformDirty();
		}
		if (MaterialDirty())
		{
			pSelectedNode->SetMaterialConstants(GetMaterial());
			ResetMaterialDirty();
		}
	}
private:
	dx::XMMATRIX GetTransform() const noxnd
	{
		assert(pSelectedNode != nullptr);
		const auto& transform = transforms.at(pSelectedNode->GetId()).tranformParams;
		return
			dx::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			dx::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}
	const Dcb::Buffer& GetMaterial() const noxnd
	{
		assert(pSelectedNode != nullptr);
		const auto& mat = transforms.at(pSelectedNode->GetId()).materialCbuf;
		assert(mat);
		return *mat;
	}
	bool TransformDirty() const noxnd
	{
		return pSelectedNode && transforms.at(pSelectedNode->GetId()).transformParamsDirty;
	}
	void ResetTransformDirty() noxnd
	{
		transforms.at(pSelectedNode->GetId()).transformParamsDirty = false;
	}
	bool MaterialDirty() const noxnd
	{
		return pSelectedNode && transforms.at(pSelectedNode->GetId()).materialCbufDirty;
	}
	void ResetMaterialDirty() noxnd
	{
		transforms.at(pSelectedNode->GetId()).materialCbufDirty = false;
	}
	bool IsDirty() const noxnd
	{
		return TransformDirty() || MaterialDirty();
	}
private:
	Node* pSelectedNode;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
	struct NodeData
	{
		TransformParameters tranformParams;
		bool transformParamsDirty;
		std::optional<Dcb::Buffer> materialCbuf;
		bool materialCbufDirty;
	};
	std::unordered_map<int, NodeData> transforms;
};

Model::Model(Graphics& gfx, const std::string& pathString, float scale)
	:
	pWindow(std::make_unique<ModelWindow>())
{
	//create assimp import 
	//read files
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(pathString.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	if (pScene == nullptr)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}

	//load all mesh  store in 	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		//analyze
		//load all the mesh into zhe meshPtr vector
		meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials, pathString, scale));
	}
	int nextId = 0;
	//recursion fuction
	pRoot = ParseNode(nextId, *pScene->mRootNode);
}
void Model::Draw(Graphics& gfx) const noxnd
{
	//if (auto node = pWindow->GetSelectedNode())
	//{
	//	node->SetAppliedTransform(pWindow->GetTransform());
	//}
	pWindow->ApplyParameters();
	pRoot->Draw(gfx, dx::XMMatrixIdentity());
}
void Model::ShowWindow(Graphics& gfx, const char* windowName) noexcept
{
	pWindow->Show(gfx, windowName, *pRoot);
}
void Model::SetRootTransform(DirectX::FXMMATRIX tf) noexcept
{
	pRoot->SetAppliedTransform(tf);
}
Model::~Model() noexcept
{
}
//analize
// take reference node struct from assimp
std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx,
	const aiMesh& mesh, 
	const aiMaterial* const* pMaterials, 
	const std::filesystem::path& path,
	float scale)
{
	using namespace std::string_literals;
	using Dvtx::VertexLayout;
	using namespace Bind;

	const auto rootPath = path.parent_path().string() + "\\";

	std::vector<std::shared_ptr<Bindable>> bindablePtrs;

	bool hasSpecularMap = false;
	bool hasAlphaGloss = false;
	bool hasAlphaDiffuse = false;
	bool hasNormalMap = false;
	bool hasDiffuseMap = false;
	float shininess = 2.0f;
	dx::XMFLOAT4 specularColor = { 0.18f,0.18f,0.18f,1.0f };
	dx::XMFLOAT4 diffuseColor = { 0.45f,0.45f,0.85f,1.0f };
	if (mesh.mMaterialIndex >= 0)
	{
		auto& material = *pMaterials[mesh.mMaterialIndex];

		aiString texFileName;

		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str());
			hasAlphaDiffuse = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasDiffuseMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));
		}

		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 1);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasSpecularMap = true;
		}
		else
		{
			material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specularColor));
		}
		if (!hasAlphaGloss)
		{
			material.Get(AI_MATKEY_SHININESS, shininess);
		}

		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
		{
			auto tex = Texture::Resolve(gfx, rootPath + texFileName.C_Str(), 2);
			hasAlphaGloss = tex->HasAlpha();
			bindablePtrs.push_back(std::move(tex));
			hasNormalMap = true;
		}

		if (hasDiffuseMap || hasSpecularMap || hasNormalMap)
		{
			bindablePtrs.push_back(Bind::Sampler::Resolve(gfx,0u,Sampler::SamplerState::SSAnistropicWrap));
		}
	}

	const auto meshTag = path.string() + "%" + mesh.mName.C_Str();

	if (hasDiffuseMap && hasNormalMap && hasSpecularMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSTBN");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx,
			hasAlphaDiffuse ? "PhongPSSpecNormalMask" : "PhongPSSpecNormalMap"
		));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout lay;
		lay.Add<Dcb::Bool>("normalMapEnabled");
		lay.Add<Dcb::Bool>("specularMapEnabled");
		lay.Add<Dcb::Bool>("hasGlossMap");
		lay.Add<Dcb::Float>("specularPower");
		lay.Add<Dcb::Float3>("specularColor");
		lay.Add<Dcb::Float>("specularMapWeight");

		auto buf = Dcb::Buffer(std::move(lay));
		buf["normalMapEnabled"] = true;
		buf["specularMapEnabled"] = true;
		buf["hasGlossMap"] = hasAlphaGloss;
		buf["specularPower"] = shininess;
		buf["specularColor"] = dx::XMFLOAT3{ 0.75f,0.75f,0.75f };
		buf["specularMapWeight"] = 0.671f;

		bindablePtrs.push_back(std::make_shared<CachingPixelConstantBufferEX>(gfx, buf, 2u));
	}
	else if (hasDiffuseMap && hasNormalMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mBitangents[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSTBN");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSNormalMap"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout layout;
		layout.Add<Dcb::Float>("specularIntensity");
		layout.Add<Dcb::Float>("specularPower");
		layout.Add<Dcb::Bool>("normalMapEnabled");

		auto cbuf = Dcb::Buffer(std::move(layout));
		cbuf["specularIntensity"] = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		cbuf["specularPower"] = shininess;
		cbuf["normalMapEnabled"] = true;

		bindablePtrs.push_back(std::make_shared<CachingPixelConstantBufferEX>(gfx, cbuf, 2u));
	}
	else if (hasDiffuseMap && !hasNormalMap && hasSpecularMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVS");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSSpec"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout lay;
		lay.Add<Dcb::Float>("specularPower");
		lay.Add<Dcb::Bool>("hasGloss");
		lay.Add<Dcb::Float>("specularMapWeight");

		auto buf = Dcb::Buffer(std::move(lay));
		buf["specularPower"] = shininess;
		buf["hasGloss"] = hasAlphaGloss;
		buf["specularMapWeight"] = 1.0f;

		bindablePtrs.push_back(std::make_unique<Bind::CachingPixelConstantBufferEX>(gfx, buf, 2u));
	}
	else if (hasDiffuseMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<dx::XMFLOAT2*>(&mesh.mTextureCoords[0][i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVS");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPS"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout lay;
		lay.Add<Dcb::Float>("specularIntensity");
		lay.Add<Dcb::Float>("specularPower");

		auto buf = Dcb::Buffer(std::move(lay));
		buf["specularIntensity"] = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
		buf["specularPower"] = shininess;
		buf["specularMapWeight"] = 1.0f;

		bindablePtrs.push_back(std::make_unique<Bind::CachingPixelConstantBufferEX>(gfx, buf, 2u));
	}
	else if (!hasDiffuseMap && !hasNormalMap && !hasSpecularMap)
	{
		Dvtx::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		));

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				dx::XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		bindablePtrs.push_back(VertexBuffer::Resolve(gfx, meshTag, vbuf));

		bindablePtrs.push_back(IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = VertexShader::Resolve(gfx, "PhongVSNotex");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(PixelShader::Resolve(gfx, "PhongPSNotex"));

		bindablePtrs.push_back(InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

		Dcb::RawLayout lay;
		lay.Add<Dcb::Float4>("materialColor");
		lay.Add<Dcb::Float4>("specularColor");
		lay.Add<Dcb::Float>("specularPower");

		auto buf = Dcb::Buffer(std::move(lay));
		buf["specularPower"] = shininess;
		buf["specularColor"] = specularColor;
		buf["materialColor"] = diffuseColor;

		bindablePtrs.push_back(std::make_unique<Bind::CachingPixelConstantBufferEX>(gfx, buf, 2u));
	}
	else
	{
		throw std::runtime_error("terrible combination of textures in material smh");
	}

	auto ras = hasAlphaDiffuse ? Rasterizer::Mode::RSNoCull : Rasterizer::Mode::RSCullBack;

	// anything with alpha diffuse is 2-sided IN SPONZA, need a better way
	// of signalling 2-sidedness to be more general in the future
	bindablePtrs.push_back(Rasterizer::Resolve(gfx, ras));

	bindablePtrs.push_back(Blender::Resolve(gfx, false));

	bindablePtrs.push_back(std::make_shared<DepthStencil>(gfx, DepthStencil::Mode::DSSOff));

	return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
}

void Model::AddShader(Graphics& gfx, std::string VS_Name, std::string PS_Name)
{
	auto pvs = Bind::VertexShader::Resolve(gfx, VS_Name);
	auto pvsbc = pvs->GetBytecode();
	bindablePtrs.push_back(std::move(pvs));

	bindablePtrs.push_back((Bind::PixelShader::Resolve(gfx, PS_Name)));

	bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

	meshPtrs.push_back(std::make_unique<Mesh>(gfx, std::move(bindablePtrs)));
}

void Model::AddPBRTexture(Graphics& gfx, std::vector<std::string>& filePaths)
{
	bindablePtrs.push_back((Bind::Texture::Resolve(gfx, filePaths[0])));
	bindablePtrs.push_back((Bind::Texture::Resolve(gfx, filePaths[1], 1u)));
	bindablePtrs.push_back((Bind::Texture::Resolve(gfx, filePaths[2], 2u)));
	bindablePtrs.push_back((Bind::Texture::Resolve(gfx, filePaths[3], 3u)));

	meshPtrs.push_back(std::make_unique<Mesh>(gfx, std::move(bindablePtrs)));
}
std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node)noexcept
{
	namespace dx = DirectX;

	// node contain transformation
	const auto transform = dx::XMMatrixTranspose(dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
	));
	//create dum point to mesh
	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
	}

	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
	}

	return pNode;
}

