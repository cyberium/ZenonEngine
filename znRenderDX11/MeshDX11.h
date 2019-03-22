#pragma once

class ConstantBuffer;

class MeshDX11 : public IMesh
{
	typedef std::map<BufferBinding, std::shared_ptr<IBuffer>> BufferMap;
public:
	MeshDX11(ID3D11Device2 * pDevice);
	virtual ~MeshDX11();

	virtual SceneNodeTypes GetType() const override;
	virtual void SetType(SceneNodeTypes type) override;

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer) override;
	virtual void SetVertexBuffer(std::shared_ptr<IBuffer> buffer) override;
	virtual void SetIndexBuffer(std::shared_ptr<IBuffer> buffer) override;

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(std::shared_ptr<const Material> material);
	virtual std::shared_ptr<const Material> GetMaterial() const;

	virtual bool Render(RenderEventArgs& renderArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

	virtual bool Accept(IVisitor& visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

private:
	SceneNodeTypes m_MeshType;
	BufferMap m_VertexBuffers;
	std::shared_ptr<IBuffer> m_VertexBuffer;
	std::shared_ptr<IBuffer> m_pIndexBuffer;
	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
	std::shared_ptr<const Material> m_pMaterial;

	// DirectX
	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;
};