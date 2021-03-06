#include "components.h"
#include "renderers.h"
#include "../audio/effects.h"
#include <cstddef>

namespace Tomahawk
{
	namespace Engine
	{
		namespace Components
		{
			RigidBody::RigidBody(Entity* Ref) : Component(Ref)
			{
			}
			RigidBody::~RigidBody()
			{
				TH_RELEASE(Instance);
			}
			void RigidBody::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				bool Extended = false;
				NMake::Unpack(Node->Find("extended"), &Extended);
				NMake::Unpack(Node->Find("kinematic"), &Kinematic);
				NMake::Unpack(Node->Find("manage"), &Manage);

				SceneGraph* Scene = Parent->GetScene();
				if (!Extended)
					return;

				float Mass = 0, CcdMotionThreshold = 0;
				NMake::Unpack(Node->Find("mass"), &Mass);
				NMake::Unpack(Node->Find("ccd-motion-threshold"), &CcdMotionThreshold);

				Core::Document* CV = nullptr;
				if ((CV = Node->Find("shape")) != nullptr)
				{
					std::string Path; uint64_t Type;
					if (NMake::Unpack(Node->Find("path"), &Path))
					{
						auto* Shape = Content->Load<Compute::UnmanagedShape>(Path);
						if (Shape != nullptr)
							Create(Shape->Shape, Mass, CcdMotionThreshold);
					}
					else if (!NMake::Unpack(CV->Find("type"), &Type))
					{
						std::vector<Compute::Vector3> Vertices;
						if (NMake::Unpack(CV->Find("data"), &Vertices))
						{
							btCollisionShape* Shape = Scene->GetSimulator()->CreateConvexHull(Vertices);
							if (Shape != nullptr)
								Create(Shape, Mass, CcdMotionThreshold);
						}
					}
					else
					{
						btCollisionShape* Shape = Scene->GetSimulator()->CreateShape((Compute::Shape)Type);
						if (Shape != nullptr)
							Create(Shape, Mass, CcdMotionThreshold);
					}
				}

				if (!Instance)
					return;

				uint64_t ActivationState;
				if (NMake::Unpack(Node->Find("activation-state"), &ActivationState))
					Instance->SetActivationState((Compute::MotionState)ActivationState);

				float AngularDamping;
				if (NMake::Unpack(Node->Find("angular-damping"), &AngularDamping))
					Instance->SetAngularDamping(AngularDamping);

				float AngularSleepingThreshold;
				if (NMake::Unpack(Node->Find("angular-sleeping-threshold"), &AngularSleepingThreshold))
					Instance->SetAngularSleepingThreshold(AngularSleepingThreshold);

				float Friction;
				if (NMake::Unpack(Node->Find("friction"), &Friction))
					Instance->SetFriction(Friction);

				float Restitution;
				if (NMake::Unpack(Node->Find("restitution"), &Restitution))
					Instance->SetRestitution(Restitution);

				float HitFraction;
				if (NMake::Unpack(Node->Find("hit-fraction"), &HitFraction))
					Instance->SetHitFraction(HitFraction);

				float LinearDamping;
				if (NMake::Unpack(Node->Find("linear-damping"), &LinearDamping))
					Instance->SetLinearDamping(LinearDamping);

				float LinearSleepingThreshold;
				if (NMake::Unpack(Node->Find("linear-sleeping-threshold"), &LinearSleepingThreshold))
					Instance->SetLinearSleepingThreshold(LinearSleepingThreshold);

				float CcdSweptSphereRadius;
				if (NMake::Unpack(Node->Find("ccd-swept-sphere-radius"), &CcdSweptSphereRadius))
					Instance->SetCcdSweptSphereRadius(CcdSweptSphereRadius);

				float ContactProcessingThreshold;
				if (NMake::Unpack(Node->Find("contact-processing-threshold"), &ContactProcessingThreshold))
					Instance->SetContactProcessingThreshold(ContactProcessingThreshold);

				float DeactivationTime;
				if (NMake::Unpack(Node->Find("deactivation-time"), &DeactivationTime))
					Instance->SetDeactivationTime(DeactivationTime);

				float RollingFriction;
				if (NMake::Unpack(Node->Find("rolling-friction"), &RollingFriction))
					Instance->SetRollingFriction(RollingFriction);

				float SpinningFriction;
				if (NMake::Unpack(Node->Find("spinning-friction"), &SpinningFriction))
					Instance->SetSpinningFriction(SpinningFriction);

				float ContactStiffness;
				if (NMake::Unpack(Node->Find("contact-stiffness"), &ContactStiffness))
					Instance->SetContactStiffness(ContactStiffness);

				float ContactDamping;
				if (NMake::Unpack(Node->Find("contact-damping"), &ContactDamping))
					Instance->SetContactDamping(ContactDamping);

				Compute::Vector3 AngularFactor;
				if (NMake::Unpack(Node->Find("angular-factor"), &AngularFactor))
					Instance->SetAngularFactor(AngularFactor);

				Compute::Vector3 AngularVelocity;
				if (NMake::Unpack(Node->Find("angular-velocity"), &AngularVelocity))
					Instance->SetAngularVelocity(AngularVelocity);

				Compute::Vector3 AnisotropicFriction;
				if (NMake::Unpack(Node->Find("anisotropic-friction"), &AnisotropicFriction))
					Instance->SetAnisotropicFriction(AnisotropicFriction);

				Compute::Vector3 Gravity;
				if (NMake::Unpack(Node->Find("gravity"), &Gravity))
					Instance->SetGravity(Gravity);

				Compute::Vector3 LinearFactor;
				if (NMake::Unpack(Node->Find("linear-factor"), &LinearFactor))
					Instance->SetLinearFactor(LinearFactor);

				Compute::Vector3 LinearVelocity;
				if (NMake::Unpack(Node->Find("linear-velocity"), &LinearVelocity))
					Instance->SetLinearVelocity(LinearVelocity);

				uint64_t CollisionFlags;
				if (NMake::Unpack(Node->Find("collision-flags"), &CollisionFlags))
					Instance->SetCollisionFlags(CollisionFlags);
			}
			void RigidBody::Serialize(ContentManager* Content, Core::Document* Node)
			{
				SceneGraph* Scene = Parent->GetScene();
				NMake::Pack(Node->Set("kinematic"), Kinematic);
				NMake::Pack(Node->Set("manage"), Manage);
				NMake::Pack(Node->Set("extended"), Instance != nullptr);
				if (!Instance)
					return;

				Core::Document* CV = Node->Set("shape");
				if (Instance->GetCollisionShapeType() == Compute::Shape::Convex_Hull)
				{
					AssetCache* Asset = Content->Find<Compute::UnmanagedShape>(Hull);
					if (!Asset || !Hull)
					{
						std::vector<Compute::Vector3> Vertices = Scene->GetSimulator()->GetShapeVertices(Instance->GetCollisionShape());
						NMake::Pack(CV->Set("data"), Vertices);
					}
					else
						NMake::Pack(CV->Set("path"), Asset->Path);
				}
				else
					NMake::Pack(CV->Set("type"), (uint64_t)Instance->GetCollisionShapeType());

				NMake::Pack(Node->Set("mass"), Instance->GetMass());
				NMake::Pack(Node->Set("ccd-motion-threshold"), Instance->GetCcdMotionThreshold());
				NMake::Pack(Node->Set("activation-state"), (uint64_t)Instance->GetActivationState());
				NMake::Pack(Node->Set("angular-damping"), Instance->GetAngularDamping());
				NMake::Pack(Node->Set("angular-sleeping-threshold"), Instance->GetAngularSleepingThreshold());
				NMake::Pack(Node->Set("friction"), Instance->GetFriction());
				NMake::Pack(Node->Set("restitution"), Instance->GetRestitution());
				NMake::Pack(Node->Set("hit-fraction"), Instance->GetHitFraction());
				NMake::Pack(Node->Set("linear-damping"), Instance->GetLinearDamping());
				NMake::Pack(Node->Set("linear-sleeping-threshold"), Instance->GetLinearSleepingThreshold());
				NMake::Pack(Node->Set("ccd-swept-sphere-radius"), Instance->GetCcdSweptSphereRadius());
				NMake::Pack(Node->Set("contact-processing-threshold"), Instance->GetContactProcessingThreshold());
				NMake::Pack(Node->Set("deactivation-time"), Instance->GetDeactivationTime());
				NMake::Pack(Node->Set("rolling-friction"), Instance->GetRollingFriction());
				NMake::Pack(Node->Set("spinning-friction"), Instance->GetSpinningFriction());
				NMake::Pack(Node->Set("contact-stiffness"), Instance->GetContactStiffness());
				NMake::Pack(Node->Set("contact-damping"), Instance->GetContactDamping());
				NMake::Pack(Node->Set("angular-factor"), Instance->GetAngularFactor());
				NMake::Pack(Node->Set("angular-velocity"), Instance->GetAngularVelocity());
				NMake::Pack(Node->Set("anisotropic-friction"), Instance->GetAnisotropicFriction());
				NMake::Pack(Node->Set("gravity"), Instance->GetGravity());
				NMake::Pack(Node->Set("linear-factor"), Instance->GetLinearFactor());
				NMake::Pack(Node->Set("linear-velocity"), Instance->GetLinearVelocity());
				NMake::Pack(Node->Set("collision-flags"), (uint64_t)Instance->GetCollisionFlags());
			}
			void RigidBody::Synchronize(Core::Timer* Time)
			{
				if (Instance && Manage)
					Instance->Synchronize(Parent->Transform, Kinematic);
			}
			void RigidBody::Asleep()
			{
				if (Instance != nullptr)
					Instance->SetAsGhost();
			}
			void RigidBody::Create(btCollisionShape* Shape, float Mass, float Anticipation)
			{
				if (!Shape || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_RELEASE(Instance);

				Compute::RigidBody::Desc I;
				I.Anticipation = Anticipation;
				I.Mass = Mass;
				I.Shape = Shape;

				Instance = Scene->GetSimulator()->CreateRigidBody(I, Parent->Transform);
				Instance->UserPointer = this;
				Instance->SetActivity(true);
				Scene->Unlock();
			}
			void RigidBody::Create(ContentManager* Content, const std::string& Path, float Mass, float Anticipation)
			{
				if (Content != nullptr)
				{
					Hull = Content->Load<Compute::UnmanagedShape>(Path);
					if (Hull != nullptr)
						Create(Hull->Shape, Mass, Anticipation);
				}
			}
			void RigidBody::Clear()
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_CLEAR(Instance);
				Scene->Unlock();
			}
			void RigidBody::SetTransform(const Compute::Vector3& Position, const Compute::Vector3& Scale, const Compute::Vector3& Rotation)
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (Scene != nullptr)
					Scene->Lock();

				Parent->Transform->SetTransform(Compute::TransformSpace::Global, Position, Scale, Rotation);
				Instance->Synchronize(Parent->Transform, true);
				Instance->SetActivity(true);

				if (Scene != nullptr)
					Scene->Unlock();
			}
			void RigidBody::SetTransform(bool Kinematics)
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (Scene != nullptr)
					Scene->Lock();

				Instance->Synchronize(Parent->Transform, Kinematics);
				Instance->SetActivity(true);

