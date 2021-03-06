#ifndef TH_GRAPHICS_OPENGL45_H
#define TH_GRAPHICS_OPENGL45_H

#include "../core/graphics.h"
#ifdef TH_MICROSOFT
#include <Windows.h>
#endif
#ifdef TH_HAS_OPENGL
#ifdef TH_HAS_GLEW
#include <GL/glew.h>
#define TH_HAS_GL
#ifndef APIENTRY
#define APIENTRY
#endif

namespace Tomahawk
{
	namespace Graphics
	{
		namespace OGL
		{
			class OGLDevice;

			struct OGLFrameBuffer
			{
				GLenum Format[8] = { GL_COLOR_ATTACHMENT0, GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_NONE };
				GLuint Texture[8] = { GL_NONE };
				GLuint Buffer = GL_NONE;
				GLuint Count = 1;
				bool Backbuffer = false;

				OGLFrameBuffer(GLuint Targets);
				void Release();
			};

			class OGLDepthStencilState final : public DepthStencilState
			{
				friend OGLDevice;

			public:
				OGLDepthStencilState(const Desc& I);
				virtual ~OGLDepthStencilState() override;
				void* GetResource() override;
			};

			class OGLRasterizerState final : public RasterizerState
			{
				friend OGLDevice;

			public:
				OGLRasterizerState(const Desc& I);
				virtual ~OGLRasterizerState() override;
				void* GetResource() override;
			};

			class OGLBlendState final : public BlendState
			{
				friend OGLDevice;

			public:
				OGLBlendState(const Desc& I);
				virtual ~OGLBlendState() override;
				void* GetResource() override;
			};

			class OGLSamplerState final : public SamplerState
			{
				friend OGLDevice;

			private:
				GLenum Resource = GL_NONE;

			public:
				OGLSamplerState(const Desc& I);
				virtual ~OGLSamplerState() override;
				void* GetResource() override;
			};

			class OGLInputLayout final : public InputLayout
			{
				friend OGLDevice;

			public:
				std::vector<std::function<void(uint64_t)>> VertexLayout;

			public:
				OGLInputLayout(const Desc& I);
				virtual ~OGLInputLayout() override;
				void* GetResource() override;
			};

			class OGLShader final : public Shader
			{
				friend OGLDevice;

			private:
				bool Compiled;

			public:
				std::unordered_map<unsigned int, GLuint> Programs;
				GLuint VertexShader = GL_NONE;
				GLuint PixelShader = GL_NONE;
				GLuint GeometryShader = GL_NONE;
				GLuint DomainShader = GL_NONE;
				GLuint HullShader = GL_NONE;
				GLuint ComputeShader = GL_NONE;
				GLuint ConstantBuffer = GL_NONE;
				size_t ConstantSize = 0;

			public:
				OGLShader(const Desc& I);
				virtual ~OGLShader() override;
				bool IsValid() override;
			};

			class OGLElementBuffer final : public ElementBuffer
			{
				friend OGLDevice;

			private:
				InputLayout* Layout = nullptr;
				GLuint Resource = GL_NONE;
				GLenum Flags = GL_NONE;

			public:
				OGLElementBuffer(const Desc& I);
				virtual ~OGLElementBuffer() override;
				void* GetResource() override;
			};

			class OGLMeshBuffer final : public MeshBuffer
			{
				friend OGLDevice;

			public:
				OGLMeshBuffer(const Desc& I);
				Compute::Vertex* GetElements(GraphicsDevice* Device) override;
			};

			class OGLSkinMeshBuffer final : public SkinMeshBuffer
			{
				friend OGLDevice;

			public:
				OGLSkinMeshBuffer(const Desc& I);
				Compute::SkinVertex* GetElements(GraphicsDevice* Device) override;
			};

			class OGLInstanceBuffer final : public InstanceBuffer
			{
				friend OGLDevice;

			public:
				OGLInstanceBuffer(const Desc& I);
				virtual ~OGLInstanceBuffer() override;
			};

