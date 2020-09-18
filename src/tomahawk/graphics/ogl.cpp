#include "ogl.h"
#ifdef THAWK_HAS_SDL2
#include <SDL2/SDL_syswm.h>
#undef DirectColor
#endif
#ifdef THAWK_HAS_GL

namespace Tomahawk
{
	namespace Graphics
	{
		namespace OGL
		{
			OGLDepthStencilState::OGLDepthStencilState(const Desc& I) : DepthStencilState(I)
			{
			}
			OGLDepthStencilState::~OGLDepthStencilState()
			{
			}
			void* OGLDepthStencilState::GetResource()
			{
				return (void*)&State;
			}

			OGLRasterizerState::OGLRasterizerState(const Desc& I) : RasterizerState(I)
			{
			}
			OGLRasterizerState::~OGLRasterizerState()
			{
			}
			void* OGLRasterizerState::GetResource()
			{
				return (void*)&State;
			}

			OGLBlendState::OGLBlendState(const Desc& I) : BlendState(I)
			{
			}
			OGLBlendState::~OGLBlendState()
			{
			}
			void* OGLBlendState::GetResource()
			{
				return (void*)&State;
			}

			OGLSamplerState::OGLSamplerState(const Desc& I) : SamplerState(I)
			{
			}
			OGLSamplerState::~OGLSamplerState()
			{
				glDeleteSamplers(1, &Resource);
			}
			void* OGLSamplerState::GetResource()
			{
				return (void*)(intptr_t)Resource;
			}

			OGLShader::OGLShader(const Desc& I) : Shader(I), Compiled(false)
			{
				GLsizei Stride = 0;
				if (I.LayoutSize > 0)
				{
					switch (I.Layout[I.LayoutSize - 1].FormatMode)
					{
						case Format_R32G32B32A32_Typeless:
							Stride += sizeof(int) * 4;
							break;
						case Format_R32G32B32A32_Float:
							Stride += sizeof(float) * 4;
							break;
						case Format_R32G32B32A32_Uint:
							Stride += sizeof(int) * 4;
							break;
						case Format_R32G32B32A32_Sint:
							Stride += sizeof(int) * 4;
							break;
						case Format_R32G32B32_Typeless:
							Stride += sizeof(int) * 3;
							break;
						case Format_R32G32B32_Float:
							Stride += sizeof(float) * 3;
							break;
						case Format_R32G32B32_Uint:
							Stride += sizeof(int) * 3;
							break;
						case Format_R32G32B32_Sint:
							Stride += sizeof(int) * 3;
							break;
						case Format_R16G16B16A16_Typeless:
							Stride += sizeof(int) * 4;
							break;
						case Format_R16G16B16A16_Float:
							Stride += sizeof(float) * 4;
							break;
						case Format_R16G16B16A16_Unorm:
							Stride += sizeof(int) * 4;
							break;
						case Format_R16G16B16A16_Uint:
							Stride += sizeof(int) * 4;
							break;
						case Format_R16G16B16A16_Snorm:
							Stride += sizeof(int) * 4;
							break;
						case Format_R16G16B16A16_Sint:
							Stride += sizeof(int) * 4;
							break;
						case Format_R32G32_Typeless:
							Stride += sizeof(int) * 2;
							break;
						case Format_R32G32_Float:
							Stride += sizeof(float) * 2;
							break;
						case Format_R32G32_Uint:
							Stride += sizeof(int) * 2;
							break;
						case Format_R32G32_Sint:
							Stride += sizeof(int) * 2;
							break;
						case Format_R10G10B10A2_Typeless:
							Stride += sizeof(int) * 4;
							break;
						case Format_R10G10B10A2_Unorm:
							Stride += sizeof(int) * 4;
							break;
						case Format_R10G10B10A2_Uint:
							Stride += sizeof(int) * 4;
							break;
						case Format_R11G11B10_Float:
							Stride += sizeof(float) * 3;
							break;
						case Format_R8G8B8A8_Typeless:
							Stride += sizeof(int) * 4;
							break;
						case Format_R8G8B8A8_Unorm:
							Stride += sizeof(int) * 4;
							break;
						case Format_R8G8B8A8_Unorm_SRGB:
							Stride += sizeof(int) * 4;
							break;
						case Format_R8G8B8A8_Uint:
							Stride += sizeof(int) * 4;
							break;
						case Format_R8G8B8A8_Snorm:
							Stride += sizeof(int) * 4;
							break;
						case Format_R8G8B8A8_Sint:
							Stride += sizeof(int) * 4;
							break;
						case Format_R16G16_Typeless:
							Stride += sizeof(int) * 2;
							break;
						case Format_R16G16_Float:
							Stride += sizeof(float) * 2;
							break;
						case Format_R16G16_Unorm:
							Stride += sizeof(int) * 2;
							break;
						case Format_R16G16_Uint:
							Stride += sizeof(int) * 2;
							break;
						case Format_R16G16_Snorm:
							Stride += sizeof(int) * 2;
							break;
						case Format_R16G16_Sint:
							Stride += sizeof(int) * 2;
							break;
						case Format_R32_Typeless:
							Stride += sizeof(int) * 1;
							break;
						case Format_D32_Float:
							Stride += sizeof(float) * 1;
							break;
						case Format_R32_Float:
							Stride += sizeof(float) * 1;
							break;
						case Format_R32_Uint:
							Stride += sizeof(int) * 1;
							break;
						case Format_R32_Sint:
							Stride += sizeof(int) * 1;
							break;
						case Format_R8G8_Typeless:
							Stride += sizeof(int) * 2;
							break;
						case Format_R8G8_Unorm:
							Stride += sizeof(int) * 2;
							break;
						case Format_R8G8_Uint:
							Stride += sizeof(int) * 2;
							break;
						case Format_R8G8_Snorm:
							Stride += sizeof(int) * 2;
							break;
						case Format_R8G8_Sint:
							Stride += sizeof(int) * 2;
							break;
						case Format_R16_Typeless:
							Stride += sizeof(int) * 1;
							break;
						case Format_R16_Float:
							Stride += sizeof(float) * 1;
							break;
						case Format_D16_Unorm:
							Stride += sizeof(int) * 1;
							break;
						case Format_R16_Unorm:
							Stride += sizeof(int) * 1;
							break;
						case Format_R16_Uint:
							Stride += sizeof(int) * 1;
							break;
						case Format_R16_Snorm:
							Stride += sizeof(int) * 1;
							break;
						case Format_R16_Sint:
							Stride += sizeof(int) * 1;
							break;
						case Format_R8_Typeless:
							Stride += sizeof(int) * 1;
							break;
						case Format_R8_Unorm:
							Stride += sizeof(int) * 1;
							break;
						case Format_R8_Uint:
							Stride += sizeof(int) * 1;
							break;
						case Format_R8_Snorm:
							Stride += sizeof(int) * 1;
							break;
						case Format_R8_Sint:
							Stride += sizeof(int) * 1;
							break;
						default:
							break;
					}
				}

				for (size_t i = 0; i < I.LayoutSize; i++)
					Stride += I.Layout[i].AlignedByteOffset;

				for (size_t i = 0; i < I.LayoutSize; i++)
				{
					InputLayout* Item = I.Layout + i;
					if (!Item)
						continue;

					bool Decimal = false;
					GLenum Format = GL_INVALID_ENUM;
					GLint Size = 0;

					switch (Item->FormatMode)
					{
						case Format_R32G32B32A32_Typeless:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R32G32B32A32_Float:
							Format = GL_FLOAT;
							Size = 4;
							Decimal = false;
							break;
						case Format_R32G32B32A32_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R32G32B32A32_Sint:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R32G32B32_Typeless:
							Format = GL_INT;
							Size = 3;
							Decimal = true;
							break;
						case Format_R32G32B32_Float:
							Format = GL_FLOAT;
							Size = 3;
							Decimal = false;
							break;
						case Format_R32G32B32_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 3;
							Decimal = true;
							break;
						case Format_R32G32B32_Sint:
							Format = GL_INT;
							Size = 3;
							Decimal = true;
							break;
						case Format_R16G16B16A16_Typeless:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R16G16B16A16_Float:
							Format = GL_FLOAT;
							Size = 4;
							Decimal = false;
							break;
						case Format_R16G16B16A16_Unorm:
							Format = GL_UNSIGNED_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R16G16B16A16_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R16G16B16A16_Snorm:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R16G16B16A16_Sint:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R32G32_Typeless:
							Format = GL_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R32G32_Float:
							Format = GL_FLOAT;
							Size = 2;
							Decimal = false;
							break;
						case Format_R32G32_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R32G32_Sint:
							Format = GL_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R10G10B10A2_Typeless:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R10G10B10A2_Unorm:
							Format = GL_UNSIGNED_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R10G10B10A2_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R11G11B10_Float:
							Format = GL_FLOAT;
							Size = 3;
							Decimal = false;
							break;
						case Format_R8G8B8A8_Typeless:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R8G8B8A8_Unorm:
							Format = GL_UNSIGNED_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R8G8B8A8_Unorm_SRGB:
							Format = GL_UNSIGNED_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R8G8B8A8_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R8G8B8A8_Snorm:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R8G8B8A8_Sint:
							Format = GL_INT;
							Size = 4;
							Decimal = true;
							break;
						case Format_R16G16_Typeless:
							Format = GL_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R16G16_Float:
							Format = GL_FLOAT;
							Size = 2;
							Decimal = false;
							break;
						case Format_R16G16_Unorm:
							Format = GL_UNSIGNED_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R16G16_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R16G16_Snorm:
							Format = GL_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R16G16_Sint:
							Format = GL_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R32_Typeless:
							Format = GL_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_D32_Float:
							Format = GL_FLOAT;
							Size = 1;
							Decimal = false;
							break;
						case Format_R32_Float:
							Format = GL_FLOAT;
							Size = 1;
							Decimal = false;
							break;
						case Format_R32_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R32_Sint:
							Format = GL_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R8G8_Typeless:
							Format = GL_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R8G8_Unorm:
							Format = GL_UNSIGNED_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R8G8_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R8G8_Snorm:
							Format = GL_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R8G8_Sint:
							Format = GL_INT;
							Size = 2;
							Decimal = true;
							break;
						case Format_R16_Typeless:
							Format = GL_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R16_Float:
							Format = GL_FLOAT;
							Size = 1;
							Decimal = false;
							break;
						case Format_D16_Unorm:
							Format = GL_UNSIGNED_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R16_Unorm:
							Format = GL_UNSIGNED_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R16_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R16_Snorm:
							Format = GL_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R16_Sint:
							Format = GL_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R8_Typeless:
							Format = GL_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R8_Unorm:
							Format = GL_UNSIGNED_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R8_Uint:
							Format = GL_UNSIGNED_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R8_Snorm:
							Format = GL_INT;
							Size = 1;
							Decimal = true;
							break;
						case Format_R8_Sint:
							Format = GL_INT;
							Size = 1;
							Decimal = true;
							break;
						default:
							break;
					}

					GLsizei Stride = Item->AlignedByteOffset;
					if (Decimal)
					{
						VertexLayout.push_back([i, Size, Format, Stride]()
						{
							glVertexAttribIPointer(i, Size, Format, 0, (void*)(Stride));
						});
					}
					else
					{
						VertexLayout.push_back([i, Size, Format, Stride]()
						{
							glVertexAttribPointerARB(i, Size, Format, GL_FALSE, 0, (void*)(Stride));
						});
					}
				}
			}
			OGLShader::~OGLShader()
			{
				glDeleteProgram(Program);
				glDeleteShader(VertexShader);
				glDeleteShader(PixelShader);
				glDeleteShader(GeometryShader);
				glDeleteShader(DomainShader);
				glDeleteShader(HullShader);
				glDeleteShader(ComputeShader);
			}
			bool OGLShader::IsValid()
			{
				return Compiled;
			}

			OGLElementBuffer::OGLElementBuffer(const Desc& I) : ElementBuffer(I)
			{
			}
			OGLElementBuffer::~OGLElementBuffer()
			{
				glDeleteBuffers(1, &Resource);
			}
			void* OGLElementBuffer::GetResource()
			{
				return (void*)(intptr_t)Resource;
			}

			OGLStructureBuffer::OGLStructureBuffer(const Desc& I) : StructureBuffer(I)
			{
			}
			OGLStructureBuffer::~OGLStructureBuffer()
			{
				glDeleteBuffers(1, &Resource);
			}
			void* OGLStructureBuffer::GetElement()
			{
				return (void*)(intptr_t)Resource;
			}
			void* OGLStructureBuffer::GetResource()
			{
				return (void*)(intptr_t)Resource;
			}

			OGLMeshBuffer::OGLMeshBuffer(const Desc& I) : MeshBuffer(I)
			{
			}
			Compute::Vertex* OGLMeshBuffer::GetElements(GraphicsDevice* Device)
			{
				MappedSubresource Resource;
				Device->Map(VertexBuffer, ResourceMap_Write, &Resource);

				Compute::Vertex* Vertices = new Compute::Vertex[(unsigned int)VertexBuffer->GetElements()];
				memcpy(Vertices, Resource.Pointer, (size_t)VertexBuffer->GetElements() * sizeof(Compute::Vertex));

				Device->Unmap(VertexBuffer, &Resource);
				return Vertices;
			}

