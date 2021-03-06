#ifndef TOMAHAWK_H
#define TOMAHAWK_H
#define TH_MAJOR_VERSION 3
#define TH_MINOR_VERSION 2
#define TH_PATCH_LEVEL 12
#define TH_VERSION(X, Y, Z) ((X) * 1000 + (Y) * 100 + (Z))
#define TH_AT_LEAST(X, Y, Z) (TH_VERSION(TH_MAJOR_VERSION, TH_MINOR_VERSION, TH_PATCH_LEVEL) >= TH_VERSION(X, Y, Z))
#include "core/core.h"
#include "core/compute.h"
#include "core/graphics.h"
#include "core/audio.h"
#include "core/network.h"
#include "core/script.h"
#include "core/engine.h"
#include "network/http.h"
#include "network/smtp.h"
#include "network/mdb.h"
#include "network/pdb.h"
#include "audio/effects.h"
#include "audio/filters.h"
#include "engine/components.h"
#include "engine/processors.h"
#include "engine/renderers.h"
#include "engine/gui.h"
#include "script/std-lib.h"
#include "script/core-lib.h"
#include "script/gui-lib.h"

namespace Tomahawk
{
	enum class Init
	{
		Core = 1,
		Debug = 2,
		Network = 4,
		SSL = 8,
		SDL2 = 16,
		Compute = 32,
		Locale = 64,
		Audio = 128,
		GLEW = 256
	};

	constexpr inline Init operator |(Init A, Init B)
	{
		return static_cast<Init>(static_cast<uint64_t>(A) | static_cast<uint64_t>(B));
	}

	enum class Preset : uint64_t
	{
		Game = (uint64_t)(Init::Core | Init::Debug | Init::Network | Init::SSL | Init::SDL2 | Init::Compute | Init::Locale | Init::Audio | Init::GLEW),
		App = (uint64_t)(Init::Core | Init::Debug | Init::Network | Init::SSL | Init::Compute | Init::Locale)
	};

	class TH_OUT Library
	{
	public:
		static void Describe();
		static bool HasDirectX();
		static bool HasOpenGL();
		static bool HasOpenSSL();
		static bool HasGLEW();
		static bool HasZLib();
		static bool HasAssimp();
		static bool HasMongoDB();
		static bool HasPostgreSQL();
		static bool HasOpenAL();
		static bool HasSDL2();
		static bool WithSIMD();
		static bool WithBullet3();
		static bool WithRmlUi();
		static bool WithWepoll();
		static int Version();
		static int DebugLevel();
		static int Architecture();
		static const char* Build();
		static const char* Compiler();
		static const char* Platform();
	};

	TH_OUT bool Initialize(uint64_t Modules = (uint64_t)Preset::App);
	TH_OUT bool Uninitialize();
}
#endif