			class OGLTexture2D final : public Texture2D
			{
				friend OGLDevice;

			public:
				GLenum Format = GL_NONE;
				GLuint Resource = GL_NONE;

			public:
				OGLTexture2D();
				OGLTexture2D(const Desc& I);
				virtual ~OGLTexture2D() override;
				void* GetResource() override;
			};

			class OGLTexture3D final : public Texture3D
			{
				friend OGLDevice;

			public:
				GLenum Format = GL_NONE;
				GLuint Resource = GL_NONE;

			public:
				OGLTexture3D();
				virtual ~OGLTexture3D() override;
				void* GetResource() override;
			};

			class OGLTextureCube final : public TextureCube
			{
				friend OGLDevice;

			public:
				GLenum Format = GL_NONE;
				GLuint Resource = GL_NONE;

			public:
				OGLTextureCube();
				OGLTextureCube(const Desc& I);
				virtual ~OGLTextureCube() override;
				void* GetResource() override;
			};

			class OGLDepthBuffer final : public DepthBuffer
			{
				friend OGLDevice;

			public:
				GLuint FrameBuffer = GL_NONE;
				GLuint DepthTexture = GL_NONE;

			public:
				OGLDepthBuffer(const Desc& I);
				virtual ~OGLDepthBuffer() override;
				void* GetResource() override;
				uint32_t GetWidth() override;
				uint32_t GetHeight() override;
			};

			class OGLRenderTarget2D final : public RenderTarget2D
			{
				friend OGLDevice;

			public:
				OGLFrameBuffer FrameBuffer;
				GLuint DepthTexture = GL_NONE;

			public:
				OGLRenderTarget2D(const Desc& I);
				virtual ~OGLRenderTarget2D() override;
				void* GetTargetBuffer() override;
				void* GetDepthBuffer() override;
				uint32_t GetWidth() override;
				uint32_t GetHeight() override;
			};

			class OGLMultiRenderTarget2D final : public MultiRenderTarget2D
			{
				friend OGLDevice;

			public:
				OGLFrameBuffer FrameBuffer;
				GLuint DepthTexture = GL_NONE;

			public:
				OGLMultiRenderTarget2D(const Desc& I);
				virtual ~OGLMultiRenderTarget2D() override;
				void* GetTargetBuffer() override;
				void* GetDepthBuffer() override;
				uint32_t GetWidth() override;
				uint32_t GetHeight() override;
			};

			class OGLRenderTargetCube final : public RenderTargetCube
			{
				friend OGLDevice;

			public:
				OGLFrameBuffer FrameBuffer;
				GLuint DepthTexture = GL_NONE;

			public:
				OGLRenderTargetCube(const Desc& I);
				virtual ~OGLRenderTargetCube() override;
				void* GetTargetBuffer() override;
				void* GetDepthBuffer() override;
				uint32_t GetWidth() override;
				uint32_t GetHeight() override;
			};

			class OGLMultiRenderTargetCube final : public MultiRenderTargetCube
			{
				friend OGLDevice;

			public:
				OGLFrameBuffer FrameBuffer;
				GLuint DepthTexture = GL_NONE;

			public:
				OGLMultiRenderTargetCube(const Desc& I);
				virtual ~OGLMultiRenderTargetCube() override;
				void* GetTargetBuffer() override;
				void* GetDepthBuffer() override;
				uint32_t GetWidth() override;
				uint32_t GetHeight() override;
			};

			class OGLCubemap final : public Cubemap
			{
				friend OGLDevice;

			public:
				GLuint Buffers[2] = { GL_NONE };
				GLuint Resource = GL_NONE;

			public:
				OGLCubemap(const Desc& I);
				virtual ~OGLCubemap() override;
			};

			class OGLQuery final : public Query
			{
			public:
				GLuint Async = GL_NONE;
				bool Predicate = false;

			public:
				OGLQuery();
				virtual ~OGLQuery() override;
				void* GetResource() override;
			};