			OGLSkinMeshBuffer::OGLSkinMeshBuffer(const Desc& I) : SkinMeshBuffer(I)
			{
			}
			Compute::SkinVertex* OGLSkinMeshBuffer::GetElements(GraphicsDevice* Device)
			{
				MappedSubresource Resource;
				Device->Map(VertexBuffer, ResourceMap_Write, &Resource);

				Compute::SkinVertex* Vertices = new Compute::SkinVertex[(unsigned int)VertexBuffer->GetElements()];
				memcpy(Vertices, Resource.Pointer, (size_t)VertexBuffer->GetElements() * sizeof(Compute::SkinVertex));

				Device->Unmap(VertexBuffer, &Resource);
				return Vertices;
			}

			OGLInstanceBuffer::OGLInstanceBuffer(const Desc& I) : InstanceBuffer(I)
			{
			}
			OGLInstanceBuffer::~OGLInstanceBuffer()
			{
				if (Device != nullptr && Sync)
					Device->ClearBuffer(this);

				glDeleteBuffers(1, &Resource);
			}

			OGLTexture2D::OGLTexture2D() : Texture2D()
			{
			}
			OGLTexture2D::OGLTexture2D(const Desc& I) : Texture2D(I)
			{
			}
			OGLTexture2D::~OGLTexture2D()
			{
				glDeleteTextures(1, &Resource);
			}
			void* OGLTexture2D::GetResource()
			{
				return (void*)(intptr_t)Resource;
			}

			OGLTexture3D::OGLTexture3D() : Texture3D()
			{
			}
			OGLTexture3D::~OGLTexture3D()
			{
				glDeleteTextures(1, &Resource);
			}
			void* OGLTexture3D::GetResource()
			{
				return (void*)(intptr_t)Resource;
			}

			OGLTextureCube::OGLTextureCube() : TextureCube()
			{
			}
			OGLTextureCube::OGLTextureCube(const Desc& I) : TextureCube(I)
			{
			}
			OGLTextureCube::~OGLTextureCube()
			{
				glDeleteTextures(1, &Resource);
			}
			void* OGLTextureCube::GetResource()
			{
				return (void*)(intptr_t)Resource;
			}

			OGLDepthBuffer::OGLDepthBuffer(const Desc& I) : Graphics::DepthBuffer(I)
			{
			}
			OGLDepthBuffer::~OGLDepthBuffer()
			{
				glDeleteFramebuffers(1, &FrameBuffer);
				glDeleteRenderbuffers(1, &DepthBuffer);
			}
			Viewport OGLDepthBuffer::GetViewport()
			{
				return View;
			}
			float OGLDepthBuffer::GetWidth()
			{
				return View.Width;
			}
			float OGLDepthBuffer::GetHeight()
			{
				return View.Height;
			}
			void* OGLDepthBuffer::GetResource()
			{
				return (void*)(intptr_t)DepthBuffer;
			}

			OGLRenderTarget2D::OGLRenderTarget2D(const Desc& I) : RenderTarget2D(I)
			{
			}
			OGLRenderTarget2D::~OGLRenderTarget2D()
			{
				glDeleteFramebuffers(1, &FrameBuffer);
				glDeleteTextures(1, &Texture);
				glDeleteRenderbuffers(1, &DepthBuffer);
			}
			Viewport OGLRenderTarget2D::GetViewport()
			{
				return View;
			}
			float OGLRenderTarget2D::GetWidth()
			{
				return View.Width;
			}
			float OGLRenderTarget2D::GetHeight()
			{
				return View.Height;
			}
			void* OGLRenderTarget2D::GetResource()
			{
				return (void*)(intptr_t)Texture;
			}

			OGLMultiRenderTarget2D::OGLMultiRenderTarget2D(const Desc& I) : MultiRenderTarget2D(I)
			{
			}
			OGLMultiRenderTarget2D::~OGLMultiRenderTarget2D()
			{
				glDeleteFramebuffers(1, &FrameBuffer);
				glDeleteRenderbuffers(1, &DepthBuffer);
				glDeleteTextures(8, Texture);
			}
			Viewport OGLMultiRenderTarget2D::GetViewport()
			{
				return View;
			}
			float OGLMultiRenderTarget2D::GetWidth()
			{
				return View.Width;
			}
			float OGLMultiRenderTarget2D::GetHeight()
			{
				return View.Height;
			}
			void* OGLMultiRenderTarget2D::GetResource(int Id)
			{
				if (Id < 0 || Id >= 8)
					return nullptr;

				return (void*)(intptr_t)Texture[Id];
			}

			OGLRenderTargetCube::OGLRenderTargetCube(const Desc& I) : RenderTargetCube(I)
			{
			}
			OGLRenderTargetCube::~OGLRenderTargetCube()
			{
				glDeleteFramebuffers(1, &FrameBuffer);
				glDeleteTextures(1, &Texture);
				glDeleteTextures(1, &DepthTexture);
			}
			Viewport OGLRenderTargetCube::GetViewport()
			{
				return View;
			}
			float OGLRenderTargetCube::GetWidth()
			{
				return View.Width;
			}
			float OGLRenderTargetCube::GetHeight()
			{
				return View.Height;
			}
			void* OGLRenderTargetCube::GetResource()
			{
				return (void*)(intptr_t)Texture;
			}

			OGLMultiRenderTargetCube::OGLMultiRenderTargetCube(const Desc& I) : MultiRenderTargetCube(I)
			{
			}
			OGLMultiRenderTargetCube::~OGLMultiRenderTargetCube()
			{
				glDeleteFramebuffers(1, &FrameBuffer);
				glDeleteTextures(8, Texture);
				glDeleteTextures(1, &DepthTexture);
			}
			Graphics::Viewport OGLMultiRenderTargetCube::GetViewport()
			{
				return View;
			}
			float OGLMultiRenderTargetCube::GetWidth()
			{
				return View.Width;
			}
			float OGLMultiRenderTargetCube::GetHeight()
			{
				return View.Height;
			}
			void* OGLMultiRenderTargetCube::GetResource(int Id)
			{
				return (void*)(intptr_t)Texture[Id];
			}

			OGLQuery::OGLQuery() : Query()
			{
			}
			OGLQuery::~OGLQuery()
			{
				glDeleteQueries(1, &Async);
			}
			void* OGLQuery::GetResource()
			{
				return (void*)(intptr_t)Async;
			}

			OGLDevice::OGLDevice(const Desc& I) : GraphicsDevice(I), Window(I.Window), Context(nullptr)
			{
				if (!Window)
				{
					THAWK_ERROR("OpenGL cannot be created without a window");
					return;
				}

#ifdef THAWK_HAS_SDL2
				SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
				SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
				SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
				SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

				if (I.Debug)
				{
					glEnable(GL_DEBUG_OUTPUT);
					glDebugMessageCallback(DebugMessage, nullptr);
				}

				Context = SDL_GL_CreateContext(Window->GetHandle());
				if (!Context)
				{
					THAWK_ERROR("OpenGL creation conflict -> %s", Window->GetError().c_str());
					return;
				}

				SDL_GL_MakeCurrent(Window->GetHandle(), Context);
				switch (VSyncMode)
				{
					case VSync_Disabled:
						SDL_GL_SetSwapInterval(0);
						break;
					case VSync_Frequency_X1:
						SDL_GL_SetSwapInterval(1);
						break;
					case VSync_Frequency_X2:
					case VSync_Frequency_X3:
					case VSync_Frequency_X4:
						if (SDL_GL_SetSwapInterval(-1) == -1)
							SDL_GL_SetSwapInterval(1);
						break;
				}
#endif
				const GLenum ErrorCode = glewInit();
				if (ErrorCode != GLEW_OK)
				{
					THAWK_ERROR("OpenGL extentions cannot be loaded -> %s", (const char*)glewGetErrorString(ErrorCode));
					return;
				}

				CreateConstantBuffer(&ConstantBuffer[0], sizeof(AnimationBuffer));
				Constants[0] = &Animation;

				CreateConstantBuffer(&ConstantBuffer[0], sizeof(RenderBuffer));
				Constants[1] = &Render;

				CreateConstantBuffer(&ConstantBuffer[0], sizeof(ViewBuffer));
				Constants[2] = &View;

				glBindBuffer(GL_UNIFORM_BUFFER, ConstantBuffer[0]);
				glBindBuffer(GL_UNIFORM_BUFFER, ConstantBuffer[1]);
				glBindBuffer(GL_UNIFORM_BUFFER, ConstantBuffer[2]);
				glEnable(GL_TEXTURE_2D);
				SetShaderModel(I.ShaderMode == ShaderModel_Auto ? GetSupportedShaderModel() : I.ShaderMode);
				ResizeBuffers(I.BufferWidth, I.BufferHeight);
				InitStates();

				Shader::Desc F = Shader::Desc();
				F.Layout = Shader::GetShapeVertexLayout();
				F.LayoutSize = 2;
				F.Filename = "basic";

				if (GetSection("standard/basic", &F.Data))
					BasicEffect = CreateShader(F);
			}
			OGLDevice::~OGLDevice()
			{
				FreeProxy();
#ifdef THAWK_HAS_SDL2
				if (Context != nullptr)
				{
					SDL_GL_DeleteContext(Context);
					Context = nullptr;
				}
#endif
				glDeleteBuffers(3, ConstantBuffer);
			}
			void OGLDevice::SetConstantBuffers()
			{
				glBindBufferBase(GL_UNIFORM_BUFFER, 0, ConstantBuffer[0]);
				glBindBufferBase(GL_UNIFORM_BUFFER, 1, ConstantBuffer[1]);
				glBindBufferBase(GL_UNIFORM_BUFFER, 2, ConstantBuffer[2]);
			}
			void OGLDevice::SetShaderModel(ShaderModel Model)
			{
				ShaderModelType = Model;
				if (ShaderModelType == ShaderModel_GLSL_1_1_0)
					ShaderVersion = "#version 110\n";
				else if (ShaderModelType == ShaderModel_GLSL_1_2_0)
					ShaderVersion = "#version 120\n";
				else if (ShaderModelType == ShaderModel_GLSL_1_3_0)
					ShaderVersion = "#version 130\n";
				else if (ShaderModelType == ShaderModel_GLSL_1_4_0)
					ShaderVersion = "#version 140\n";
				else if (ShaderModelType == ShaderModel_GLSL_1_5_0)
					ShaderVersion = "#version 150\n";
				else if (ShaderModelType == ShaderModel_GLSL_3_3_0)
					ShaderVersion = "#version 330\n";
				else if (ShaderModelType == ShaderModel_GLSL_4_0_0)
					ShaderVersion = "#version 400\n";
				else if (ShaderModelType == ShaderModel_GLSL_4_1_0)
					ShaderVersion = "#version 410\n";
				else if (ShaderModelType == ShaderModel_GLSL_4_2_0)
					ShaderVersion = "#version 420\n";
				else if (ShaderModelType == ShaderModel_GLSL_4_3_0)
					ShaderVersion = "#version 430\n";
				else if (ShaderModelType == ShaderModel_GLSL_4_4_0)
					ShaderVersion = "#version 440\n";
				else if (ShaderModelType == ShaderModel_GLSL_4_5_0)
					ShaderVersion = "#version 450\n";
				else if (ShaderModelType == ShaderModel_GLSL_4_6_0)
					ShaderVersion = "#version 460\n";
				else
					SetShaderModel(ShaderModel_GLSL_1_1_0);
			}
			void OGLDevice::SetSamplerState(SamplerState* State)
			{
				glBindSampler(0, (GLuint)(State ? State->As<OGLSamplerState>()->Resource : GL_INVALID_VALUE));
			}
			void OGLDevice::SetBlendState(BlendState* State)
			{
				if (!State)
					return;

				OGLBlendState* V = (OGLBlendState*)State;
				if (V->State.AlphaToCoverageEnable)
				{
					glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
					glSampleCoverage(1.0f, GL_FALSE);
				}
				else
				{
					glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
					glSampleCoverage(0.0f, GL_FALSE);
				}

				if (!V->State.IndependentBlendEnable)
				{
					for (int i = 0; i < 8; i++)
					{
						if (V->State.RenderTarget[i].BlendEnable)
							glEnablei(GL_BLEND, i);
						else
							glDisablei(GL_BLEND, i);

						glBlendEquationSeparatei(i, GetBlendOperation(V->State.RenderTarget[i].BlendOperationMode), GetBlendOperation(V->State.RenderTarget[i].BlendOperationAlpha));
						glBlendFuncSeparatei(i, GetBlend(V->State.RenderTarget[i].SrcBlend), GetBlend(V->State.RenderTarget[i].DestBlend), GetBlend(V->State.RenderTarget[i].SrcBlendAlpha), GetBlend(V->State.RenderTarget[i].DestBlendAlpha));
					}
				}
				else
				{
					if (V->State.RenderTarget[0].BlendEnable)
						glEnable(GL_BLEND);
					else
						glDisable(GL_BLEND);

					glBlendEquationSeparate(GetBlendOperation(V->State.RenderTarget[0].BlendOperationMode), GetBlendOperation(V->State.RenderTarget[0].BlendOperationAlpha));
					glBlendFuncSeparate(GetBlend(V->State.RenderTarget[0].SrcBlend), GetBlend(V->State.RenderTarget[0].DestBlend), GetBlend(V->State.RenderTarget[0].SrcBlendAlpha), GetBlend(V->State.RenderTarget[0].DestBlendAlpha));
				}
			}
			void OGLDevice::SetRasterizerState(RasterizerState* State)
			{
				if (!State)
					return;

				OGLRasterizerState* V = (OGLRasterizerState*)State;
				if (V->State.AntialiasedLineEnable || V->State.MultisampleEnable)
					glEnable(GL_MULTISAMPLE);
				else
					glDisable(GL_MULTISAMPLE);

				if (V->State.CullMode == VertexCull_Back)
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
				}
				else if (V->State.CullMode == VertexCull_Front)
				{
					glEnable(GL_CULL_FACE);
					glCullFace(GL_FRONT);
				}
				else
					glDisable(GL_CULL_FACE);

