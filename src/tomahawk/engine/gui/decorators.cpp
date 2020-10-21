#include "../gui.h"
#include <RmlUi/Core.h>
#include <Source/Core/TransformState.h>

namespace Tomahawk
{
	namespace Engine
	{
		namespace GUI
		{
			static void SetWorldViewProjection(Graphics::GraphicsDevice* Device, Rml::Element* Element, const Rml::Vector2f& Position, const Rml::Vector2f& Size, const Compute::Vector2& Mul = 1.0f)
			{
				Compute::Vector3 Scale(Size.x / 2.0f, Size.y / 2.0f);
				Compute::Vector3 Offset(Position.x + Scale.X, Position.y + Scale.Y);
				Compute::Matrix4x4& Ortho = *Subsystem::GetProjection();

				const Rml::TransformState* State = Element->GetTransformState();
				if (State != nullptr && State->GetTransform() != nullptr)
				{
					Compute::Matrix4x4 View = Subsystem::ToMatrix(State->GetTransform());
					Device->Render.WorldViewProjection = Compute::Matrix4x4::CreateTranslatedScale(Offset, Scale + Mul) * View * Ortho;
					Device->Render.World = (Compute::Matrix4x4::CreateTranslation(Compute::Vector2(Position.x, Position.y)) * View * Ortho).Invert();
				}
				else
				{
					Device->Render.WorldViewProjection = Compute::Matrix4x4::CreateTranslatedScale(Offset, Scale + Mul) * Ortho;
					Device->Render.World = (Compute::Matrix4x4::CreateTranslation(Compute::Vector2(Position.x, Position.y)) * Ortho).Invert();
				}
			}

			class BoxShadowInstancer : public Rml::DecoratorInstancer
			{
			public:
				struct RenderConstant
				{
					Compute::Vector4 Color;
					Compute::Vector4 Radius;
					Compute::Vector2 Size;
					Compute::Vector2 Position;
					Compute::Vector2 Offset;
					float Softness;
					float Padding;
				} RenderPass;

			public:
				Rml::PropertyId Color;
				Rml::PropertyId OffsetX;
				Rml::PropertyId OffsetY;
				Rml::PropertyId Softness;

			public:
				Graphics::ElementBuffer* VertexBuffer;
				Graphics::Shader* Shader;
				Graphics::GraphicsDevice* Device;

			public:
				BoxShadowInstancer(Graphics::GraphicsDevice* NewDevice);
				virtual ~BoxShadowInstancer() override;
				virtual Rml::SharedPtr<Rml::Decorator> InstanceDecorator(const Rml::String& Name, const Rml::PropertyDictionary& Props, const Rml::DecoratorInstancerInterface& Interface) override;
			}* IBoxShadow = nullptr;

			class BoxBlurInstancer : public Rml::DecoratorInstancer
			{
			public:
				struct RenderConstant
				{
					Compute::Vector4 Radius;
					Compute::Vector2 Texel;
					Compute::Vector2 Size;
					Compute::Vector2 Position;
					float Softness;
					float Alpha;
				} RenderPass;

			public:
				Rml::PropertyId Color;
				Rml::PropertyId Softness;

			public:
				Graphics::ElementBuffer* VertexBuffer;
				Graphics::Texture2D* Background;
				Graphics::Shader* Shader;
				Graphics::GraphicsDevice* Device;

			public:
				BoxBlurInstancer(Graphics::GraphicsDevice* NewDevice);
				virtual ~BoxBlurInstancer() override;
				virtual Rml::SharedPtr<Rml::Decorator> InstanceDecorator(const Rml::String& Name, const Rml::PropertyDictionary& Props, const Rml::DecoratorInstancerInterface& Interface) override;
			}* IBoxBlur = nullptr;

			class BoxShadow : public Rml::Decorator
			{
			private:
				Compute::Vector4 Color;
				Compute::Vector2 Offset;
				float Softness;