			class OGLDevice final : public GraphicsDevice
			{
			private:
				struct ConstantBuffer
				{
					Compute::Matrix4x4 WorldViewProjection;
					Compute::Vector4 Padding;
				};

				struct
				{
					GLuint VertexShader;
					GLuint PixelShader;
					GLuint Program;
					GLuint VertexBuffer;
				} DirectRenderer;

			private:
				const char* ShaderVersion;
				OGLInputLayout* Layout;
				PrimitiveTopology Primitive;
				GLenum IndexType;

			public:
				GLuint ConstantBuffer[3];
				size_t ConstantSize[3];
				Activity* Window;
				void* Context;

			public:
				OGLDevice(const Desc& I);
				virtual ~OGLDevice() override;
				void SetConstantBuffers() override;
				void SetShaderModel(ShaderModel Model) override;
				void SetBlendState(BlendState* State) override;
				void SetRasterizerState(RasterizerState* State) override;
				void SetDepthStencilState(DepthStencilState* State) override;
				void SetInputLayout(InputLayout* State) override;
				void SetShader(Shader* Resource, unsigned int Type) override;
				void SetSamplerState(SamplerState* State, unsigned int Slot, unsigned int Type) override;
				void SetBuffer(Shader* Resource, unsigned int Slot, unsigned int Type) override;
				void SetBuffer(InstanceBuffer* Resource, unsigned int Slot, unsigned int Type) override;
				void SetStructureBuffer(ElementBuffer* Resource, unsigned int Slot, unsigned int Type) override;
				void SetIndexBuffer(ElementBuffer* Resource, Format FormatMode) override;
				void SetVertexBuffer(ElementBuffer* Resource, unsigned int Slot) override;
				void SetTexture2D(Texture2D* Resource, unsigned int Slot, unsigned int Type) override;
				void SetTexture3D(Texture3D* Resource, unsigned int Slot, unsigned int Type) override;
				void SetTextureCube(TextureCube* Resource, unsigned int Slot, unsigned int Type) override;
				void SetWriteable(ElementBuffer** Resource, unsigned int Count, unsigned int Slot, bool Computable) override;
				void SetWriteable(Texture2D** Resource, unsigned int Count, unsigned int Slot, bool Computable) override;
				void SetWriteable(Texture3D** Resource, unsigned int Count, unsigned int Slot, bool Computable) override;
				void SetWriteable(TextureCube** Resource, unsigned int Count, unsigned int Slot, bool Computable) override;
				void SetTarget(float R, float G, float B) override;
				void SetTarget() override;
				void SetTarget(DepthBuffer* Resource) override;
				void SetTarget(Graphics::RenderTarget* Resource, unsigned int Target, float R, float G, float B) override;
				void SetTarget(Graphics::RenderTarget* Resource, unsigned int Target) override;
				void SetTarget(Graphics::RenderTarget* Resource, float R, float G, float B) override;
				void SetTarget(Graphics::RenderTarget* Resource) override;
				void SetTargetMap(Graphics::RenderTarget* Resource, bool Enabled[8]) override;
				void SetTargetRect(unsigned int Width, unsigned int Height) override;
				void SetViewports(unsigned int Count, Viewport* Viewports) override;
				void SetScissorRects(unsigned int Count, Compute::Rectangle* Value) override;
				void SetPrimitiveTopology(PrimitiveTopology Topology) override;
				void FlushTexture2D(unsigned int Slot, unsigned int Count, unsigned int Type) override;
				void FlushTexture3D(unsigned int Slot, unsigned int Count, unsigned int Type) override;
				void FlushTextureCube(unsigned int Slot, unsigned int Count, unsigned int Type) override;
				void FlushState() override;
				bool Map(ElementBuffer* Resource, ResourceMap Mode, MappedSubresource* Map) override;
				bool Unmap(ElementBuffer* Resource, MappedSubresource* Map) override;
				bool UpdateBuffer(ElementBuffer* Resource, void* Data, uint64_t Size) override;
				bool UpdateBuffer(Shader* Resource, const void* Data) override;
				bool UpdateBuffer(MeshBuffer* Resource, Compute::Vertex* Data) override;
				bool UpdateBuffer(SkinMeshBuffer* Resource, Compute::SkinVertex* Data) override;
				bool UpdateBuffer(InstanceBuffer* Resource) override;
				bool UpdateBuffer(RenderBufferType Buffer) override;
				bool UpdateBufferSize(Shader* Resource, size_t Size) override;
				bool UpdateBufferSize(InstanceBuffer* Resource, uint64_t Size) override;
				void ClearBuffer(InstanceBuffer* Resource) override;
				void ClearWritable(Texture2D* Resource) override;
				void ClearWritable(Texture2D* Resource, float R, float G, float B) override;
				void ClearWritable(Texture3D* Resource) override;
				void ClearWritable(Texture3D* Resource, float R, float G, float B) override;
				void ClearWritable(TextureCube* Resource) override;
				void ClearWritable(TextureCube* Resource, float R, float G, float B) override;
				void Clear(float R, float G, float B) override;
				void Clear(Graphics::RenderTarget* Resource, unsigned int Target, float R, float G, float B) override;
				void ClearDepth() override;
				void ClearDepth(DepthBuffer* Resource) override;
				void ClearDepth(Graphics::RenderTarget* Resource) override;
				void DrawIndexed(unsigned int Count, unsigned int IndexLocation, unsigned int BaseLocation) override;
				void DrawIndexed(MeshBuffer* Resource) override;
				void DrawIndexed(SkinMeshBuffer* Resource) override;
				void Draw(unsigned int Count, unsigned int Location) override;
				void Dispatch(unsigned int GroupX, unsigned int GroupY, unsigned int GroupZ) override;
				bool CopyTexture2D(Texture2D* Resource, Texture2D** Result) override;
				bool CopyTexture2D(Graphics::RenderTarget* Resource, unsigned int Target, Texture2D** Result) override;
				bool CopyTexture2D(RenderTargetCube* Resource, unsigned int Face, Texture2D** Result) override;
				bool CopyTexture2D(MultiRenderTargetCube* Resource, unsigned int Cube, unsigned int Face, Texture2D** Result) override;
				bool CopyTextureCube(RenderTargetCube* Resource, TextureCube** Result) override;
				bool CopyTextureCube(MultiRenderTargetCube* Resource, unsigned int Cube, TextureCube** Result) override;
				bool CopyTarget(Graphics::RenderTarget* From, unsigned int FromTarget, Graphics::RenderTarget* To, unsigned ToTarget) override;
				bool CubemapBegin(Cubemap* Resource) override;
				bool CubemapFace(Cubemap* Resource, unsigned int Target, unsigned int Face) override;
				bool CubemapEnd(Cubemap* Resource, TextureCube* Result) override;
				bool CopyBackBuffer(Texture2D** Result) override;
				bool CopyBackBufferMSAA(Texture2D** Result) override;
				bool CopyBackBufferNoAA(Texture2D** Result) override;
				void GetViewports(unsigned int* Count, Viewport* Out) override;
				void GetScissorRects(unsigned int* Count, Compute::Rectangle* Out) override;
				bool ResizeBuffers(unsigned int Width, unsigned int Height) override;
				bool GenerateTexture(Texture2D* Resource) override;
				bool GenerateTexture(Texture3D* Resource) override;
				bool GenerateTexture(TextureCube* Resource) override;
				bool GetQueryData(Query* Resource, uint64_t* Result, bool Flush) override;
				bool GetQueryData(Query* Resource, bool* Result, bool Flush) override;
				void QueryBegin(Query* Resource) override;
				void QueryEnd(Query* Resource) override;
				void GenerateMips(Texture2D* Resource) override;
				void GenerateMips(Texture3D* Resource) override;
				void GenerateMips(TextureCube* Resource) override;
				bool Begin() override;
				void Transform(const Compute::Matrix4x4& Transform) override;
				void Topology(PrimitiveTopology Topology) override;
				void Emit() override;
				void Texture(Texture2D* In) override;
				void Color(float X, float Y, float Z, float W) override;
				void Intensity(float Intensity) override;
				void TexCoord(float X, float Y) override;
				void TexCoordOffset(float X, float Y) override;
				void Position(float X, float Y, float Z) override;
				bool End() override;
				bool Submit() override;
				DepthStencilState* CreateDepthStencilState(const DepthStencilState::Desc& I) override;
				BlendState* CreateBlendState(const BlendState::Desc& I) override;
				RasterizerState* CreateRasterizerState(const RasterizerState::Desc& I) override;
				SamplerState* CreateSamplerState(const SamplerState::Desc& I) override;
				InputLayout* CreateInputLayout(const InputLayout::Desc& I) override;
				Shader* CreateShader(const Shader::Desc& I) override;
				ElementBuffer* CreateElementBuffer(const ElementBuffer::Desc& I) override;
				MeshBuffer* CreateMeshBuffer(const MeshBuffer::Desc& I) override;
				SkinMeshBuffer* CreateSkinMeshBuffer(const SkinMeshBuffer::Desc& I) override;
				InstanceBuffer* CreateInstanceBuffer(const InstanceBuffer::Desc& I) override;
				Texture2D* CreateTexture2D() override;
				Texture2D* CreateTexture2D(const Texture2D::Desc& I) override;
				Texture3D* CreateTexture3D() override;
				Texture3D* CreateTexture3D(const Texture3D::Desc& I) override;
				TextureCube* CreateTextureCube() override;
				TextureCube* CreateTextureCube(const TextureCube::Desc& I) override;
				TextureCube* CreateTextureCube(Texture2D* Resource[6]) override;
				TextureCube* CreateTextureCube(Texture2D* Resource) override;
				DepthBuffer* CreateDepthBuffer(const DepthBuffer::Desc& I) override;
				RenderTarget2D* CreateRenderTarget2D(const RenderTarget2D::Desc& I) override;
				MultiRenderTarget2D* CreateMultiRenderTarget2D(const MultiRenderTarget2D::Desc& I) override;
				RenderTargetCube* CreateRenderTargetCube(const RenderTargetCube::Desc& I) override;
				MultiRenderTargetCube* CreateMultiRenderTargetCube(const MultiRenderTargetCube::Desc& I) override;
				Cubemap* CreateCubemap(const Cubemap::Desc& I) override;
				Query* CreateQuery(const Query::Desc& I) override;
				PrimitiveTopology GetPrimitiveTopology() override;
				ShaderModel GetSupportedShaderModel() override;
				void* GetDevice() override;
				void* GetContext() override;
				bool IsValid() override;
				const char* GetShaderVersion();
				void CopyConstantBuffer(GLuint Buffer, void* Data, size_t Size);
				int CreateConstantBuffer(GLuint* Buffer, size_t Size);
				bool CreateDirectBuffer(uint64_t Size);
				std::string CompileState(GLuint Handle);

			protected:
				TextureCube* CreateTextureCubeInternal(void* Resources[6]) override;

			public:
				static GLenum GetFormat(Format Value);
				static GLenum GetTextureAddress(TextureAddress Value);
				static GLenum GetComparison(Comparison Value);
				static GLenum GetPixelFilter(PixelFilter Value, bool Mag);
				static GLenum GetBlendOperation(BlendOperation Value);
				static GLenum GetBlend(Blend Value);
				static GLenum GetStencilOperation(StencilOperation Value);
				static GLenum GetPrimitiveTopology(PrimitiveTopology Value);
				static GLenum GetPrimitiveTopologyDraw(PrimitiveTopology Value);
				static GLenum GetResourceBind(ResourceBind Value);
				static GLenum GetResourceMap(ResourceMap Value);
				static void APIENTRY DebugMessage(GLenum Source, GLenum Type, GLuint Id, GLenum Severity, GLsizei Length, const GLchar* Message, const void* Data);
			};
		}
	}
}
#endif
#endif
#endif