				if (V->State.FillMode == SurfaceFill_Solid)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				if (V->State.FrontCounterClockwise)
					glFrontFace(GL_CW);
				else
					glFrontFace(GL_CCW);
			}
			void OGLDevice::SetDepthStencilState(DepthStencilState* State)
			{
				if (!State)
					return;

				OGLDepthStencilState* V = (OGLDepthStencilState*)State;
				if (V->State.DepthEnable)
					glEnable(GL_DEPTH);
				else
					glDisable(GL_DEPTH);

				if (V->State.StencilEnable)
					glEnable(GL_STENCIL);
				else
					glDisable(GL_STENCIL);

				glDepthFunc(GetComparison(V->State.DepthFunction));
				glDepthMask(V->State.DepthWriteMask == DepthWrite_All ? GL_TRUE : GL_FALSE);
				glStencilMask((GLuint)V->State.StencilWriteMask);
				glStencilFuncSeparate(GL_FRONT, GetComparison(V->State.FrontFaceStencilFunction), 0, 1);
				glStencilOpSeparate(GL_FRONT, GetStencilOperation(V->State.FrontFaceStencilFailOperation), GetStencilOperation(V->State.FrontFaceStencilDepthFailOperation), GetStencilOperation(V->State.FrontFaceStencilPassOperation));
				glStencilFuncSeparate(GL_BACK, GetComparison(V->State.BackFaceStencilFunction), 0, 1);
				glStencilOpSeparate(GL_BACK, GetStencilOperation(V->State.BackFaceStencilFailOperation), GetStencilOperation(V->State.BackFaceStencilDepthFailOperation), GetStencilOperation(V->State.BackFaceStencilPassOperation));
			}
			void OGLDevice::SetShader(Shader* Resource, unsigned int Type)
			{
				OGLShader* IResource = Resource->As<OGLShader>();
				/* TODO: IMPL */
			}
			void OGLDevice::SetBuffer(Shader* Resource, unsigned int Slot, unsigned int Type)
			{
				OGLShader* IResource = Resource->As<OGLShader>();
				/* TODO: IMPL */
			}
			void OGLDevice::SetBuffer(StructureBuffer* Resource, unsigned int Slot)
			{
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, Resource ? Resource->As<OGLStructureBuffer>()->Resource : GL_INVALID_VALUE);
			}
			void OGLDevice::SetBuffer(InstanceBuffer* Resource, unsigned int Slot)
			{
				glBindBuffer(GL_ARRAY_BUFFER, Resource ? Resource->As<OGLInstanceBuffer>()->Resource : GL_INVALID_VALUE);
			}
			void OGLDevice::SetIndexBuffer(ElementBuffer* Resource, Format FormatMode, unsigned int Offset)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Resource ? Resource->As<OGLElementBuffer>()->Resource : GL_INVALID_VALUE);
			}
			void OGLDevice::SetVertexBuffer(ElementBuffer* Resource, unsigned int Slot, unsigned int Stride, unsigned int Offset)
			{
				glBindBuffer(GL_ARRAY_BUFFER, Resource ? Resource->As<OGLElementBuffer>()->Resource : GL_INVALID_VALUE);
			}
			void OGLDevice::SetTexture2D(Texture2D* Resource, unsigned int Slot)
			{
				glActiveTexture(GL_TEXTURE0 + Slot);
				glBindTexture(GL_TEXTURE_2D, Resource ? Resource->As<OGLTexture2D>()->Resource : GL_INVALID_VALUE);
			}
			void OGLDevice::SetTexture3D(Texture3D* Resource, unsigned int Slot)
			{
				glActiveTexture(GL_TEXTURE0 + Slot);
				glBindTexture(GL_TEXTURE_3D, Resource ? Resource->As<OGLTexture3D>()->Resource : GL_INVALID_VALUE);
			}
			void OGLDevice::SetTextureCube(TextureCube* Resource, unsigned int Slot)
			{
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(float R, float G, float B)
			{
				SetTarget(RenderTarget, R, G, B);
			}
			void OGLDevice::SetTarget()
			{
				SetTarget(RenderTarget);
			}
			void OGLDevice::SetTarget(DepthBuffer* Resource)
			{
				OGLDepthBuffer* IResource = (OGLDepthBuffer*)Resource;
				if (!IResource)
					return;

				glBindFramebuffer(GL_FRAMEBUFFER, IResource->FrameBuffer != GL_INVALID_VALUE ? IResource->FrameBuffer : 0);
				glViewport((GLuint)IResource->View.TopLeftX, (GLuint)IResource->View.TopLeftY, (GLuint)IResource->View.Width, (GLuint)IResource->View.Height);
			}
			void OGLDevice::SetTarget(RenderTarget2D* Resource, float R, float G, float B)
			{
				OGLRenderTarget2D* IResource = (OGLRenderTarget2D*)Resource;
				if (!IResource)
					return;

				glBindFramebuffer(GL_FRAMEBUFFER, IResource->FrameBuffer != GL_INVALID_VALUE ? IResource->FrameBuffer : 0);
				glViewport((GLuint)IResource->View.TopLeftX, (GLuint)IResource->View.TopLeftY, (GLuint)IResource->View.Width, (GLuint)IResource->View.Height);
				glClearColor(R, G, B, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			}
			void OGLDevice::SetTarget(RenderTarget2D* Resource)
			{
				OGLRenderTarget2D* IResource = (OGLRenderTarget2D*)Resource;
				if (!IResource)
					return;

				glBindFramebuffer(GL_FRAMEBUFFER, IResource->FrameBuffer != GL_INVALID_VALUE ? IResource->FrameBuffer : 0);
				glViewport((GLuint)IResource->View.TopLeftX, (GLuint)IResource->View.TopLeftY, (GLuint)IResource->View.Width, (GLuint)IResource->View.Height);
			}
			void OGLDevice::SetTarget(MultiRenderTarget2D* Resource, unsigned int Target, float R, float G, float B)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(MultiRenderTarget2D* Resource, unsigned int Target)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(MultiRenderTarget2D* Resource, float R, float G, float B)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(MultiRenderTarget2D* Resource)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(RenderTargetCube* Resource, float R, float G, float B)
			{
				OGLRenderTargetCube* IResource = (OGLRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(RenderTargetCube* Resource)
			{
				OGLRenderTargetCube* IResource = (OGLRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(MultiRenderTargetCube* Resource, unsigned int Target, float R, float G, float B)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(MultiRenderTargetCube* Resource, unsigned int Target)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(MultiRenderTargetCube* Resource, float R, float G, float B)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTarget(MultiRenderTargetCube* Resource)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetTargetMap(MultiRenderTarget2D* Resource, bool Enabled[8])
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetViewport(const Viewport& In)
			{
				SetViewport(RenderTarget, In);
			}
			void OGLDevice::SetViewport(RenderTarget2D* Resource, const Viewport& In)
			{
				OGLRenderTarget2D* IResource = (OGLRenderTarget2D*)Resource;
				if (!IResource)
					return;

				IResource->View = In;
				glViewport((GLuint)IResource->View.TopLeftX, (GLuint)IResource->View.TopLeftY, (GLuint)IResource->View.Width, (GLuint)IResource->View.Height);
			}
			void OGLDevice::SetViewport(MultiRenderTarget2D* Resource, const Viewport& In)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetViewport(RenderTargetCube* Resource, const Viewport& In)
			{
				OGLRenderTargetCube* IResource = (OGLRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetViewport(MultiRenderTargetCube* Resource, const Viewport& In)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SetViewports(unsigned int Count, Viewport* Value)
			{
				if (Count > 0 && Value)
					glViewport(Value[0].TopLeftX, Value[0].TopLeftY, Value[0].Width, Value[0].Height);
			}
			void OGLDevice::SetScissorRects(unsigned int Count, Rectangle* Value)
			{
				if (Count > 0 && Value)
					glScissor(Value[0].Left, Value[0].Top, Value[0].Right - Value[0].Left, Value[0].Top - Value[0].Bottom);
			}
			void OGLDevice::SetPrimitiveTopology(PrimitiveTopology _Topology)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GetPrimitiveTopology(_Topology));
				Topology = _Topology;
			}
			void OGLDevice::FlushTexture2D(unsigned int Slot, unsigned int Count)
			{
				if (Count <= 0 || Count > 31)
					return;

				for (unsigned int i = 0; i < Count; i++)
				{
					glActiveTexture(GL_TEXTURE0 + Slot + i);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			void OGLDevice::FlushTexture3D(unsigned int Slot, unsigned int Count)
			{
				if (Count <= 0 || Count > 31)
					return;

				for (unsigned int i = 0; i < Count; i++)
				{
					glActiveTexture(GL_TEXTURE0 + Slot + i);
					glEnable(GL_TEXTURE_3D);
					glBindTexture(GL_TEXTURE_3D, 0);
				}
			}
			void OGLDevice::FlushTextureCube(unsigned int Slot, unsigned int Count)
			{
				if (Count <= 0 || Count > 31)
					return;

				for (unsigned int i = 0; i < Count; i++)
				{
					glActiveTexture(GL_TEXTURE0 + Slot + i);
					glEnable(GL_TEXTURE_CUBE_MAP);
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				}
			}
			void OGLDevice::FlushState()
			{
			}
			bool OGLDevice::Map(ElementBuffer* Resource, ResourceMap Mode, MappedSubresource* Map)
			{
				OGLElementBuffer* IResource = (OGLElementBuffer*)Resource;
				if (!IResource)
					return false;

				GLint Size;
				glBindBuffer(IResource->Flags, IResource->Resource);
				glGetBufferParameteriv(IResource->Flags, GL_BUFFER_SIZE, &Size);
				Map->Pointer = glMapBuffer(IResource->Flags, OGLDevice::GetResourceMap(Mode));
				Map->RowPitch = Size;
				Map->DepthPitch = 1;

				return Map->Pointer != nullptr;
			}
			bool OGLDevice::Map(StructureBuffer* Resource, ResourceMap Mode, MappedSubresource* Map)
			{
				OGLStructureBuffer* IResource = (OGLStructureBuffer*)Resource;
				if (!IResource)
					return false;

				GLint Size;
				glBindBuffer(IResource->Flags, IResource->Resource);
				glGetBufferParameteriv(IResource->Flags, GL_BUFFER_SIZE, &Size);
				Map->Pointer = glMapBuffer(IResource->Flags, OGLDevice::GetResourceMap(Mode));
				Map->RowPitch = Size;
				Map->DepthPitch = 1;

				return Map->Pointer != nullptr;
			}
			bool OGLDevice::Unmap(ElementBuffer* Resource, MappedSubresource* Map)
			{
				OGLElementBuffer* IResource = (OGLElementBuffer*)Resource;
				if (!IResource)
					return false;

				glBindBuffer(IResource->Flags, IResource->Resource);
				glUnmapBuffer(IResource->Flags);
				return true;
			}
			bool OGLDevice::Unmap(StructureBuffer* Resource, MappedSubresource* Map)
			{
				OGLStructureBuffer* IResource = (OGLStructureBuffer*)Resource;
				if (!IResource)
					return false;

				glBindBuffer(IResource->Flags, IResource->Resource);
				glUnmapBuffer(IResource->Flags);
				return true;
			}
			bool OGLDevice::UpdateBuffer(StructureBuffer* Resource, void* Data, uint64_t Size)
			{
				OGLStructureBuffer* IResource = (OGLStructureBuffer*)Resource;
				if (!IResource)
					return false;

				glBindBuffer(IResource->Flags, IResource->Resource);
				glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)Size, Data, GL_DYNAMIC_DRAW);
				return true;
			}
			bool OGLDevice::UpdateBuffer(Shader* Resource, const void* Data)
			{
				OGLShader* IResource = (OGLShader*)Resource;
				if (!IResource)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::UpdateBuffer(MeshBuffer* Resource, Compute::Vertex* Data)
			{
				OGLMeshBuffer* IResource = (OGLMeshBuffer*)Resource;
				if (!IResource)
					return false;

				MappedSubresource MappedResource;
				if (!Map(IResource->VertexBuffer, ResourceMap_Write, &MappedResource))
					return false;

				memcpy(MappedResource.Pointer, Data, (size_t)IResource->VertexBuffer->GetElements() * sizeof(Compute::Vertex));
				return Unmap(IResource->VertexBuffer, &MappedResource);
			}
			bool OGLDevice::UpdateBuffer(SkinMeshBuffer* Resource, Compute::SkinVertex* Data)
			{
				OGLSkinMeshBuffer* IResource = (OGLSkinMeshBuffer*)Resource;
				if (!IResource)
					return false;

				MappedSubresource MappedResource;
				if (!Map(IResource->VertexBuffer, ResourceMap_Write, &MappedResource))
					return false;

				memcpy(MappedResource.Pointer, Data, (size_t)IResource->VertexBuffer->GetElements() * sizeof(Compute::SkinVertex));
				return Unmap(IResource->VertexBuffer, &MappedResource);
			}
			bool OGLDevice::UpdateBuffer(InstanceBuffer* Resource)
			{
				OGLInstanceBuffer* IResource = (OGLInstanceBuffer*)Resource;
				if (!IResource || IResource->Array.Size() <= 0 || IResource->Array.Size() > IResource->ElementLimit)
					return false;

				OGLElementBuffer* Element = IResource->Elements->As<OGLElementBuffer>();
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::UpdateBuffer(RenderBufferType Buffer)
			{
				glBindBufferBase(GL_UNIFORM_BUFFER, (int)Buffer, ConstantBuffer[Buffer]);
				return true;
			}
			bool OGLDevice::UpdateBufferSize(Shader* Resource, size_t Size)
			{
				OGLShader* IResource = (OGLShader*)Resource;
				if (!IResource || !Size)
					return false;

				if (IResource->ConstantBuffer != GL_INVALID_VALUE)
					glDeleteBuffers(1, &IResource->ConstantBuffer);

				return CreateConstantBuffer(&IResource->ConstantBuffer, Size) >= 0;
			}
			bool OGLDevice::UpdateBufferSize(InstanceBuffer* Resource, uint64_t Size)
			{
				OGLInstanceBuffer* IResource = (OGLInstanceBuffer*)Resource;
				if (!IResource)
					return false;
				/* TODO: IMPL */
				return true;
			}
			void OGLDevice::ClearBuffer(InstanceBuffer* Resource)
			{
				OGLInstanceBuffer* IResource = (OGLInstanceBuffer*)Resource;
				if (!IResource || !IResource->Sync)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::Clear(float R, float G, float B)
			{
				Clear(RenderTarget, R, G, B);
			}
			void OGLDevice::Clear(RenderTarget2D* Resource, float R, float G, float B)
			{
				OGLRenderTarget2D* IResource = (OGLRenderTarget2D*)Resource;
				if (!IResource)
					return;

				glBindFramebuffer(GL_FRAMEBUFFER, IResource->FrameBuffer != GL_INVALID_VALUE ? IResource->FrameBuffer : 0);
				glViewport((GLuint)IResource->View.TopLeftX, (GLuint)IResource->View.TopLeftY, (GLuint)IResource->View.Width, (GLuint)IResource->View.Height);
				glClearColor(R, G, B, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			}
			void OGLDevice::Clear(MultiRenderTarget2D* Resource, unsigned int Target, float R, float G, float B)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::Clear(RenderTargetCube* Resource, float R, float G, float B)
			{
				OGLRenderTargetCube* IResource = (OGLRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::Clear(MultiRenderTargetCube* Resource, unsigned int Target, float R, float G, float B)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::ClearDepth()
			{
				ClearDepth(RenderTarget);
			}
			void OGLDevice::ClearDepth(DepthBuffer* Resource)
			{
				glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}
			void OGLDevice::ClearDepth(RenderTarget2D* Resource)
			{
				glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}
			void OGLDevice::ClearDepth(MultiRenderTarget2D* Resource)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::ClearDepth(RenderTargetCube* Resource)
			{
				OGLRenderTargetCube* IResource = (OGLRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::ClearDepth(MultiRenderTargetCube* Resource)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::DrawIndexed(unsigned int Count, unsigned int IndexLocation, unsigned int BaseLocation)
			{
				glDrawElements(GetPrimitiveTopologyDraw(Topology), Count, GL_UNSIGNED_INT, &IndexLocation);
			}
			void OGLDevice::DrawIndexed(MeshBuffer* Resource)
			{
				if (!Resource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::DrawIndexed(SkinMeshBuffer* Resource)
			{
				if (!Resource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::Draw(unsigned int Count, unsigned int Location)
			{
				glDrawArrays(GetPrimitiveTopologyDraw(Topology), (GLint)Location, (GLint)Count);
			}
			bool OGLDevice::CopyTexture2D(Texture2D** Result)
			{
				return CopyTexture2D(RenderTarget, Result);
			}
			bool OGLDevice::CopyTexture2D(RenderTarget2D* Resource, Texture2D** Result)
			{
				OGLRenderTarget2D* IResource = (OGLRenderTarget2D*)Resource;
				if (!IResource || !IResource->Texture || !Result)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTexture2D(MultiRenderTarget2D* Resource, unsigned int Target, Texture2D** Result)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource || Target >= IResource->SVTarget || !Result)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTexture2D(RenderTargetCube* Resource, unsigned int Face, Texture2D** Result)
			{
				OGLRenderTargetCube* IResource = (OGLRenderTargetCube*)Resource;
				if (!IResource || Face >= 6 || !Result)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTexture2D(MultiRenderTargetCube* Resource, unsigned int Cube, unsigned int Face, Texture2D** Result)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource || Cube >= IResource->SVTarget || Face >= 6 || !Result)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTextureCube(RenderTargetCube* Resource, TextureCube** Result)
			{
				OGLRenderTargetCube* IResource = (OGLRenderTargetCube*)Resource;
				if (!IResource || !Result)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTextureCube(MultiRenderTargetCube* Resource, unsigned int Cube, TextureCube** Result)
			{
				OGLMultiRenderTargetCube* IResource = (OGLMultiRenderTargetCube*)Resource;
				if (!IResource || Cube >= IResource->SVTarget || !Result)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTargetTo(MultiRenderTarget2D* Resource, unsigned int Target, RenderTarget2D* To)
			{
				if (!Resource || Target >= Resource->GetSVTarget() || !To)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTargetFrom(MultiRenderTarget2D* Resource, unsigned int Target, RenderTarget2D* From)
			{
				if (!Resource || Target >= Resource->GetSVTarget() || !From)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTargetDepth(RenderTarget2D* From, RenderTarget2D* To)
			{
				OGLRenderTarget2D* IResource1 = (OGLRenderTarget2D*)From;
				OGLRenderTarget2D* IResource2 = (OGLRenderTarget2D*)To;
				if (!IResource1 || !IResource2)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTargetDepth(MultiRenderTarget2D* From, MultiRenderTarget2D* To)
			{
				OGLMultiRenderTarget2D* IResource1 = (OGLMultiRenderTarget2D*)From;
				OGLMultiRenderTarget2D* IResource2 = (OGLMultiRenderTarget2D*)To;
				if (!IResource1 || !IResource2)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTargetDepth(RenderTargetCube* From, RenderTargetCube* To)
			{
				OGLRenderTargetCube* IResource1 = (OGLRenderTargetCube*)From;
				OGLRenderTargetCube* IResource2 = (OGLRenderTargetCube*)To;
				if (!IResource1 || !IResource2)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyTargetDepth(MultiRenderTargetCube* From, MultiRenderTargetCube* To)
			{
				OGLMultiRenderTargetCube* IResource1 = (OGLMultiRenderTargetCube*)From;
				OGLMultiRenderTargetCube* IResource2 = (OGLMultiRenderTargetCube*)To;
				if (!IResource1 || !IResource2)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyBegin(MultiRenderTarget2D* Resource, unsigned int Target, unsigned int MipLevels, unsigned int Size)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource || Target >= IResource->SVTarget)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyFace(MultiRenderTarget2D* Resource, unsigned int Target, unsigned int Face)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource || Target >= IResource->SVTarget || Face >= 6)
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::CopyEnd(MultiRenderTarget2D* Resource, TextureCube* Result)
			{
				OGLMultiRenderTarget2D* IResource = (OGLMultiRenderTarget2D*)Resource;
				if (!IResource || !Result)
					return false;
				/* TODO: IMPL */
				return true;
			}
			void OGLDevice::SwapTargetDepth(RenderTarget2D* From, RenderTarget2D* To)
			{
				OGLRenderTarget2D* IResource1 = (OGLRenderTarget2D*)From;
				OGLRenderTarget2D* IResource2 = (OGLRenderTarget2D*)To;
				if (!IResource1 || !IResource2)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SwapTargetDepth(MultiRenderTarget2D* From, MultiRenderTarget2D* To)
			{
				OGLMultiRenderTarget2D* IResource1 = (OGLMultiRenderTarget2D*)From;
				OGLMultiRenderTarget2D* IResource2 = (OGLMultiRenderTarget2D*)To;
				if (!IResource1 || !IResource2)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SwapTargetDepth(RenderTargetCube* From, RenderTargetCube* To)
			{
				OGLRenderTargetCube* IResource1 = (OGLRenderTargetCube*)From;
				OGLRenderTargetCube* IResource2 = (OGLRenderTargetCube*)To;
				if (!IResource1 || !IResource2)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::SwapTargetDepth(MultiRenderTargetCube* From, MultiRenderTargetCube* To)
			{
				OGLMultiRenderTargetCube* IResource1 = (OGLMultiRenderTargetCube*)From;
				OGLMultiRenderTargetCube* IResource2 = (OGLMultiRenderTargetCube*)To;
				if (!IResource1 || !IResource2)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::FetchViewports(unsigned int* Count, Viewport* Out)
			{
				GLint Viewport[4];
				glGetIntegerv(GL_VIEWPORT, Viewport);
				if (Count != nullptr)
					*Count = 1;

				if (!Out)
					return;

				Out[0].TopLeftX = Viewport[0];
				Out[0].TopLeftY = Viewport[1];
				Out[0].Width = Viewport[2];
				Out[0].Height = Viewport[3];
			}
			void OGLDevice::FetchScissorRects(unsigned int* Count, Rectangle* Out)
			{
				GLint Rect[4];
				glGetIntegerv(GL_SCISSOR_BOX, Rect);
				if (Count != nullptr)
					*Count = 1;

				if (!Out)
					return;

				Out[0].Left = Rect[0];
				Out[0].Right = Rect[2] + Rect[0];
				Out[0].Top = Rect[1];
				Out[0].Bottom = Rect[1] - Rect[3];
			}
			bool OGLDevice::ResizeBuffers(unsigned int Width, unsigned int Height)
			{
				RenderTarget2D::Desc F = RenderTarget2D::Desc();
				F.Width = Width;
				F.Height = Height;
				F.MipLevels = 1;
				F.MiscFlags = ResourceMisc_None;
				F.FormatMode = Format_R8G8B8A8_Unorm;
				F.Usage = ResourceUsage_Default;
				F.AccessFlags = CPUAccess_Invalid;
				F.BindFlags = ResourceBind_Render_Target | ResourceBind_Shader_Input;
				F.RenderSurface = (void*)this;

				delete RenderTarget;
				RenderTarget = CreateRenderTarget2D(F);
				SetTarget();

				return true;
			}
			bool OGLDevice::GenerateTexture(Texture2D* Resource)
			{
				OGLTexture2D* IResource = (OGLTexture2D*)Resource;
				return (IResource && IResource->Resource != GL_INVALID_VALUE);
			}
			bool OGLDevice::GenerateTexture(Texture3D* Resource)
			{
				OGLTexture3D* IResource = (OGLTexture3D*)Resource;
				return (IResource && IResource->Resource != GL_INVALID_VALUE);
			}
			bool OGLDevice::GenerateTexture(TextureCube* Resource)
			{
				OGLTextureCube* IResource = (OGLTextureCube*)Resource;
				return (IResource && IResource->Resource != GL_INVALID_VALUE);
			}
			bool OGLDevice::GetQueryData(Query* Resource, uint64_t* Result, bool Flush)
			{
				OGLQuery* IResource = (OGLQuery*)Resource;
				if (!Result || !IResource)
					return false;

				GLint Available = 0;
				glGetQueryObjectiv(IResource->Async, GL_QUERY_RESULT_AVAILABLE, &Available);
				if (Available == GL_FALSE)
					return false;

				GLint64 Data = 0;
				glGetQueryObjecti64v(IResource->Async, GL_QUERY_RESULT, &Data);
				*Result = (uint64_t)Data;

				return true;
			}
			bool OGLDevice::GetQueryData(Query* Resource, bool* Result, bool Flush)
			{
				OGLQuery* IResource = (OGLQuery*)Resource;
				if (!Result || !IResource)
					return false;

				GLint Available = 0;
				glGetQueryObjectiv(IResource->Async, GL_QUERY_RESULT_AVAILABLE, &Available);
				if (Available == GL_FALSE)
					return false;

				GLint Data = 0;
				glGetQueryObjectiv(IResource->Async, GL_QUERY_RESULT, &Data);
				*Result = (Data == GL_TRUE);

				return true;
			}
			void OGLDevice::QueryBegin(Query* Resource)
			{
				OGLQuery* IResource = (OGLQuery*)Resource;
				if (!IResource)
					return;

				glBeginQuery(IResource->Predicate ? GL_ANY_SAMPLES_PASSED : GL_SAMPLES_PASSED, IResource->Async);
			}
			void OGLDevice::QueryEnd(Query* Resource)
			{
				OGLQuery* IResource = (OGLQuery*)Resource;
				if (!IResource)
					return;

				glEndQuery(IResource->Predicate ? GL_ANY_SAMPLES_PASSED : GL_SAMPLES_PASSED);
			}
			void OGLDevice::GenerateMips(Texture2D* Resource)
			{
				OGLTexture2D* IResource = (OGLTexture2D*)Resource;
				if (!IResource || IResource->Resource != GL_INVALID_VALUE)
					return;

#ifdef glGenerateTextureMipmap
				glGenerateTextureMipmap(IResource->Resource);
#endif
			}
			void OGLDevice::GenerateMips(Texture3D* Resource)
			{
				OGLTexture3D* IResource = (OGLTexture3D*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			void OGLDevice::GenerateMips(TextureCube* Resource)
			{
				OGLTextureCube* IResource = (OGLTextureCube*)Resource;
				if (!IResource)
					return;
				/* TODO: IMPL */
			}
			bool OGLDevice::DirectBegin()
			{
				/* TODO: IMPL */

				Primitives = PrimitiveTopology_Triangle_List;
				Direct.WorldViewProjection = Compute::Matrix4x4::Identity();
				Direct.Padding = { 0, 0, 0, 1 };
				ViewResource = nullptr;

				Elements.clear();
				return true;
			}
			void OGLDevice::DirectTransform(const Compute::Matrix4x4& Transform)
			{
				Direct.WorldViewProjection = Direct.WorldViewProjection * Transform;
			}
			void OGLDevice::DirectTopology(PrimitiveTopology Topology)
			{
				Primitives = Topology;
			}
			void OGLDevice::DirectEmit()
			{
				Elements.push_back({ 0, 0, 0, 0, 0, 1, 1, 1, 1 });
			}
			void OGLDevice::DirectTexture(Texture2D* In)
			{
				ViewResource = In;
				Direct.Padding.Z = 1;
			}
			void OGLDevice::DirectColor(float X, float Y, float Z, float W)
			{
				if (Elements.empty())
					return;

				auto& Element = Elements.back();
				Element.CX = X;
				Element.CY = Y;
				Element.CZ = Z;
				Element.CW = W;
			}
			void OGLDevice::DirectIntensity(float Intensity)
			{
				Direct.Padding.W = Intensity;
			}
			void OGLDevice::DirectTexCoord(float X, float Y)
			{
				if (Elements.empty())
					return;

				auto& Element = Elements.back();
				Element.TX = X;
				Element.TY = Y;
			}
			void OGLDevice::DirectTexCoordOffset(float X, float Y)
			{
				Direct.Padding.X = X;
				Direct.Padding.Y = Y;
			}
			void OGLDevice::DirectPosition(float X, float Y, float Z)
			{
				if (Elements.empty())
					return;

				auto& Element = Elements.back();
				Element.PX = X;
				Element.PY = Y;
				Element.PZ = Z;
			}
			bool OGLDevice::DirectEnd()
			{
				if (Elements.empty())
					return false;
				/* TODO: IMPL */
				return true;
			}
			bool OGLDevice::Submit()
			{
#ifdef THAWK_HAS_SDL2
				SDL_GL_SwapWindow(Window->GetHandle());
#endif
				return true;
			}
			DepthStencilState* OGLDevice::CreateDepthStencilState(const DepthStencilState::Desc& I)
			{
				return new OGLDepthStencilState(I);
			}
			BlendState* OGLDevice::CreateBlendState(const BlendState::Desc& I)
			{
				return new OGLBlendState(I);
			}
			RasterizerState* OGLDevice::CreateRasterizerState(const RasterizerState::Desc& I)
			{
				return new OGLRasterizerState(I);
			}
			SamplerState* OGLDevice::CreateSamplerState(const SamplerState::Desc& I)
			{
				GLuint DeviceState = 0;
				glGenSamplers(1, &DeviceState);
				glSamplerParameteri(DeviceState, GL_TEXTURE_WRAP_S, OGLDevice::GetTextureAddress(I.AddressU));
				glSamplerParameteri(DeviceState, GL_TEXTURE_WRAP_T, OGLDevice::GetTextureAddress(I.AddressV));
				glSamplerParameteri(DeviceState, GL_TEXTURE_WRAP_R, OGLDevice::GetTextureAddress(I.AddressW));
				glSamplerParameteri(DeviceState, GL_TEXTURE_MAG_FILTER, OGLDevice::GetPixelFilter(I.Filter, true));
				glSamplerParameteri(DeviceState, GL_TEXTURE_MIN_FILTER, OGLDevice::GetPixelFilter(I.Filter, false));
				glSamplerParameteri(DeviceState, GL_TEXTURE_COMPARE_FUNC, OGLDevice::GetComparison(I.ComparisonFunction));
				glSamplerParameterf(DeviceState, GL_TEXTURE_LOD_BIAS, I.MipLODBias);
				glSamplerParameterf(DeviceState, GL_TEXTURE_MAX_LOD, I.MaxLOD);
				glSamplerParameterf(DeviceState, GL_TEXTURE_MIN_LOD, I.MinLOD);
				glSamplerParameterfv(DeviceState, GL_TEXTURE_BORDER_COLOR, (GLfloat*)I.BorderColor);

#ifdef GL_TEXTURE_MAX_ANISOTROPY
				if (I.Filter & Graphics::PixelFilter_Anistropic || I.Filter & Graphics::PixelFilter_Compare_Anistropic)
					glSamplerParameterf(DeviceState, GL_TEXTURE_MAX_ANISOTROPY, (float)I.MaxAnisotropy);
#elif defined(GL_TEXTURE_MAX_ANISOTROPY_EXT)
				if (I.Filter & Graphics::PixelFilter_Anistropic || I.Filter & Graphics::PixelFilter_Compare_Anistropic)
					glSamplerParameterf(DeviceState, GL_TEXTURE_MAX_ANISOTROPY_EXT, (float)I.MaxAnisotropy);
#endif
				OGLSamplerState* Result = new OGLSamplerState(I);
				Result->Resource = DeviceState;

				return Result;
			}
			Shader* OGLDevice::CreateShader(const Shader::Desc& I)
			{
				OGLShader* Result = new OGLShader(I);
				Shader::Desc F(I);

				if (!Preprocess(F))
				{
					THAWK_ERROR("shader preprocessing failed");
					return Result;
				}

				Rest::Stroke::Settle Start;
				Rest::Stroke Code(&F.Data);
				uint64_t Length = Code.Size();
				GLint Status = 0;

				if ((Start = Code.Find("VS_Main")).Found)
				{
					Rest::Stroke::Settle End = Code.Find("#program", Start.End);
					if (!End.Found)
						End.Start = Code.Size() - 1;

					Rest::Stroke Sub(F.Data);
					Sub.Splice(Start.End, End.Start);

					char* Buffer = Sub.Value();
					GLint Size = Sub.Size();

					Result->VertexShader = glCreateShader(GL_VERTEX_SHADER);
					glShaderSourceARB(Result->VertexShader, 1, &Buffer, &Size);
					glCompileShaderARB(Result->VertexShader);
					glGetShaderiv(Result->VertexShader, GL_COMPILE_STATUS, &Status);

					if (Status == GL_FALSE)
					{
						glGetShaderiv(Result->VertexShader, GL_INFO_LOG_LENGTH, &Size);
						Buffer = (char*)malloc(sizeof(char) * (Size + 1));
						glGetShaderInfoLog(Result->VertexShader, Size, &Size, Buffer);
						THAWK_ERROR("couldn't compile vertex shader\n\t%.*s", Size, Buffer);
						free(Buffer);
					}
				}

				if ((Start = Code.Find("PS_Main")).Found)
				{
					Rest::Stroke::Settle End = Code.Find("#program", Start.End);
					if (!End.Found)
						End.Start = Code.Size() - 1;

					Rest::Stroke Sub(F.Data);
					Sub.Splice(Start.End, End.Start);

					char* Buffer = Sub.Value();
					GLint Size = Sub.Size();

					Result->PixelShader = glCreateShader(GL_FRAGMENT_SHADER);
					glShaderSourceARB(Result->PixelShader, 1, &Buffer, &Size);
					glCompileShaderARB(Result->PixelShader);
					glGetShaderiv(Result->PixelShader, GL_COMPILE_STATUS, &Status);

					if (Status == GL_FALSE)
					{
						glGetShaderiv(Result->PixelShader, GL_INFO_LOG_LENGTH, &Size);
						Buffer = (char*)malloc(sizeof(char) * (Size + 1));
						glGetShaderInfoLog(Result->PixelShader, Size, &Size, Buffer);
						THAWK_ERROR("couldn't compile pixel shader\n\t%.*s", Size, Buffer);
						free(Buffer);
					}
				}

				if ((Start = Code.Find("GS_Main")).Found)
				{
					Rest::Stroke::Settle End = Code.Find("#program", Start.End);
					if (!End.Found)
						End.Start = Code.Size() - 1;

					Rest::Stroke Sub(F.Data);
					Sub.Splice(Start.End, End.Start);

					char* Buffer = Sub.Value();
					GLint Size = Sub.Size();

					Result->GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
					glShaderSourceARB(Result->GeometryShader, 1, &Buffer, &Size);
					glCompileShaderARB(Result->GeometryShader);
					glGetShaderiv(Result->GeometryShader, GL_COMPILE_STATUS, &Status);

					if (Status == GL_FALSE)
					{
						glGetShaderiv(Result->GeometryShader, GL_INFO_LOG_LENGTH, &Size);
						Buffer = (char*)malloc(sizeof(char) * (Size + 1));
						glGetShaderInfoLog(Result->GeometryShader, Size, &Size, Buffer);
						THAWK_ERROR("couldn't compile geometry shader\n\t%.*s", Size, Buffer);
						free(Buffer);
					}
				}

				if ((Start = Code.Find("DS_Main")).Found)
				{
					Rest::Stroke::Settle End = Code.Find("#program", Start.End);
					if (!End.Found)
						End.Start = Code.Size() - 1;

					Rest::Stroke Sub(F.Data);
					Sub.Splice(Start.End, End.Start);

					char* Buffer = Sub.Value();
					GLint Size = Sub.Size();

					Result->DomainShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
					glShaderSourceARB(Result->DomainShader, 1, &Buffer, &Size);
					glCompileShaderARB(Result->DomainShader);
					glGetShaderiv(Result->DomainShader, GL_COMPILE_STATUS, &Status);

					if (Status == GL_FALSE)
					{
						glGetShaderiv(Result->DomainShader, GL_INFO_LOG_LENGTH, &Size);
						Buffer = (char*)malloc(sizeof(char) * (Size + 1));
						glGetShaderInfoLog(Result->DomainShader, Size, &Size, Buffer);
						THAWK_ERROR("couldn't compile domain shader\n\t%.*s", Size, Buffer);
						free(Buffer);
					}
				}

				if ((Start = Code.Find("HS_Main")).Found)
				{
					Rest::Stroke::Settle End = Code.Find("#program", Start.End);
					if (!End.Found)
						End.Start = Code.Size() - 1;

					Rest::Stroke Sub(F.Data);
					Sub.Splice(Start.End, End.Start);

					char* Buffer = Sub.Value();
					GLint Size = Sub.Size();

					Result->HullShader = glCreateShader(GL_TESS_CONTROL_SHADER);
					glShaderSourceARB(Result->HullShader, 1, &Buffer, &Size);
					glCompileShaderARB(Result->HullShader);
					glGetShaderiv(Result->HullShader, GL_COMPILE_STATUS, &Status);

					if (Status == GL_FALSE)
					{
						glGetShaderiv(Result->HullShader, GL_INFO_LOG_LENGTH, &Size);
						Buffer = (char*)malloc(sizeof(char) * (Size + 1));
						glGetShaderInfoLog(Result->HullShader, Size, &Size, Buffer);
						THAWK_ERROR("couldn't compile hull shader\n\t%.*s", Size, Buffer);
						free(Buffer);
					}
				}

				if ((Start = Code.Find("CS_Main")).Found)
				{
					Rest::Stroke::Settle End = Code.Find("#program", Start.End);
					if (!End.Found)
						End.Start = Code.Size() - 1;

					Rest::Stroke Sub(F.Data);
					Sub.Splice(Start.End, End.Start);

					char* Buffer = Sub.Value();
					GLint Size = Sub.Size();

					Result->ComputeShader = glCreateShader(GL_COMPUTE_SHADER);
					glShaderSourceARB(Result->ComputeShader, 1, &Buffer, &Size);
					glCompileShaderARB(Result->ComputeShader);
					glGetShaderiv(Result->ComputeShader, GL_COMPILE_STATUS, &Status);

					if (Status == GL_FALSE)
					{
						glGetShaderiv(Result->ComputeShader, GL_INFO_LOG_LENGTH, &Size);
						Buffer = (char*)malloc(sizeof(char) * (Size + 1));
						glGetShaderInfoLog(Result->ComputeShader, Size, &Size, Buffer);
						THAWK_ERROR("couldn't compile compute shader\n\t%.*s", Size, Buffer);
						free(Buffer);
					}
				}

				Result->Program = glCreateProgram();
				if (Result->VertexShader != GL_INVALID_VALUE)
					glAttachShader(Result->Program, Result->VertexShader);

				if (Result->PixelShader != GL_INVALID_VALUE)
					glAttachShader(Result->Program, Result->PixelShader);

				if (Result->GeometryShader != GL_INVALID_VALUE)
					glAttachShader(Result->Program, Result->GeometryShader);

				if (Result->DomainShader != GL_INVALID_VALUE)
					glAttachShader(Result->Program, Result->DomainShader);

				if (Result->HullShader != GL_INVALID_VALUE)
					glAttachShader(Result->Program, Result->HullShader);

				if (Result->ComputeShader != GL_INVALID_VALUE)
					glAttachShader(Result->Program, Result->ComputeShader);

				glLinkProgramARB(Result->Program);
				glGetProgramiv(Result->Program, GL_LINK_STATUS, &Status);

				if (Status == GL_TRUE)
				{
					GLuint AnimationId = glGetUniformBlockIndex(Result->Program, "Animation");
					if (AnimationId != GL_INVALID_INDEX)
						glUniformBlockBinding(Result->Program, AnimationId, 0);

					GLuint ObjectId = glGetUniformBlockIndex(Result->Program, "Object");
					if (ObjectId != GL_INVALID_INDEX)
						glUniformBlockBinding(Result->Program, ObjectId, 1);

					GLuint ViewerId = glGetUniformBlockIndex(Result->Program, "Viewer");
					if (ViewerId != GL_INVALID_INDEX)
						glUniformBlockBinding(Result->Program, ViewerId, 2);

					GLuint RenderConstantId = glGetUniformBlockIndex(Result->Program, "RenderConstant");
					if (RenderConstantId != GL_INVALID_INDEX)
						glUniformBlockBinding(Result->Program, RenderConstantId, 3);

					return Result;
				}

				GLint Size = 0;
				glGetProgramiv(Result->Program, GL_INFO_LOG_LENGTH, &Size);

				char* Buffer = (char*)malloc(sizeof(char) * (Size + 1));
				glGetProgramInfoLog(Result->Program, Size, &Size, Buffer);
				THAWK_ERROR("couldn't link shaders\n\t%.*s", Size, Buffer);
				free(Buffer);

				return Result;
			}
			ElementBuffer* OGLDevice::CreateElementBuffer(const ElementBuffer::Desc& I)
			{
				OGLElementBuffer* Result = new OGLElementBuffer(I);
				Result->Flags = OGLDevice::GetResourceBind(I.BindFlags);

				glGenBuffers(1, &Result->Resource);
				glBindBuffer(Result->Flags, Result->Resource);

				if (I.UseSubresource)
					glBufferData(Result->Flags, I.ElementCount * I.ElementWidth, I.Elements, GL_STATIC_DRAW);

				return Result;
			}
			StructureBuffer* OGLDevice::CreateStructureBuffer(const StructureBuffer::Desc& I)
			{
				OGLStructureBuffer* Result = new OGLStructureBuffer(I);
				Result->Flags = OGLDevice::GetResourceBind(I.BindFlags);

				glGenBuffers(1, &Result->Resource);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, Result->Resource);
				glBufferData(GL_SHADER_STORAGE_BUFFER, I.ElementWidth * I.ElementCount, I.Elements, GL_DYNAMIC_DRAW);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, Result->Resource);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
				return Result;
			}
			MeshBuffer* OGLDevice::CreateMeshBuffer(const MeshBuffer::Desc& I)
			{
				ElementBuffer::Desc F = ElementBuffer::Desc();
				F.AccessFlags = I.AccessFlags;
				F.Usage = I.Usage;
				F.BindFlags = ResourceBind_Vertex_Buffer;
				F.ElementCount = (unsigned int)I.Elements.size();
				F.UseSubresource = true;
				F.Elements = (void*)I.Elements.data();
				F.ElementWidth = sizeof(Compute::Vertex);

				OGLMeshBuffer* Result = new OGLMeshBuffer(I);
				Result->VertexBuffer = CreateElementBuffer(F);

				F = ElementBuffer::Desc();
				F.AccessFlags = I.AccessFlags;
				F.Usage = I.Usage;
				F.BindFlags = ResourceBind_Index_Buffer;
				F.ElementCount = (unsigned int)I.Indices.size();
				F.ElementWidth = sizeof(int);
				F.Elements = (void*)I.Indices.data();
				F.UseSubresource = true;

				Result->IndexBuffer = CreateElementBuffer(F);
				return Result;
			}
			SkinMeshBuffer* OGLDevice::CreateSkinMeshBuffer(const SkinMeshBuffer::Desc& I)
			{
				ElementBuffer::Desc F = ElementBuffer::Desc();
				F.AccessFlags = I.AccessFlags;
				F.Usage = I.Usage;
				F.BindFlags = ResourceBind_Vertex_Buffer;
				F.ElementCount = (unsigned int)I.Elements.size();
				F.UseSubresource = true;
				F.Elements = (void*)I.Elements.data();
				F.ElementWidth = sizeof(Compute::SkinVertex);

				OGLSkinMeshBuffer* Result = new OGLSkinMeshBuffer(I);
				Result->VertexBuffer = CreateElementBuffer(F);

				F = ElementBuffer::Desc();
				F.AccessFlags = I.AccessFlags;
				F.Usage = I.Usage;
				F.BindFlags = ResourceBind_Index_Buffer;
				F.ElementCount = (unsigned int)I.Indices.size();
				F.ElementWidth = sizeof(int);
				F.Elements = (void*)I.Indices.data();
				F.UseSubresource = true;

				Result->IndexBuffer = CreateElementBuffer(F);
				return Result;
			}
			InstanceBuffer* OGLDevice::CreateInstanceBuffer(const InstanceBuffer::Desc& I)
			{
				OGLInstanceBuffer* Result = new OGLInstanceBuffer(I);
				glGenBuffers(1, &Result->Resource);
				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, Result->Resource);
				glBufferData(GL_SHADER_STORAGE_BUFFER, I.ElementLimit * I.ElementWidth, nullptr, GL_DYNAMIC_DRAW);

				return Result;
			}
			Texture2D* OGLDevice::CreateTexture2D()
			{
				return new OGLTexture2D();
			}
			Texture2D* OGLDevice::CreateTexture2D(const Texture2D::Desc& I)
			{
				OGLTexture2D* Result = new OGLTexture2D(I);
				glGenTextures(1, &Result->Resource);
				glBindTexture(GL_TEXTURE_2D, Result->Resource);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, OGLDevice::GetFormat(Result->FormatMode), Result->Width, Result->Height, 0, OGLDevice::GetFormat(Result->FormatMode), GL_UNSIGNED_BYTE, I.Data);

				if (!GenerateTexture(Result))
					return Result;

				if (Result->MipLevels != 0)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}

				return Result;
			}
			Texture3D* OGLDevice::CreateTexture3D()
			{
				return new OGLTexture3D();
			}
			Texture3D* OGLDevice::CreateTexture3D(const Texture3D::Desc& I)
			{
				OGLTexture3D* Result = new OGLTexture3D();
				glGenTextures(1, &Result->Resource);
				glBindTexture(GL_TEXTURE_3D, Result->Resource);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
				glTexImage3D(GL_TEXTURE_3D, 0, OGLDevice::GetFormat(Result->FormatMode), Result->Width, Result->Height, Result->Depth, 0, OGLDevice::GetFormat(Result->FormatMode), GL_UNSIGNED_BYTE, nullptr);

				if (Result->MipLevels != 0)
				{
					glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glGenerateMipmap(GL_TEXTURE_3D);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}

				return Result;
			}
			TextureCube* OGLDevice::CreateTextureCube()
			{
				return new OGLTextureCube();
			}
			TextureCube* OGLDevice::CreateTextureCube(const TextureCube::Desc& I)
			{
				OGLTextureCube* Result = new OGLTextureCube(I);
				glGenTextures(1, &Result->Resource);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				GLint Format = OGLDevice::GetFormat(Result->FormatMode);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, Format, Result->Width, Result->Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, Format, Result->Width, Result->Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, Format, Result->Width, Result->Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, Format, Result->Width, Result->Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, Format, Result->Width, Result->Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, Format, Result->Width, Result->Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);

				if (Result->MipLevels != 0)
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}

				return Result;
			}
			TextureCube* OGLDevice::CreateTextureCube(Texture2D* Resource[6])
			{
				if (!Resource[0] || !Resource[1] || !Resource[2] || !Resource[3] || !Resource[4] || !Resource[5])
				{
					THAWK_ERROR("couldn't create texture cube without proper mapping");
					return nullptr;
				}

				void* Resources[6];
				for (unsigned int i = 0; i < 6; i++)
					Resources[i] = (void*)Resource[i]->As<OGLTexture2D>();

				return CreateTextureCubeInternal(Resources);
			}
			TextureCube* OGLDevice::CreateTextureCube(Texture2D* Resource)
			{
				OGLTexture2D* IResource = (OGLTexture2D*)Resource;
				if (!IResource)
				{
					THAWK_ERROR("couldn't create texture cube without proper mapping");
					return nullptr;
				}

				unsigned int Width = IResource->Width / 4;
				unsigned int Height = Width;
				unsigned int MipLevels = GetMipLevel(Width, Height);

				if (IResource->Width % 4 != 0 || IResource->Height % 3 != 0)
				{
					THAWK_ERROR("couldn't create texture cube because width or height cannot be not divided");
					return nullptr;
				}

				OGLTextureCube* Result = new OGLTextureCube();
				glGenTextures(1, &Result->Resource);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				GLint Format = OGLDevice::GetFormat(Result->FormatMode);
				GLsizei Size = sizeof(GLubyte) * Width * Height;
				GLubyte* Pixels = (GLubyte*)malloc(Size);
				Result->FormatMode = IResource->FormatMode;
				Result->Width = IResource->Width;
				Result->Height = IResource->Height;
				Result->MipLevels = IResource->MipLevels;

				glBindTexture(GL_TEXTURE_CUBE_MAP, IResource->Resource);
				glGetTextureSubImage(GL_TEXTURE_2D, 0, Width * 2, Height, 0, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Size, Pixels);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, IResource->MipLevels, Format, IResource->Width, IResource->Height, 0, Format, GL_UNSIGNED_BYTE, Pixels);

				glBindTexture(GL_TEXTURE_CUBE_MAP, IResource->Resource);
				glGetTextureSubImage(GL_TEXTURE_2D, 0, Width, Height * 2, 0, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Size, Pixels);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, IResource->MipLevels, Format, IResource->Width, IResource->Height, 0, Format, GL_UNSIGNED_BYTE, Pixels);

				glBindTexture(GL_TEXTURE_CUBE_MAP, IResource->Resource);
				glGetTextureSubImage(GL_TEXTURE_2D, 0, Width * 4, Height, 0, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Size, Pixels);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, IResource->MipLevels, Format, IResource->Width, IResource->Height, 0, Format, GL_UNSIGNED_BYTE, Pixels);

				glBindTexture(GL_TEXTURE_CUBE_MAP, IResource->Resource);
				glGetTextureSubImage(GL_TEXTURE_2D, 0, 0, Height, 0, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Size, Pixels);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, IResource->MipLevels, Format, IResource->Width, IResource->Height, 0, Format, GL_UNSIGNED_BYTE, Pixels);

				glBindTexture(GL_TEXTURE_CUBE_MAP, IResource->Resource);
				glGetTextureSubImage(GL_TEXTURE_2D, 0, Width, 0, 0, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Size, Pixels);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, IResource->MipLevels, Format, IResource->Width, IResource->Height, 0, Format, GL_UNSIGNED_BYTE, Pixels);

				glBindTexture(GL_TEXTURE_CUBE_MAP, IResource->Resource);
				glGetTextureSubImage(GL_TEXTURE_2D, 0, Width, Height, 0, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Size, Pixels);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, IResource->MipLevels, Format, IResource->Width, IResource->Height, 0, Format, GL_UNSIGNED_BYTE, Pixels);

				free(Pixels);
				if (IResource->MipLevels != 0)
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}

				return Result;
			}
			TextureCube* OGLDevice::CreateTextureCubeInternal(void* Basis[6])
			{
				if (!Basis[0] || !Basis[1] || !Basis[2] || !Basis[3] || !Basis[4] || !Basis[5])
				{
					THAWK_ERROR("couldn't create texture cube without proper mapping");
					return nullptr;
				}

				OGLTexture2D* Resources[6];
				for (unsigned int i = 0; i < 6; i++)
					Resources[i] = (OGLTexture2D*)Basis[i];

				OGLTextureCube* Result = new OGLTextureCube();
				glGenTextures(1, &Result->Resource);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				GLint Format = OGLDevice::GetFormat(Result->FormatMode);
				GLubyte* Pixels = (GLubyte*)malloc(sizeof(GLubyte) * Resources[0]->Width * Resources[0]->Height);
				Result->FormatMode = Resources[0]->FormatMode;
				Result->Width = Resources[0]->Width;
				Result->Height = Resources[0]->Height;
				Result->MipLevels = Resources[0]->MipLevels;

				for (unsigned int i = 0; i < 6; i++)
				{
					OGLTexture2D* Ref = Resources[i];
					glBindTexture(GL_TEXTURE_CUBE_MAP, Ref->Resource);
					glGetTexImage(GL_TEXTURE_2D, 0, Format, GL_UNSIGNED_BYTE, Pixels);
					glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Resource);
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Ref->MipLevels, Format, Ref->Width, Ref->Height, 0, Format, GL_UNSIGNED_BYTE, Pixels);
				}

				free(Pixels);
				if (Resources[0]->MipLevels != 0)
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				}

				return Result;
			}
			DepthBuffer* OGLDevice::CreateDepthBuffer(const DepthBuffer::Desc& I)
			{
				OGLDepthBuffer* Result = new OGLDepthBuffer(I);
				glGenFramebuffers(1, &Result->FrameBuffer);
				glGenRenderbuffers(1, &Result->DepthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, Result->DepthBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, I.Width, I.Height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Result->DepthBuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
				Result->View.Width = (float)I.Width;
				Result->View.Height = (float)I.Height;
				Result->View.TopLeftX = 0.0f;
				Result->View.TopLeftY = 0.0f;
				Result->View.MinDepth = 0.0f;
				Result->View.MaxDepth = 1.0f;

				return Result;
			}
			RenderTarget2D* OGLDevice::CreateRenderTarget2D(const RenderTarget2D::Desc& I)
			{
				OGLRenderTarget2D* Result = new OGLRenderTarget2D(I);
				GLenum DrawBuffers = GL_COLOR_ATTACHMENT0;

				if (!I.RenderSurface)
				{
					GLenum Format = OGLDevice::GetFormat(I.FormatMode);
					glGenFramebuffers(1, &Result->FrameBuffer);
					glGenTextures(1, &Result->Texture);
					glBindTexture(GL_TEXTURE_2D, Result->Texture);
					glTexImage2D(GL_TEXTURE_2D, I.MipLevels, Format, I.Width, I.Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glBindFramebuffer(GL_FRAMEBUFFER, Result->FrameBuffer);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Result->Texture, 0);
					glGenRenderbuffers(1, &Result->DepthBuffer);
					glBindRenderbuffer(GL_RENDERBUFFER, Result->DepthBuffer);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, I.Width, I.Height);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Result->DepthBuffer);
					glDrawBuffers(1, &DrawBuffers);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
				else if (I.RenderSurface != (void*)this)
				{
					Result->Texture = *(GLuint*)I.RenderSurface;
					glGenFramebuffers(1, &Result->FrameBuffer);
					glBindFramebuffer(GL_FRAMEBUFFER, Result->FrameBuffer);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Result->Texture, 0);
					glGenRenderbuffers(1, &Result->DepthBuffer);
					glBindRenderbuffer(GL_RENDERBUFFER, Result->DepthBuffer);
					glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, I.Width, I.Height);
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Result->DepthBuffer);
					glDrawBuffers(1, &DrawBuffers);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}

				Result->View.Width = (float)I.Width;
				Result->View.Height = (float)I.Height;
				Result->View.TopLeftX = 0.0f;
				Result->View.TopLeftY = 0.0f;
				Result->View.MinDepth = 0.0f;
				Result->View.MaxDepth = 1.0f;

				return Result;
			}
			MultiRenderTarget2D* OGLDevice::CreateMultiRenderTarget2D(const MultiRenderTarget2D::Desc& I)
			{
				OGLMultiRenderTarget2D* Result = new OGLMultiRenderTarget2D(I);
				glGenFramebuffers(1, &Result->FrameBuffer);
				glBindFramebuffer(GL_FRAMEBUFFER, Result->FrameBuffer);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glGenRenderbuffers(1, &Result->DepthBuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, Result->DepthBuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, I.Width, I.Height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, Result->DepthBuffer);

				GLenum DrawBuffers[8] = { GL_INVALID_VALUE };
				for (unsigned int i = 0; i < (unsigned int)I.SVTarget; i++)
				{
					GLenum Format = OGLDevice::GetFormat(I.FormatMode[i]);
					glGenTextures(1, &Result->Texture[i]);
					glBindTexture(GL_TEXTURE_2D, Result->Texture[i]);
					glTexImage2D(GL_TEXTURE_2D, I.MipLevels, Format, I.Width, I.Height, 0, Format, GL_UNSIGNED_BYTE, nullptr);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Result->Texture[i], 0);
					DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
				}

				glDrawBuffers(I.SVTarget, DrawBuffers);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				Result->View.Width = (float)I.Width;
				Result->View.Height = (float)I.Height;
				Result->View.TopLeftX = 0.0f;
				Result->View.TopLeftY = 0.0f;
				Result->View.MinDepth = 0.0f;
				Result->View.MaxDepth = 1.0f;

				return Result;
			}
			RenderTargetCube* OGLDevice::CreateRenderTargetCube(const RenderTargetCube::Desc& I)
			{
				OGLRenderTargetCube* Result = new OGLRenderTargetCube(I);
				glGenFramebuffers(1, &Result->FrameBuffer);
				glGenTextures(1, &Result->DepthTexture);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->DepthTexture);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);

				GLenum DrawBuffers = GL_COLOR_ATTACHMENT0;
				for (unsigned int i = 0; i < 6; i++)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, I.Size, I.Size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Result->DepthTexture, 0);
				glGenTextures(1, &Result->Texture);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Texture);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				GLenum Format = OGLDevice::GetFormat(I.FormatMode);
				for (unsigned int i = 0; i < 6; i++)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Format, I.Size, I.Size, 0, Format, GL_UNSIGNED_BYTE, 0);

				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Result->Texture, 0);
				glDrawBuffers(1, &DrawBuffers);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				Result->View.Width = (float)I.Size;
				Result->View.Height = (float)I.Size;
				Result->View.TopLeftX = 0.0f;
				Result->View.TopLeftY = 0.0f;
				Result->View.MinDepth = 0.0f;
				Result->View.MaxDepth = 1.0f;

				return Result;
			}
			MultiRenderTargetCube* OGLDevice::CreateMultiRenderTargetCube(const MultiRenderTargetCube::Desc& I)
			{
				OGLMultiRenderTargetCube* Result = new OGLMultiRenderTargetCube(I);
				glGenFramebuffers(1, &Result->FrameBuffer);
				glGenTextures(1, &Result->DepthTexture);
				glBindTexture(GL_TEXTURE_CUBE_MAP, Result->DepthTexture);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);

				GLenum DrawBuffers[8] = { GL_INVALID_VALUE };
				for (unsigned int i = 0; i < 6; i++)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, I.Size, I.Size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Result->DepthTexture, 0);
				for (unsigned int j = 0; j < (unsigned int)I.Target; j++)
				{
					glGenTextures(1, &Result->Texture[j]);
					glBindTexture(GL_TEXTURE_CUBE_MAP, Result->Texture[j]);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

					GLenum Format = OGLDevice::GetFormat(I.FormatMode[j]);
					for (unsigned int i = 0; i < 6; i++)
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Format, I.Size, I.Size, 0, Format, GL_UNSIGNED_BYTE, 0);

					glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + j, Result->Texture[j], 0);
					DrawBuffers[j] = GL_COLOR_ATTACHMENT0 + j;
				}
				
				glDrawBuffers(I.Target, DrawBuffers);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				Result->View.Width = (float)I.Size;
				Result->View.Height = (float)I.Size;
				Result->View.TopLeftX = 0.0f;
				Result->View.TopLeftY = 0.0f;
				Result->View.MinDepth = 0.0f;
				Result->View.MaxDepth = 1.0f;

				return Result;
			}
			Query* OGLDevice::CreateQuery(const Query::Desc& I)
			{
				OGLQuery* Result = new OGLQuery();
				Result->Predicate = I.Predicate;
				glGenQueries(1, &Result->Async);
				
				return Result;
			}
			PrimitiveTopology OGLDevice::GetPrimitiveTopology()
			{
				return Topology;
			}
			ShaderModel OGLDevice::GetSupportedShaderModel()
			{
				const char* Version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
				if (!Version)
					return ShaderModel_Invalid;

				int Major, Minor;
				if (sscanf(Version, "%i.%i", &Major, &Minor) != 2)
					return ShaderModel_Invalid;

				if (Major >= 1 && Major <= 2)
				{
					if (Minor <= 10)
						return ShaderModel_GLSL_1_1_0;

					if (Minor <= 20)
						return ShaderModel_GLSL_1_2_0;

					if (Minor <= 30)
						return ShaderModel_GLSL_1_3_0;

					if (Minor <= 40)
						return ShaderModel_GLSL_1_4_0;

					if (Minor <= 50)
						return ShaderModel_GLSL_1_5_0;

					return ShaderModel_GLSL_1_5_0;
				}
				else if (Major >= 2 && Major <= 3)
				{
					if (Minor <= 30)
						return ShaderModel_GLSL_3_3_0;

					return ShaderModel_GLSL_1_5_0;
				}
				else if (Major >= 3 && Major <= 4)
				{
					if (Minor <= 10)
						return ShaderModel_GLSL_4_1_0;

					if (Minor <= 20)
						return ShaderModel_GLSL_4_2_0;

					if (Minor <= 30)
						return ShaderModel_GLSL_4_3_0;

					if (Minor <= 40)
						return ShaderModel_GLSL_4_4_0;

					if (Minor <= 50)
						return ShaderModel_GLSL_4_5_0;

					if (Minor <= 60)
						return ShaderModel_GLSL_4_6_0;

					return ShaderModel_GLSL_4_6_0;
				}

				return ShaderModel_Invalid;
			}
			void* OGLDevice::GetBackBuffer()
			{
				GLuint Resource;
				glGenTextures(1, &Resource);
				glBindTexture(GL_TEXTURE_2D, Resource);
				glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (GLsizei)RenderTarget->GetWidth(), (GLsizei)RenderTarget->GetHeight());

				return (void*)(intptr_t)Resource;
			}
			void* OGLDevice::GetBackBufferMSAA()
			{
				return GetBackBuffer();
			}
			void* OGLDevice::GetBackBufferNoAA()
			{
				return GetBackBuffer();
			}
			void* OGLDevice::GetDevice()
			{
				return Context;
			}
			void* OGLDevice::GetContext()
			{
				return Context;
			}
			bool OGLDevice::IsValid()
			{
				return BasicEffect != nullptr;
			}
			const char* OGLDevice::GetShaderVersion()
			{
				return ShaderVersion;
			}
			void OGLDevice::CopyConstantBuffer(GLuint Id, void* Buffer, size_t Size)
			{
				if (!Buffer || !Size)
					return;

				glBindBuffer(GL_UNIFORM_BUFFER, Id);
				GLvoid* Data = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
				memcpy(Data, Buffer, Size);
				glUnmapBuffer(GL_UNIFORM_BUFFER);
			}
			int OGLDevice::CreateConstantBuffer(GLuint* Buffer, size_t Size)
			{
				if (!Buffer)
					return -1;

				glGenBuffers(1, Buffer);
				glBindBuffer(GL_UNIFORM_BUFFER, *Buffer);
				glBufferData(GL_UNIFORM_BUFFER, Size, nullptr, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_UNIFORM_BUFFER, 0);

				return (int)*Buffer;
			}
			std::string OGLDevice::CompileState(GLuint Handle)
			{
				GLint Stat = 0, Size = 0;
				glGetShaderiv(Handle, GL_COMPILE_STATUS, &Stat);
				glGetShaderiv(Handle, GL_INFO_LOG_LENGTH, &Size);

				if ((GLboolean)Stat == GL_TRUE || !Size)
					return "";

				GLchar* Buffer = (GLchar*)malloc(sizeof(GLchar) * Size);
				glGetShaderInfoLog(Handle, Size, NULL, Buffer);

				std::string Result((char*)Buffer, Size);
				free(Buffer);

				return Result;
			}
			GLenum OGLDevice::GetFormat(Format Value)
			{
				switch (Value)
				{
					case Format_R32G32B32A32_Typeless:
						return GL_RGBA32UI;
					case Format_R32G32B32A32_Float:
						return GL_RGBA32F;
					case Format_R32G32B32A32_Uint:
						return GL_RGBA32UI;
					case Format_R32G32B32A32_Sint:
						return GL_RGBA32I;
					case Format_R32G32B32_Typeless:
						return GL_RGB32UI;
					case Format_R32G32B32_Float:
						return GL_RGB32F;
					case Format_R32G32B32_Uint:
						return GL_RGB32UI;
					case Format_R32G32B32_Sint:
						return GL_RGB32I;
					case Format_R16G16B16A16_Typeless:
						return GL_RGBA16UI;
					case Format_R16G16B16A16_Float:
						return GL_RGBA16F;
					case Format_R16G16B16A16_Unorm:
						return GL_RGBA16;
					case Format_R16G16B16A16_Uint:
						return GL_RGBA16UI;
					case Format_R16G16B16A16_Snorm:
						return GL_RGBA16I;
					case Format_R16G16B16A16_Sint:
						return GL_RGBA16I;
					case Format_R32G32_Typeless:
						return GL_RG16UI;
					case Format_R32G32_Float:
						return GL_RG16F;
					case Format_R32G32_Uint:
						return GL_RG16UI;
					case Format_R32G32_Sint:
						return GL_RG16I;
					case Format_R32G8X24_Typeless:
						return GL_R32UI;
					case Format_D32_Float_S8X24_Uint:
						return GL_R32UI;
					case Format_R32_Float_X8X24_Typeless:
						return GL_R32UI;
					case Format_X32_Typeless_G8X24_Uint:
						return GL_R32UI;
					case Format_R10G10B10A2_Typeless:
						return GL_RGB10_A2;
					case Format_R10G10B10A2_Unorm:
						return GL_RGB10_A2;
					case Format_R10G10B10A2_Uint:
						return GL_RGB10_A2UI;
					case Format_R11G11B10_Float:
						return GL_RGB12;
					case Format_R8G8B8A8_Typeless:
						return GL_RGBA8UI;
					case Format_R8G8B8A8_Unorm:
						return GL_RGBA;
					case Format_R8G8B8A8_Unorm_SRGB:
						return GL_RGBA;
					case Format_R8G8B8A8_Uint:
						return GL_RGBA8UI;
					case Format_R8G8B8A8_Snorm:
						return GL_RGBA8I;
					case Format_R8G8B8A8_Sint:
						return GL_RGBA8I;
					case Format_R16G16_Typeless:
						return GL_RG16UI;
					case Format_R16G16_Float:
						return GL_RG16F;
					case Format_R16G16_Unorm:
						return GL_RG16;
					case Format_R16G16_Uint:
						return GL_RG16UI;
					case Format_R16G16_Snorm:
						return GL_RG16_SNORM;
					case Format_R16G16_Sint:
						return GL_RG16I;
					case Format_R32_Typeless:
						return GL_R32UI;
					case Format_D32_Float:
						return GL_R32F;
					case Format_R32_Float:
						return GL_R32F;
					case Format_R32_Uint:
						return GL_R32UI;
					case Format_R32_Sint:
						return GL_R32I;
					case Format_R24G8_Typeless:
						return GL_R32UI;
					case Format_D24_Unorm_S8_Uint:
						return GL_R32UI;
					case Format_R24_Unorm_X8_Typeless:
						return GL_R32UI;
					case Format_X24_Typeless_G8_Uint:
						return GL_R32UI;
					case Format_R8G8_Typeless:
						return GL_RG8UI;
					case Format_R8G8_Unorm:
						return GL_RG8;
					case Format_R8G8_Uint:
						return GL_RG8UI;
					case Format_R8G8_Snorm:
						return GL_RG8I;
					case Format_R8G8_Sint:
						return GL_RG8I;
					case Format_R16_Typeless:
						return GL_R16UI;
					case Format_R16_Float:
						return GL_R16F;
					case Format_D16_Unorm:
						return GL_R16;
					case Format_R16_Unorm:
						return GL_R16;
					case Format_R16_Uint:
						return GL_R16UI;
					case Format_R16_Snorm:
						return GL_R16I;
					case Format_R16_Sint:
						return GL_R16I;
					case Format_R8_Typeless:
						return GL_R8UI;
					case Format_R8_Unorm:
						return GL_R8;
					case Format_R8_Uint:
						return GL_R8UI;
					case Format_R8_Snorm:
						return GL_R8I;
					case Format_R8_Sint:
						return GL_R8I;
					case Format_A8_Unorm:
						return GL_ALPHA8_EXT;
					case Format_R1_Unorm:
						return GL_R8;
					case Format_R9G9B9E5_Share_Dexp:
						return GL_RGB9_E5;
					case Format_R8G8_B8G8_Unorm:
						return GL_RGB;
					default:
						break;
				}

				return GL_RGB;
			}
			GLenum OGLDevice::GetTextureAddress(TextureAddress Value)
			{
				switch (Value)
				{
					case TextureAddress_Wrap:
						return GL_REPEAT;
					case TextureAddress_Mirror:
						return GL_MIRRORED_REPEAT;
					case TextureAddress_Clamp:
						return GL_CLAMP_TO_EDGE;
					case TextureAddress_Border:
						return GL_CLAMP_TO_BORDER;
					case TextureAddress_Mirror_Once:
						return GL_MIRROR_CLAMP_TO_EDGE;
				}

				return GL_REPEAT;
			}
			GLenum OGLDevice::GetComparison(Comparison Value)
			{
				switch (Value)
				{
					case Comparison_Never:
						return GL_NEVER;
					case Comparison_Less:
						return GL_LESS;
					case Comparison_Equal:
						return GL_EQUAL;
					case Comparison_Less_Equal:
						return GL_LEQUAL;
					case Comparison_Greater:
						return GL_GREATER;
					case Comparison_Not_Equal:
						return GL_NOTEQUAL;
					case Comparison_Greater_Equal:
						return GL_GEQUAL;
					case Comparison_Always:
						return GL_ALWAYS;
				}

				return GL_ALWAYS;
			}
			GLenum OGLDevice::GetPixelFilter(PixelFilter Value, bool Mag)
			{
				switch (Value)
				{
					case PixelFilter_Min_Mag_Mip_Point:
					case PixelFilter_Compare_Min_Mag_Mip_Point:
						return GL_NEAREST;
					case PixelFilter_Min_Mag_Point_Mip_Linear:
					case PixelFilter_Compare_Min_Mag_Point_Mip_Linear:
						return (Mag ? GL_NEAREST : GL_NEAREST_MIPMAP_LINEAR);
					case PixelFilter_Min_Point_Mag_Linear_Mip_Point:
					case PixelFilter_Compare_Min_Point_Mag_Linear_Mip_Point:
						return (Mag ? GL_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
					case PixelFilter_Min_Point_Mag_Mip_Linear:
					case PixelFilter_Compare_Min_Point_Mag_Mip_Linear:
						return (Mag ? GL_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
					case PixelFilter_Min_Linear_Mag_Mip_Point:
					case PixelFilter_Compare_Min_Linear_Mag_Mip_Point:
						return (Mag ? GL_NEAREST : GL_LINEAR_MIPMAP_NEAREST);
					case PixelFilter_Min_Linear_Mag_Point_Mip_Linear:
					case PixelFilter_Compare_Min_Linear_Mag_Point_Mip_Linear:
						return (Mag ? GL_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
					case PixelFilter_Min_Mag_Linear_Mip_Point:
					case PixelFilter_Compare_Min_Mag_Linear_Mip_Point:
						return (Mag ? GL_LINEAR : GL_LINEAR_MIPMAP_NEAREST);
					case PixelFilter_Min_Mag_Mip_Linear:
					case PixelFilter_Compare_Min_Mag_Mip_Linear:
						return (Mag ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
					case PixelFilter_Anistropic:
					case PixelFilter_Compare_Anistropic:
						return GL_LINEAR;
				}

				return GL_LINEAR;
			}
			GLenum OGLDevice::GetBlendOperation(BlendOperation Value)
			{
				switch (Value)
				{
					case BlendOperation_Add:
						return GL_FUNC_ADD;
					case BlendOperation_Subtract:
						return GL_FUNC_SUBTRACT;
					case BlendOperation_Subtract_Reverse:
						return GL_FUNC_REVERSE_SUBTRACT;
					case BlendOperation_Min:
						return GL_MIN;
					case BlendOperation_Max:
						return GL_MAX;
				}

				return GL_FUNC_ADD;
			}
			GLenum OGLDevice::GetBlend(Blend Value)
			{
				switch (Value)
				{
					case Blend_Zero:
						return GL_ZERO;
					case Blend_One:
						return GL_ONE;
					case Blend_Source_Color:
						return GL_SRC_COLOR;
					case Blend_Source_Color_Invert:
						return GL_ONE_MINUS_SRC_COLOR;
					case Blend_Source_Alpha:
						return GL_SRC_ALPHA;
					case Blend_Source_Alpha_Invert:
						return GL_ONE_MINUS_SRC_ALPHA;
					case Blend_Destination_Alpha:
						return GL_DST_ALPHA;
					case Blend_Destination_Alpha_Invert:
						return GL_ONE_MINUS_DST_ALPHA;
					case Blend_Destination_Color:
						return GL_DST_COLOR;
					case Blend_Destination_Color_Invert:
						return GL_ONE_MINUS_DST_COLOR;
					case Blend_Source_Alpha_SAT:
						return GL_SRC_ALPHA_SATURATE;
					case Blend_Blend_Factor:
						return GL_CONSTANT_COLOR;
					case Blend_Blend_Factor_Invert:
						return GL_ONE_MINUS_CONSTANT_ALPHA;
					case Blend_Source1_Color:
						return GL_SRC1_COLOR;
					case Blend_Source1_Color_Invert:
						return GL_ONE_MINUS_SRC1_COLOR;
					case Blend_Source1_Alpha:
						return GL_SRC1_ALPHA;
					case Blend_Source1_Alpha_Invert:
						return GL_ONE_MINUS_SRC1_ALPHA;
				}

				return GL_ONE;
			}
			GLenum OGLDevice::GetStencilOperation(StencilOperation Value)
			{
				switch (Value)
				{
					case StencilOperation_Keep:
						return GL_KEEP;
					case StencilOperation_Zero:
						return GL_ZERO;
					case StencilOperation_Replace:
						return GL_REPLACE;
					case StencilOperation_SAT_Add:
						return GL_INCR_WRAP;
					case StencilOperation_SAT_Subtract:
						return GL_DECR_WRAP;
					case StencilOperation_Invert:
						return GL_INVERT;
					case StencilOperation_Add:
						return GL_INCR;
					case StencilOperation_Subtract:
						return GL_DECR;
				}

				return GL_KEEP;
			}
			GLenum OGLDevice::GetPrimitiveTopology(PrimitiveTopology Value)
			{
				switch (Value)
				{
					case PrimitiveTopology_Point_List:
						return GL_POINT;
					case PrimitiveTopology_Line_List:
					case PrimitiveTopology_Line_Strip:
					case PrimitiveTopology_Line_List_Adj:
					case PrimitiveTopology_Line_Strip_Adj:
						return GL_LINE;
					case PrimitiveTopology_Triangle_List:
					case PrimitiveTopology_Triangle_Strip:
					case PrimitiveTopology_Triangle_List_Adj:
					case PrimitiveTopology_Triangle_Strip_Adj:
						return GL_FILL;
					default:
						break;
				}

				return GL_FILL;
			}
			GLenum OGLDevice::GetPrimitiveTopologyDraw(PrimitiveTopology Value)
			{
				switch (Value)
				{
					case PrimitiveTopology_Point_List:
						return GL_POINTS;
					case PrimitiveTopology_Line_List:
						return GL_LINES;
					case PrimitiveTopology_Line_Strip:
						return GL_LINE_STRIP;
					case PrimitiveTopology_Line_List_Adj:
						return GL_LINES_ADJACENCY;
					case PrimitiveTopology_Line_Strip_Adj:
						return GL_LINE_STRIP_ADJACENCY;
					case PrimitiveTopology_Triangle_List:
						return GL_TRIANGLES;
					case PrimitiveTopology_Triangle_Strip:
						return GL_TRIANGLE_STRIP;
					case PrimitiveTopology_Triangle_List_Adj:
						return GL_TRIANGLES_ADJACENCY;
					case PrimitiveTopology_Triangle_Strip_Adj:
						return GL_TRIANGLE_STRIP_ADJACENCY;
					default:
						break;
				}

				return GL_TRIANGLES;
			}
			GLenum OGLDevice::GetResourceBind(ResourceBind Value)
			{
				switch (Value)
				{
					case ResourceBind_Vertex_Buffer:
						return GL_ARRAY_BUFFER;
					case ResourceBind_Index_Buffer:
						return GL_ELEMENT_ARRAY_BUFFER;
					case ResourceBind_Constant_Buffer:
						return GL_UNIFORM_BUFFER;
					case ResourceBind_Shader_Input:
						return GL_SHADER_STORAGE_BUFFER;
					case ResourceBind_Stream_Output:
						return GL_TEXTURE_BUFFER;
					case ResourceBind_Render_Target:
						return GL_DRAW_INDIRECT_BUFFER;
					case ResourceBind_Depth_Stencil:
						return GL_DRAW_INDIRECT_BUFFER;
					case ResourceBind_Unordered_Access:
						return GL_DISPATCH_INDIRECT_BUFFER;
				}

				return GL_ARRAY_BUFFER;
			}
			GLenum OGLDevice::GetResourceMap(ResourceMap Value)
			{
				switch (Value)
				{
					case ResourceMap_Read:
						return GL_READ_ONLY;
					case ResourceMap_Write:
						return GL_WRITE_ONLY;
					case ResourceMap_Read_Write:
						return GL_READ_WRITE;
					case ResourceMap_Write_Discard:
						return GL_WRITE_DISCARD_NV;
					case ResourceMap_Write_No_Overwrite:
						return GL_WRITE_ONLY;
				}

				return GL_READ_ONLY;
			}
			void OGLDevice::DebugMessage(GLenum Source, GLenum Type, GLuint Id, GLenum Severity, GLsizei Length, const GLchar* Message, const void* Data)
			{
				const char* _Source, * _Type;
				switch (Source)
				{
					case GL_DEBUG_SOURCE_API:
						_Source = "API";
						break;
					case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
						_Source = "WINDOW SYSTEM";
						break;
					case GL_DEBUG_SOURCE_SHADER_COMPILER:
						_Source = "SHADER COMPILER";
						break;
					case GL_DEBUG_SOURCE_THIRD_PARTY:
						_Source = "THIRD PARTY";
						break;
					case GL_DEBUG_SOURCE_APPLICATION:
						_Source = "APPLICATION";
						break;
					default:
						_Source = "NONE";
						break;
				}

				switch (Type)
				{
					case GL_DEBUG_TYPE_ERROR:
						_Type = "ERROR";
						break;
					case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
						_Type = "DEPRECATED";
						break;
					case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
						_Type = "UDEFINED";
						break;
					case GL_DEBUG_TYPE_PORTABILITY:
						_Type = "PORTABILITY";
						break;
					case GL_DEBUG_TYPE_PERFORMANCE:
						_Type = "PERFORMANCE";
						break;
					case GL_DEBUG_TYPE_OTHER:
						_Type = "OTHER";
						break;
					case GL_DEBUG_TYPE_MARKER:
						_Type = "MARKER";
						break;
					default:
						_Type = "NONE";
						break;
				}

				switch (Severity)
				{
					case GL_DEBUG_SEVERITY_HIGH:
						THAWK_ERROR("%s (%s:%d): %s %s", _Source, _Type, Id, Message);
						break;
					case GL_DEBUG_SEVERITY_MEDIUM:
						THAWK_WARN("%s (%s:%d): %s %s", _Source, _Type, Id, Message);
						break;
					default:
						THAWK_INFO("%s (%s:%d): %s %s", _Source, _Type, Id, Message);
						break;
				}
			}
		}
	}
}
#endif