				if (Scene != nullptr)
					Scene->Unlock();
			}
			void RigidBody::SetMass(float Mass)
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene)
					return;

				Scene->Lock();
				Instance->SetMass(Mass);
				Scene->Unlock();
			}
			Component* RigidBody::Copy(Entity* New)
			{
				RigidBody* Target = new RigidBody(New);
				Target->Kinematic = Kinematic;

				if (Instance != nullptr)
				{
					Target->Instance = Instance->Copy();
					Target->Instance->UserPointer = Target;
				}

				return Target;
			}
			Compute::RigidBody* RigidBody::GetBody() const
			{
				return Instance;
			}

			SoftBody::SoftBody(Entity* Ref) : Drawable(Ref, SoftBody::GetTypeId(), false)
			{
			}
			SoftBody::~SoftBody()
			{
				TH_RELEASE(Instance);
			}
			void SoftBody::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				uint64_t Slot = -1;
				if (NMake::Unpack(Node->Find("material"), &Slot))
					SetMaterial(nullptr, Parent->GetScene()->GetMaterial((uint64_t)Slot));

				bool Extended = false;
				bool Transparent = false;
				std::string Path;

				NMake::Unpack(Node->Find("texcoord"), &TexCoord);
				NMake::Unpack(Node->Find("extended"), &Extended);
				NMake::Unpack(Node->Find("kinematic"), &Kinematic);
				NMake::Unpack(Node->Find("manage"), &Manage);
				NMake::Unpack(Node->Find("static"), &Static);
				NMake::Unpack(Node->Find("transparency"), &Transparent);
				SetTransparency(Transparent);

				if (!Extended)
					return;

				float CcdMotionThreshold = 0;
				NMake::Unpack(Node->Find("ccd-motion-threshold"), &CcdMotionThreshold);

				Core::Document* CV = nullptr;
				if ((CV = Node->Find("shape")) != nullptr)
				{
					if (NMake::Unpack(Node->Find("path"), &Path))
					{
						auto* Shape = Content->Load<Compute::UnmanagedShape>(Path);
						if (Shape != nullptr)
							Create(Shape, CcdMotionThreshold);
					}
				}
				else if ((CV = Node->Find("ellipsoid")) != nullptr)
				{
					Compute::SoftBody::Desc::CV::SEllipsoid Shape;
					NMake::Unpack(CV->Get("center"), &Shape.Center);
					NMake::Unpack(CV->Get("radius"), &Shape.Radius);
					NMake::Unpack(CV->Get("count"), &Shape.Count);
					CreateEllipsoid(Shape, CcdMotionThreshold);
				}
				else if ((CV = Node->Find("patch")) != nullptr)
				{
					Compute::SoftBody::Desc::CV::SPatch Shape;
					NMake::Unpack(CV->Get("corner-00"), &Shape.Corner00);
					NMake::Unpack(CV->Get("corner-00-fixed"), &Shape.Corner00Fixed);
					NMake::Unpack(CV->Get("corner-01"), &Shape.Corner01);
					NMake::Unpack(CV->Get("corner-01-fixed"), &Shape.Corner01Fixed);
					NMake::Unpack(CV->Get("corner-10"), &Shape.Corner10);
					NMake::Unpack(CV->Get("corner-10-fixed"), &Shape.Corner10Fixed);
					NMake::Unpack(CV->Get("corner-11"), &Shape.Corner11);
					NMake::Unpack(CV->Get("corner-11-fixed"), &Shape.Corner11Fixed);
					NMake::Unpack(CV->Get("count-x"), &Shape.CountX);
					NMake::Unpack(CV->Get("count-y"), &Shape.CountY);
					NMake::Unpack(CV->Get("diagonals"), &Shape.GenerateDiagonals);
					CreatePatch(Shape, CcdMotionThreshold);
				}
				else if ((CV = Node->Find("rope")) != nullptr)
				{
					Compute::SoftBody::Desc::CV::SRope Shape;
					NMake::Unpack(CV->Get("start"), &Shape.Start);
					NMake::Unpack(CV->Get("start-fixed"), &Shape.StartFixed);
					NMake::Unpack(CV->Get("end"), &Shape.End);
					NMake::Unpack(CV->Get("end-fixed"), &Shape.EndFixed);
					NMake::Unpack(CV->Get("count"), &Shape.Count);
					CreateRope(Shape, CcdMotionThreshold);
				}

				if (!Instance)
					return;

				Core::Document* Conf = Node->Get("config");
				if (Conf != nullptr)
				{
					Compute::SoftBody::Desc::SConfig I;
					NMake::Unpack(Conf->Get("vcf"), &I.VCF);
					NMake::Unpack(Conf->Get("dp"), &I.DP);
					NMake::Unpack(Conf->Get("dg"), &I.DG);
					NMake::Unpack(Conf->Get("lf"), &I.LF);
					NMake::Unpack(Conf->Get("pr"), &I.PR);
					NMake::Unpack(Conf->Get("vc"), &I.VC);
					NMake::Unpack(Conf->Get("df"), &I.DF);
					NMake::Unpack(Conf->Get("mt"), &I.MT);
					NMake::Unpack(Conf->Get("chr"), &I.CHR);
					NMake::Unpack(Conf->Get("khr"), &I.KHR);
					NMake::Unpack(Conf->Get("shr"), &I.SHR);
					NMake::Unpack(Conf->Get("ahr"), &I.AHR);
					NMake::Unpack(Conf->Get("srhr-cl"), &I.SRHR_CL);
					NMake::Unpack(Conf->Get("skhr-cl"), &I.SKHR_CL);
					NMake::Unpack(Conf->Get("sshr-cl"), &I.SSHR_CL);
					NMake::Unpack(Conf->Get("sr-splt-cl"), &I.SR_SPLT_CL);
					NMake::Unpack(Conf->Get("sk-splt-cl"), &I.SK_SPLT_CL);
					NMake::Unpack(Conf->Get("ss-splt-cl"), &I.SS_SPLT_CL);
					NMake::Unpack(Conf->Get("max-volume"), &I.MaxVolume);
					NMake::Unpack(Conf->Get("time-scale"), &I.TimeScale);
					NMake::Unpack(Conf->Get("drag"), &I.Drag);
					NMake::Unpack(Conf->Get("max-stress"), &I.MaxStress);
					NMake::Unpack(Conf->Get("constraints"), &I.Constraints);
					NMake::Unpack(Conf->Get("clusters"), &I.Clusters);
					NMake::Unpack(Conf->Get("v-it"), &I.VIterations);
					NMake::Unpack(Conf->Get("p-it"), &I.PIterations);
					NMake::Unpack(Conf->Get("d-it"), &I.DIterations);
					NMake::Unpack(Conf->Get("c-it"), &I.CIterations);
					NMake::Unpack(Conf->Get("collisions"), &I.Collisions);

					uint64_t AeroModel;
					if (NMake::Unpack(Conf->Get("aero-model"), &AeroModel))
						I.AeroModel = (Compute::SoftAeroModel)AeroModel;

					Instance->SetConfig(I);
				}

				uint64_t ActivationState;
				if (NMake::Unpack(Node->Find("activation-state"), &ActivationState))
					Instance->SetActivationState((Compute::MotionState)ActivationState);

				float Friction;
				if (NMake::Unpack(Node->Find("friction"), &Friction))
					Instance->SetFriction(Friction);

				float Restitution;
				if (NMake::Unpack(Node->Find("restitution"), &Restitution))
					Instance->SetRestitution(Restitution);

				float HitFraction;
				if (NMake::Unpack(Node->Find("hit-fraction"), &HitFraction))
					Instance->SetHitFraction(HitFraction);

				float CcdSweptSphereRadius;
				if (NMake::Unpack(Node->Find("ccd-swept-sphere-radius"), &CcdSweptSphereRadius))
					Instance->SetCcdSweptSphereRadius(CcdSweptSphereRadius);

				float ContactProcessingThreshold;
				if (NMake::Unpack(Node->Find("contact-processing-threshold"), &ContactProcessingThreshold))
					Instance->SetContactProcessingThreshold(ContactProcessingThreshold);

				float DeactivationTime;
				if (NMake::Unpack(Node->Find("deactivation-time"), &DeactivationTime))
					Instance->SetDeactivationTime(DeactivationTime);

				float RollingFriction;
				if (NMake::Unpack(Node->Find("rolling-friction"), &RollingFriction))
					Instance->SetRollingFriction(RollingFriction);

				float SpinningFriction;
				if (NMake::Unpack(Node->Find("spinning-friction"), &SpinningFriction))
					Instance->SetSpinningFriction(SpinningFriction);

				float ContactStiffness;
				if (NMake::Unpack(Node->Find("contact-stiffness"), &ContactStiffness))
					Instance->SetContactStiffness(ContactStiffness);

				float ContactDamping;
				if (NMake::Unpack(Node->Find("contact-damping"), &ContactDamping))
					Instance->SetContactDamping(ContactDamping);

				Compute::Vector3 AnisotropicFriction;
				if (NMake::Unpack(Node->Find("anisotropic-friction"), &AnisotropicFriction))
					Instance->SetAnisotropicFriction(AnisotropicFriction);

				Compute::Vector3 WindVelocity;
				if (NMake::Unpack(Node->Find("wind-velocity"), &WindVelocity))
					Instance->SetWindVelocity(WindVelocity);

				float TotalMass;
				if (NMake::Unpack(Node->Find("total-mass"), &TotalMass))
					Instance->SetTotalMass(TotalMass);

				float RestLengthScale;
				if (NMake::Unpack(Node->Find("core-length-scale"), &RestLengthScale))
					Instance->SetRestLengthScale(RestLengthScale);
			}
			void SoftBody::Serialize(ContentManager* Content, Core::Document* Node)
			{
				Material* Slot = GetMaterial();
				if (Slot != nullptr)
					NMake::Pack(Node->Set("material"), Slot->GetSlot());

				NMake::Pack(Node->Set("texcoord"), TexCoord);
				NMake::Pack(Node->Set("transparency"), HasTransparency());
				NMake::Pack(Node->Set("kinematic"), Kinematic);
				NMake::Pack(Node->Set("manage"), Manage);
				NMake::Pack(Node->Set("extended"), Instance != nullptr);
				NMake::Pack(Node->Set("static"), Static);

				if (!Instance)
					return;

				Compute::SoftBody::Desc& I = Instance->GetInitialState();
				Core::Document* Conf = Node->Set("config");
				NMake::Pack(Conf->Set("aero-model"), (uint64_t)I.Config.AeroModel);
				NMake::Pack(Conf->Set("vcf"), I.Config.VCF);
				NMake::Pack(Conf->Set("dp"), I.Config.DP);
				NMake::Pack(Conf->Set("dg"), I.Config.DG);
				NMake::Pack(Conf->Set("lf"), I.Config.LF);
				NMake::Pack(Conf->Set("pr"), I.Config.PR);
				NMake::Pack(Conf->Set("vc"), I.Config.VC);
				NMake::Pack(Conf->Set("df"), I.Config.DF);
				NMake::Pack(Conf->Set("mt"), I.Config.MT);
				NMake::Pack(Conf->Set("chr"), I.Config.CHR);
				NMake::Pack(Conf->Set("khr"), I.Config.KHR);
				NMake::Pack(Conf->Set("shr"), I.Config.SHR);
				NMake::Pack(Conf->Set("ahr"), I.Config.AHR);
				NMake::Pack(Conf->Set("srhr-cl"), I.Config.SRHR_CL);
				NMake::Pack(Conf->Set("skhr-cl"), I.Config.SKHR_CL);
				NMake::Pack(Conf->Set("sshr-cl"), I.Config.SSHR_CL);
				NMake::Pack(Conf->Set("sr-splt-cl"), I.Config.SR_SPLT_CL);
				NMake::Pack(Conf->Set("sk-splt-cl"), I.Config.SK_SPLT_CL);
				NMake::Pack(Conf->Set("ss-splt-cl"), I.Config.SS_SPLT_CL);
				NMake::Pack(Conf->Set("max-volume"), I.Config.MaxVolume);
				NMake::Pack(Conf->Set("time-scale"), I.Config.TimeScale);
				NMake::Pack(Conf->Set("drag"), I.Config.Drag);
				NMake::Pack(Conf->Set("max-stress"), I.Config.MaxStress);
				NMake::Pack(Conf->Set("constraints"), I.Config.Constraints);
				NMake::Pack(Conf->Set("clusters"), I.Config.Clusters);
				NMake::Pack(Conf->Set("v-it"), I.Config.VIterations);
				NMake::Pack(Conf->Set("p-it"), I.Config.PIterations);
				NMake::Pack(Conf->Set("d-it"), I.Config.DIterations);
				NMake::Pack(Conf->Set("c-it"), I.Config.CIterations);
				NMake::Pack(Conf->Set("collisions"), I.Config.Collisions);

				auto& Desc = Instance->GetInitialState();
				if (Desc.Shape.Convex.Enabled)
				{
					if (Instance->GetCollisionShapeType() == Compute::Shape::Convex_Hull)
					{
						AssetCache* Asset = Content->Find<Compute::UnmanagedShape>(Desc.Shape.Convex.Hull);
						if (Asset != nullptr)
						{
							Core::Document* Shape = Node->Set("shape");
							NMake::Pack(Shape->Set("path"), Asset->Path);
						}
					}
				}
				else if (Desc.Shape.Ellipsoid.Enabled)
				{
					Core::Document* Shape = Node->Set("ellipsoid");
					NMake::Pack(Shape->Set("center"), Desc.Shape.Ellipsoid.Center);
					NMake::Pack(Shape->Set("radius"), Desc.Shape.Ellipsoid.Radius);
					NMake::Pack(Shape->Set("count"), Desc.Shape.Ellipsoid.Count);
				}
				else if (Desc.Shape.Patch.Enabled)
				{
					Core::Document* Shape = Node->Set("patch");
					NMake::Pack(Shape->Set("corner-00"), Desc.Shape.Patch.Corner00);
					NMake::Pack(Shape->Set("corner-00-fixed"), Desc.Shape.Patch.Corner00Fixed);
					NMake::Pack(Shape->Set("corner-01"), Desc.Shape.Patch.Corner01);
					NMake::Pack(Shape->Set("corner-01-fixed"), Desc.Shape.Patch.Corner01Fixed);
					NMake::Pack(Shape->Set("corner-10"), Desc.Shape.Patch.Corner10);
					NMake::Pack(Shape->Set("corner-10-fixed"), Desc.Shape.Patch.Corner10Fixed);
					NMake::Pack(Shape->Set("corner-11"), Desc.Shape.Patch.Corner11);
					NMake::Pack(Shape->Set("corner-11-fixed"), Desc.Shape.Patch.Corner11Fixed);
					NMake::Pack(Shape->Set("count-x"), Desc.Shape.Patch.CountX);
					NMake::Pack(Shape->Set("count-y"), Desc.Shape.Patch.CountY);
					NMake::Pack(Shape->Set("diagonals"), Desc.Shape.Patch.GenerateDiagonals);
				}
				else if (Desc.Shape.Rope.Enabled)
				{
					Core::Document* Shape = Node->Set("rope");
					NMake::Pack(Shape->Set("start"), Desc.Shape.Rope.Start);
					NMake::Pack(Shape->Set("start-fixed"), Desc.Shape.Rope.StartFixed);
					NMake::Pack(Shape->Set("end"), Desc.Shape.Rope.End);
					NMake::Pack(Shape->Set("end-fixed"), Desc.Shape.Rope.EndFixed);
					NMake::Pack(Shape->Set("count"), Desc.Shape.Rope.Count);
				}

				NMake::Pack(Node->Set("ccd-motion-threshold"), Instance->GetCcdMotionThreshold());
				NMake::Pack(Node->Set("activation-state"), (uint64_t)Instance->GetActivationState());
				NMake::Pack(Node->Set("friction"), Instance->GetFriction());
				NMake::Pack(Node->Set("restitution"), Instance->GetRestitution());
				NMake::Pack(Node->Set("hit-fraction"), Instance->GetHitFraction());
				NMake::Pack(Node->Set("ccd-swept-sphere-radius"), Instance->GetCcdSweptSphereRadius());
				NMake::Pack(Node->Set("contact-processing-threshold"), Instance->GetContactProcessingThreshold());
				NMake::Pack(Node->Set("deactivation-time"), Instance->GetDeactivationTime());
				NMake::Pack(Node->Set("rolling-friction"), Instance->GetRollingFriction());
				NMake::Pack(Node->Set("spinning-friction"), Instance->GetSpinningFriction());
				NMake::Pack(Node->Set("contact-stiffness"), Instance->GetContactStiffness());
				NMake::Pack(Node->Set("contact-damping"), Instance->GetContactDamping());
				NMake::Pack(Node->Set("angular-velocity"), Instance->GetAngularVelocity());
				NMake::Pack(Node->Set("anisotropic-friction"), Instance->GetAnisotropicFriction());
				NMake::Pack(Node->Set("linear-velocity"), Instance->GetLinearVelocity());
				NMake::Pack(Node->Set("collision-flags"), (uint64_t)Instance->GetCollisionFlags());
				NMake::Pack(Node->Set("wind-velocity"), Instance->GetWindVelocity());
				NMake::Pack(Node->Set("total-mass"), Instance->GetTotalMass());
				NMake::Pack(Node->Set("core-length-scale"), Instance->GetRestLengthScale());
			}
			void SoftBody::Synchronize(Core::Timer* Time)
			{
				if (!Instance)
					return;

				if (Manage)
					Instance->Synchronize(Parent->Transform, Kinematic);

				if (Visibility <= 0.0f)
					return;

				Instance->GetVertices(&Vertices);
				if (Indices.empty())
					Instance->GetIndices(&Indices);
			}
			void SoftBody::Awake(Component* New)
			{
				if (!New)
					Attach();
			}
			void SoftBody::Asleep()
			{
				Detach();
				if (Instance != nullptr)
					Instance->SetAsGhost();
			}
			void SoftBody::Create(Compute::UnmanagedShape* Shape, float Anticipation)
			{
				if (!Shape || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_RELEASE(Instance);

				Compute::SoftBody::Desc I;
				I.Anticipation = Anticipation;
				I.Shape.Convex.Hull = Shape;
				I.Shape.Convex.Enabled = true;

				Instance = Scene->GetSimulator()->CreateSoftBody(I, Parent->Transform);
				if (!Instance)
				{
					TH_ERROR("cannot create soft body");
					return Scene->Unlock();
				}

				Vertices.clear();
				Indices.clear();

				Instance->UserPointer = this;
				Instance->SetActivity(true);
				Scene->Unlock();
			}
			void SoftBody::Create(ContentManager* Content, const std::string& Path, float Anticipation)
			{
				if (Content != nullptr)
				{
					Compute::UnmanagedShape* Hull = Content->Load<Compute::UnmanagedShape>(Path);
					if (Hull != nullptr)
						Create(Hull, Anticipation);
				}
			}
			void SoftBody::CreateEllipsoid(const Compute::SoftBody::Desc::CV::SEllipsoid& Shape, float Anticipation)
			{
				if (!Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_RELEASE(Instance);

				Compute::SoftBody::Desc I;
				I.Anticipation = Anticipation;
				I.Shape.Ellipsoid = Shape;
				I.Shape.Ellipsoid.Enabled = true;

				Instance = Scene->GetSimulator()->CreateSoftBody(I, Parent->Transform);
				if (!Instance)
				{
					TH_ERROR("cannot create soft body");
					return Scene->Unlock();
				}

				Vertices.clear();
				Indices.clear();

				Instance->UserPointer = this;
				Instance->SetActivity(true);
				Scene->Unlock();
			}
			void SoftBody::CreatePatch(const Compute::SoftBody::Desc::CV::SPatch& Shape, float Anticipation)
			{
				if (!Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_RELEASE(Instance);

				Compute::SoftBody::Desc I;
				I.Anticipation = Anticipation;
				I.Shape.Patch = Shape;
				I.Shape.Patch.Enabled = true;

				Instance = Scene->GetSimulator()->CreateSoftBody(I, Parent->Transform);
				if (!Instance)
				{
					TH_ERROR("cannot create soft body");
					return Scene->Unlock();
				}

				Vertices.clear();
				Indices.clear();

				Instance->UserPointer = this;
				Instance->SetActivity(true);
				Scene->Unlock();
			}
			void SoftBody::CreateRope(const Compute::SoftBody::Desc::CV::SRope& Shape, float Anticipation)
			{
				if (!Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_RELEASE(Instance);

				Compute::SoftBody::Desc I;
				I.Anticipation = Anticipation;
				I.Shape.Rope = Shape;
				I.Shape.Rope.Enabled = true;

				Instance = Scene->GetSimulator()->CreateSoftBody(I, Parent->Transform);
				if (!Instance)
				{
					TH_ERROR("cannot create soft body");
					return Scene->Unlock();
				}

				Vertices.clear();
				Indices.clear();

				Instance->UserPointer = this;
				Instance->SetActivity(true);
				Scene->Unlock();
			}
			void SoftBody::Fill(Graphics::GraphicsDevice* Device, Graphics::ElementBuffer* IndexBuffer, Graphics::ElementBuffer* VertexBuffer)
			{
				Graphics::MappedSubresource Map;
				if (VertexBuffer != nullptr && !Vertices.empty())
				{
					Device->Map(VertexBuffer, Graphics::ResourceMap::Write_Discard, &Map);
					memcpy(Map.Pointer, (void*)Vertices.data(), Vertices.size() * sizeof(Compute::Vertex));
					Device->Unmap(VertexBuffer, &Map);
				}

				if (IndexBuffer != nullptr && !Indices.empty())
				{
					Device->Map(IndexBuffer, Graphics::ResourceMap::Write_Discard, &Map);
					memcpy(Map.Pointer, (void*)Indices.data(), Indices.size() * sizeof(int));
					Device->Unmap(IndexBuffer, &Map);
				}
			}
			void SoftBody::Regenerate()
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				Compute::SoftBody::Desc I = Instance->GetInitialState();
				TH_RELEASE(Instance);

				Instance = Scene->GetSimulator()->CreateSoftBody(I, Parent->Transform);
				if (!Instance)
					TH_ERROR("cannot regenerate soft body");
				Scene->Unlock();
			}
			void SoftBody::Clear()
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_CLEAR(Instance);
				Scene->Unlock();
			}
			void SoftBody::SetTransform(const Compute::Vector3& Position, const Compute::Vector3& Scale, const Compute::Vector3& Rotation)
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (Scene != nullptr)
					Scene->Lock();

				Parent->Transform->SetTransform(Compute::TransformSpace::Global, Position, Scale, Rotation);
				Instance->Synchronize(Parent->Transform, true);
				Instance->SetActivity(true);

				if (Scene != nullptr)
					Scene->Unlock();
			}
			void SoftBody::SetTransform(bool Kinematics)
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (Scene != nullptr)
					Scene->Lock();

				Instance->Synchronize(Parent->Transform, Kinematics);
				Instance->SetActivity(true);

				if (Scene != nullptr)
					Scene->Unlock();
			}
			float SoftBody::Cull(const Viewer& View)
			{
				float Result = 0.0f;
				if (Instance != nullptr)
				{
					Compute::Matrix4x4 Box = GetBoundingBox();
					Result = IsVisible(View, &Box) ? 1.0f : 0.0f;
				}

				return Result;
			}
			Compute::Matrix4x4 SoftBody::GetBoundingBox()
			{
				if (!Instance)
					return Parent->Transform->GetWorld();

				Compute::Vector3 Min, Max;
				Instance->GetBoundingBox(&Min, &Max);

				return Compute::Matrix4x4::Create((Max + Min).Div(2.0f), Parent->Transform->Scale * Instance->GetScale(), Parent->Transform->Rotation);
			}
			Component* SoftBody::Copy(Entity* New)
			{
				SoftBody* Target = new SoftBody(New);
				Target->SetTransparency(HasTransparency());
				Target->Kinematic = Kinematic;

				if (Instance != nullptr)
				{
					Target->Instance = Instance->Copy();
					Target->Instance->UserPointer = Target;
				}

				return Target;
			}
			Compute::SoftBody* SoftBody::GetBody()
			{
				return Instance;
			}
			std::vector<Compute::Vertex>& SoftBody::GetVertices()
			{
				return Vertices;
			}
			std::vector<int>& SoftBody::GetIndices()
			{
				return Indices;
			}

			SliderConstraint::SliderConstraint(Entity* Ref) : Component(Ref), Instance(nullptr), Connection(nullptr)
			{
			}
			SliderConstraint::~SliderConstraint()
			{
				TH_RELEASE(Instance);
			}
			void SliderConstraint::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				bool Extended;
				NMake::Unpack(Node->Find("extended"), &Extended);
				if (!Extended)
					return;

				uint64_t Index;
				if (NMake::Unpack(Node->Find("connection"), &Index))
					Wanted.Connection = Index;

				NMake::Unpack(Node->Find("collision-state"), &Wanted.Ghost);
				NMake::Unpack(Node->Find("linear-power-state"), &Wanted.Linear);
				Create(Connection, Wanted.Ghost, Wanted.Linear);

				if (!Instance)
					return;

				float AngularMotorVelocity;
				if (NMake::Unpack(Node->Find("angular-motor-velocity"), &AngularMotorVelocity))
					Instance->SetAngularMotorVelocity(AngularMotorVelocity);

				float LinearMotorVelocity;
				if (NMake::Unpack(Node->Find("linear-motor-velocity"), &LinearMotorVelocity))
					Instance->SetLinearMotorVelocity(LinearMotorVelocity);

				float UpperLinearLimit;
				if (NMake::Unpack(Node->Find("upper-linear-limit"), &UpperLinearLimit))
					Instance->SetUpperLinearLimit(UpperLinearLimit);

				float LowerLinearLimit;
				if (NMake::Unpack(Node->Find("lower-linear-limit"), &LowerLinearLimit))
					Instance->SetLowerLinearLimit(LowerLinearLimit);

				float AngularDampingDirection;
				if (NMake::Unpack(Node->Find("angular-damping-direction"), &AngularDampingDirection))
					Instance->SetAngularDampingDirection(AngularDampingDirection);

				float LinearDampingDirection;
				if (NMake::Unpack(Node->Find("linear-damping-direction"), &LinearDampingDirection))
					Instance->SetLinearDampingDirection(LinearDampingDirection);

				float AngularDampingLimit;
				if (NMake::Unpack(Node->Find("angular-damping-limit"), &AngularDampingLimit))
					Instance->SetAngularDampingLimit(AngularDampingLimit);

				float LinearDampingLimit;
				if (NMake::Unpack(Node->Find("linear-damping-limit"), &LinearDampingLimit))
					Instance->SetLinearDampingLimit(LinearDampingLimit);

				float AngularDampingOrtho;
				if (NMake::Unpack(Node->Find("angular-damping-ortho"), &AngularDampingOrtho))
					Instance->SetAngularDampingOrtho(AngularDampingOrtho);

				float LinearDampingOrtho;
				if (NMake::Unpack(Node->Find("linear-damping-ortho"), &LinearDampingOrtho))
					Instance->SetLinearDampingOrtho(LinearDampingOrtho);

				float UpperAngularLimit;
				if (NMake::Unpack(Node->Find("upper-angular-limit"), &UpperAngularLimit))
					Instance->SetUpperAngularLimit(UpperAngularLimit);

				float LowerAngularLimit;
				if (NMake::Unpack(Node->Find("lower-angular-limit"), &LowerAngularLimit))
					Instance->SetLowerAngularLimit(LowerAngularLimit);

				float MaxAngularMotorForce;
				if (NMake::Unpack(Node->Find("max-angular-motor-force"), &MaxAngularMotorForce))
					Instance->SetMaxAngularMotorForce(MaxAngularMotorForce);

				float MaxLinearMotorForce;
				if (NMake::Unpack(Node->Find("max-linear-motor-force"), &MaxLinearMotorForce))
					Instance->SetMaxLinearMotorForce(MaxLinearMotorForce);

				float AngularRestitutionDirection;
				if (NMake::Unpack(Node->Find("angular-restitution-direction"), &AngularRestitutionDirection))
					Instance->SetAngularRestitutionDirection(AngularRestitutionDirection);

				float LinearRestitutionDirection;
				if (NMake::Unpack(Node->Find("linear-restitution-direction"), &LinearRestitutionDirection))
					Instance->SetLinearRestitutionDirection(LinearRestitutionDirection);

				float AngularRestitutionLimit;
				if (NMake::Unpack(Node->Find("angular-restitution-limit"), &AngularRestitutionLimit))
					Instance->SetAngularRestitutionLimit(AngularRestitutionLimit);

				float LinearRestitutionLimit;
				if (NMake::Unpack(Node->Find("linear-restitution-limit"), &LinearRestitutionLimit))
					Instance->SetLinearRestitutionLimit(LinearRestitutionLimit);

				float AngularRestitutionOrtho;
				if (NMake::Unpack(Node->Find("angular-restitution-ortho"), &AngularRestitutionOrtho))
					Instance->SetAngularRestitutionOrtho(AngularRestitutionOrtho);

				float LinearRestitutionOrtho;
				if (NMake::Unpack(Node->Find("linear-restitution-ortho"), &LinearRestitutionOrtho))
					Instance->SetLinearRestitutionOrtho(LinearRestitutionOrtho);

				float AngularSoftnessDirection;
				if (NMake::Unpack(Node->Find("angular-softness-direction"), &AngularSoftnessDirection))
					Instance->SetAngularSoftnessDirection(AngularSoftnessDirection);

				float LinearSoftnessDirection;
				if (NMake::Unpack(Node->Find("linear-softness-direction"), &LinearSoftnessDirection))
					Instance->SetLinearSoftnessDirection(LinearSoftnessDirection);

				float AngularSoftnessLimit;
				if (NMake::Unpack(Node->Find("angular-softness-limit"), &AngularSoftnessLimit))
					Instance->SetAngularSoftnessLimit(AngularSoftnessLimit);

				float LinearSoftnessLimit;
				if (NMake::Unpack(Node->Find("linear-softness-limit"), &LinearSoftnessLimit))
					Instance->SetLinearSoftnessLimit(LinearSoftnessLimit);

				float AngularSoftnessOrtho;
				if (NMake::Unpack(Node->Find("angular-softness-ortho"), &AngularSoftnessOrtho))
					Instance->SetAngularSoftnessOrtho(AngularSoftnessOrtho);

				float LinearSoftnessOrtho;
				if (NMake::Unpack(Node->Find("linear-softness-ortho"), &LinearSoftnessOrtho))
					Instance->SetLinearSoftnessOrtho(LinearSoftnessOrtho);

				bool PoweredAngularMotor;
				if (NMake::Unpack(Node->Find("powered-angular-motor"), &PoweredAngularMotor))
					Instance->SetPoweredAngularMotor(PoweredAngularMotor);

				bool PoweredLinearMotor;
				if (NMake::Unpack(Node->Find("powered-linear-motor"), &PoweredLinearMotor))
					Instance->SetPoweredLinearMotor(PoweredLinearMotor);

				bool Enabled;
				if (NMake::Unpack(Node->Find("enabled"), &Enabled))
					Instance->SetEnabled(Enabled);
			}
			void SliderConstraint::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("extended"), Instance != nullptr);
				if (!Instance)
					return;

				NMake::Pack(Node->Set("collision-state"), Instance->GetInitialState().UseCollisions);
				NMake::Pack(Node->Set("linear-power-state"), Instance->GetInitialState().UseLinearPower);
				NMake::Pack(Node->Set("connection"), (uint64_t)(Connection ? Connection->Id : -1));
				NMake::Pack(Node->Set("angular-motor-velocity"), Instance->GetAngularMotorVelocity());
				NMake::Pack(Node->Set("linear-motor-velocity"), Instance->GetLinearMotorVelocity());
				NMake::Pack(Node->Set("upper-linear-limit"), Instance->GetUpperLinearLimit());
				NMake::Pack(Node->Set("lower-linear-limit"), Instance->GetLowerLinearLimit());
				NMake::Pack(Node->Set("breaking-impulse-threshold"), Instance->GetBreakingImpulseThreshold());
				NMake::Pack(Node->Set("angular-damping-direction"), Instance->GetAngularDampingDirection());
				NMake::Pack(Node->Set("linear-amping-direction"), Instance->GetLinearDampingDirection());
				NMake::Pack(Node->Set("angular-damping-limit"), Instance->GetAngularDampingLimit());
				NMake::Pack(Node->Set("linear-damping-limit"), Instance->GetLinearDampingLimit());
				NMake::Pack(Node->Set("angular-damping-ortho"), Instance->GetAngularDampingOrtho());
				NMake::Pack(Node->Set("linear-damping-ortho"), Instance->GetLinearDampingOrtho());
				NMake::Pack(Node->Set("upper-angular-limit"), Instance->GetUpperAngularLimit());
				NMake::Pack(Node->Set("lower-angular-limit"), Instance->GetLowerAngularLimit());
				NMake::Pack(Node->Set("max-angular-motor-force"), Instance->GetMaxAngularMotorForce());
				NMake::Pack(Node->Set("max-linear-motor-force"), Instance->GetMaxLinearMotorForce());
				NMake::Pack(Node->Set("angular-restitution-direction"), Instance->GetAngularRestitutionDirection());
				NMake::Pack(Node->Set("linear-restitution-direction"), Instance->GetLinearRestitutionDirection());
				NMake::Pack(Node->Set("angular-restitution-limit"), Instance->GetAngularRestitutionLimit());
				NMake::Pack(Node->Set("linear-restitution-limit"), Instance->GetLinearRestitutionLimit());
				NMake::Pack(Node->Set("angular-restitution-ortho"), Instance->GetAngularRestitutionOrtho());
				NMake::Pack(Node->Set("linear-restitution-ortho"), Instance->GetLinearRestitutionOrtho());
				NMake::Pack(Node->Set("angular-softness-direction"), Instance->GetAngularSoftnessDirection());
				NMake::Pack(Node->Set("linear-softness-direction"), Instance->GetLinearSoftnessDirection());
				NMake::Pack(Node->Set("angular-softness-limit"), Instance->GetAngularSoftnessLimit());
				NMake::Pack(Node->Set("linear-softness-limit"), Instance->GetLinearSoftnessLimit());
				NMake::Pack(Node->Set("angular-softness-ortho"), Instance->GetAngularSoftnessOrtho());
				NMake::Pack(Node->Set("linear-softness-ortho"), Instance->GetLinearSoftnessOrtho());
				NMake::Pack(Node->Set("powered-angular-motor"), Instance->GetPoweredAngularMotor());
				NMake::Pack(Node->Set("powered-linear-motor"), Instance->GetPoweredLinearMotor());
				NMake::Pack(Node->Set("enabled"), Instance->IsEnabled());
			}
			void SliderConstraint::Synchronize(Core::Timer* Time)
			{
				if (Wanted.Connection < 0)
					return;

				if (!Connection)
					Create(Parent->GetScene()->GetEntity(Wanted.Connection), Wanted.Ghost, Wanted.Linear);

				Wanted.Connection = -1;
			}
			void SliderConstraint::Create(Entity* Other, bool IsGhosted, bool IsLinear)
			{
				if (Parent == Other || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_RELEASE(Instance);

				Connection = Other;
				if (!Connection)
					return Scene->Unlock();

				RigidBody* FirstBody = Parent->GetComponent<RigidBody>();
				RigidBody* SecondBody = Connection->GetComponent<RigidBody>();
				if (!FirstBody || !SecondBody)
					return Scene->Unlock();

				Compute::SliderConstraint::Desc I;
				I.Target1 = FirstBody->GetBody();
				I.Target2 = SecondBody->GetBody();
				I.UseCollisions = !IsGhosted;
				I.UseLinearPower = IsLinear;

				if (!I.Target1 || !I.Target2)
					return Scene->Unlock();

				Instance = Scene->GetSimulator()->CreateSliderConstraint(I);
				Scene->Unlock();
			}
			void SliderConstraint::Clear()
			{
				if (!Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene || !Scene->GetSimulator())
					return;

				Scene->Lock();
				TH_CLEAR(Instance);
				Connection = nullptr;
				Scene->Unlock();
			}
			Component* SliderConstraint::Copy(Entity* New)
			{
				SliderConstraint* Target = new SliderConstraint(New);
				Target->Connection = Connection;

				if (!Instance)
					return Target;

				RigidBody* FirstBody = New->GetComponent<RigidBody>();
				if (!FirstBody)
					FirstBody = Parent->GetComponent<RigidBody>();

				if (!FirstBody)
					return Target;

				Compute::SliderConstraint::Desc I(Instance->GetInitialState());
				Instance->GetInitialState().Target1 = FirstBody->GetBody();
				Target->Instance = Instance->Copy();
				Instance->GetInitialState() = I;

				return Target;
			}
			Compute::SliderConstraint* SliderConstraint::GetConstraint() const
			{
				return Instance;
			}
			Entity* SliderConstraint::GetConnection() const
			{
				return Connection;
			}

			Acceleration::Acceleration(Entity* Ref) : Component(Ref)
			{
			}
			void Acceleration::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Unpack(Node->Find("amplitude-velocity"), &AmplitudeVelocity);
				NMake::Unpack(Node->Find("amplitude-torque"), &AmplitudeTorque);
				NMake::Unpack(Node->Find("constant-velocity"), &ConstantVelocity);
				NMake::Unpack(Node->Find("constant-torque"), &ConstantTorque);
				NMake::Unpack(Node->Find("constant-center"), &ConstantCenter);
				NMake::Unpack(Node->Find("kinematic"), &Kinematic);
			}
			void Acceleration::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("amplitude-velocity"), AmplitudeVelocity);
				NMake::Pack(Node->Set("amplitude-torque"), AmplitudeTorque);
				NMake::Pack(Node->Set("constant-velocity"), ConstantVelocity);
				NMake::Pack(Node->Set("constant-torque"), ConstantTorque);
				NMake::Pack(Node->Set("constant-center"), ConstantCenter);
				NMake::Pack(Node->Set("kinematic"), Kinematic);
			}
			void Acceleration::Awake(Component* New)
			{
				if (RigidBody)
					return;

				Components::RigidBody* Component = Parent->GetComponent<Components::RigidBody>();
				if (Component != nullptr)
					RigidBody = Component->GetBody();
			}
			void Acceleration::Update(Core::Timer* Time)
			{
				if (!RigidBody)
					return;

				float DeltaTime = (float)Time->GetDeltaTime();
				if (Kinematic)
				{
					RigidBody->SetLinearVelocity(ConstantVelocity);
					RigidBody->SetAngularVelocity(ConstantTorque);
				}
				else
					RigidBody->Push(ConstantVelocity * DeltaTime, ConstantTorque * DeltaTime, ConstantCenter);

				Compute::Vector3 Force = RigidBody->GetLinearVelocity();
				Compute::Vector3 Torque = RigidBody->GetAngularVelocity();
				Compute::Vector3 ACT = ConstantTorque.Abs();
				Compute::Vector3 ACV = ConstantVelocity.Abs();

				if (AmplitudeVelocity.X > 0 && Force.X > AmplitudeVelocity.X)
					ConstantVelocity.X = -ACV.X;
				else if (AmplitudeVelocity.X > 0 && Force.X < -AmplitudeVelocity.X)
					ConstantVelocity.X = ACV.X;

				if (AmplitudeVelocity.Y > 0 && Force.Y > AmplitudeVelocity.Y)
					ConstantVelocity.Y = -ACV.Y;
				else if (AmplitudeVelocity.Y > 0 && Force.Y < -AmplitudeVelocity.Y)
					ConstantVelocity.Y = ACV.Y;

				if (AmplitudeVelocity.Z > 0 && Force.Z > AmplitudeVelocity.Z)
					ConstantVelocity.Z = -ACV.Z;
				else if (AmplitudeVelocity.Z > 0 && Force.Z < -AmplitudeVelocity.Z)
					ConstantVelocity.Z = ACV.Z;

				if (AmplitudeTorque.X > 0 && Torque.X > AmplitudeTorque.X)
					ConstantTorque.X = -ACT.X;
				else if (AmplitudeTorque.X > 0 && Torque.X < -AmplitudeTorque.X)
					ConstantTorque.X = ACT.X;

				if (AmplitudeTorque.Y > 0 && Torque.Y > AmplitudeTorque.Y)
					ConstantTorque.Y = -ACT.Y;
				else if (AmplitudeTorque.Y > 0 && Torque.Y < -AmplitudeTorque.Y)
					ConstantTorque.Y = ACT.Y;

				if (AmplitudeTorque.Z > 0 && Torque.Z > AmplitudeTorque.Z)
					ConstantTorque.Z = -ACT.Z;
				else if (AmplitudeTorque.Z > 0 && Torque.Z < -AmplitudeTorque.Z)
					ConstantTorque.Z = ACT.Z;
			}
			Component* Acceleration::Copy(Entity* New)
			{
				Acceleration* Target = new Acceleration(New);
				Target->Kinematic = Kinematic;
				Target->AmplitudeTorque = AmplitudeTorque;
				Target->AmplitudeVelocity = AmplitudeVelocity;
				Target->ConstantCenter = ConstantCenter;
				Target->ConstantTorque = ConstantTorque;
				Target->ConstantVelocity = ConstantVelocity;
				Target->RigidBody = RigidBody;

				return Target;
			}
			Compute::RigidBody* Acceleration::GetBody() const
			{
				return RigidBody;
			}

			Model::Model(Entity* Ref) : Drawable(Ref, Model::GetTypeId(), true)
			{
			}
			Model::~Model()
			{
				TH_RELEASE(Instance);
			}
			void Model::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				std::string Path;
				if (NMake::Unpack(Node->Find("model"), &Path))
				{
					TH_RELEASE(Instance);
					Instance = Content->Load<Graphics::Model>(Path);
				}

				SceneGraph* Scene = Parent->GetScene();
				std::vector<Core::Document*> Slots = Node->FetchCollection("materials.material");
				for (auto&& Material : Slots)
				{
					uint64_t Slot = 0;
					NMake::Unpack(Material->Find("name"), &Path);
					NMake::Unpack(Material->Find("slot"), &Slot);

					Graphics::MeshBuffer* Surface = (Instance ? Instance->FindMesh(Path) : nullptr);
					if (Surface != nullptr)
						Materials[Surface] = Scene->GetMaterial(Slot);
				}

				bool Transparent = false;
				NMake::Unpack(Node->Find("texcoord"), &TexCoord);
				NMake::Unpack(Node->Find("transparency"), &Transparent);
				NMake::Unpack(Node->Find("static"), &Static);
				SetTransparency(Transparent);
			}
			void Model::Serialize(ContentManager* Content, Core::Document* Node)
			{
				AssetCache* Asset = Content->Find<Graphics::Model>(Instance);
				if (Asset != nullptr)
					NMake::Pack(Node->Set("model"), Asset->Path);

                Core::Document* Slots = Node->Set("materials", Core::Var::Array());
				for (auto&& Slot : Materials)
				{
					if (Slot.first != nullptr)
					{
						Core::Document* Material = Slots->Set("material");
						NMake::Pack(Material->Set("name"), ((Graphics::MeshBuffer*)Slot.first)->Name);

						if (Slot.second != nullptr)
							NMake::Pack(Material->Set("slot"), Slot.second->GetSlot());
					}
				}

				NMake::Pack(Node->Set("texcoord"), TexCoord);
				NMake::Pack(Node->Set("transparency"), HasTransparency());
				NMake::Pack(Node->Set("static"), Static);
			}
			void Model::Awake(Component* New)
			{
				if (!New)
					Attach();
			}
			void Model::Asleep()
			{
				Detach();
			}
			void Model::SetDrawable(Graphics::Model* Drawable)
			{
				TH_RELEASE(Instance);
				Instance = Drawable;
			}
			float Model::Cull(const Viewer& View)
			{
				float Result = 0.0f;
				if (Instance != nullptr)
				{
					Compute::Matrix4x4 Box = GetBoundingBox();
					Result = IsVisible(View, &Box) ? 1.0f : 0.0f;
				}

				return Result;
			}
			Compute::Matrix4x4 Model::GetBoundingBox()
			{
				if (!Instance)
					return Parent->Transform->GetWorld();

				return Compute::Matrix4x4::Create(Parent->Transform->Position, Parent->Transform->Scale * Compute::Vector3(Instance->Min.W - Instance->Max.W).Div(2.0f).Abs(), Parent->Transform->Rotation);
			}
			Component* Model::Copy(Entity* New)
			{
				Model* Target = new Model(New);
				Target->SetTransparency(HasTransparency());
				Target->Visibility = Visibility;
				Target->Instance = Instance;
				Target->Materials = Materials;

				if (Target->Instance != nullptr)
					Target->Instance->AddRef();

				return Target;
			}
			Graphics::Model* Model::GetDrawable()
			{
				return Instance;
			}

			Skin::Skin(Entity* Ref) : Drawable(Ref, Skin::GetTypeId(), true)
			{
			}
			Skin::~Skin()
			{
				TH_RELEASE(Instance);
			}
			void Skin::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				std::string Path;
				if (NMake::Unpack(Node->Find("skin-model"), &Path))
				{
					TH_RELEASE(Instance);
					Instance = Content->Load<Graphics::SkinModel>(Path);
				}

				SceneGraph* Scene = Parent->GetScene();
				std::vector<Core::Document*> Slots = Node->FetchCollection("materials.material");
				for (auto&& Material : Slots)
				{
					uint64_t Slot = 0;
					NMake::Unpack(Material->Find("name"), &Path);
					NMake::Unpack(Material->Find("slot"), &Slot);

					Graphics::SkinMeshBuffer* Surface = (Instance ? Instance->FindMesh(Path) : nullptr);
					if (Surface != nullptr)
						Materials[Surface] = Scene->GetMaterial(Slot);
				}


				std::vector<Core::Document*> Poses = Node->FetchCollection("poses.pose");
				for (auto&& Pose : Poses)
				{
					int64_t Index;
					NMake::Unpack(Pose->Find("index"), &Index);

					auto& Offset = Skeleton.Pose[Index];
					NMake::Unpack(Pose->Find("position"), &Offset.Position);
					NMake::Unpack(Pose->Find("rotation"), &Offset.Rotation);
				}

				bool Transparent = false;
				NMake::Unpack(Node->Find("texcoord"), &TexCoord);
				NMake::Unpack(Node->Find("transparency"), &Transparent);
				NMake::Unpack(Node->Find("static"), &Static);
				SetTransparency(Transparent);
			}
			void Skin::Serialize(ContentManager* Content, Core::Document* Node)
			{
				AssetCache* Asset = Content->Find<Graphics::SkinModel>(Instance);
				if (Asset != nullptr)
					NMake::Pack(Node->Set("skin-model"), Asset->Path);

                Core::Document* Slots = Node->Set("materials", Core::Var::Array());
				for (auto&& Slot : Materials)
				{
					if (Slot.first != nullptr)
					{
						Core::Document* Material = Slots->Set("material");
						NMake::Pack(Material->Set("name"), ((Graphics::MeshBuffer*)Slot.first)->Name);

						if (Slot.second != nullptr)
							NMake::Pack(Material->Set("slot"), Slot.second->GetSlot());
					}
				}

				NMake::Pack(Node->Set("texcoord"), TexCoord);
				NMake::Pack(Node->Set("transparency"), HasTransparency());
				NMake::Pack(Node->Set("static"), Static);

                Core::Document* Poses = Node->Set("poses", Core::Var::Array());
				for (auto&& Pose : Skeleton.Pose)
				{
					Core::Document* Value = Poses->Set("pose");
					NMake::Pack(Value->Set("index"), Pose.first);
					NMake::Pack(Value->Set("position"), Pose.second.Position);
					NMake::Pack(Value->Set("rotation"), Pose.second.Rotation);
				}
			}
			void Skin::Synchronize(Core::Timer* Time)
			{
				if (Instance != nullptr)
					Instance->ComputePose(&Skeleton);
			}
			void Skin::Awake(Component* New)
			{
				if (!New)
					Attach();
			}
			void Skin::Asleep()
			{
				Detach();
			}
			void Skin::SetDrawable(Graphics::SkinModel* Drawable)
			{
				TH_RELEASE(Instance);
				Instance = Drawable;
			}
			float Skin::Cull(const Viewer& View)
			{
				float Result = 0.0f;
				if (Instance != nullptr)
				{
					Compute::Matrix4x4 Box = GetBoundingBox();
					Result = IsVisible(View, &Box) ? 1.0f : 0.0f;
				}

				return Result;
			}
			Compute::Matrix4x4 Skin::GetBoundingBox()
			{
				if (!Instance)
					return Parent->Transform->GetWorld();

				return Compute::Matrix4x4::Create(Parent->Transform->Position, Parent->Transform->Scale * Compute::Vector3(Instance->Min.W - Instance->Max.W).Div(2.0f).Abs(), Parent->Transform->Rotation);
			}
			Component* Skin::Copy(Entity* New)
			{
				Skin* Target = new Skin(New);
				Target->SetTransparency(HasTransparency());
				Target->Visibility = Visibility;
				Target->Instance = Instance;
				Target->Materials = Materials;

				if (Target->Instance != nullptr)
					Target->Instance->AddRef();

				return Target;
			}
			Graphics::SkinModel* Skin::GetDrawable()
			{
				return Instance;
			}

			Emitter::Emitter(Entity* Ref) : Drawable(Ref, Emitter::GetTypeId(), false)
			{
			}
			Emitter::~Emitter()
			{
				TH_RELEASE(Instance);
			}
			void Emitter::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				SceneGraph* Scene = Parent->GetScene(); uint64_t Slot = -1;
				if (NMake::Unpack(Node->Find("material"), &Slot))
					SetMaterial(nullptr, Scene->GetMaterial((uint64_t)Slot));

				bool Transparent = false;
				NMake::Unpack(Node->Find("texcoord"), &TexCoord);
				NMake::Unpack(Node->Find("transparency"), &Transparent);
				NMake::Unpack(Node->Find("quad-based"), &QuadBased);
				NMake::Unpack(Node->Find("connected"), &Connected);
				NMake::Unpack(Node->Find("static"), &Static);
				NMake::Unpack(Node->Find("volume"), &Volume);
				SetTransparency(Transparent);

				uint64_t Limit;
				if (NMake::Unpack(Node->Find("limit"), &Limit))
				{
					std::vector<Compute::ElementVertex> Vertices;
					if (Instance != nullptr)
					{
						Scene->GetDevice()->UpdateBufferSize(Instance, Limit);
						if (NMake::Unpack(Node->Find("elements"), &Vertices))
						{
							Instance->GetArray()->Reserve(Vertices.size());
							for (auto&& Vertex : Vertices)
								Instance->GetArray()->Add(Vertex);
						}
					}
				}
			}
			void Emitter::Serialize(ContentManager* Content, Core::Document* Node)
			{
				Material* Slot = GetMaterial();
				if (Slot != nullptr)
					NMake::Pack(Node->Set("material"), Slot->GetSlot());

				NMake::Pack(Node->Set("texcoord"), TexCoord);
				NMake::Pack(Node->Set("transparency"), HasTransparency());
				NMake::Pack(Node->Set("quad-based"), QuadBased);
				NMake::Pack(Node->Set("connected"), Connected);
				NMake::Pack(Node->Set("static"), Static);
				NMake::Pack(Node->Set("volume"), Volume);

				if (Instance != nullptr)
				{
					auto* fArray = Instance->GetArray();
					std::vector<Compute::ElementVertex> Vertices;
					Vertices.reserve(fArray->Size());

					for (auto It = fArray->Begin(); It != fArray->End(); It++)
						Vertices.emplace_back(*It);

					NMake::Pack(Node->Set("limit"), Instance->GetElementLimit());
					NMake::Pack(Node->Set("elements"), Vertices);
				}
			}
			void Emitter::Awake(Component* New)
			{
				if (Instance || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene)
					return;

				Graphics::InstanceBuffer::Desc I = Graphics::InstanceBuffer::Desc();
				I.ElementLimit = 1 << 10;

				Instance = Scene->GetDevice()->CreateInstanceBuffer(I);
				Attach();
			}
			void Emitter::Asleep()
			{
				Detach();
			}
			float Emitter::Cull(const Viewer& View)
			{
				float Result = 1.0f - Parent->Transform->Position.Distance(View.WorldPosition) / (View.FarPlane);
				if (Result > 0.0f)
					Result = Compute::Common::IsCubeInFrustum(Compute::Matrix4x4::CreateScale(Volume) * Parent->Transform->GetWorldUnscaled() * View.ViewProjection, 1.5f) == -1.0f ? Result : 0.0f;

				return Result;
			}
			Component* Emitter::Copy(Entity* New)
			{
				Emitter* Target = new Emitter(New);
				Target->SetTransparency(HasTransparency());
				Target->Visibility = Visibility;
				Target->Volume = Volume;
				Target->Connected = Connected;
				Target->Materials = Materials;
				Target->Instance->GetArray()->Copy(*Instance->GetArray());

				return Target;
			}
			Graphics::InstanceBuffer* Emitter::GetBuffer()
			{
				return Instance;
			}

			Decal::Decal(Entity* Ref) : Drawable(Ref, Decal::GetTypeId(), false)
			{
			}
			void Decal::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				uint64_t Slot = -1;
				if (NMake::Unpack(Node->Find("material"), &Slot))
					SetMaterial(nullptr, Parent->GetScene()->GetMaterial((uint64_t)Slot));

				bool Transparent = false;
				NMake::Unpack(Node->Find("texcoord"), &TexCoord);
				NMake::Unpack(Node->Find("projection"), &Projection);
				NMake::Unpack(Node->Find("view"), &View);
				NMake::Unpack(Node->Find("field-of-view"), &FieldOfView);
				NMake::Unpack(Node->Find("distance"), &Distance);
				NMake::Unpack(Node->Find("static"), &Static);
				NMake::Unpack(Node->Find("transparency"), &Transparent);
				SetTransparency(Transparent);
			}
			void Decal::Serialize(ContentManager* Content, Core::Document* Node)
			{
				Material* Slot = GetMaterial();
				if (Slot != nullptr)
					NMake::Pack(Node->Set("material"), Slot->GetSlot());

				NMake::Pack(Node->Set("texcoord"), TexCoord);
				NMake::Pack(Node->Set("projection"), Projection);
				NMake::Pack(Node->Set("view"), View);
				NMake::Pack(Node->Set("field-of-view"), FieldOfView);
				NMake::Pack(Node->Set("distance"), Distance);
				NMake::Pack(Node->Set("static"), Static);
				NMake::Pack(Node->Set("transparency"), HasTransparency());
			}
			void Decal::Synchronize(Core::Timer* Time)
			{
				Projection = Compute::Matrix4x4::CreatePerspective(FieldOfView, 1, 0.1f, Distance);
				View = Compute::Matrix4x4::CreateTranslation(-Parent->Transform->Position) * Compute::Matrix4x4::CreateCameraRotation(-Parent->Transform->Rotation);
			}
			void Decal::Awake(Component* New)
			{
				if (!New)
					Attach();
			}
			void Decal::Asleep()
			{
				Detach();
			}
			float Decal::Cull(const Viewer& fView)
			{
				float Result = 1.0f - Parent->Transform->Position.Distance(fView.WorldPosition) / fView.FarPlane;
				if (Result > 0.0f)
					Result = Compute::Common::IsCubeInFrustum(Parent->Transform->GetWorld() * fView.ViewProjection, GetRange()) == -1.0f ? Result : 0.0f;

				return Result;
			}
			Component* Decal::Copy(Entity* New)
			{
				Decal* Target = new Decal(New);
				Target->Visibility = Visibility;
				Target->Materials = Materials;

				return Target;
			}

			SkinAnimator::SkinAnimator(Entity* Ref) : Component(Ref)
			{
				Current.Pose.resize(96);
				Bind.Pose.resize(96);
				Default.Pose.resize(96);
			}
			SkinAnimator::~SkinAnimator()
			{
			}
			void SkinAnimator::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				std::string Path;
				if (!NMake::Unpack(Node->Find("path"), &Path))
					NMake::Unpack(Node->Find("animation"), &Clips);
				else
					GetAnimation(Content, Path);

				NMake::Unpack(Node->Find("state"), &State);
				NMake::Unpack(Node->Find("bind"), &Bind);
				NMake::Unpack(Node->Find("current"), &Current);
			}
			void SkinAnimator::Serialize(ContentManager* Content, Core::Document* Node)
			{
				if (!Reference.empty())
					NMake::Pack(Node->Set("path"), Reference);
				else
					NMake::Pack(Node->Set("animation"), Clips);

				NMake::Pack(Node->Set("state"), State);
				NMake::Pack(Node->Set("bind"), Bind);
				NMake::Pack(Node->Set("current"), Current);
			}
			void SkinAnimator::Awake(Component* New)
			{
				Components::Skin* Base = Parent->GetComponent<Components::Skin>();
				if (Base != nullptr && Base->GetDrawable() != nullptr)
				{
					Instance = Base;
					Instance->Skeleton.GetPose(Instance->GetDrawable(), &Default.Pose);
				}
				else
					Instance = nullptr;

				SetActive(Instance != nullptr);
			}
			void SkinAnimator::Synchronize(Core::Timer* Time)
			{
				if (!Parent->GetScene()->IsActive())
					return;

				if (!State.Blended)
				{
					if (State.Paused || State.Clip < 0 || State.Clip >= Clips.size() || State.Frame < 0 || State.Frame >= Clips[State.Clip].Keys.size())
						return;

					Compute::SkinAnimatorClip* Clip = &Clips[State.Clip];
					auto& NextKey = Clip->Keys[State.Frame + 1 >= Clip->Keys.size() ? 0 : State.Frame + 1];
					auto& PrevKey = Clip->Keys[State.Frame];

					State.Duration = Clip->Duration;
					State.Rate = Clip->Rate * NextKey.Time;
					State.Time = Compute::Mathf::Min(State.Time + State.Rate * (float)Time->GetDeltaTime() / State.Duration, State.Duration);

					for (auto&& Pose : Instance->Skeleton.Pose)
					{
						Compute::AnimatorKey* Prev = &PrevKey.Pose[Pose.first];
						Compute::AnimatorKey* Next = &NextKey.Pose[Pose.first];
						Compute::AnimatorKey* Set = &Current.Pose[Pose.first];
						float T = Compute::Mathf::Min(State.Time / State.Duration, 1.0f);

						Set->Position = Prev->Position.Lerp(Next->Position, T);
						Pose.second.Position = Set->Position;

						Set->Rotation = Prev->Rotation.aLerp(Next->Rotation, T);
						Pose.second.Rotation = Set->Rotation;
					}

					if (State.Time >= State.Duration)
					{
						if (State.Frame + 1 >= Clip->Keys.size())
						{
							if (!State.Looped)
								BlendAnimation(-1, -1);

							State.Frame = -1;
						}

						State.Time = 0.0f;
						if (State.Looped || State.Frame != -1)
							State.Frame++;
					}
				}
				else if (State.Time < State.Duration)
				{
					Compute::SkinAnimatorKey* Key = GetFrame(State.Clip, State.Frame);
					if (!Key)
						Key = &Bind;

					State.Time = Compute::Mathf::Min(State.Time + State.Rate * (float)Time->GetDeltaTime() / State.Duration, State.Duration);
					float T = Compute::Mathf::Min(State.Time / State.Duration, 1.0f);

					for (auto&& Pose : Instance->Skeleton.Pose)
					{
						Compute::AnimatorKey* Prev = &Current.Pose[Pose.first];
						Compute::AnimatorKey* Next = &Key->Pose[Pose.first];

						Pose.second.Position = Prev->Position.Lerp(Next->Position, T);
						Pose.second.Rotation = Prev->Rotation.aLerp(Next->Rotation, T);
					}
				}
				else
				{
					State.Blended = false;
					State.Time = 0.0f;
				}
			}
			void SkinAnimator::BlendAnimation(int64_t Clip, int64_t Frame_)
			{
				State.Blended = true;
				State.Time = 0.0f;
				State.Frame = Frame_;
				State.Clip = Clip;

				if (State.Clip >= 0 && State.Clip < Clips.size())
				{
					Compute::SkinAnimatorClip* CurrentClip = &Clips[State.Clip];
					if (State.Frame < 0 || State.Frame >= CurrentClip->Keys.size())
						State.Frame = -1;

					if (State.Duration <= 0.0f)
					{
						State.Duration = CurrentClip->Duration;
						State.Rate = CurrentClip->Rate;
					}
				}
				else
					State.Clip = -1;
			}
			bool SkinAnimator::GetAnimation(ContentManager* Content, const std::string& Path)
			{
				if (!Content)
					return false;

				Core::Document* Result = Content->Load<Core::Document>(Path);
				if (!Result)
					return false;

				ClearAnimation();
				if (NMake::Unpack(Result, &Clips))
					Reference = Core::Parser(Path).Replace(Content->GetEnvironment(), "./").Replace('\\', '/').R();

				TH_RELEASE(Result);
				return true;
			}
			void SkinAnimator::GetPose(Compute::SkinAnimatorKey* Result)
			{
				if (!Result)
					return;

				Result->Pose.resize(Default.Pose.size());
				Result->Time = Default.Time;

				for (auto&& Pose : Instance->Skeleton.Pose)
				{
					Compute::AnimatorKey* Frame = &Result->Pose[Pose.first];
					Frame->Position = Pose.second.Position;
					Frame->Rotation = Pose.second.Rotation;
				}
			}
			void SkinAnimator::ClearAnimation()
			{
				Reference.clear();
				Clips.clear();
			}
			void SkinAnimator::Stop()
			{
				Bind = Default;
				State.Paused = false;
				BlendAnimation(-1, -1);
			}
			void SkinAnimator::Pause()
			{
				State.Paused = true;
			}
			void SkinAnimator::Play(int64_t Clip, int64_t Frame_)
			{
				if (State.Paused)
				{
					State.Paused = false;
					return;
				}

				State.Time = 0.0f;
				State.Frame = (Frame_ == -1 ? 0 : Frame_);
				State.Clip = (Clip == -1 ? 0 : Clip);

				if (State.Clip >= 0 && State.Clip < Clips.size())
				{
					Compute::SkinAnimatorClip* CurrentClip = &Clips[State.Clip];
					if (State.Frame < 0 || State.Frame >= CurrentClip->Keys.size())
						State.Frame = -1;
				}
				else
					State.Clip = -1;

				GetPose(&Bind);
				Current = Bind;

				if (!IsPosed(State.Clip, State.Frame))
					BlendAnimation(State.Clip, State.Frame);
			}
			bool SkinAnimator::IsPosed(int64_t Clip, int64_t Frame_)
			{
				Compute::SkinAnimatorKey* Key = GetFrame(Clip, Frame_);
				if (!Key)
					Key = &Bind;

				for (auto&& Pose : Instance->Skeleton.Pose)
				{
					Compute::AnimatorKey* Frame = &Key->Pose[Pose.first];
					if (Pose.second.Position != Frame->Position || Pose.second.Rotation != Frame->Rotation)
						return false;
				}

				return true;
			}
			Compute::SkinAnimatorKey* SkinAnimator::GetFrame(int64_t Clip, int64_t Frame)
			{
				if (Clip < 0 || Clip >= Clips.size() || Frame < 0 || Frame >= Clips[Clip].Keys.size())
					return nullptr;

				return &Clips[Clip].Keys[Frame];
			}
			std::vector<Compute::SkinAnimatorKey>* SkinAnimator::GetClip(int64_t Clip)
			{
				if (Clip < 0 || Clip >= Clips.size())
					return nullptr;

				return &Clips[Clip].Keys;
			}
			std::string SkinAnimator::GetPath()
			{
				return Reference;
			}
			Component* SkinAnimator::Copy(Entity* New)
			{
				SkinAnimator* Target = new SkinAnimator(New);
				Target->Clips = Clips;
				Target->State = State;
				Target->Bind = Bind;
				Target->Current = Current;

				return Target;
			}
			Skin* SkinAnimator::GetSkin() const
			{
				return Instance;
			}

			KeyAnimator::KeyAnimator(Entity* Ref) : Component(Ref)
			{
			}
			KeyAnimator::~KeyAnimator()
			{
			}
			void KeyAnimator::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				std::string Path;
				if (!NMake::Unpack(Node->Find("path"), &Path))
					NMake::Unpack(Node->Find("animation"), &Clips);
				else
					GetAnimation(Content, Path);

				NMake::Unpack(Node->Find("state"), &State);
				NMake::Unpack(Node->Find("bind"), &Bind);
				NMake::Unpack(Node->Find("current"), &Current);
			}
			void KeyAnimator::Serialize(ContentManager* Content, Core::Document* Node)
			{
				if (Reference.empty())
					NMake::Pack(Node->Set("animation"), Clips);
				else
					NMake::Pack(Node->Set("path"), Reference);

				NMake::Pack(Node->Set("state"), State);
				NMake::Pack(Node->Set("bind"), Bind);
				NMake::Pack(Node->Set("current"), Current);
			}
			void KeyAnimator::Synchronize(Core::Timer* Time)
			{
				if (!Parent->GetScene()->IsActive())
					return;

				Compute::Vector3& Position = *Parent->Transform->GetLocalPosition();
				Compute::Vector3& Rotation = *Parent->Transform->GetLocalRotation();
				Compute::Vector3& Scale = *Parent->Transform->GetLocalScale();

				if (!State.Blended)
				{
					if (State.Paused || State.Clip < 0 || State.Clip >= Clips.size() || State.Frame < 0 || State.Frame >= Clips[State.Clip].Keys.size())
						return;

					Compute::KeyAnimatorClip* Clip = &Clips[State.Clip];
					Compute::AnimatorKey& NextKey = Clip->Keys[State.Frame + 1 >= Clip->Keys.size() ? 0 : State.Frame + 1];
					Compute::AnimatorKey& PrevKey = Clip->Keys[State.Frame];

					State.Duration = Clip->Duration;
					State.Rate = Clip->Rate * NextKey.Time;
					State.Time = Compute::Mathf::Min(State.Time + State.Rate * (float)Time->GetDeltaTime() / State.Duration, State.Duration);

					float T = Compute::Mathf::Min(State.Time / State.Duration, 1.0f);
					Position = Current.Position = PrevKey.Position.Lerp(NextKey.Position, T);
					Rotation = Current.Rotation = PrevKey.Rotation.aLerp(NextKey.Rotation, T);
					Scale = Current.Scale = PrevKey.Scale.Lerp(NextKey.Scale, T);

					if (State.Time >= State.Duration)
					{
						if (State.Frame + 1 >= Clip->Keys.size())
						{
							if (!State.Looped)
								BlendAnimation(-1, -1);

							State.Frame = -1;
						}

						State.Time = 0.0f;
						if (State.Looped || State.Frame != -1)
							State.Frame++;
					}
				}
				else if (State.Time < State.Duration)
				{
					Compute::AnimatorKey* Key = GetFrame(State.Clip, State.Frame);
					if (!Key)
						Key = &Bind;

					if (State.Paused)
						return;

					State.Time = Compute::Mathf::Min(State.Time + State.Rate * (float)Time->GetDeltaTime() / State.Duration, State.Duration);
					float T = Compute::Mathf::Min(State.Time / State.Duration, 1.0f);

					Position = Current.Position.Lerp(Key->Position, T);
					Rotation = Current.Rotation.aLerp(Key->Rotation, T);
					Scale = Current.Scale.Lerp(Key->Scale, T);
				}
				else
				{
					State.Blended = false;
					State.Time = 0.0f;
				}
			}
			bool KeyAnimator::GetAnimation(ContentManager* Content, const std::string& Path)
			{
				if (!Content)
					return false;

				Core::Document* Result = Content->Load<Core::Document>(Path);
				if (!Result)
					return false;

				ClearAnimation();
				if (NMake::Unpack(Result, &Clips))
					Reference = Core::Parser(Path).Replace(Content->GetEnvironment(), "./").Replace('\\', '/').R();

				TH_RELEASE(Result);
				return true;
			}
			void KeyAnimator::GetPose(Compute::AnimatorKey* Result)
			{
				if (!Result)
					return;

				Result->Position = *Parent->Transform->GetLocalPosition();
				Result->Rotation = *Parent->Transform->GetLocalRotation();
				Result->Scale = *Parent->Transform->GetLocalScale();
			}
			void KeyAnimator::ClearAnimation()
			{
				Reference.clear();
				Clips.clear();
			}
			void KeyAnimator::BlendAnimation(int64_t Clip, int64_t Frame_)
			{
				State.Blended = true;
				State.Time = 0.0f;
				State.Frame = Frame_;
				State.Clip = Clip;

				if (State.Clip >= 0 && State.Clip < Clips.size())
				{
					Compute::KeyAnimatorClip* CurrentClip = &Clips[State.Clip];
					if (State.Frame < 0 || State.Frame >= CurrentClip->Keys.size())
						State.Frame = -1;
				}
				else
					State.Clip = -1;
			}
			void KeyAnimator::Stop()
			{
				State.Paused = false;
				BlendAnimation(-1, -1);
			}
			void KeyAnimator::Pause()
			{
				State.Paused = true;
			}
			void KeyAnimator::Play(int64_t Clip, int64_t Frame_)
			{
				if (State.Paused)
				{
					State.Paused = false;
					return;
				}

				State.Time = 0.0f;
				State.Frame = (Frame_ == -1 ? 0 : Frame_);
				State.Clip = (Clip == -1 ? 0 : Clip);

				if (State.Clip >= 0 && State.Clip < Clips.size())
				{
					Compute::KeyAnimatorClip* CurrentClip = &Clips[State.Clip];
					if (State.Frame < 0 || State.Frame >= CurrentClip->Keys.size())
						State.Frame = -1;

					if (State.Duration <= 0.0f)
					{
						State.Duration = CurrentClip->Duration;
						State.Rate = CurrentClip->Rate;
					}
				}
				else
					State.Clip = -1;

				GetPose(&Bind);
				Current = Bind;

				if (!IsPosed(State.Clip, State.Frame))
					BlendAnimation(State.Clip, State.Frame);
			}
			bool KeyAnimator::IsPosed(int64_t Clip, int64_t Frame_)
			{
				Compute::AnimatorKey* Key = GetFrame(Clip, Frame_);
				if (!Key)
					Key = &Bind;

				return *Parent->Transform->GetLocalPosition() == Key->Position && *Parent->Transform->GetLocalRotation() == Key->Rotation && *Parent->Transform->GetLocalScale() == Key->Scale;
			}
			Compute::AnimatorKey* KeyAnimator::GetFrame(int64_t Clip, int64_t Frame)
			{
				if (Clip < 0 || Clip >= Clips.size() || Frame < 0 || Frame >= Clips[Clip].Keys.size())
					return nullptr;

				return &Clips[Clip].Keys[Frame];
			}
			std::vector<Compute::AnimatorKey>* KeyAnimator::GetClip(int64_t Clip)
			{
				if (Clip < 0 || Clip >= Clips.size())
					return nullptr;

				return &Clips[Clip].Keys;
			}
			std::string KeyAnimator::GetPath()
			{
				return Reference;
			}
			Component* KeyAnimator::Copy(Entity* New)
			{
				KeyAnimator* Target = new KeyAnimator(New);
				Target->Clips = Clips;
				Target->State = State;
				Target->Bind = Bind;
				Target->Current = Current;

				return Target;
			}

			EmitterAnimator::EmitterAnimator(Entity* Ref) : Component(Ref)
			{
				Spawner.Scale.Max = 1;
				Spawner.Scale.Min = 1;
				Spawner.Rotation.Max = 0;
				Spawner.Rotation.Min = 0;
				Spawner.Angular.Max = 0;
				Spawner.Angular.Min = 0;
				Spawner.Diffusion.Min = 1;
				Spawner.Diffusion.Max = 1;
				Spawner.Velocity.Min = 0;
				Spawner.Velocity.Max = 0;
				Spawner.Position.Min = -1;
				Spawner.Position.Max = 1;
				Spawner.Noise.Min = -1;
				Spawner.Noise.Max = 1;
				Spawner.Iterations = 1;
			}
			void EmitterAnimator::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Unpack(Node->Find("diffuse"), &Diffuse);
				NMake::Unpack(Node->Find("position"), &Position);
				NMake::Unpack(Node->Find("velocity"), &Velocity);
				NMake::Unpack(Node->Find("spawner"), &Spawner);
				NMake::Unpack(Node->Find("noise"), &Noise);
				NMake::Unpack(Node->Find("rotation-speed"), &RotationSpeed);
				NMake::Unpack(Node->Find("scale-speed"), &ScaleSpeed);
				NMake::Unpack(Node->Find("simulate"), &Simulate);
			}
			void EmitterAnimator::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("diffuse"), Diffuse);
				NMake::Pack(Node->Set("position"), Position);
				NMake::Pack(Node->Set("velocity"), Velocity);
				NMake::Pack(Node->Set("spawner"), Spawner);
				NMake::Pack(Node->Set("noise"), Noise);
				NMake::Pack(Node->Set("rotation-speed"), RotationSpeed);
				NMake::Pack(Node->Set("scale-speed"), ScaleSpeed);
				NMake::Pack(Node->Set("simulate"), Simulate);
			}
			void EmitterAnimator::Awake(Component* New)
			{
				Base = Parent->GetComponent<Emitter>();
				SetActive(Base != nullptr);
			}
			void EmitterAnimator::Synchronize(Core::Timer* Time)
			{
				static size_t PositionX = offsetof(Compute::ElementVertex, PositionX);
				static size_t VelocityX = offsetof(Compute::ElementVertex, VelocityX);
				static size_t ColorX = offsetof(Compute::ElementVertex, ColorX);

				if (!Parent->GetScene()->IsActive() || !Base || !Base->GetBuffer())
					return;

				Core::Pool<Compute::ElementVertex>* Array = Base->GetBuffer()->GetArray();
				for (int i = 0; i < Spawner.Iterations; i++)
				{
					if (Array->Size() >= Array->Capacity())
						break;

					Compute::Vector3 FPosition = (Base->Connected ? Spawner.Position.Generate() : Spawner.Position.Generate() + Parent->Transform->Position.InvZ());
					Compute::Vector3 FVelocity = Spawner.Velocity.Generate();
					Compute::Vector4 FDiffusion = Spawner.Diffusion.Generate();

					Compute::ElementVertex Element;
					memcpy(&Element + PositionX, &FPosition, sizeof(float) * 3);
					memcpy(&Element + VelocityX, &FVelocity, sizeof(float) * 3);
					memcpy(&Element + ColorX, &FDiffusion, sizeof(float) * 4);
					Element.Angular = Spawner.Angular.Generate();
					Element.Rotation = Spawner.Rotation.Generate();
					Element.Scale = Spawner.Scale.Generate();
					Array->Add(Element);
				}

				if (!Simulate)
					return;

				float DeltaTime = (float)Time->GetDeltaTime();
				if (Noise != 0.0f)
					AccurateSynchronization(DeltaTime);
				else
					FastSynchronization(DeltaTime);
			}
			void EmitterAnimator::AccurateSynchronization(float DeltaTime)
			{
				Core::Pool<Compute::ElementVertex>* Array = Base->GetBuffer()->GetArray();
				float L = Velocity.Length();

				for (auto It = Array->Begin(); It != Array->End(); It++)
				{
					Compute::Vector3 NextVelocity(It->VelocityX, It->VelocityY, It->VelocityZ);
					Compute::Vector3 NextNoise = Spawner.Noise.Generate() / Noise;
					Compute::Vector3 NextPosition(It->PositionX, It->PositionY, It->PositionZ);
					Compute::Vector4 NextDiffuse(It->ColorX, It->ColorY, It->ColorZ, It->ColorW);
					NextPosition += (NextVelocity + Position + NextNoise) * DeltaTime;
					NextDiffuse += Diffuse * DeltaTime;
					memcpy(&It->PositionX, &NextPosition, sizeof(float) * 3);
					memcpy(&It->ColorX, &NextDiffuse, sizeof(float) * 4);
					It->Rotation += (It->Angular + RotationSpeed) * DeltaTime;
					It->Scale += ScaleSpeed * DeltaTime;

					if (L > 0)
					{
						NextVelocity -= (NextVelocity / Velocity) * DeltaTime;
						memcpy(&It->VelocityX, &NextVelocity, sizeof(float) * 3);
					}

					if (It->ColorW <= 0 || It->Scale <= 0)
					{
						Array->RemoveAt(It);
						It--;
					}
				}
			}
			void EmitterAnimator::FastSynchronization(float DeltaTime)
			{
				Core::Pool<Compute::ElementVertex>* Array = Base->GetBuffer()->GetArray();
				float L = Velocity.Length();

				for (auto It = Array->Begin(); It != Array->End(); It++)
				{
					Compute::Vector3 NextVelocity(It->VelocityX, It->VelocityY, It->VelocityZ);
					Compute::Vector3 NextPosition(It->PositionX, It->PositionY, It->PositionZ);
					Compute::Vector4 NextDiffuse(It->ColorX, It->ColorY, It->ColorZ, It->ColorW);
					NextPosition += (NextVelocity + Position) * DeltaTime;
					NextDiffuse += Diffuse * DeltaTime;
					memcpy(&It->PositionX, &NextPosition, sizeof(float) * 3);
					memcpy(&It->ColorX, &NextDiffuse, sizeof(float) * 4);
					It->Rotation += (It->Angular + RotationSpeed) * DeltaTime;
					It->Scale += ScaleSpeed * DeltaTime;

					if (L > 0)
					{
						NextVelocity -= (NextVelocity / Velocity) * DeltaTime;
						memcpy(&It->VelocityX, &NextVelocity, sizeof(float) * 3);
					}

					if (It->ColorW <= 0 || It->Scale <= 0)
					{
						Array->RemoveAt(It);
						It--;
					}
				}
			}
			Component* EmitterAnimator::Copy(Entity* New)
			{
				EmitterAnimator* Target = new EmitterAnimator(New);
				Target->Diffuse = Diffuse;
				Target->Position = Position;
				Target->Velocity = Velocity;
				Target->ScaleSpeed = ScaleSpeed;
				Target->RotationSpeed = RotationSpeed;
				Target->Spawner = Spawner;
				Target->Noise = Noise;
				Target->Simulate = Simulate;

				return Target;
			}
			Emitter* EmitterAnimator::GetEmitter() const
			{
				return Base;
			}

			FreeLook::FreeLook(Entity* Ref) : Component(Ref), Activity(nullptr), Rotate(Graphics::KeyCode::CURSORRIGHT), Sensivity(0.005f)
			{
			}
			void FreeLook::Awake(Component* New)
			{
				Application* App = Application::Get();
				if (App == nullptr)
					return SetActive(false);

				Activity = App->Activity;
				SetActive(Activity != nullptr);
			}
			void FreeLook::Update(Core::Timer* Time)
			{
				if (!Activity)
					return;

				if (Activity->IsKeyDown(Rotate))
				{
					Compute::Vector2 Cursor = Activity->GetGlobalCursorPosition();
					if (!Activity->IsKeyDownHit(Rotate))
					{
						Compute::Vector2 Next = (Cursor - Position) * Sensivity;
						Parent->Transform->Rotation += Compute::Vector3(Next.Y, Next.X);
						Parent->Transform->Rotation.X = Compute::Mathf::Clamp(Parent->Transform->Rotation.X, -1.57079632679f, 1.57079632679f);
					}
					else
						Position = Cursor;

					if ((int)Cursor.X != (int)Position.X || (int)Cursor.Y != (int)Position.Y)
						Activity->SetGlobalCursorPosition(Position);
				}
			}
			Component* FreeLook::Copy(Entity* New)
			{
				FreeLook* Target = new FreeLook(New);
				Target->Activity = Activity;
				Target->Position = Position;
				Target->Rotate = Rotate;
				Target->Sensivity = Sensivity;

				return Target;
			}
			Graphics::Activity* FreeLook::GetActivity() const
			{
				return Activity;
			}

			Fly::Fly(Entity* Ref) : Component(Ref), Activity(nullptr)
			{
			}
			void Fly::Awake(Component* New)
			{
				Application* App = Application::Get();
				if (App == nullptr)
					return SetActive(false);

				Activity = App->Activity;
				SetActive(Activity != nullptr);
			}
			void Fly::Update(Core::Timer* Time)
			{
				if (!Activity)
					return;

				float DeltaTime = (float)Time->GetDeltaTime();
				Compute::Vector3 Speed = Axis * DeltaTime * SpeedNormal;

				if (Activity->IsKeyDown(Fast))
					Speed = Axis * DeltaTime * SpeedUp;

				if (Activity->IsKeyDown(Slow))
					Speed = Axis * DeltaTime * SpeedDown;

				if (Activity->IsKeyDown(Forward))
					Parent->Transform->Position += Parent->Transform->Forward() * Speed;

				if (Activity->IsKeyDown(Backward))
					Parent->Transform->Position -= Parent->Transform->Forward() * Speed;

				if (Activity->IsKeyDown(Right))
					Parent->Transform->Position += Parent->Transform->Right() * Speed;

				if (Activity->IsKeyDown(Left))
					Parent->Transform->Position -= Parent->Transform->Right() * Speed;

				if (Activity->IsKeyDown(Up))
					Parent->Transform->Position += Parent->Transform->Up() * Speed;

				if (Activity->IsKeyDown(Down))
					Parent->Transform->Position -= Parent->Transform->Up() * Speed;
			}
			Component* Fly::Copy(Entity* New)
			{
				Fly* Target = new Fly(New);
				Target->Activity = Activity;
				Target->Forward = Forward;
				Target->Backward = Backward;
				Target->Right = Right;
				Target->Left = Left;
				Target->Up = Up;
				Target->Down = Down;
				Target->Fast = Fast;
				Target->Slow = Slow;
				Target->Axis = Axis;
				Target->SpeedNormal = SpeedNormal;
				Target->SpeedUp = SpeedUp;
				Target->SpeedDown = SpeedDown;

				return Target;
			}
			Graphics::Activity* Fly::GetActivity() const
			{
				return Activity;
			}

			AudioSource::AudioSource(Entity* Ref) : Component(Ref)
			{
				Source = new Audio::AudioSource();
			}
			AudioSource::~AudioSource()
			{
				TH_RELEASE(Source);
			}
			void AudioSource::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				std::string Path;
				if (NMake::Unpack(Node->Find("audio-clip"), &Path))
					Source->SetClip(Content->Load<Audio::AudioClip>(Path));

				NMake::Unpack(Node->Find("velocity"), &Sync.Velocity);
				NMake::Unpack(Node->Find("direction"), &Sync.Direction);
				NMake::Unpack(Node->Find("rolloff"), &Sync.Rolloff);
				NMake::Unpack(Node->Find("cone-inner-angle"), &Sync.ConeInnerAngle);
				NMake::Unpack(Node->Find("cone-outer-angle"), &Sync.ConeOuterAngle);
				NMake::Unpack(Node->Find("cone-outer-gain"), &Sync.ConeOuterGain);
				NMake::Unpack(Node->Find("distance"), &Sync.Distance);
				NMake::Unpack(Node->Find("gain"), &Sync.Gain);
				NMake::Unpack(Node->Find("pitch"), &Sync.Pitch);
				NMake::Unpack(Node->Find("ref-distance"), &Sync.RefDistance);
				NMake::Unpack(Node->Find("position"), &Sync.Position);
				NMake::Unpack(Node->Find("relative"), &Sync.IsRelative);
				NMake::Unpack(Node->Find("looped"), &Sync.IsLooped);
				NMake::Unpack(Node->Find("distance"), &Sync.Distance);
				NMake::Unpack(Node->Find("air-absorption"), &Sync.AirAbsorption);
				NMake::Unpack(Node->Find("room-roll-off"), &Sync.RoomRollOff);

				std::vector<Core::Document*> Effects = Node->FetchCollection("effects.effect");
				for (auto& Effect : Effects)
				{
					uint64_t Id;
					if (!NMake::Unpack(Effect->Find("id"), &Id))
						continue;

					Audio::AudioEffect* Target = Core::Composer::Create<Audio::AudioEffect>(Id);
					if (!Target)
					{
						TH_WARN("audio effect with id %llu cannot be created", Id);
						continue;
					}

					Core::Document* Meta = Effect->Find("metadata");
					if (!Meta)
						Meta = Effect->Set("metadata");

					Target->Deserialize(Meta);
					Source->AddEffect(Target);
				}

				bool Autoplay;
				if (NMake::Unpack(Node->Find("autoplay"), &Autoplay) && Autoplay && Source->GetClip())
					Source->Play();

				ApplyPlayingPosition();
				Synchronize(nullptr);
			}
			void AudioSource::Serialize(ContentManager* Content, Core::Document* Node)
			{
				AssetCache* Asset = Content->Find<Audio::AudioClip>(Source->GetClip());
				if (Asset != nullptr)
					NMake::Pack(Node->Set("audio-clip"), Asset->Path);

                Core::Document* Effects = Node->Set("effects", Core::Var::Array());
				for (auto* Effect : *Source->GetEffects())
				{
					if (!Effect)
						continue;

					Core::Document* Element = Effects->Set("effect");
					NMake::Pack(Element->Set("id"), Effect->GetId());
					Effect->Serialize(Element->Set("metadata"));
				}

				NMake::Pack(Node->Set("velocity"), Sync.Velocity);
				NMake::Pack(Node->Set("direction"), Sync.Direction);
				NMake::Pack(Node->Set("rolloff"), Sync.Rolloff);
				NMake::Pack(Node->Set("cone-inner-angle"), Sync.ConeInnerAngle);
				NMake::Pack(Node->Set("cone-outer-angle"), Sync.ConeOuterAngle);
				NMake::Pack(Node->Set("cone-outer-gain"), Sync.ConeOuterGain);
				NMake::Pack(Node->Set("distance"), Sync.Distance);
				NMake::Pack(Node->Set("gain"), Sync.Gain);
				NMake::Pack(Node->Set("pitch"), Sync.Pitch);
				NMake::Pack(Node->Set("ref-distance"), Sync.RefDistance);
				NMake::Pack(Node->Set("position"), Sync.Position);
				NMake::Pack(Node->Set("relative"), Sync.IsRelative);
				NMake::Pack(Node->Set("looped"), Sync.IsLooped);
				NMake::Pack(Node->Set("distance"), Sync.Distance);
				NMake::Pack(Node->Set("autoplay"), Source->IsPlaying());
				NMake::Pack(Node->Set("air-absorption"), Sync.AirAbsorption);
				NMake::Pack(Node->Set("room-roll-off"), Sync.RoomRollOff);
			}
			void AudioSource::Synchronize(Core::Timer* Time)
			{
				if (Time != nullptr && Time->GetDeltaTime() > 0.0)
				{
					Sync.Velocity = (Parent->Transform->Position - LastPosition) * Time->GetDeltaTime();
					LastPosition = Parent->Transform->Position;
				}

				if (Source->GetClip())
					Source->Synchronize(&Sync, Parent->Transform->Position);
			}
			void AudioSource::ApplyPlayingPosition()
			{
				Audio::AudioContext::SetSourceData1F(Source->GetInstance(), Audio::SoundEx::Seconds_Offset, Sync.Position);
			}
			Component* AudioSource::Copy(Entity* New)
			{
				AudioSource* Target = new AudioSource(New);
				Target->LastPosition = LastPosition;
				Target->Source->SetClip(Source->GetClip());
				Target->Sync = Sync;

				for (auto* Effect : *Source->GetEffects())
				{
					if (Effect != nullptr)
						Target->Source->AddEffect(Effect->Copy());
				}

				return Target;
			}
			Audio::AudioSource* AudioSource::GetSource() const
			{
				return Source;
			}
			Audio::AudioSync& AudioSource::GetSync()
			{
				return Sync;
			}

			AudioListener::AudioListener(Entity* Ref) : Component(Ref)
			{
			}
			AudioListener::~AudioListener()
			{
				Asleep();
			}
			void AudioListener::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Unpack(Node->Find("gain"), &Gain);
			}
			void AudioListener::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("gain"), Gain);
			}
			void AudioListener::Synchronize(Core::Timer* Time)
			{
				Compute::Vector3 Velocity;
				if (Time != nullptr && Time->GetDeltaTime() > 0.0)
				{
					Velocity = (Parent->Transform->Position - LastPosition) * Time->GetDeltaTime();
					LastPosition = Parent->Transform->Position;
				}

				Compute::Vector3 Rotation = Parent->Transform->Rotation.dDirection();
				float LookAt[6] = { Rotation.X, Rotation.Y, Rotation.Z, 0.0f, 1.0f, 0.0f };

				Audio::AudioContext::SetListenerData3F(Audio::SoundEx::Velocity, Velocity.X, Velocity.Y, Velocity.Z);
				Audio::AudioContext::SetListenerData3F(Audio::SoundEx::Position, -Parent->Transform->Position.X, -Parent->Transform->Position.Y, Parent->Transform->Position.Z);
				Audio::AudioContext::SetListenerDataVF(Audio::SoundEx::Orientation, LookAt);
				Audio::AudioContext::SetListenerData1F(Audio::SoundEx::Gain, Gain);
			}
			void AudioListener::Asleep()
			{
				float LookAt[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };

				Audio::AudioContext::SetListenerData3F(Audio::SoundEx::Velocity, 0.0f, 0.0f, 0.0f);
				Audio::AudioContext::SetListenerData3F(Audio::SoundEx::Position, 0.0f, 0.0f, 0.0f);
				Audio::AudioContext::SetListenerDataVF(Audio::SoundEx::Orientation, LookAt);
				Audio::AudioContext::SetListenerData1F(Audio::SoundEx::Gain, 0.0f);
			}
			Component* AudioListener::Copy(Entity* New)
			{
				AudioListener* Target = new AudioListener(New);
				Target->LastPosition = LastPosition;
				Target->Gain = Gain;

				return Target;
			}

			PointLight::PointLight(Entity* Ref) : Cullable(Ref)
			{
			}
			void PointLight::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Unpack(Node->Find("projection"), &Projection);
				NMake::Unpack(Node->Find("view"), &View);
				NMake::Unpack(Node->Find("size"), &Size);
				NMake::Unpack(Node->Find("diffuse"), &Diffuse);
				NMake::Unpack(Node->Find("emission"), &Emission);
				NMake::Unpack(Node->Find("disperse"), &Disperse);
				NMake::Unpack(Node->Find("shadow-softness"), &Shadow.Softness);
				NMake::Unpack(Node->Find("shadow-distance"), &Shadow.Distance);
				NMake::Unpack(Node->Find("shadow-bias"), &Shadow.Bias);
				NMake::Unpack(Node->Find("shadow-iterations"), &Shadow.Iterations);
				NMake::Unpack(Node->Find("shadow-enabled"), &Shadow.Enabled);
			}
			void PointLight::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("projection"), Projection);
				NMake::Pack(Node->Set("view"), View);
				NMake::Pack(Node->Set("size"), Size);
				NMake::Pack(Node->Set("diffuse"), Diffuse);
				NMake::Pack(Node->Set("emission"), Emission);
				NMake::Pack(Node->Set("disperse"), Disperse);
				NMake::Pack(Node->Set("shadow-softness"), Shadow.Softness);
				NMake::Pack(Node->Set("shadow-distance"), Shadow.Distance);
				NMake::Pack(Node->Set("shadow-bias"), Shadow.Bias);
				NMake::Pack(Node->Set("shadow-iterations"), Shadow.Iterations);
				NMake::Pack(Node->Set("shadow-enabled"), Shadow.Enabled);
			}
			float PointLight::Cull(const Viewer& Base)
			{
				float Result = 1.0f - Parent->Transform->Position.Distance(Base.WorldPosition) / Base.FarPlane;
				if (Result > 0.0f)
					Result = Compute::Common::IsCubeInFrustum(Parent->Transform->GetWorldUnscaled() * Base.ViewProjection, GetBoxRange()) == -1.0f ? Result : 0.0f;

				return Result;
			}
			bool PointLight::IsVisible(const Viewer& fView, Compute::Matrix4x4* World)
			{
				if (Parent->Transform->Position.Distance(fView.WorldPosition) > fView.FarPlane + GetBoxRange())
					return false;

				return Compute::Common::IsCubeInFrustum((World ? *World : Parent->Transform->GetWorld()) * fView.ViewProjection, 1.65f) == -1.0f;
			}
			bool PointLight::IsNear(const Viewer& fView)
			{
				return Parent->Transform->Position.Distance(fView.WorldPosition) <= fView.FarPlane + GetBoxRange();
			}
			Component* PointLight::Copy(Entity* New)
			{
				PointLight* Target = new PointLight(New);
				Target->Diffuse = Diffuse;
				Target->Emission = Emission;
				Target->Visibility = Visibility;
				Target->Projection = Projection;
				Target->View = View;
				Target->Size = Size;
				memcpy(&Target->Shadow, &Shadow, sizeof(Shadow));

				return Target;
			}
			void PointLight::AssembleDepthOrigin()
			{
				Projection = Compute::Matrix4x4::CreatePerspective(90.0f, 1.0f, 0.1f, Shadow.Distance);
				View = Compute::Matrix4x4::CreateCubeMapLookAt(0, Parent->Transform->Position.InvZ());
			}
			float PointLight::GetBoxRange() const
			{
				return Size.Range * 1.25;
			}
			
			SpotLight::SpotLight(Entity* Ref) : Cullable(Ref)
			{
			}
			void SpotLight::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Unpack(Node->Find("projection"), &Projection);
				NMake::Unpack(Node->Find("view"), &View);
				NMake::Unpack(Node->Find("size"), &Size);
				NMake::Unpack(Node->Find("diffuse"), &Diffuse);
				NMake::Unpack(Node->Find("emission"), &Emission);
				NMake::Unpack(Node->Find("disperse"), &Disperse);
				NMake::Unpack(Node->Find("cutoff"), &Cutoff);
				NMake::Unpack(Node->Find("shadow-bias"), &Shadow.Bias);
				NMake::Unpack(Node->Find("shadow-distance"), &Shadow.Distance);
				NMake::Unpack(Node->Find("shadow-softness"), &Shadow.Softness);
				NMake::Unpack(Node->Find("shadow-iterations"), &Shadow.Iterations);
				NMake::Unpack(Node->Find("shadow-enabled"), &Shadow.Enabled);
			}
			void SpotLight::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("projection"), Projection);
				NMake::Pack(Node->Set("view"), View);
				NMake::Pack(Node->Set("size"), Size);
				NMake::Pack(Node->Set("diffuse"), Diffuse);
				NMake::Pack(Node->Set("emission"), Emission);
				NMake::Pack(Node->Set("disperse"), Disperse);
				NMake::Pack(Node->Set("cutoff"), Cutoff);
				NMake::Pack(Node->Set("shadow-bias"), Shadow.Bias);
				NMake::Pack(Node->Set("shadow-distance"), Shadow.Distance);
				NMake::Pack(Node->Set("shadow-softness"), Shadow.Softness);
				NMake::Pack(Node->Set("shadow-iterations"), Shadow.Iterations);
				NMake::Pack(Node->Set("shadow-enabled"), Shadow.Enabled);
			}
			void SpotLight::Synchronize(Core::Timer* Time)
			{
				Cutoff = Compute::Mathf::Clamp(Cutoff, 0.0f, 180.0f);
			}
			float SpotLight::Cull(const Viewer& fView)
			{
				float Result = 1.0f - Parent->Transform->Position.Distance(fView.WorldPosition) / fView.FarPlane;
				if (Result > 0.0f)
					Result = Compute::Common::IsCubeInFrustum(Parent->Transform->GetWorldUnscaled() * fView.ViewProjection, GetBoxRange()) == -1.0f ? Result : 0.0f;

				return Result;
			}
			bool SpotLight::IsVisible(const Viewer& fView, Compute::Matrix4x4* World)
			{
				if (Parent->Transform->Position.Distance(fView.WorldPosition) > fView.FarPlane + GetBoxRange())
					return false;

				return Compute::Common::IsCubeInFrustum((World ? *World : Parent->Transform->GetWorld()) * fView.ViewProjection, 1.65f) == -1.0f;
			}
			bool SpotLight::IsNear(const Viewer& fView)
			{
				return Parent->Transform->Position.Distance(fView.WorldPosition) <= fView.FarPlane + GetBoxRange();
			}
			Component* SpotLight::Copy(Entity* New)
			{
				SpotLight* Target = new SpotLight(New);
				Target->Diffuse = Diffuse;
				Target->Projection = Projection;
				Target->View = View;
				Target->Size = Size;
				Target->Cutoff = Cutoff;
				Target->Emission = Emission;
				memcpy(&Target->Shadow, &Shadow, sizeof(Shadow));

				return Target;
			}
			void SpotLight::AssembleDepthOrigin()
			{
				Projection = Compute::Matrix4x4::CreatePerspective(Cutoff, 1, 0.1f, Shadow.Distance);
				View = Compute::Matrix4x4::CreateTranslation(-Parent->Transform->Position) * Compute::Matrix4x4::CreateCameraRotation(-Parent->Transform->Rotation);
			}
			float SpotLight::GetBoxRange() const
			{
				return Size.Range * 1.25;
			}

			LineLight::LineLight(Entity* Ref) : Component(Ref)
			{
			}
			void LineLight::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Unpack(Node->Find("diffuse"), &Diffuse);
				NMake::Unpack(Node->Find("emission"), &Emission);
				NMake::Unpack(Node->Find("disperse"), &Disperse);

				for (uint32_t i = 0; i < 6; i++)
				{
					NMake::Unpack(Node->Find("projection-" + std::to_string(i)), &Projection[i]);
					NMake::Unpack(Node->Find("view-" + std::to_string(i)), &View[i]);
				}

				for (uint32_t i = 0; i < 6; i++)
					NMake::Unpack(Node->Find("shadow-distance-" + std::to_string(i)), &Shadow.Distance[i]);

				NMake::Unpack(Node->Find("shadow-cascades"), &Shadow.Cascades);
				NMake::Unpack(Node->Find("shadow-bias"), &Shadow.Bias);
				NMake::Unpack(Node->Find("shadow-offset"), &Shadow.Offset);
				NMake::Unpack(Node->Find("shadow-softness"), &Shadow.Softness);
				NMake::Unpack(Node->Find("shadow-iterations"), &Shadow.Iterations);
				NMake::Unpack(Node->Find("shadow-enabled"), &Shadow.Enabled);
				NMake::Unpack(Node->Find("rlh-emission"), &Sky.RlhEmission);
				NMake::Unpack(Node->Find("mie-emission"), &Sky.MieEmission);
				NMake::Unpack(Node->Find("rlh-height"), &Sky.RlhHeight);
				NMake::Unpack(Node->Find("mie-height"), &Sky.MieEmission);
				NMake::Unpack(Node->Find("mie-direction"), &Sky.MieDirection);
				NMake::Unpack(Node->Find("inner-radius"), &Sky.InnerRadius);
				NMake::Unpack(Node->Find("outer-radius"), &Sky.OuterRadius);
				NMake::Unpack(Node->Find("sky-intensity"), &Sky.Intensity);
			}
			void LineLight::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("diffuse"), Diffuse);
				NMake::Pack(Node->Set("emission"), Emission);
				NMake::Pack(Node->Set("disperse"), Disperse);

				for (uint32_t i = 0; i < 6; i++)
				{
					NMake::Pack(Node->Set("projection-" + std::to_string(i)), Projection[i]);
					NMake::Pack(Node->Set("view-" + std::to_string(i)), View[i]);
				}

				for (uint32_t i = 0; i < 6; i++)
					NMake::Pack(Node->Set("shadow-distance-" + std::to_string(i)), Shadow.Distance[i]);

				NMake::Pack(Node->Set("shadow-cascades"), Shadow.Cascades);
				NMake::Pack(Node->Set("shadow-bias"), Shadow.Bias);
				NMake::Pack(Node->Set("shadow-offset"), Shadow.Offset);
				NMake::Pack(Node->Set("shadow-softness"), Shadow.Softness);
				NMake::Pack(Node->Set("shadow-iterations"), Shadow.Iterations);
				NMake::Pack(Node->Set("shadow-enabled"), Shadow.Enabled);
				NMake::Pack(Node->Set("rlh-emission"), Sky.RlhEmission);
				NMake::Pack(Node->Set("mie-emission"), Sky.MieEmission);
				NMake::Pack(Node->Set("rlh-height"), Sky.RlhHeight);
				NMake::Pack(Node->Set("mie-height"), Sky.MieEmission);
				NMake::Pack(Node->Set("mie-direction"), Sky.MieDirection);
				NMake::Pack(Node->Set("inner-radius"), Sky.InnerRadius);
				NMake::Pack(Node->Set("outer-radius"), Sky.OuterRadius);
				NMake::Pack(Node->Set("sky-intensity"), Sky.Intensity);
			}
			Component* LineLight::Copy(Entity* New)
			{
				LineLight* Target = new LineLight(New);
				Target->Diffuse = Diffuse;
				Target->Emission = Emission;
				memcpy(Target->Projection, Projection, sizeof(Compute::Matrix4x4) * 6);
				memcpy(Target->View, View, sizeof(Compute::Matrix4x4) * 6);
				memcpy(&Target->Shadow, &Shadow, sizeof(Shadow));
				memcpy(&Target->Sky, &Sky, sizeof(Sky));

				return Target;
			}
			void LineLight::AssembleDepthOrigin()
			{
				auto* Viewer = Parent->GetScene()->GetCamera()->As<Camera>();
				auto* Transform = Viewer->GetEntity()->Transform;
				Compute::Vector3 Eye = Transform->Position * Compute::Vector3(1.0f, 0.1f, 1.0f);
				Compute::Vector3 Up = Transform->GetWorld().Right();
				Compute::Matrix4x4 Look = Compute::Matrix4x4::CreateLockedLookAt(Parent->Transform->Position, Eye, Up);
				float Near = -Viewer->FarPlane - Viewer->NearPlane;
				float Far = Viewer->FarPlane;

				if (Shadow.Cascades > 6)
					return;

				for (uint32_t i = 0; i < Shadow.Cascades; i++)
				{
					float Distance = Shadow.Distance[i];
					Projection[i] = Compute::Matrix4x4::CreateOrthographic(Distance, Distance, Near, Far) *
						Compute::Matrix4x4::CreateTranslation(Compute::Vector3((float)i * Shadow.Offset, 0.0));
					View[i] = Look;
				}
			}

			SurfaceLight::SurfaceLight(Entity* Ref) : Cullable(Ref), Projection(Compute::Matrix4x4::CreatePerspectiveRad(1.57079632679f, 1, 0.01f, 100.0f))
			{
			}
			SurfaceLight::~SurfaceLight()
			{
				TH_RELEASE(DiffuseMapX[0]);
				TH_RELEASE(DiffuseMapX[1]);
				TH_RELEASE(DiffuseMapY[0]);
				TH_RELEASE(DiffuseMapY[1]);
				TH_RELEASE(DiffuseMapZ[0]);
				TH_RELEASE(DiffuseMapZ[1]);
				TH_RELEASE(DiffuseMap);
				TH_RELEASE(Probe);
			}
			void SurfaceLight::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				std::string Path;
				if (!NMake::Unpack(Node->Find("diffuse-map"), &Path))
				{
					if (NMake::Unpack(Node->Find("diffuse-map-px"), &Path))
					{
						TH_RELEASE(DiffuseMapX[0]);
						DiffuseMapX[0] = Content->Load<Graphics::Texture2D>(Path);
						DiffuseMapX[0]->AddRef();
					}

					if (NMake::Unpack(Node->Find("diffuse-map-nx"), &Path))
					{
						TH_RELEASE(DiffuseMapX[1]);
						DiffuseMapX[1] = Content->Load<Graphics::Texture2D>(Path);
						DiffuseMapX[1]->AddRef();
					}

					if (NMake::Unpack(Node->Find("diffuse-map-py"), &Path))
					{
						TH_RELEASE(DiffuseMapY[0]);
						DiffuseMapY[0] = Content->Load<Graphics::Texture2D>(Path);
						DiffuseMapY[0]->AddRef();
					}

					if (NMake::Unpack(Node->Find("diffuse-map-ny"), &Path))
					{
						TH_RELEASE(DiffuseMapY[1]);
						DiffuseMapY[1] = Content->Load<Graphics::Texture2D>(Path);
						DiffuseMapY[1]->AddRef();
					}

					if (NMake::Unpack(Node->Find("diffuse-map-pz"), &Path))
					{
						TH_RELEASE(DiffuseMapZ[0]);
						DiffuseMapZ[0] = Content->Load<Graphics::Texture2D>(Path);
						DiffuseMapZ[0]->AddRef();
					}

					if (NMake::Unpack(Node->Find("diffuse-map-nz"), &Path))
					{
						TH_RELEASE(DiffuseMapZ[1]);
						DiffuseMapZ[1] = Content->Load<Graphics::Texture2D>(Path);
						DiffuseMapZ[1]->AddRef();
					}
				}
				else
				{
					TH_RELEASE(DiffuseMap);
					DiffuseMap = Content->Load<Graphics::Texture2D>(Path);
					DiffuseMap->AddRef();
				}

				std::vector<Compute::Matrix4x4> Views;
				NMake::Unpack(Node->Find("projection"), &Projection);
				NMake::Unpack(Node->Find("view"), &Views);
				NMake::Unpack(Node->Find("tick"), &Tick);
				NMake::Unpack(Node->Find("diffuse"), &Diffuse);
				NMake::Unpack(Node->Find("size"), &Size);
				NMake::Unpack(Node->Find("emission"), &Emission);
				NMake::Unpack(Node->Find("infinity"), &Infinity);
				NMake::Unpack(Node->Find("parallax"), &Parallax);
				NMake::Unpack(Node->Find("static-mask"), &StaticMask);

				int64_t Count = Compute::Math<int64_t>::Min((int64_t)Views.size(), 6);
				for (int64_t i = 0; i < Count; i++)
					View[i] = Views[i];

				if (!DiffuseMap)
					SetDiffuseMap(DiffuseMapX, DiffuseMapY, DiffuseMapZ);
				else
					SetDiffuseMap(DiffuseMap);
			}
			void SurfaceLight::Serialize(ContentManager* Content, Core::Document* Node)
			{
				AssetCache* Asset = nullptr;
				if (!DiffuseMap)
				{
					Asset = Content->Find<Graphics::Texture2D>(DiffuseMapX[0]);
					if (Asset != nullptr)
						NMake::Pack(Node->Set("diffuse-map-px"), Asset->Path);

					Asset = Content->Find<Graphics::Texture2D>(DiffuseMapX[1]);
					if (Asset != nullptr)
						NMake::Pack(Node->Set("diffuse-map-nx"), Asset->Path);

					Asset = Content->Find<Graphics::Texture2D>(DiffuseMapY[0]);
					if (Asset != nullptr)
						NMake::Pack(Node->Set("diffuse-map-py"), Asset->Path);

					Asset = Content->Find<Graphics::Texture2D>(DiffuseMapY[1]);
					if (Asset != nullptr)
						NMake::Pack(Node->Set("diffuse-map-ny"), Asset->Path);

					Asset = Content->Find<Graphics::Texture2D>(DiffuseMapZ[0]);
					if (Asset != nullptr)
						NMake::Pack(Node->Set("diffuse-map-pz"), Asset->Path);

					Asset = Content->Find<Graphics::Texture2D>(DiffuseMapZ[1]);
					if (Asset != nullptr)
						NMake::Pack(Node->Set("diffuse-map-nz"), Asset->Path);
				}
				else
				{
					Asset = Content->Find<Graphics::Texture2D>(DiffuseMap);
					if (Asset != nullptr)
						NMake::Pack(Node->Set("diffuse-map"), Asset->Path);
				}

				std::vector<Compute::Matrix4x4> Views;
				for (int64_t i = 0; i < 6; i++)
					Views.push_back(View[i]);

				NMake::Pack(Node->Set("projection"), Projection);
				NMake::Pack(Node->Set("view"), Views);
				NMake::Pack(Node->Set("tick"), Tick);
				NMake::Pack(Node->Set("size"), Size);
				NMake::Pack(Node->Set("diffuse"), Diffuse);
				NMake::Pack(Node->Set("emission"), Emission);
				NMake::Pack(Node->Set("infinity"), Infinity);
				NMake::Pack(Node->Set("parallax"), Parallax);
				NMake::Pack(Node->Set("static-mask"), StaticMask);
			}
			float SurfaceLight::Cull(const Viewer& fView)
			{
				float Result = 1.0f;
				if (Infinity <= 0.0f)
				{
					Result = 1.0f - Parent->Transform->Position.Distance(fView.WorldPosition) / fView.FarPlane;
					if (Result > 0.0f)
						Result = Compute::Common::IsCubeInFrustum(Parent->Transform->GetWorldUnscaled() * fView.ViewProjection, GetBoxRange()) == -1.0f ? Result : 0.0f;
				}

				return Result;
			}
			Component* SurfaceLight::Copy(Entity* New)
			{
				SurfaceLight* Target = new SurfaceLight(New);
				Target->Projection = Projection;
				Target->Diffuse = Diffuse;
				Target->Visibility = Visibility;
				Target->Emission = Emission;
				Target->Size = Size;
				Target->Tick = Tick;
				memcpy(Target->View, View, 6 * sizeof(Compute::Matrix4x4));

				if (!DiffuseMap)
					Target->SetDiffuseMap(DiffuseMapX, DiffuseMapY, DiffuseMapZ);
				else
					Target->SetDiffuseMap(DiffuseMap);

				return Target;
			}
			void SurfaceLight::SetProbeCache(Graphics::TextureCube* NewCache)
			{
				Probe = NewCache;
			}
			bool SurfaceLight::SetDiffuseMap(Graphics::Texture2D* Map)
			{
				if (!Map)
				{
					TH_CLEAR(DiffuseMapX[0]);
					TH_CLEAR(DiffuseMapX[1]);
					TH_CLEAR(DiffuseMapY[0]);
					TH_CLEAR(DiffuseMapY[1]);
					TH_CLEAR(DiffuseMapZ[0]);
					TH_CLEAR(DiffuseMapZ[1]);
					TH_CLEAR(DiffuseMap);
					return false;
				}

				TH_CLEAR(DiffuseMapX[0]);
				TH_CLEAR(DiffuseMapX[1]);
				TH_CLEAR(DiffuseMapY[0]);
				TH_CLEAR(DiffuseMapY[1]);
				TH_CLEAR(DiffuseMapZ[0]);
				TH_CLEAR(DiffuseMapZ[1]);
				TH_RELEASE(DiffuseMap);
				DiffuseMap = Map;
				Map->AddRef();

				TH_RELEASE(Probe);
				Probe = Parent->GetScene()->GetDevice()->CreateTextureCube(DiffuseMap);
				return Probe != nullptr;
			}
			bool SurfaceLight::SetDiffuseMap(Graphics::Texture2D* MapX[2], Graphics::Texture2D* MapY[2], Graphics::Texture2D* MapZ[2])
			{
				if (!MapX[0] || !MapX[1] || !MapY[0] || !MapY[1] || !MapZ[0] || !MapZ[1])
				{
					TH_CLEAR(DiffuseMapX[0]);
					TH_CLEAR(DiffuseMapX[1]);
					TH_CLEAR(DiffuseMapY[0]);
					TH_CLEAR(DiffuseMapY[1]);
					TH_CLEAR(DiffuseMapZ[0]);
					TH_CLEAR(DiffuseMapZ[1]);
					TH_CLEAR(DiffuseMap);
					return false;
				}

				TH_RELEASE(DiffuseMapX[0]);
				TH_RELEASE(DiffuseMapX[1]);
				TH_RELEASE(DiffuseMapY[0]);
				TH_RELEASE(DiffuseMapY[1]);
				TH_RELEASE(DiffuseMapZ[0]);
				TH_RELEASE(DiffuseMapZ[1]);
				TH_CLEAR(DiffuseMap);

				Graphics::Texture2D* Resources[6];
				Resources[0] = DiffuseMapX[0] = MapX[0]; MapX[0]->AddRef();
				Resources[1] = DiffuseMapX[1] = MapX[1]; MapX[1]->AddRef();
				Resources[2] = DiffuseMapY[0] = MapY[0]; MapY[0]->AddRef();
				Resources[3] = DiffuseMapY[1] = MapY[1]; MapY[1]->AddRef();
				Resources[4] = DiffuseMapZ[0] = MapZ[0]; MapZ[0]->AddRef();
				Resources[5] = DiffuseMapZ[1] = MapZ[1]; MapZ[1]->AddRef();

				TH_RELEASE(Probe);
				Probe = Parent->GetScene()->GetDevice()->CreateTextureCube(Resources);
				return Probe != nullptr;
			}
			bool SurfaceLight::IsImageBased() const
			{
				return DiffuseMapX[0] != nullptr || DiffuseMap != nullptr;
			}
			float SurfaceLight::GetBoxRange() const
			{
				return Size.Range * 1.25;
			}
			Graphics::TextureCube* SurfaceLight::GetProbeCache() const
			{
				return Probe;
			}
			Graphics::Texture2D* SurfaceLight::GetDiffuseMapXP()
			{
				return DiffuseMapX[0];
			}
			Graphics::Texture2D* SurfaceLight::GetDiffuseMapXN()
			{
				return DiffuseMapX[1];
			}
			Graphics::Texture2D* SurfaceLight::GetDiffuseMapYP()
			{
				return DiffuseMapY[0];
			}
			Graphics::Texture2D* SurfaceLight::GetDiffuseMapYN()
			{
				return DiffuseMapY[1];
			}
			Graphics::Texture2D* SurfaceLight::GetDiffuseMapZP()
			{
				return DiffuseMapZ[0];
			}
			Graphics::Texture2D* SurfaceLight::GetDiffuseMapZN()
			{
				return DiffuseMapZ[1];
			}
			Graphics::Texture2D* SurfaceLight::GetDiffuseMap()
			{
				return DiffuseMap;
			}

			Illuminator::Illuminator(Entity* Ref) : Cullable(Ref), Buffer(nullptr), MipLevels(0), Size(64)
			{
				Tick.Delay = 16.666;
				RayStep = 1.0f;
				MaxSteps = 32.0f;
				Distance = 1.0f;
				Intensity = 1.0f;
				Occlusion = 0.9f;
				Shadows = 0.25f;
				Bleeding = 1.0f;
			}
			Illuminator::~Illuminator()
			{
				TH_RELEASE(Buffer);
			}
			void Illuminator::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Unpack(Node->Find("size"), &Size);
				NMake::Unpack(Node->Find("ray-step"), &RayStep);
				NMake::Unpack(Node->Find("max-steps"), &MaxSteps);
				NMake::Unpack(Node->Find("distance"), &Distance);
				NMake::Unpack(Node->Find("intensity"), &Intensity);
				NMake::Unpack(Node->Find("occlusion"), &Occlusion);
				NMake::Unpack(Node->Find("shadows"), &Shadows);
				NMake::Unpack(Node->Find("bleeding"), &Bleeding);
				SetBufferSize(Size);
			}
			void Illuminator::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("size"), Size);
				NMake::Pack(Node->Set("ray-step"), RayStep);
				NMake::Pack(Node->Set("max-steps"), MaxSteps);
				NMake::Pack(Node->Set("distance"), Distance);
				NMake::Pack(Node->Set("intensity"), Intensity);
				NMake::Pack(Node->Set("occlusion"), Occlusion);
				NMake::Pack(Node->Set("shadows"), Shadows);
				NMake::Pack(Node->Set("bleeding"), Bleeding);
			}
			void Illuminator::Asleep()
			{
				if (!Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene)
					return;

				RenderSystem* System = (RenderSystem*)Scene->GetRenderer();
				if (System != nullptr)
				{
					auto* Lighting = System->GetRenderer<Renderers::Lighting>();
					if (Lighting != nullptr)
						Lighting->ClearStorage();
				}
			}
			float Illuminator::Cull(const Viewer& View)
			{
				Compute::Matrix4x4 Box = Parent->Transform->GetWorld();
				return IsVisible(View, &Box) ? 1.0f : 0.0f;
			}
			Component* Illuminator::Copy(Entity* New)
			{
				Illuminator* Target = new Illuminator(New);
				Target->Tick = Tick;
				Target->RayStep = RayStep;
				Target->MaxSteps = MaxSteps;
				Target->Intensity = Intensity;
				Target->Occlusion = Occlusion;
				Target->Shadows = Shadows;

				if (Buffer != nullptr)
					Target->SetBufferSize(Size);

				return Target;
			}
			void Illuminator::SetBufferSize(size_t NewSize)
			{
				if (NewSize % 8 != 0)
					NewSize = Size;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene)
					return;

				Graphics::GraphicsDevice* Device = Scene->GetDevice();
				if (!Device)
					return;

				Graphics::Texture3D::Desc I;
				I.Width = I.Height = I.Depth = Size = NewSize;
				I.MipLevels = MipLevels = Device->GetMipLevel(Size, Size);
				I.Writable = true;

				TH_RELEASE(Buffer);
				Buffer = Device->CreateTexture3D(I);

				if (Size >= Scene->GetVoxelBufferSize())
					Scene->SetVoxelBufferSize(Size);
			}
			Graphics::Texture3D* Illuminator::GetBuffer()
			{
				return Buffer;
			}
			size_t Illuminator::GetBufferSize()
			{
				return Size;
			}
			size_t Illuminator::GetMipLevels()
			{
				return MipLevels;
			}

			Camera::Camera(Entity* Ref) : Component(Ref), Mode(ProjectionMode_Perspective), Viewport({ 0, 0, 512, 512, 0, 1 })
			{
			}
			Camera::~Camera()
			{
				TH_RELEASE(Renderer);
			}
			void Camera::Awake(Component* New)
			{
				if (!Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (!Scene)
					return;

				Viewport = Scene->GetDevice()->GetRenderTarget()->GetViewport();
				if (New && New != this)
					return;

				if (!Renderer)
					Renderer = new RenderSystem(Scene->GetDevice());

				if (Renderer->GetScene() != Scene)
				{
					Renderer->SetScene(Scene);
					Renderer->Remount();
				}
				
				if (New == this)
					Renderer->Unmount();
				else if (!New)
					Renderer->Mount();
			}
			void Camera::Asleep()
			{
				if (!Renderer || !Parent)
					return;

				SceneGraph* Scene = Parent->GetScene();
				if (Scene != nullptr && Scene->GetCamera() == this)
					Scene->SetCamera(nullptr);
			}
			void Camera::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				int _Mode = 0;
				if (NMake::Unpack(Node->Find("mode"), &_Mode))
					Mode = (ProjectionMode)_Mode;

				NMake::Unpack(Node->Find("projection"), &Projection);
				NMake::Unpack(Node->Find("field-of-view"), &FieldOfView);
				NMake::Unpack(Node->Find("far-plane"), &FarPlane);
				NMake::Unpack(Node->Find("near-plane"), &NearPlane);
				NMake::Unpack(Node->Find("width"), &Width);
				NMake::Unpack(Node->Find("height"), &Height);

				SceneGraph* Scene = Parent->GetScene();
				if (!Renderer)
				{
					if (Scene != nullptr && Scene->GetDevice())
						Renderer = new RenderSystem(Scene->GetDevice());
					else
						Renderer = new RenderSystem(Content->GetDevice());
				}

				size_t Size = Renderer->GetDepthSize();
				NMake::Unpack(Node->Find("occlusion-delay"), &Renderer->Occlusion.Delay);
				NMake::Unpack(Node->Find("occlusion-stall"), &Renderer->StallFrames);
				NMake::Unpack(Node->Find("occlusion-size"), &Size);
				NMake::Unpack(Node->Find("sorting-delay"), &Renderer->Sorting.Delay);

				bool FC = true, OC = false;
				NMake::Unpack(Node->Find("frustum-cull"), &FC);
				NMake::Unpack(Node->Find("occlusion-cull"), &OC);

				Renderer->SetFrustumCulling(FC);
				Renderer->SetOcclusionCulling(OC);

				std::vector<Core::Document*> Renderers = Node->FetchCollection("renderers.renderer");
				Renderer->SetScene(Scene);
				Renderer->SetDepthSize(Size);

				for (auto& Render : Renderers)
				{
					uint64_t Id;
					if (!NMake::Unpack(Render->Find("id"), &Id))
						continue;

					Engine::Renderer* Target = Core::Composer::Create<Engine::Renderer>(Id, Renderer);
					if (!Renderer || !Renderer->AddRenderer(Target))
					{
						TH_WARN("cannot create renderer with id %llu", Id);
						continue;
					}

					Core::Document* Meta = Render->Find("metadata");
					if (!Meta)
						Meta = Render->Set("metadata");

					Target->Deactivate();
					Target->Deserialize(Content, Meta);
					Target->Activate();

					NMake::Unpack(Render->Find("active"), &Target->Active);
				}
			}
			void Camera::Serialize(ContentManager* Content, Core::Document* Node)
			{
				NMake::Pack(Node->Set("mode"), (int)Mode);
				NMake::Pack(Node->Set("projection"), Projection);
				NMake::Pack(Node->Set("field-of-view"), FieldOfView);
				NMake::Pack(Node->Set("far-plane"), FarPlane);
				NMake::Pack(Node->Set("near-plane"), NearPlane);
				NMake::Pack(Node->Set("width"), Width);
				NMake::Pack(Node->Set("height"), Height);
				NMake::Pack(Node->Set("occlusion-delay"), Renderer->Occlusion.Delay);
				NMake::Pack(Node->Set("occlusion-stall"), Renderer->StallFrames);
				NMake::Pack(Node->Set("occlusion-size"), Renderer->GetDepthSize());
				NMake::Pack(Node->Set("sorting-delay"), Renderer->Sorting.Delay);
				NMake::Pack(Node->Set("frustum-cull"), Renderer->HasFrustumCulling());
				NMake::Pack(Node->Set("occlusion-cull"), Renderer->HasOcclusionCulling());

                Core::Document* Renderers = Node->Set("renderers", Core::Var::Array());
				for (auto& Ref : *Renderer->GetRenderers())
				{
					Core::Document* Render = Renderers->Set("renderer");
					NMake::Pack(Render->Set("id"), Ref->GetId());
					NMake::Pack(Render->Set("active"), Ref->Active);
					Ref->Serialize(Content, Render->Set("metadata"));
				}
			}
			void Camera::Synchronize(Core::Timer* Time)
			{
				float W = Width, H = Height;
				if (W <= 0 || H <= 0)
				{
					W = Viewport.Width;
					H = Viewport.Height;
				}

				if (Mode == ProjectionMode_Perspective)
					Projection = Compute::Matrix4x4::CreatePerspective(FieldOfView, W / H, NearPlane, FarPlane);
				else if (Mode == ProjectionMode_Orthographic)
					Projection = Compute::Matrix4x4::CreateOrthographic(W, H, NearPlane, FarPlane);

				SceneGraph* Scene = Parent->GetScene();
				if (Scene != nullptr && Scene->GetCamera() == this)
					Renderer->Synchronize(Time, FieldView);
			}
			void Camera::GetViewer(Viewer* View)
			{
				if (!View)
					return;

				Compute::Vector3 Position = Parent->Transform->Position.InvX().InvY();
				Compute::Matrix4x4 World = Compute::Matrix4x4::CreateCamera(Position, -Parent->Transform->Rotation);
				View->Set(World, Projection, Parent->Transform->Position, NearPlane, FarPlane);
				View->WorldRotation = Parent->Transform->Rotation;
				View->Renderer = Renderer;
				FieldView = *View;
			}
			void Camera::ResizeBuffers()
			{
				if (!Renderer)
					return;

				Renderer->SetScene(Parent->GetScene());
				auto* RenderStages = Renderer->GetRenderers();
				for (auto* Item : *RenderStages)
					Item->ResizeBuffers();
			}
			Viewer Camera::GetViewer()
			{
				return FieldView;
			}
			RenderSystem* Camera::GetRenderer()
			{
				return Renderer;
			}
			Compute::Matrix4x4 Camera::GetProjection()
			{
				return Projection;
			}
			Compute::Vector3 Camera::GetViewPosition()
			{
				return Compute::Vector3(-Parent->Transform->Position.X, -Parent->Transform->Position.Y, Parent->Transform->Position.Z);
			}
			Compute::Matrix4x4 Camera::GetViewProjection()
			{
				return GetView() * Projection;
			}
			Compute::Matrix4x4 Camera::GetView()
			{
				return Compute::Matrix4x4::CreateCamera(GetViewPosition(), -Parent->Transform->Rotation);
			}
			Compute::Ray Camera::GetScreenRay(const Compute::Vector2& Position)
			{
				float W = Width, H = Height;
				if ((W <= 0 || H <= 0) && Renderer != nullptr)
				{
					Graphics::Viewport V = Renderer->GetDevice()->GetRenderTarget()->GetViewport();
					W = V.Width; H = V.Height;
				}

				return Compute::Common::CreateCursorRay(Parent->Transform->Position, Position, Compute::Vector2(W, H), Projection.Inv(), GetView().Inv());
			}
			float Camera::GetDistance(Entity* Other)
			{
				if (!Other)
					return -1.0f;

				return Other->Transform->Position.Distance(FieldView.WorldPosition);
			}
			float Camera::GetWidth()
			{
				float W = Width;
				if (W <= 0)
					W = Viewport.Width;

				return W;
			}
			float Camera::GetHeight()
			{
				float H = Height;
				if (H <= 0)
					H = Viewport.Height;

				return H;
			}
			float Camera::GetAspect()
			{
				float W = Width, H = Height;
				if (W <= 0 || H <= 0)
				{
					W = Viewport.Width;
					H = Viewport.Height;
				}

				return W / H;
			}
			bool Camera::RayTest(const Compute::Ray& Ray, Entity* Other)
			{
				if (!Other)
					return false;

				return Compute::Common::CursorRayTest(Ray, Other->Transform->GetWorld());
			}
			bool Camera::RayTest(const Compute::Ray& Ray, const Compute::Matrix4x4& World)
			{
				return Compute::Common::CursorRayTest(Ray, World);
			}
			Component* Camera::Copy(Entity* New)
			{
				Camera* Target = new Camera(New);
				Target->FarPlane = FarPlane;
				Target->NearPlane = NearPlane;
				Target->Width = Width;
				Target->Height = Height;
				Target->Mode = Mode;
				Target->FieldOfView = FieldOfView;
				Target->Projection = Projection;

				return Target;
			}

			Scriptable::Scriptable(Entity* Ref) : Component(Ref), Compiler(nullptr), Source(SourceType_Resource), Invoke(InvokeType_Typeless)
			{
			}
			Scriptable::~Scriptable()
			{
				TH_RELEASE(Compiler);
			}
			void Scriptable::Deserialize(ContentManager* Content, Core::Document* Node)
			{
				std::string Type;
				if (NMake::Unpack(Node->Find("source"), &Type))
				{
					if (Type == "memory")
						Source = SourceType_Memory;
					else if (Type == "resource")
						Source = SourceType_Resource;
				}

				if (NMake::Unpack(Node->Find("invoke"), &Type))
				{
					if (Type == "typeless")
						Invoke = InvokeType_Typeless;
					else if (Type == "normal")
						Invoke = InvokeType_Normal;
				}

				if (!NMake::Unpack(Node->Find("resource"), &Type))
					return;

				Resource = Core::OS::Path::Resolve(Type.c_str(), Content->GetEnvironment());
				if (Resource.empty())
					Resource = std::move(Type);

				if (SetSource() < 0)
					return;

				Core::Document* Cache = Node->Find("cache");
				if (Cache != nullptr)
				{
					for (auto& Var : *Cache->GetNodes())
					{
						int TypeId = -1;
						if (!NMake::Unpack(Var->Find("type"), &TypeId))
							continue;

						switch ((Script::VMTypeId)TypeId)
						{
							case Script::VMTypeId::BOOL:
							{
								bool Result = false;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), Result);
								break;
							}
							case Script::VMTypeId::INT8:
							{
								int64_t Result = 0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), (char)Result);
								break;
							}
							case Script::VMTypeId::INT16:
							{
								int64_t Result = 0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), (short)Result);
								break;
							}
							case Script::VMTypeId::INT32:
							{
								int64_t Result = 0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), (int)Result);
								break;
							}
							case Script::VMTypeId::INT64:
							{
								int64_t Result = 0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), Result);
								break;
							}
							case Script::VMTypeId::UINT8:
							{
								int64_t Result = 0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), (unsigned char)Result);
								break;
							}
							case Script::VMTypeId::UINT16:
							{
								int64_t Result = 0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), (unsigned short)Result);
								break;
							}
							case Script::VMTypeId::UINT32:
							{
								int64_t Result = 0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), (unsigned int)Result);
								break;
							}
							case Script::VMTypeId::UINT64:
							{
								int64_t Result = 0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), (uint64_t)Result);
								break;
							}
							case Script::VMTypeId::FLOAT:
							{
								float Result = 0.0f;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), Result);
								break;
							}
							case Script::VMTypeId::DOUBLE:
							{
								double Result = 0.0;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), Result);
								break;
							}
							default:
							{
								std::string Result;
								if (NMake::Unpack(Var->Find("data"), &Result))
									SetTypePropertyByName(Var->Key.c_str(), Result);
								break;
							}
						}
					}
				}

				Call(Entry.Deserialize, [this, &Content, &Node](Script::VMContext* Context)
				{
					if (Invoke == InvokeType_Typeless)
						return;

					Component* Current = this;
					Context->SetArgObject(0, Current);
					Context->SetArgObject(1, Content);
					Context->SetArgObject(2, Node);
				});
			}
			void Scriptable::Serialize(ContentManager* Content, Core::Document* Node)
			{
				if (Source == SourceType_Memory)
					NMake::Pack(Node->Set("source"), "memory");
				else if (Source == SourceType_Resource)
					NMake::Pack(Node->Set("source"), "resource");

				if (Invoke == InvokeType_Typeless)
					NMake::Pack(Node->Set("invoke"), "typeless");
				else if (Invoke == InvokeType_Normal)
					NMake::Pack(Node->Set("invoke"), "normal");

				int Count = GetPropertiesCount();
				NMake::Pack(Node->Set("resource"), Core::Parser(Resource).Replace(Content->GetEnvironment(), "./").Replace('\\', '/').R());

				Core::Document* Cache = Node->Set("cache");
				for (int i = 0; i < Count; i++)
				{
					Script::VMProperty Result;
					if (!GetPropertyByIndex(i, &Result) || !Result.Name || !Result.Pointer)
						continue;

					Core::Document* Var = Core::Document::Object();
					NMake::Pack(Var->Set("type"), Result.TypeId);

					switch ((Script::VMTypeId)Result.TypeId)
					{
						case Script::VMTypeId::BOOL:
							NMake::Pack(Var->Set("data"), *(bool*)Result.Pointer);
							break;
						case Script::VMTypeId::INT8:
							NMake::Pack(Var->Set("data"), (int64_t)*(char*)Result.Pointer);
							break;
						case Script::VMTypeId::INT16:
							NMake::Pack(Var->Set("data"), (int64_t)*(short*)Result.Pointer);
							break;
						case Script::VMTypeId::INT32:
							NMake::Pack(Var->Set("data"), (int64_t)*(int*)Result.Pointer);
							break;
						case Script::VMTypeId::INT64:
							NMake::Pack(Var->Set("data"), *(int64_t*)Result.Pointer);
							break;
						case Script::VMTypeId::UINT8:
							NMake::Pack(Var->Set("data"), (int64_t)*(unsigned char*)Result.Pointer);
							break;
						case Script::VMTypeId::UINT16:
							NMake::Pack(Var->Set("data"), (int64_t)*(unsigned short*)Result.Pointer);
							break;
						case Script::VMTypeId::UINT32:
							NMake::Pack(Var->Set("data"), (int64_t)*(unsigned int*)Result.Pointer);
							break;
						case Script::VMTypeId::UINT64:
							NMake::Pack(Var->Set("data"), (int64_t)*(uint64_t*)Result.Pointer);
							break;
						case Script::VMTypeId::FLOAT:
							NMake::Pack(Var->Set("data"), (double)*(float*)Result.Pointer);
							break;
						case Script::VMTypeId::DOUBLE:
							NMake::Pack(Var->Set("data"), *(double*)Result.Pointer);
							break;
						default:
						{
							Script::VMTypeInfo Type = GetCompiler()->GetManager()->Global().GetTypeInfoById(Result.TypeId);
							if (Type.IsValid() && strcmp(Type.GetName(), "String") == 0)
								NMake::Pack(Var->Set("data"), *(std::string*)Result.Pointer);
							else
								TH_CLEAR(Var);
							break;
						}
					}

					if (Var != nullptr)
						Cache->Set(Result.Name, Var);
				}

				Call(Entry.Serialize, [this, &Content, &Node](Script::VMContext* Context)
				{
					if (Invoke == InvokeType_Typeless)
						return;

					Component* Current = this;
					Context->SetArgObject(0, Current);
					Context->SetArgObject(1, Content);
					Context->SetArgObject(2, Node);
				});
			}
			void Scriptable::Awake(Component* New)
			{
				if (!Parent->GetScene()->IsActive())
					return;

				Call(Entry.Awake, [this, &New](Script::VMContext* Context)
				{
					if (Invoke == InvokeType_Typeless)
						return;

					Component* Current = this;
					Context->SetArgObject(0, Current);
					Context->SetArgObject(1, New);
				});
			}
			void Scriptable::Synchronize(Core::Timer* Time)
			{
				Call(Entry.Synchronize, [this, &Time](Script::VMContext* Context)
				{
					if (Invoke == InvokeType_Typeless)
						return;

					Component* Current = this;
					Context->SetArgObject(0, Current);
					Context->SetArgObject(1, Time);
				});
			}
			void Scriptable::Asleep()
			{
				Call(Entry.Asleep, [this](Script::VMContext* Context)
				{
					if (Invoke == InvokeType_Typeless)
						return;

					Component* Current = this;
					Context->SetArgObject(0, Current);
				});
			}
			void Scriptable::Update(Core::Timer* Time)
			{
				Call(Entry.Update, [this, &Time](Script::VMContext* Context)
				{
					if (Invoke == InvokeType_Typeless)
						return;

					Component* Current = this;
					Context->SetArgObject(0, Current);
					Context->SetArgObject(1, Time);
				});
			}
			void Scriptable::Message(Event* Value)
			{
				Call(Entry.Message, [this, &Value](Script::VMContext* Context)
				{
					if (Invoke == InvokeType_Typeless)
						return;

					Component* Current = this;
					Context->SetArgObject(0, Current);
					Context->SetArgObject(1, Value);
				});
			}
			Component* Scriptable::Copy(Entity* New)
			{
				Scriptable* Target = new Scriptable(New);
				Target->Invoke = Invoke;
				Target->SetSource(Source, Resource);

				if (!Compiler || !Target->Compiler)
					return Target;

				if (Compiler->GetContext()->GetState() == Tomahawk::Script::VMExecState::ACTIVE)
					return Target;

				if (Target->Compiler->GetContext()->GetState() == Tomahawk::Script::VMExecState::ACTIVE)
					return Target;

				Script::VMModule From = Compiler->GetModule();
				Script::VMModule To = Target->Compiler->GetModule();
				Script::VMManager* Manager = Compiler->GetManager();

				if (!From.IsValid() || !To.IsValid())
					return Target;

				Safe.lock();
				int Count = From.GetPropertiesCount();
				for (int i = 0; i < Count; i++)
				{
					Script::VMProperty fSource;
					if (From.GetProperty(i, &fSource) < 0)
						continue;

					Script::VMProperty Dest;
					if (To.GetProperty(i, &Dest) < 0)
						continue;

					if (fSource.TypeId != Dest.TypeId)
						continue;

					if (fSource.TypeId < (int)Script::VMTypeId::BOOL || fSource.TypeId >(int)Script::VMTypeId::DOUBLE)
					{
						Script::VMTypeInfo Type = Manager->Global().GetTypeInfoById(fSource.TypeId);
						if (fSource.Pointer != nullptr && Type.IsValid())
						{
							void* Object = Manager->CreateObjectCopy(fSource.Pointer, Type);
							if (Object != nullptr)
								Manager->AssignObject(Dest.Pointer, Object, Type);
						}
					}
					else
					{
						int Size = Manager->Global().GetSizeOfPrimitiveType(fSource.TypeId);
						if (Size > 0)
							memcpy(Dest.Pointer, fSource.Pointer, (size_t)Size);
					}
				}

				Safe.unlock();
				return Target;
			}
			int Scriptable::Call(const std::string& Name, unsigned int Args, Script::ArgsCallback&& ArgCallback)
			{
				if (!Compiler)
					return (int)Script::VMResult::INVALID_CONFIGURATION;

				return Call(GetFunctionByName(Name, Args).GetFunction(), std::move(ArgCallback));
			}
			int Scriptable::Call(Script::VMCFunction* Function, Script::ArgsCallback&& ArgCallback)
			{
				if (!Compiler)
					return (int)Script::VMResult::INVALID_CONFIGURATION;

				if (!Function)
					return (int)Script::VMResult::INVALID_ARG;

				Safe.lock();
				Core::Async<int> Result = Compiler->GetContext()->Execute(Function, false, std::move(ArgCallback));
				Safe.unlock();

				if (Result.IsPending())
					return (int)Script::VMResult::CONTEXT_ACTIVE;

				return Result.Get();
			}
			int Scriptable::CallEntry(const std::string& Name)
			{
				return Call(GetFunctionByName(Name, Invoke == InvokeType_Typeless ? 0 : 1).GetFunction(), [this](Script::VMContext* Context)
				{
					if (Invoke == InvokeType_Typeless)
						return;

					Component* Current = this;
					Context->SetArgObject(0, Current);
				});
			}
			int Scriptable::SetSource()
			{
				return SetSource(Source, Resource);
			}
			int Scriptable::SetSource(SourceType Type, const std::string& Data)
			{
				SceneGraph* Scene = Parent->GetScene();
				if (!Scene)
					return (int)Script::VMResult::INVALID_CONFIGURATION;

				if (Compiler != nullptr)
				{
					auto* VM = Compiler->GetContext();
					if (VM->GetState() == Script::VMExecState::ACTIVE)
						return (int)Script::VMResult::MODULE_IS_IN_USE;
				}
				else
				{
					auto* Manager = Scene->GetConf().Manager;
					if (!Manager)
						return (int)Script::VMResult::INVALID_CONFIGURATION;

					Compiler = Manager->CreateCompiler();
					Compiler->SetPragmaCallback([this](Compute::Preprocessor*, const std::string& Name, const std::vector<std::string>& Args)
					{
						if (Name == "name" && Args.size() == 1)
							Module = Args[0];

						return true;
					});
				}

				Safe.lock();
				Source = Type;
				Resource = Data;

				if (Resource.empty())
				{
					Entry.Serialize = nullptr;
					Entry.Deserialize = nullptr;
					Entry.Awake = nullptr;
					Entry.Asleep = nullptr;
					Entry.Synchronize = nullptr;
					Entry.Update = nullptr;
					Entry.Message = nullptr;
					Compiler->Clear();
					Safe.unlock();

					return (int)Script::VMResult::SUCCESS;
				}

				int R = Compiler->Prepare("base", Source == SourceType_Resource ? Resource : "anonymous", true, true);
				if (R < 0)
				{
					Safe.unlock();
					return R;
				}

				R = (Source == SourceType_Resource ? Compiler->LoadFile(Resource) : Compiler->LoadCode("anonymous", Resource));
				if (R < 0)
				{
					Safe.unlock();
					return R;
				}

				R = Compiler->Compile(true);
				if (R < 0)
				{
					Safe.unlock();
					return R;
				}

				Safe.unlock();
				Entry.Serialize = GetFunctionByName("Serialize", Invoke == InvokeType_Typeless ? 0 : 3).GetFunction();
				Entry.Deserialize = GetFunctionByName("Deserialize", Invoke == InvokeType_Typeless ? 0 : 3).GetFunction();
				Entry.Awake = GetFunctionByName("Awake", Invoke == InvokeType_Typeless ? 0 : 2).GetFunction();
				Entry.Asleep = GetFunctionByName("Asleep", Invoke == InvokeType_Typeless ? 0 : 1).GetFunction();
				Entry.Synchronize = GetFunctionByName("Synchronize", Invoke == InvokeType_Typeless ? 0 : 2).GetFunction();
				Entry.Update = GetFunctionByName("Update", Invoke == InvokeType_Typeless ? 0 : 2).GetFunction();
				Entry.Message = GetFunctionByName("Message", Invoke == InvokeType_Typeless ? 0 : 2).GetFunction();

				return R;
			}
			void Scriptable::SetInvocation(InvokeType Type)
			{
				Invoke = Type;
			}
			void Scriptable::UnsetSource()
			{
				SetSource(Source, "");
			}
			Script::VMCompiler* Scriptable::GetCompiler()
			{
				return Compiler;
			}
			Script::VMFunction Scriptable::GetFunctionByName(const std::string& Name, unsigned int Args)
			{
				if (Name.empty() || !Compiler)
					return nullptr;

				auto* VM = Compiler->GetContext();
				if (VM->GetState() == Script::VMExecState::ACTIVE)
					return nullptr;

				Safe.lock();
				auto Result = Compiler->GetModule().GetFunctionByName(Name.c_str());
				if (Result.IsValid() && Result.GetArgsCount() != Args)
				{
					Safe.unlock();
					return nullptr;
				}

				Safe.unlock();
				return Result;
			}
			Script::VMFunction Scriptable::GetFunctionByIndex(int Index, unsigned int Args)
			{
				if (Index < 0 || !Compiler)
					return nullptr;

				auto* VM = Compiler->GetContext();
				if (VM->GetState() == Script::VMExecState::ACTIVE)
					return nullptr;

				Safe.lock();
				auto Result = Compiler->GetModule().GetFunctionByIndex(Index);
				if (Result.IsValid() && Result.GetArgsCount() != Args)
				{
					Safe.unlock();
					return nullptr;
				}

				Safe.unlock();
				return Result;
			}
			bool Scriptable::GetPropertyByName(const char* Name, Script::VMProperty* Result)
			{
				if (!Name || !Compiler)
					return false;

				auto* VM = Compiler->GetContext();
				if (VM->GetState() == Tomahawk::Script::VMExecState::ACTIVE)
					return false;

				Safe.lock();
				Script::VMModule fModule = Compiler->GetModule();
				if (!fModule.IsValid())
				{
					Safe.unlock();
					return false;
				}

				int Index = fModule.GetPropertyIndexByName(Name);
				if (Index < 0)
				{
					Safe.unlock();
					return false;
				}

				if (fModule.GetProperty(Index, Result) < 0)
				{
					Safe.unlock();
					return false;
				}

				Safe.unlock();
				return true;
			}
			bool Scriptable::GetPropertyByIndex(int Index, Script::VMProperty* Result)
			{
				if (Index < 0 || !Compiler)
					return false;

				auto* VM = Compiler->GetContext();
				if (VM->GetState() == Tomahawk::Script::VMExecState::ACTIVE)
					return false;

				Safe.lock();
				Script::VMModule fModule = Compiler->GetModule();
				if (!fModule.IsValid())
				{
					Safe.unlock();
					return false;
				}

				if (fModule.GetProperty(Index, Result) < 0)
				{
					Safe.unlock();
					return false;
				}

				Safe.unlock();
				return true;
			}
			Scriptable::SourceType Scriptable::GetSourceType()
			{
				return Source;
			}
			Scriptable::InvokeType Scriptable::GetInvokeType()
			{
				return Invoke;
			}
			int Scriptable::GetPropertiesCount()
			{
				if (!Compiler)
					return (int)Script::VMResult::INVALID_ARG;

				auto* VM = Compiler->GetContext();
				if (VM->GetState() == Tomahawk::Script::VMExecState::ACTIVE)
					return (int)Script::VMResult::MODULE_IS_IN_USE;

				Safe.lock();
				Script::VMModule fModule = Compiler->GetModule();
				if (!fModule.IsValid())
				{
					Safe.unlock();
					return 0;
				}

				int Result = fModule.GetPropertiesCount();
				Safe.unlock();

				return Result;
			}
			int Scriptable::GetFunctionsCount()
			{
				if (!Compiler)
					return (int)Script::VMResult::INVALID_ARG;

				auto* VM = Compiler->GetContext();
				if (VM->GetState() == Tomahawk::Script::VMExecState::ACTIVE)
					return (int)Script::VMResult::MODULE_IS_IN_USE;

				Safe.lock();
				Script::VMModule fModule = Compiler->GetModule();
				if (!fModule.IsValid())
				{
					Safe.unlock();
					return 0;
				}

				int Result = fModule.GetFunctionCount();
				Safe.unlock();

				return Result;
			}
			const std::string& Scriptable::GetSource()
			{
				return Resource;
			}
			const std::string& Scriptable::GetModuleName()
			{
				return Module;
			}
		}
	}
}