			public:
				BoxShadow(const Compute::Vector4& NewColor, const Compute::Vector2& NewOffset, float NewSoftness) : Color(NewColor.Div(255.0f)), Offset(NewOffset), Softness(NewSoftness)
				{
				}
				virtual ~BoxShadow() override
				{
				}
				virtual Rml::DecoratorDataHandle GenerateElementData(Rml::Element* Element) const override
				{
					return Rml::Decorator::INVALID_DECORATORDATAHANDLE;
				}
				virtual void ReleaseElementData(Rml::DecoratorDataHandle ElementData) const override
				{
				}
				virtual void RenderElement(Rml::Element* Element, Rml::DecoratorDataHandle ElementData) const override
				{
					Rml::Vector2f Position = Element->GetAbsoluteOffset(Rml::Box::PADDING).Round();
					Rml::Vector2f Size = Element->GetBox().GetSize(Rml::Box::PADDING).Round();
					float Alpha = Element->GetProperty<float>("opacity");
					float Radius = Softness * 0.85f;

					Graphics::GraphicsDevice* Device = IBoxShadow->Device;
					IBoxShadow->RenderPass.Position = Compute::Vector2(Position.x, Position.y);
					IBoxShadow->RenderPass.Size = Compute::Vector2(Size.x, Size.y);
					IBoxShadow->RenderPass.Color = Color;
					IBoxShadow->RenderPass.Color.W *= Alpha;
					IBoxShadow->RenderPass.Offset = Offset;
					IBoxShadow->RenderPass.Softness = Softness;
					IBoxShadow->RenderPass.Radius.X = Element->GetProperty<float>("border-bottom-left-radius");
					IBoxShadow->RenderPass.Radius.Y = Element->GetProperty<float>("border-bottom-right-radius");
					IBoxShadow->RenderPass.Radius.Z = Element->GetProperty<float>("border-top-right-radius");
					IBoxShadow->RenderPass.Radius.W = Element->GetProperty<float>("border-top-left-radius");

					SetWorldViewProjection(Device, Element, Position, Size, Offset.Abs() + Radius + 4096.0f);
					Device->SetShader(IBoxShadow->Shader, Graphics::ShaderType_Vertex | Graphics::ShaderType_Pixel);
					Device->SetBuffer(IBoxShadow->Shader, 3, Graphics::ShaderType_Pixel);
					Device->SetVertexBuffer(IBoxShadow->VertexBuffer, 0);
					Device->UpdateBuffer(IBoxShadow->Shader, &IBoxShadow->RenderPass);
					Device->UpdateBuffer(Graphics::RenderBufferType_Render);
					Device->Draw(IBoxShadow->VertexBuffer->GetElements(), 0);
				}
			};

			class BoxBlur : public Rml::Decorator
			{
			private:
				Compute::Vector4 Color;
				float Softness;

			public:
				BoxBlur(const Compute::Vector4& NewColor, float NewSoftness) : Color(NewColor.Div(255)), Softness(NewSoftness)
				{
				}
				virtual ~BoxBlur() override
				{
				}
				virtual Rml::DecoratorDataHandle GenerateElementData(Rml::Element* Element) const override
				{
					return Rml::Decorator::INVALID_DECORATORDATAHANDLE;
				}
				virtual void ReleaseElementData(Rml::DecoratorDataHandle ElementData) const override
				{
				}
				virtual void RenderElement(Rml::Element* Element, Rml::DecoratorDataHandle ElementData) const override
				{
					Graphics::Texture2D* Background = Subsystem::GetBackground();
					if (!Background)
						return;

					Rml::Vector2i Screen = Element->GetContext()->GetDimensions();
					Rml::Vector2f Position = Element->GetAbsoluteOffset(Rml::Box::PADDING).Round();
					Rml::Vector2f Size = Element->GetBox().GetSize(Rml::Box::PADDING).Round();
					float Alpha = Element->GetProperty<float>("opacity");

					Graphics::GraphicsDevice* Device = IBoxBlur->Device;
					IBoxBlur->RenderPass.Texel = Compute::Vector2(Screen.x, Screen.y);
					IBoxBlur->RenderPass.Position = Compute::Vector2(Position.x, Position.y);
					IBoxBlur->RenderPass.Size = Compute::Vector2(Size.x, Size.y);
					IBoxBlur->RenderPass.Softness = Softness;
					IBoxBlur->RenderPass.Alpha = Color.W * Alpha;
					IBoxBlur->RenderPass.Radius.X = Element->GetProperty<float>("border-bottom-left-radius");
					IBoxBlur->RenderPass.Radius.Y = Element->GetProperty<float>("border-bottom-right-radius");
					IBoxBlur->RenderPass.Radius.Z = Element->GetProperty<float>("border-top-right-radius");
					IBoxBlur->RenderPass.Radius.W = Element->GetProperty<float>("border-top-left-radius");

					SetWorldViewProjection(Device, Element, Position, Size);
					Device->Render.Diffuse = Color;
					Device->CopyTexture2D(Background, &IBoxBlur->Background);
					Device->SetTexture2D(IBoxBlur->Background, 1);
					Device->SetShader(IBoxBlur->Shader, Graphics::ShaderType_Vertex | Graphics::ShaderType_Pixel);
					Device->SetBuffer(IBoxBlur->Shader, 3, Graphics::ShaderType_Pixel);
					Device->SetVertexBuffer(IBoxBlur->VertexBuffer, 0);
					Device->UpdateBuffer(IBoxBlur->Shader, &IBoxBlur->RenderPass);
					Device->UpdateBuffer(Graphics::RenderBufferType_Render);
					Device->Draw(IBoxBlur->VertexBuffer->GetElements(), 0);
				}
			};

			BoxShadowInstancer::BoxShadowInstancer(Graphics::GraphicsDevice* NewDevice) : Device(NewDevice), Shader(nullptr)
			{
				Graphics::Shader::Desc I = Graphics::Shader::Desc();
				I.Filename = "box-shadow";

				if (Device->GetSection("geometry/gui/box-shadow", &I.Data))
				{
					Shader = Device->CreateShader(I);
					if (Shader != nullptr)
						Device->UpdateBufferSize(Shader, sizeof(RenderPass));
				}

				std::vector<Rml::Vertex> Elements;
				Elements.push_back({ Rml::Vector2f(-1.0f, -1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(-1, 0) });
				Elements.push_back({ Rml::Vector2f(-1.0f, 1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(-1, 1) });
				Elements.push_back({ Rml::Vector2f(1.0f, 1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(0, 1) });
				Elements.push_back({ Rml::Vector2f(-1.0f, -1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(-1, 0) });
				Elements.push_back({ Rml::Vector2f(1.0f, 1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(0, 1) });
				Elements.push_back({ Rml::Vector2f(1.0f, -1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(0, 0) });

				Graphics::ElementBuffer::Desc F = Graphics::ElementBuffer::Desc();
				F.AccessFlags = Graphics::CPUAccess_Invalid;
				F.Usage = Graphics::ResourceUsage_Default;
				F.BindFlags = Graphics::ResourceBind_Vertex_Buffer;
				F.ElementCount = 6;
				F.ElementWidth = sizeof(Rml::Vertex);
				F.Elements = &Elements[0];

				VertexBuffer = Device->CreateElementBuffer(F);

				Color = RegisterProperty("color", "#000").AddParser("color").GetId();
				Softness = RegisterProperty("softness", "60").AddParser("number").GetId();
				OffsetX = RegisterProperty("x", "0").AddParser("number").GetId();
				OffsetY = RegisterProperty("y", "10").AddParser("number").GetId();
				RegisterShorthand("decorator", "x, y, softness, color", Rml::ShorthandType::FallThrough);
			}
			BoxShadowInstancer::~BoxShadowInstancer()
			{
				TH_RELEASE(Shader);
				TH_RELEASE(VertexBuffer);
			}
			Rml::SharedPtr<Rml::Decorator> BoxShadowInstancer::InstanceDecorator(const Rml::String& Name, const Rml::PropertyDictionary& Props, const Rml::DecoratorInstancerInterface& Interface)
			{
				const Rml::Property* SColor = Props.GetProperty(Color);
				const Rml::Property* SSoftness = Props.GetProperty(Softness);
				const Rml::Property* SOffsetX = Props.GetProperty(OffsetX);
				const Rml::Property* SOffsetY = Props.GetProperty(OffsetY);

				Rml::Colourb IColor = SColor->Get<Rml::Colourb>();
				float ISoftness = SSoftness->Get<float>();
				float IOffsetX = SOffsetX->Get<float>();
				float IOffsetY = SOffsetY->Get<float>();

				return Rml::MakeShared<BoxShadow>(
					Compute::Vector4(IColor.red, IColor.green, IColor.blue, IColor.alpha),
					Compute::Vector2(IOffsetX, IOffsetY), ISoftness);
			}

			BoxBlurInstancer::BoxBlurInstancer(Graphics::GraphicsDevice* NewDevice) : Device(NewDevice), Shader(nullptr), Background(nullptr)
			{
				Graphics::Shader::Desc I = Graphics::Shader::Desc();
				I.Filename = "box-blur";

				if (Device->GetSection("geometry/gui/box-blur", &I.Data))
				{
					Shader = Device->CreateShader(I);
					if (Shader != nullptr)
						Device->UpdateBufferSize(Shader, sizeof(RenderPass));
				}

				std::vector<Rml::Vertex> Elements;
				Elements.push_back({ Rml::Vector2f(-1.0f, -1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(-1, 0) });
				Elements.push_back({ Rml::Vector2f(-1.0f, 1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(-1, 1) });
				Elements.push_back({ Rml::Vector2f(1.0f, 1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(0, 1) });
				Elements.push_back({ Rml::Vector2f(-1.0f, -1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(-1, 0) });
				Elements.push_back({ Rml::Vector2f(1.0f, 1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(0, 1) });
				Elements.push_back({ Rml::Vector2f(1.0f, -1.0f), Rml::Colourb(0, 0, 0, 0), Rml::Vector2f(0, 0) });

				Graphics::ElementBuffer::Desc F = Graphics::ElementBuffer::Desc();
				F.AccessFlags = Graphics::CPUAccess_Invalid;
				F.Usage = Graphics::ResourceUsage_Default;
				F.BindFlags = Graphics::ResourceBind_Vertex_Buffer;
				F.ElementCount = 6;
				F.ElementWidth = sizeof(Rml::Vertex);
				F.Elements = &Elements[0];

				VertexBuffer = Device->CreateElementBuffer(F);

				Color = RegisterProperty("color", "#fff").AddParser("color").GetId();
				Softness = RegisterProperty("softness", "8").AddParser("number").GetId();
				RegisterShorthand("decorator", "softness, color", Rml::ShorthandType::FallThrough);
			}
			BoxBlurInstancer::~BoxBlurInstancer()
			{
				TH_RELEASE(Shader);
				TH_RELEASE(VertexBuffer);
				TH_RELEASE(Background);
			}
			Rml::SharedPtr<Rml::Decorator> BoxBlurInstancer::InstanceDecorator(const Rml::String& Name, const Rml::PropertyDictionary& Props, const Rml::DecoratorInstancerInterface& Interface)
			{
				const Rml::Property* SColor = Props.GetProperty(Color);
				const Rml::Property* SSoftness = Props.GetProperty(Softness);

				Rml::Colourb IColor = SColor->Get<Rml::Colourb>();
				float ISoftness = SSoftness->Get<float>();

				return Rml::MakeShared<BoxBlur>(Compute::Vector4(IColor.red, IColor.green, IColor.blue, IColor.alpha), ISoftness);
			}

			void Subsystem::ResizeDecorators()
			{
				if (IBoxBlur != nullptr)
					TH_CLEAR(IBoxBlur->Background);
			}
			void Subsystem::CreateDecorators(Graphics::GraphicsDevice* Device)
			{
				if (!IBoxShadow)
				{
					IBoxShadow = new BoxShadowInstancer(Device);
					Rml::Factory::RegisterDecoratorInstancer("box-shadow", IBoxShadow);
				}

				if (!IBoxBlur)
				{
					IBoxBlur = new BoxBlurInstancer(Device);
					Rml::Factory::RegisterDecoratorInstancer("box-blur", IBoxBlur);
				}
			}
			void Subsystem::ReleaseDecorators()
			{
				delete IBoxShadow;
				IBoxShadow = nullptr;

				delete IBoxBlur;
				IBoxBlur = nullptr;
			}
		}
	}
}