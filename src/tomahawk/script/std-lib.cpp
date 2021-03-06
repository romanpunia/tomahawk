#include "std-lib.h"
#include <new>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#ifndef __psp2__
#include <locale.h>
#endif
#ifndef ANGELSCRIPT_H 
#include <angelscript.h>
#endif
#ifdef __BORLANDC__
#include <cmath>
#if __BORLANDC__ < 0x580
inline float cosf(float arg)
{
	return std::cos(arg);
}
inline float sinf(float arg)
{
	return std::sin(arg);
}
inline float tanf(float arg)
{
	return std::tan(arg);
}
inline float atan2f(float y, float x)
{
	return std::atan2(y, x);
}
inline float logf(float arg)
{
	return std::log(arg);
}
inline float powf(float x, float y)
{
	return std::pow(x, y);
}
#endif
inline float acosf(float arg)
{
	return std::acos(arg);
}
inline float asinf(float arg)
{
	return std::asin(arg);
}
inline float atanf(float arg)
{
	return std::atan(arg);
}
inline float coshf(float arg)
{
	return std::cosh(arg);
}
inline float sinhf(float arg)
{
	return std::sinh(arg);
}
inline float tanhf(float arg)
{
	return std::tanh(arg);
}
inline float log10f(float arg)
{
	return std::log10(arg);
}
inline float ceilf(float arg)
{
	return std::ceil(arg);
}
inline float fabsf(float arg)
{
	return std::fabs(arg);
}
inline float floorf(float arg)
{
	return std::floor(arg);
}
inline float modff(float x, float *y)
{
	double d;
	float f = (float)modf((double)x, &d);
	*y = (float)d;
	return f;
}
inline float sqrtf(float x)
{
	return sqrt(x);
}
#endif
#ifndef _WIN32_WCE
float fracf(float v)
{
	float intPart;
	return modff(v, &intPart);
}
#else
double frac(double v)
{
	return v;
}
#endif
#define ARRAY_CACHE 1000
#define MAP_CACHE 1003

namespace Tomahawk
{
	namespace Script
	{
		class StringFactory : public asIStringFactory
		{
		private:
			static StringFactory* Base;

		public:
			std::unordered_map<std::string, int> Cache;

		public:
			StringFactory()
			{
			}
			~StringFactory()
			{
				assert(Cache.size() == 0);
			}
			const void *GetStringConstant(const char* Buffer, asUINT Length)
			{
				asAcquireExclusiveLock();
				std::string Source(Buffer, Length);
				auto It = Cache.find(Source);

				if (It == Cache.end())
					It = Cache.insert(std::make_pair(std::move(Source), 1)).first;
				else
					It->second++;

				asReleaseExclusiveLock();
				return reinterpret_cast<const void*>(&It->first);
			}
			int ReleaseStringConstant(const void* Source)
			{
				if (!Source)
					return asERROR;

				asAcquireExclusiveLock();
				auto It = Cache.find(*reinterpret_cast<const std::string*>(Source));
				if (It == Cache.end())
				{
					asReleaseExclusiveLock();
					return asERROR;
				}

				It->second--;
				if (It->second <= 0)
					Cache.erase(It);

				asReleaseExclusiveLock();
				return asSUCCESS;
			}
			int GetRawStringData(const void* Source, char* Buffer, asUINT* Length) const
			{
				if (!Source)
					return asERROR;

				if (Length != nullptr)
					*Length = (as_size_t)reinterpret_cast<const std::string*>(Source)->length();

				if (Buffer != nullptr)
					memcpy(Buffer, reinterpret_cast<const std::string*>(Source)->c_str(), reinterpret_cast<const std::string*>(Source)->length());

				return asSUCCESS;
			}

		public:
			static StringFactory* Get()
			{
				if (!Base)
					Base = TH_NEW(StringFactory);

				return Base;
			}
			static void Free()
			{
				if (Base != nullptr && Base->Cache.empty())
					TH_DELETE(StringFactory, Base);
			}
		};
		StringFactory* StringFactory::Base = nullptr;

		void VMCString::Construct(std::string *thisPointer)
		{
			new(thisPointer) std::string();
		}
		void VMCString::CopyConstruct(const std::string &other, std::string *thisPointer)
		{
			new(thisPointer) std::string(other);
		}
		void VMCString::Destruct(std::string *thisPointer)
		{
			thisPointer->~basic_string();
		}
		std::string& VMCString::AddAssignTo(const std::string &str, std::string &dest)
		{
			dest += str;
			return dest;
		}
		bool VMCString::IsEmpty(const std::string &str)
		{
			return str.empty();
		}
		void* VMCString::ToPtr(const std::string& Value)
		{
			return (void*)Value.c_str();
		}
		std::string VMCString::Reverse(const std::string& Value)
		{
			Core::Parser Result(Value);
			Result.Reverse();
			return Result.R();
		}
		std::string& VMCString::AssignUInt64To(as_uint64_t i, std::string &dest)
		{
			std::ostringstream stream;
			stream << i;
			dest = stream.str();
			return dest;
		}
		std::string& VMCString::AddAssignUInt64To(as_uint64_t i, std::string &dest)
		{
			std::ostringstream stream;
			stream << i;
			dest += stream.str();
			return dest;
		}
		std::string VMCString::AddUInt641(const std::string &str, as_uint64_t i)
		{
			std::ostringstream stream;
			stream << i;
			return str + stream.str();
		}
		std::string VMCString::AddInt641(as_int64_t i, const std::string &str)
		{
			std::ostringstream stream;
			stream << i;
			return stream.str() + str;
		}
		std::string& VMCString::AssignInt64To(as_int64_t i, std::string &dest)
		{
			std::ostringstream stream;
			stream << i;
			dest = stream.str();
			return dest;
		}
		std::string& VMCString::AddAssignInt64To(as_int64_t i, std::string &dest)
		{
			std::ostringstream stream;
			stream << i;
			dest += stream.str();
			return dest;
		}
		std::string VMCString::AddInt642(const std::string &str, as_int64_t i)
		{
			std::ostringstream stream;
			stream << i;
			return str + stream.str();
		}
		std::string VMCString::AddUInt642(as_uint64_t i, const std::string &str)
		{
			std::ostringstream stream;
			stream << i;
			return stream.str() + str;
		}
		std::string& VMCString::AssignDoubleTo(double f, std::string &dest)
		{
			std::ostringstream stream;
			stream << f;
			dest = stream.str();
			return dest;
		}
		std::string& VMCString::AddAssignDoubleTo(double f, std::string &dest)
		{
			std::ostringstream stream;
			stream << f;
			dest += stream.str();
			return dest;
		}
		std::string& VMCString::AssignFloatTo(float f, std::string &dest)
		{
			std::ostringstream stream;
			stream << f;
			dest = stream.str();
			return dest;
		}
		std::string& VMCString::AddAssignFloatTo(float f, std::string &dest)
		{
			std::ostringstream stream;
			stream << f;
			dest += stream.str();
			return dest;
		}
		std::string& VMCString::AssignBoolTo(bool b, std::string &dest)
		{
			std::ostringstream stream;
			stream << (b ? "true" : "false");
			dest = stream.str();
			return dest;
		}
		std::string& VMCString::AddAssignBoolTo(bool b, std::string &dest)
		{
			std::ostringstream stream;
			stream << (b ? "true" : "false");
			dest += stream.str();
			return dest;
		}
		std::string VMCString::AddDouble1(const std::string &str, double f)
		{
			std::ostringstream stream;
			stream << f;
			return str + stream.str();
		}
		std::string VMCString::AddDouble2(double f, const std::string &str)
		{
			std::ostringstream stream;
			stream << f;
			return stream.str() + str;
		}
		std::string VMCString::AddFloat1(const std::string &str, float f)
		{
			std::ostringstream stream;
			stream << f;
			return str + stream.str();
		}
		std::string VMCString::AddFloat2(float f, const std::string &str)
		{
			std::ostringstream stream;
			stream << f;
			return stream.str() + str;
		}
		std::string VMCString::AddBool1(const std::string &str, bool b)
		{
			std::ostringstream stream;
			stream << (b ? "true" : "false");
			return str + stream.str();
		}
		std::string VMCString::AddBool2(bool b, const std::string &str)
		{
			std::ostringstream stream;
			stream << (b ? "true" : "false");
			return stream.str() + str;
		}
		char* VMCString::CharAt(unsigned int i, std::string &str)
		{
			if (i >= str.size())
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx != nullptr)
					ctx->SetException("Out of range");

				return 0;
			}

			return &str[i];
		}
		int VMCString::Cmp(const std::string &a, const std::string &b)
		{
			int cmp = 0;
			if (a < b)
				cmp = -1;
			else if (a > b)
				cmp = 1;

			return cmp;
		}
		int VMCString::FindFirst(const std::string &sub, as_size_t start, const std::string &str)
		{
			return (int)str.find(sub, (size_t)start);
		}
		int VMCString::FindFirstOf(const std::string &sub, as_size_t start, const std::string &str)
		{
			return (int)str.find_first_of(sub, (size_t)start);
		}
		int VMCString::FindLastOf(const std::string &sub, as_size_t start, const std::string &str)
		{
			return (int)str.find_last_of(sub, (size_t)start);
		}
		int VMCString::FindFirstNotOf(const std::string &sub, as_size_t start, const std::string &str)
		{
			return (int)str.find_first_not_of(sub, (size_t)start);
		}
		int VMCString::FindLastNotOf(const std::string &sub, as_size_t start, const std::string &str)
		{
			return (int)str.find_last_not_of(sub, (size_t)start);
		}
		int VMCString::FindLast(const std::string &sub, int start, const std::string &str)
		{
			return (int)str.rfind(sub, (size_t)start);
		}
		void VMCString::Insert(unsigned int pos, const std::string &other, std::string &str)
		{
			str.insert(pos, other);
		}
		void VMCString::Erase(unsigned int pos, int count, std::string &str)
		{
			str.erase(pos, (size_t)(count < 0 ? std::string::npos : count));
		}
		as_size_t VMCString::Length(const std::string &str)
		{
			return (as_size_t)str.length();
		}
		void VMCString::Resize(as_size_t l, std::string &str)
		{
			str.resize(l);
		}
		std::string VMCString::Replace(const std::string& a, const std::string& b, uint64_t o, const std::string& base)
		{
			return Tomahawk::Core::Parser(base).Replace(a, b, o).R();
		}
		as_int64_t VMCString::IntStore(const std::string &val, as_size_t base, as_size_t *byteCount)
		{
			if (base != 10 && base != 16)
			{
				if (byteCount)
					*byteCount = 0;

				return 0;
			}

			const char *end = &val[0];
			bool sign = false;
			if (*end == '-')
			{
				sign = true;
				end++;
			}
			else if (*end == '+')
				end++;

			as_int64_t res = 0;
			if (base == 10)
			{
				while (*end >= '0' && *end <= '9')
				{
					res *= 10;
					res += *end++ - '0';
				}
			}
			else
			{
				while ((*end >= '0' && *end <= '9') || (*end >= 'a' && *end <= 'f') || (*end >= 'A' && *end <= 'F'))
				{
					res *= 16;
					if (*end >= '0' && *end <= '9')
						res += *end++ - '0';
					else if (*end >= 'a' && *end <= 'f')
						res += *end++ - 'a' + 10;
					else if (*end >= 'A' && *end <= 'F')
						res += *end++ - 'A' + 10;
				}
			}

			if (byteCount)
				*byteCount = as_size_t(size_t(end - val.c_str()));

			if (sign)
				res = -res;

			return res;
		}
		as_uint64_t VMCString::UIntStore(const std::string &val, as_size_t base, as_size_t *byteCount)
		{
			if (base != 10 && base != 16)
			{
				if (byteCount)
					*byteCount = 0;
				return 0;
			}

			const char *end = &val[0];
			as_uint64_t res = 0;

			if (base == 10)
			{
				while (*end >= '0' && *end <= '9')
				{
					res *= 10;
					res += *end++ - '0';
				}
			}
			else
			{
				while ((*end >= '0' && *end <= '9') || (*end >= 'a' && *end <= 'f') || (*end >= 'A' && *end <= 'F'))
				{
					res *= 16;
					if (*end >= '0' && *end <= '9')
						res += *end++ - '0';
					else if (*end >= 'a' && *end <= 'f')
						res += *end++ - 'a' + 10;
					else if (*end >= 'A' && *end <= 'F')
						res += *end++ - 'A' + 10;
				}
			}

			if (byteCount)
				*byteCount = as_size_t(size_t(end - val.c_str()));

			return res;
		}
		double VMCString::FloatStore(const std::string &val, as_size_t *byteCount)
		{
			char *end;
#if !defined(_WIN32_WCE) && !defined(ANDROID) && !defined(__psp2__)
			char *tmp = setlocale(LC_NUMERIC, 0);
			std::string orig = tmp ? tmp : "C";
			setlocale(LC_NUMERIC, "C");
#endif

			double res = strtod(val.c_str(), &end);
#if !defined(_WIN32_WCE) && !defined(ANDROID) && !defined(__psp2__)
			setlocale(LC_NUMERIC, orig.c_str());
#endif
			if (byteCount)
				*byteCount = as_size_t(size_t(end - val.c_str()));

			return res;
		}
		std::string VMCString::Sub(as_size_t start, int count, const std::string &str)
		{
			std::string ret;
			if (start < str.length() && count != 0)
				ret = str.substr(start, (size_t)(count < 0 ? std::string::npos : count));

			return ret;
		}
		bool VMCString::Equals(const std::string& lhs, const std::string& rhs)
		{
			return lhs == rhs;
		}
		std::string VMCString::ToLower(const std::string& Symbol)
		{
			return Tomahawk::Core::Parser(Symbol).ToLower().R();
		}
		std::string VMCString::ToUpper(const std::string& Symbol)
		{
			return Tomahawk::Core::Parser(Symbol).ToUpper().R();
		}
		std::string VMCString::ToInt8(char Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToInt16(short Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToInt(int Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToInt64(int64_t Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToUInt8(unsigned char Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToUInt16(unsigned short Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToUInt(unsigned int Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToUInt64(uint64_t Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToFloat(float Value)
		{
			return std::to_string(Value);
		}
		std::string VMCString::ToDouble(double Value)
		{
			return std::to_string(Value);
		}
		VMCArray* VMCString::Split(const std::string &delim, const std::string &str)
		{
			VMCContext *ctx = asGetActiveContext();
			VMCManager *engine = ctx->GetEngine();
			asITypeInfo *arrayType = engine->GetTypeInfoByDecl("Array<String>");
			VMCArray *array = VMCArray::Create(arrayType);

			int pos = 0, prev = 0, count = 0;
			while ((pos = (int)str.find(delim, prev)) != (int)std::string::npos)
			{
				array->Resize(array->GetSize() + 1);
				((std::string*)array->At(count))->assign(&str[prev], pos - prev);
				count++;
				prev = pos + (int)delim.length();
			}

			array->Resize(array->GetSize() + 1);
			((std::string*)array->At(count))->assign(&str[prev]);
			return array;
		}
		std::string VMCString::Join(const VMCArray &array, const std::string &delim)
		{
			std::string str = "";
			if (!array.GetSize())
				return str;

			int n;
			for (n = 0; n < (int)array.GetSize() - 1; n++)
			{
				str += *(std::string*)array.At(n);
				str += delim;
			}

			str += *(std::string*)array.At(n);
			return str;
		}
		char VMCString::ToChar(const std::string& Symbol)
		{
			return Symbol.empty() ? '\0' : Symbol[0];
		}

		VMCMutex::VMCMutex() : Ref(1)
		{
		}
		void VMCMutex::Release()
		{
			if (asAtomicDec(Ref) <= 0)
			{
				this->~VMCMutex();
				asFreeMem((void*)this);
			}
		}
		void VMCMutex::AddRef()
		{
			asAtomicInc(Ref);
		}
		bool VMCMutex::TryLock()
		{
			return Base.try_lock();
		}
		void VMCMutex::Lock()
		{
			Base.lock();
		}
		void VMCMutex::Unlock()
		{
			Base.unlock();
		}
		VMCMutex* VMCMutex::Factory()
		{
			void* Data = asAllocMem(sizeof(VMCMutex));
			if (!Data)
			{
				VMCContext* Context = asGetActiveContext();
				if (Context != nullptr)
					Context->SetException("Out of memory");

				return nullptr;
			}

			return new(Data) VMCMutex();
		}

		float VMCMath::FpFromIEEE(as_size_t raw)
		{
			return *reinterpret_cast<float*>(&raw);
		}
		as_size_t VMCMath::FpToIEEE(float fp)
		{
			return *reinterpret_cast<as_size_t*>(&fp);
		}
		double VMCMath::FpFromIEEE(as_uint64_t raw)
		{
			return *reinterpret_cast<double*>(&raw);
		}
		as_uint64_t VMCMath::FpToIEEE(double fp)
		{
			return *reinterpret_cast<as_uint64_t*>(&fp);
		}
		bool VMCMath::CloseTo(float a, float b, float epsilon)
		{
			if (a == b)
				return true;

			float diff = fabsf(a - b);
			if ((a == 0 || b == 0) && (diff < epsilon))
				return true;

			return diff / (fabs(a) + fabs(b)) < epsilon;
		}
		bool VMCMath::CloseTo(double a, double b, double epsilon)
		{
			if (a == b)
				return true;

			double diff = fabs(a - b);
			if ((a == 0 || b == 0) && (diff < epsilon))
				return true;

			return diff / (fabs(a) + fabs(b)) < epsilon;
		}

		void VMCException::Throw(const std::string& In)
		{
			VMCContext* Context = asGetActiveContext();
			if (Context != nullptr)
				Context->SetException(In.empty() ? "runtime exception" : In.c_str());
		}
		std::string VMCException::GetException()
		{
			VMCContext* Context = asGetActiveContext();
			if (!Context)
				return "";

			const char* Message = Context->GetExceptionString();
			if (!Message)
				return "";

			return Message;
		}

		VMCAny::VMCAny(VMCManager* fEngine)
		{
			Engine = fEngine;
			RefCount = 1;
			GCFlag = false;
			Value.TypeId = 0;
			Value.ValueInt = 0;

			Engine->NotifyGarbageCollectorOfNewObject(this, Engine->GetTypeInfoByName("Any"));
		}
		VMCAny::VMCAny(void* Ref, int RefTypeId, VMCManager* fEngine)
		{
			Engine = fEngine;
			RefCount = 1;
			GCFlag = false;
			Value.TypeId = 0;
			Value.ValueInt = 0;

			Engine->NotifyGarbageCollectorOfNewObject(this, Engine->GetTypeInfoByName("Any"));
			Store(Ref, RefTypeId);
		}
		VMCAny::VMCAny(const VMCAny& Other)
		{
			Engine = Other.Engine;
			RefCount = 1;
			GCFlag = false;
			Value.TypeId = 0;
			Value.ValueInt = 0;

			Engine->NotifyGarbageCollectorOfNewObject(this, Engine->GetTypeInfoByName("Any"));
			if ((Other.Value.TypeId & asTYPEID_MASK_OBJECT))
			{
				VMCTypeInfo* T = Engine->GetTypeInfoById(Other.Value.TypeId);
				if (T)
					T->AddRef();
			}

			Value.TypeId = Other.Value.TypeId;
			if (Value.TypeId & asTYPEID_OBJHANDLE)
			{
				Value.ValueObj = Other.Value.ValueObj;
				Engine->AddRefScriptObject(Value.ValueObj, Engine->GetTypeInfoById(Value.TypeId));
			}
			else if (Value.TypeId & asTYPEID_MASK_OBJECT)
				Value.ValueObj = Engine->CreateScriptObjectCopy(Other.Value.ValueObj, Engine->GetTypeInfoById(Value.TypeId));
			else
				Value.ValueInt = Other.Value.ValueInt;
		}
		VMCAny::~VMCAny()
		{
			FreeObject();
		}
		VMCAny &VMCAny::operator=(const VMCAny& Other)
		{
			if ((Other.Value.TypeId & asTYPEID_MASK_OBJECT))
			{
				VMCTypeInfo* T = Engine->GetTypeInfoById(Other.Value.TypeId);
				if (T)
					T->AddRef();
			}

			FreeObject();

			Value.TypeId = Other.Value.TypeId;
			if (Value.TypeId & asTYPEID_OBJHANDLE)
			{
				Value.ValueObj = Other.Value.ValueObj;
				Engine->AddRefScriptObject(Value.ValueObj, Engine->GetTypeInfoById(Value.TypeId));
			}
			else if (Value.TypeId & asTYPEID_MASK_OBJECT)
				Value.ValueObj = Engine->CreateScriptObjectCopy(Other.Value.ValueObj, Engine->GetTypeInfoById(Value.TypeId));
			else
				Value.ValueInt = Other.Value.ValueInt;

			return *this;
		}
		int VMCAny::CopyFrom(const VMCAny* Other)
		{
			if (Other == 0)
				return asINVALID_ARG;

			*this = *(VMCAny*)Other;
			return 0;
		}
		void VMCAny::Store(void* Ref, int RefTypeId)
		{
			if ((RefTypeId & asTYPEID_MASK_OBJECT))
			{
				VMCTypeInfo* T = Engine->GetTypeInfoById(RefTypeId);
				if (T)
					T->AddRef();
			}

			FreeObject();

			Value.TypeId = RefTypeId;
			if (Value.TypeId & asTYPEID_OBJHANDLE)
			{
				Value.ValueObj = *(void**)Ref;
				Engine->AddRefScriptObject(Value.ValueObj, Engine->GetTypeInfoById(Value.TypeId));
			}
			else if (Value.TypeId & asTYPEID_MASK_OBJECT)
			{
				Value.ValueObj = Engine->CreateScriptObjectCopy(Ref, Engine->GetTypeInfoById(Value.TypeId));
			}
			else
			{
				Value.ValueInt = 0;

				int Size = Engine->GetSizeOfPrimitiveType(Value.TypeId);
				memcpy(&Value.ValueInt, Ref, Size);
			}
		}
		bool VMCAny::Retrieve(void* Ref, int RefTypeId) const
		{
			if (RefTypeId & asTYPEID_OBJHANDLE)
			{
				if ((Value.TypeId & asTYPEID_MASK_OBJECT))
				{
					if ((Value.TypeId & asTYPEID_HANDLETOCONST) && !(RefTypeId & asTYPEID_HANDLETOCONST))
						return false;

					Engine->RefCastObject(Value.ValueObj, Engine->GetTypeInfoById(Value.TypeId), Engine->GetTypeInfoById(RefTypeId), reinterpret_cast<void**>(Ref));
					if (*(asPWORD*)Ref == 0)
						return false;

					return true;
				}
			}
			else if (RefTypeId & asTYPEID_MASK_OBJECT)
			{
				if (Value.TypeId == RefTypeId)
				{
					Engine->AssignScriptObject(Ref, Value.ValueObj, Engine->GetTypeInfoById(Value.TypeId));
					return true;
				}
			}
			else
			{
				int Size1 = Engine->GetSizeOfPrimitiveType(Value.TypeId);
				int Size2 = Engine->GetSizeOfPrimitiveType(RefTypeId);

				if (Size1 == Size2)
				{
					memcpy(Ref, &Value.ValueInt, Size1);
					return true;
				}
			}

			return false;
		}
		int VMCAny::GetTypeId() const
		{
			return Value.TypeId;
		}
		void VMCAny::FreeObject()
		{
			if (Value.TypeId & asTYPEID_MASK_OBJECT)
			{
				VMCTypeInfo* T = Engine->GetTypeInfoById(Value.TypeId);
				Engine->ReleaseScriptObject(Value.ValueObj, T);

				if (T)
					T->Release();

				Value.ValueObj = 0;
				Value.TypeId = 0;
			}
		}
		void VMCAny::EnumReferences(VMCManager* InEngine)
		{
			if (Value.ValueObj && (Value.TypeId & asTYPEID_MASK_OBJECT))
			{
				VMCTypeInfo* SubType = Engine->GetTypeInfoById(Value.TypeId);
				if ((SubType->GetFlags() & asOBJ_REF))
					InEngine->GCEnumCallback(Value.ValueObj);
				else if ((SubType->GetFlags() & asOBJ_VALUE) && (SubType->GetFlags() & asOBJ_GC))
					Engine->ForwardGCEnumReferences(Value.ValueObj, SubType);

				VMCTypeInfo* T = InEngine->GetTypeInfoById(Value.TypeId);
				if (T)
					InEngine->GCEnumCallback(T);
			}
		}
		void VMCAny::ReleaseAllHandles(VMCManager*)
		{
			FreeObject();
		}
		int VMCAny::AddRef() const
		{
			GCFlag = false;
			return asAtomicInc(RefCount);
		}
		int VMCAny::Release() const
		{
			GCFlag = false;
			if (asAtomicDec(RefCount) == 0)
			{
				this->~VMCAny();
				asFreeMem((void*)this);
				return 0;
			}

			return RefCount;
		}
		int VMCAny::GetRefCount()
		{
			return RefCount;
		}
		void VMCAny::SetFlag()
		{
			GCFlag = true;
		}
		bool VMCAny::GetFlag()
		{
			return GCFlag;
		}
		VMCAny* VMCAny::Create(int TypeId, void* Ref)
		{
			VMCContext* Context = asGetActiveContext();
			if (!Context)
				return nullptr;

			void* Data = asAllocMem(sizeof(VMCAny));
			return new(Data) VMCAny(Ref, TypeId, Context->GetEngine());
		}
		VMCAny* VMCAny::Create(const char* Decl, void* Ref)
		{
			VMCContext* Context = asGetActiveContext();
			if (!Context)
				return nullptr;

			VMCManager* Engine = Context->GetEngine();
			void* Data = asAllocMem(sizeof(VMCAny));
			return new(Data) VMCAny(Ref, Engine->GetTypeIdByDecl(Decl), Engine);
		}
		void VMCAny::Factory1(VMCGeneric* G)
		{
			VMCManager* Engine = G->GetEngine();
			void* Mem = asAllocMem(sizeof(VMCAny));
			*(VMCAny**)G->GetAddressOfReturnLocation() = new(Mem) VMCAny(Engine);
		}
		void VMCAny::Factory2(VMCGeneric* G)
		{
			VMCManager* Engine = G->GetEngine();
			void* Ref = (void*)G->GetArgAddress(0);
			void* Mem = asAllocMem(sizeof(VMCAny));
			int RefType = G->GetArgTypeId(0);

			*(VMCAny**)G->GetAddressOfReturnLocation() = new(Mem) VMCAny(Ref, RefType, Engine);
		}
		VMCAny& VMCAny::Assignment(VMCAny* Other, VMCAny* Self)
		{
			return *Self = *Other;
		}

		VMCArray &VMCArray::operator=(const VMCArray &other)
		{
			if (&other != this && other.GetArrayObjectType() == GetArrayObjectType())
			{
				Resize(other.Buffer->NumElements);
				CopyBuffer(Buffer, other.Buffer);
			}

			return *this;
		}
		VMCArray::VMCArray(VMCTypeInfo *TI, void *buf)
		{
			assert(TI && std::string(TI->GetName()) == "Array");
			RefCount = 1;
			GCFlag = false;
			ObjType = TI;
			ObjType->AddRef();
			Buffer = 0;
			Precache();

			VMCManager *engine = TI->GetEngine();
			if (SubTypeId & asTYPEID_MASK_OBJECT)
				ElementSize = sizeof(asPWORD);
			else
				ElementSize = engine->GetSizeOfPrimitiveType(SubTypeId);

			as_size_t length = *(as_size_t*)buf;
			if (!CheckMaxSize(length))
				return;

			if ((TI->GetSubTypeId() & asTYPEID_MASK_OBJECT) == 0)
			{
				CreateBuffer(&Buffer, length);
				if (length > 0)
					memcpy(At(0), (((as_size_t*)buf) + 1), (size_t)length * (size_t)ElementSize);
			}
			else if (TI->GetSubTypeId() & asTYPEID_OBJHANDLE)
			{
				CreateBuffer(&Buffer, length);
				if (length > 0)
					memcpy(At(0), (((as_size_t*)buf) + 1), (size_t)length * (size_t)ElementSize);

				memset((((as_size_t*)buf) + 1), 0, (size_t)length * (size_t)ElementSize);
			}
			else if (TI->GetSubType()->GetFlags() & asOBJ_REF)
			{
				SubTypeId |= asTYPEID_OBJHANDLE;
				CreateBuffer(&Buffer, length);
				SubTypeId &= ~asTYPEID_OBJHANDLE;

				if (length > 0)
					memcpy(Buffer->Data, (((as_size_t*)buf) + 1), (size_t)length * (size_t)ElementSize);

				memset((((as_size_t*)buf) + 1), 0, (size_t)length * (size_t)ElementSize);
			}
			else
			{
				CreateBuffer(&Buffer, length);
				for (as_size_t n = 0; n < length; n++)
				{
					void *obj = At(n);
					unsigned char *srcObj = (unsigned char*)buf;
					srcObj += 4 + n * TI->GetSubType()->GetSize();
					engine->AssignScriptObject(obj, srcObj, TI->GetSubType());
				}
			}

			if (ObjType->GetFlags() & asOBJ_GC)
				ObjType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, ObjType);
		}
		VMCArray::VMCArray(as_size_t length, VMCTypeInfo *TI)
		{
			assert(TI && std::string(TI->GetName()) == "Array");
			RefCount = 1;
			GCFlag = false;
			ObjType = TI;
			ObjType->AddRef();
			Buffer = 0;
			Precache();

			if (SubTypeId & asTYPEID_MASK_OBJECT)
				ElementSize = sizeof(asPWORD);
			else
				ElementSize = ObjType->GetEngine()->GetSizeOfPrimitiveType(SubTypeId);

			if (!CheckMaxSize(length))
				return;

			CreateBuffer(&Buffer, length);
			if (ObjType->GetFlags() & asOBJ_GC)
				ObjType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, ObjType);
		}
		VMCArray::VMCArray(const VMCArray &other)
		{
			RefCount = 1;
			GCFlag = false;
			ObjType = other.ObjType;
			ObjType->AddRef();
			Buffer = 0;
			Precache();

			ElementSize = other.ElementSize;
			if (ObjType->GetFlags() & asOBJ_GC)
				ObjType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, ObjType);

			CreateBuffer(&Buffer, 0);
			*this = other;
		}
		VMCArray::VMCArray(as_size_t length, void *defVal, VMCTypeInfo *TI)
		{
			assert(TI && std::string(TI->GetName()) == "Array");
			RefCount = 1;
			GCFlag = false;
			ObjType = TI;
			ObjType->AddRef();
			Buffer = 0;
			Precache();

			if (SubTypeId & asTYPEID_MASK_OBJECT)
				ElementSize = sizeof(asPWORD);
			else
				ElementSize = ObjType->GetEngine()->GetSizeOfPrimitiveType(SubTypeId);

			if (!CheckMaxSize(length))
				return;

			CreateBuffer(&Buffer, length);
			if (ObjType->GetFlags() & asOBJ_GC)
				ObjType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, ObjType);

			for (as_size_t n = 0; n < GetSize(); n++)
				SetValue(n, defVal);
		}
		void VMCArray::SetValue(as_size_t index, void *value)
		{
			void *ptr = At(index);
			if (ptr == 0)
				return;

			if ((SubTypeId & ~asTYPEID_MASK_SEQNBR) && !(SubTypeId & asTYPEID_OBJHANDLE))
				ObjType->GetEngine()->AssignScriptObject(ptr, value, ObjType->GetSubType());
			else if (SubTypeId & asTYPEID_OBJHANDLE)
			{
				void *tmp = *(void**)ptr;
				*(void**)ptr = *(void**)value;
				ObjType->GetEngine()->AddRefScriptObject(*(void**)value, ObjType->GetSubType());
				if (tmp)
					ObjType->GetEngine()->ReleaseScriptObject(tmp, ObjType->GetSubType());
			}
			else if (SubTypeId == asTYPEID_BOOL ||
				SubTypeId == asTYPEID_INT8 ||
				SubTypeId == asTYPEID_UINT8)
				*(char*)ptr = *(char*)value;
			else if (SubTypeId == asTYPEID_INT16 ||
				SubTypeId == asTYPEID_UINT16)
				*(short*)ptr = *(short*)value;
			else if (SubTypeId == asTYPEID_INT32 ||
				SubTypeId == asTYPEID_UINT32 ||
				SubTypeId == asTYPEID_FLOAT ||
				SubTypeId > asTYPEID_DOUBLE)
				*(int*)ptr = *(int*)value;
			else if (SubTypeId == asTYPEID_INT64 ||
				SubTypeId == asTYPEID_UINT64 ||
				SubTypeId == asTYPEID_DOUBLE)
				*(double*)ptr = *(double*)value;
		}
		VMCArray::~VMCArray()
		{
			if (Buffer)
			{
				DeleteBuffer(Buffer);
				Buffer = 0;
			}
			if (ObjType)
				ObjType->Release();
		}
		as_size_t VMCArray::GetSize() const
		{
			return Buffer->NumElements;
		}
		bool VMCArray::IsEmpty() const
		{
			return Buffer->NumElements == 0;
		}
		void VMCArray::Reserve(as_size_t MaxElements)
		{
			if (MaxElements <= Buffer->MaxElements)
				return;

			if (!CheckMaxSize(MaxElements))
				return;

			SBuffer* newBuffer = reinterpret_cast<SBuffer*>(asAllocMem(sizeof(SBuffer) - 1 + (size_t)ElementSize * (size_t)MaxElements));
			if (newBuffer)
			{
				newBuffer->NumElements = Buffer->NumElements;
				newBuffer->MaxElements = MaxElements;
			}
			else
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");
				return;
			}

			memcpy(newBuffer->Data, Buffer->Data, (size_t)Buffer->NumElements * (size_t)ElementSize);
			asFreeMem(Buffer);
			Buffer = newBuffer;
		}
		void VMCArray::Resize(as_size_t NumElements)
		{
			if (!CheckMaxSize(NumElements))
				return;

			Resize((int)NumElements - (int)Buffer->NumElements, (as_size_t)-1);
		}
		void VMCArray::RemoveRange(as_size_t start, as_size_t count)
		{
			if (count == 0)
				return;

			if (Buffer == 0 || start > Buffer->NumElements)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Index out of bounds");
				return;
			}

			if (start + count > Buffer->NumElements)
				count = Buffer->NumElements - start;

			Destruct(Buffer, start, start + count);
			memmove(Buffer->Data + start * (as_size_t)ElementSize, Buffer->Data + (start + count) * (as_size_t)ElementSize, (size_t)(Buffer->NumElements - start - count) * (size_t)ElementSize);
			Buffer->NumElements -= count;
		}
		void VMCArray::Resize(int delta, as_size_t at)
		{
			if (delta < 0)
			{
				if (-delta > (int)Buffer->NumElements)
					delta = -(int)Buffer->NumElements;
				if (at > Buffer->NumElements + delta)
					at = Buffer->NumElements + delta;
			}
			else if (delta > 0)
			{
				if (!CheckMaxSize(Buffer->NumElements + delta))
					return;

				if (at > Buffer->NumElements)
					at = Buffer->NumElements;
			}

			if (delta == 0)
				return;

			if (Buffer->MaxElements < Buffer->NumElements + delta)
			{
				size_t Count = (size_t)Buffer->NumElements + (size_t)delta, Size = (size_t)ElementSize;
				SBuffer *newBuffer = reinterpret_cast<SBuffer*>(asAllocMem(sizeof(SBuffer) - 1 + Size * Count));
				if (newBuffer)
				{
					newBuffer->NumElements = Buffer->NumElements + delta;
					newBuffer->MaxElements = newBuffer->NumElements;
				}
				else
				{
					VMCContext *ctx = asGetActiveContext();
					if (ctx)
						ctx->SetException("Out of memory");
					return;
				}

				memcpy(newBuffer->Data, Buffer->Data, (size_t)at * (size_t)ElementSize);
				if (at < Buffer->NumElements)
					memcpy(newBuffer->Data + (at + delta) * (as_size_t)ElementSize, Buffer->Data + at * (as_size_t)ElementSize, (size_t)(Buffer->NumElements - at) * (size_t)ElementSize);

				Construct(newBuffer, at, at + delta);
				asFreeMem(Buffer);
				Buffer = newBuffer;
			}
			else if (delta < 0)
			{
				Destruct(Buffer, at, at - delta);
				memmove(Buffer->Data + at * (as_size_t)ElementSize, Buffer->Data + (at - delta) * (as_size_t)ElementSize, (size_t)(Buffer->NumElements - (at - delta)) * (size_t)ElementSize);
				Buffer->NumElements += delta;
			}
			else
			{
				memmove(Buffer->Data + (at + delta) * (as_size_t)ElementSize, Buffer->Data + at * (as_size_t)ElementSize, (size_t)(Buffer->NumElements - at) * (size_t)ElementSize);
				Construct(Buffer, at, at + delta);
				Buffer->NumElements += delta;
			}
		}
		bool VMCArray::CheckMaxSize(as_size_t NumElements)
		{
			as_size_t maxSize = 0xFFFFFFFFul - sizeof(SBuffer) + 1;
			if (ElementSize > 0)
				maxSize /= (as_size_t)ElementSize;

			if (NumElements > maxSize)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Too large Array size");

				return false;
			}

			return true;
		}
		VMCTypeInfo *VMCArray::GetArrayObjectType() const
		{
			return ObjType;
		}
		int VMCArray::GetArrayTypeId() const
		{
			return ObjType->GetTypeId();
		}
		int VMCArray::GetElementTypeId() const
		{
			return SubTypeId;
		}
		void VMCArray::InsertAt(as_size_t index, void *value)
		{
			if (index > Buffer->NumElements)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Index out of bounds");
				return;
			}

			Resize(1, index);
			SetValue(index, value);
		}
		void VMCArray::InsertAt(as_size_t index, const VMCArray &arr)
		{
			if (index > Buffer->NumElements)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Index out of bounds");
				return;
			}

			if (ObjType != arr.ObjType)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Mismatching Array types");
				return;
			}

			as_size_t elements = arr.GetSize();
			Resize(elements, index);
			if (&arr != this)
			{
				for (as_size_t n = 0; n < arr.GetSize(); n++)
				{
					void *value = const_cast<void*>(arr.At(n));
					SetValue(index + n, value);
				}
			}
			else
			{
				for (as_size_t n = 0; n < index; n++)
				{
					void *value = const_cast<void*>(arr.At(n));
					SetValue(index + n, value);
				}

				for (as_size_t n = index + elements, m = 0; n < arr.GetSize(); n++, m++)
				{
					void *value = const_cast<void*>(arr.At(n));
					SetValue(index + index + m, value);
				}
			}
		}
		void VMCArray::InsertLast(void *value)
		{
			InsertAt(Buffer->NumElements, value);
		}
		void VMCArray::RemoveAt(as_size_t index)
		{
			if (index >= Buffer->NumElements)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Index out of bounds");
				return;
			}

			Resize(-1, index);
		}
		void VMCArray::RemoveLast()
		{
			RemoveAt(Buffer->NumElements - 1);
		}
		const void *VMCArray::At(as_size_t index) const
		{
			if (Buffer == 0 || index >= Buffer->NumElements)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Index out of bounds");
				return 0;
			}

			if ((SubTypeId & asTYPEID_MASK_OBJECT) && !(SubTypeId & asTYPEID_OBJHANDLE))
				return *(void**)(Buffer->Data + (as_size_t)ElementSize * index);
			else
				return Buffer->Data + (as_size_t)ElementSize * index;
		}
		void *VMCArray::At(as_size_t index)
		{
			return const_cast<void*>(const_cast<const VMCArray *>(this)->At(index));
		}
		void *VMCArray::GetBuffer()
		{
			return Buffer->Data;
		}
		void VMCArray::CreateBuffer(SBuffer **buf, as_size_t NumElements)
		{
			*buf = reinterpret_cast<SBuffer*>(asAllocMem(sizeof(SBuffer) - 1 + (size_t)ElementSize * (size_t)NumElements));
			if (*buf)
			{
				(*buf)->NumElements = NumElements;
				(*buf)->MaxElements = NumElements;
				Construct(*buf, 0, NumElements);
			}
			else
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");
			}
		}
		void VMCArray::DeleteBuffer(SBuffer *buf)
		{
			Destruct(buf, 0, buf->NumElements);
			asFreeMem(buf);
		}
		void VMCArray::Construct(SBuffer *buf, as_size_t start, as_size_t end)
		{
			if ((SubTypeId & asTYPEID_MASK_OBJECT) && !(SubTypeId & asTYPEID_OBJHANDLE))
			{
				void **max = (void**)(buf->Data + end * sizeof(void*));
				void **d = (void**)(buf->Data + start * sizeof(void*));

				VMCManager *engine = ObjType->GetEngine();
				VMCTypeInfo *subType = ObjType->GetSubType();

				for (; d < max; d++)
				{
					*d = (void*)engine->CreateScriptObject(subType);
					if (*d == 0)
					{
						memset(d, 0, sizeof(void*)*(max - d));
						return;
					}
				}
			}
			else
			{
				void *d = (void*)(buf->Data + start * (as_size_t)ElementSize);
				memset(d, 0, (size_t)(end - start) * (size_t)ElementSize);
			}
		}
		void VMCArray::Destruct(SBuffer *buf, as_size_t start, as_size_t end)
		{
			if (SubTypeId & asTYPEID_MASK_OBJECT)
			{
				VMCManager *engine = ObjType->GetEngine();
				void **max = (void**)(buf->Data + end * sizeof(void*));
				void **d = (void**)(buf->Data + start * sizeof(void*));

				for (; d < max; d++)
				{
					if (*d)
						engine->ReleaseScriptObject(*d, ObjType->GetSubType());
				}
			}
		}
		bool VMCArray::Less(const void *a, const void *b, bool asc, VMCContext *ctx, SCache *cache)
		{
			if (!asc)
			{
				const void *TEMP = a;
				a = b;
				b = TEMP;
			}

			if (!(SubTypeId & ~asTYPEID_MASK_SEQNBR))
			{
				switch (SubTypeId)
				{
#define COMPARE(T) *((T*)a) < *((T*)b)
					case asTYPEID_BOOL: return COMPARE(bool);
					case asTYPEID_INT8: return COMPARE(signed char);
					case asTYPEID_UINT8: return COMPARE(unsigned char);
					case asTYPEID_INT16: return COMPARE(signed short);
					case asTYPEID_UINT16: return COMPARE(unsigned short);
					case asTYPEID_INT32: return COMPARE(signed int);
					case asTYPEID_UINT32: return COMPARE(unsigned int);
					case asTYPEID_FLOAT: return COMPARE(float);
					case asTYPEID_DOUBLE: return COMPARE(double);
					default: return COMPARE(signed int); // All enums fall in this case
#undef COMPARE
				}
			}
			else
			{
				int r = 0;
				if (SubTypeId & asTYPEID_OBJHANDLE)
				{
					if (*(void**)a == 0)
						return true;

					if (*(void**)b == 0)
						return false;
				}

				if (cache && cache->CmpFunc)
				{
					r = ctx->Prepare(cache->CmpFunc); assert(r >= 0);
					if (SubTypeId & asTYPEID_OBJHANDLE)
					{
						r = ctx->SetObject(*((void**)a)); assert(r >= 0);
						r = ctx->SetArgObject(0, *((void**)b)); assert(r >= 0);
					}
					else
					{
						r = ctx->SetObject((void*)a); assert(r >= 0);
						r = ctx->SetArgObject(0, (void*)b); assert(r >= 0);
					}

					r = ctx->Execute();
					if (r == asEXECUTION_FINISHED)
						return (int)ctx->GetReturnDWord() < 0;
				}
			}

			return false;
		}
		void VMCArray::Reverse()
		{
			as_size_t size = GetSize();
			if (size >= 2)
			{
				unsigned char TEMP[16];
				for (as_size_t i = 0; i < size / 2; i++)
				{
					Copy(TEMP, GetArrayItemPointer(i));
					Copy(GetArrayItemPointer(i), GetArrayItemPointer(size - i - 1));
					Copy(GetArrayItemPointer(size - i - 1), TEMP);
				}
			}
		}
		bool VMCArray::operator==(const VMCArray &other) const
		{
			if (ObjType != other.ObjType)
				return false;

			if (GetSize() != other.GetSize())
				return false;

			VMCContext *cmpContext = 0;
			bool isNested = false;

			if (SubTypeId & ~asTYPEID_MASK_SEQNBR)
			{
				cmpContext = asGetActiveContext();
				if (cmpContext)
				{
					if (cmpContext->GetEngine() == ObjType->GetEngine() && cmpContext->PushState() >= 0)
						isNested = true;
					else
						cmpContext = 0;
				}

				if (cmpContext == 0)
					cmpContext = ObjType->GetEngine()->CreateContext();
			}

			bool isEqual = true;
			SCache *cache = reinterpret_cast<SCache*>(ObjType->GetUserData(ARRAY_CACHE));
			for (as_size_t n = 0; n < GetSize(); n++)
			{
				if (!Equals(At(n), other.At(n), cmpContext, cache))
				{
					isEqual = false;
					break;
				}
			}

			if (cmpContext)
			{
				if (isNested)
				{
					asEContextState state = cmpContext->GetState();
					cmpContext->PopState();
					if (state == asEXECUTION_ABORTED)
						cmpContext->Abort();
				}
				else
					cmpContext->Release();
			}

			return isEqual;
		}
		bool VMCArray::Equals(const void *a, const void *b, VMCContext *ctx, SCache *cache) const
		{
			if (!(SubTypeId & ~asTYPEID_MASK_SEQNBR))
			{
				switch (SubTypeId)
				{
#define COMPARE(T) *((T*)a) == *((T*)b)
					case asTYPEID_BOOL: return COMPARE(bool);
					case asTYPEID_INT8: return COMPARE(signed char);
					case asTYPEID_UINT8: return COMPARE(unsigned char);
					case asTYPEID_INT16: return COMPARE(signed short);
					case asTYPEID_UINT16: return COMPARE(unsigned short);
					case asTYPEID_INT32: return COMPARE(signed int);
					case asTYPEID_UINT32: return COMPARE(unsigned int);
					case asTYPEID_FLOAT: return COMPARE(float);
					case asTYPEID_DOUBLE: return COMPARE(double);
					default: return COMPARE(signed int);
#undef COMPARE
				}
			}
			else
			{
				int r = 0;
				if (SubTypeId & asTYPEID_OBJHANDLE)
				{
					if (*(void**)a == *(void**)b)
						return true;
				}

				if (cache && cache->EqFunc)
				{
					r = ctx->Prepare(cache->EqFunc); assert(r >= 0);

					if (SubTypeId & asTYPEID_OBJHANDLE)
					{
						r = ctx->SetObject(*((void**)a)); assert(r >= 0);
						r = ctx->SetArgObject(0, *((void**)b)); assert(r >= 0);
					}
					else
					{
						r = ctx->SetObject((void*)a); assert(r >= 0);
						r = ctx->SetArgObject(0, (void*)b); assert(r >= 0);
					}

					r = ctx->Execute();
					if (r == asEXECUTION_FINISHED)
						return ctx->GetReturnByte() != 0;

					return false;
				}

				if (cache && cache->CmpFunc)
				{
					r = ctx->Prepare(cache->CmpFunc); assert(r >= 0);
					if (SubTypeId & asTYPEID_OBJHANDLE)
					{
						r = ctx->SetObject(*((void**)a)); assert(r >= 0);
						r = ctx->SetArgObject(0, *((void**)b)); assert(r >= 0);
					}
					else
					{
						r = ctx->SetObject((void*)a); assert(r >= 0);
						r = ctx->SetArgObject(0, (void*)b); assert(r >= 0);
					}

					r = ctx->Execute();
					if (r == asEXECUTION_FINISHED)
						return (int)ctx->GetReturnDWord() == 0;

					return false;
				}
			}

			return false;
		}
		int VMCArray::FindByRef(void *ref) const
		{
			return FindByRef(0, ref);
		}
		int VMCArray::FindByRef(as_size_t startAt, void *ref) const
		{
			as_size_t size = GetSize();
			if (SubTypeId & asTYPEID_OBJHANDLE)
			{
				ref = *(void**)ref;
				for (as_size_t i = startAt; i < size; i++)
				{
					if (*(void**)At(i) == ref)
						return i;
				}
			}
			else
			{
				for (as_size_t i = startAt; i < size; i++)
				{
					if (At(i) == ref)
						return i;
				}
			}

			return -1;
		}
		int VMCArray::Find(void *value) const
		{
			return Find(0, value);
		}
		int VMCArray::Find(as_size_t startAt, void *value) const
		{
			SCache *cache = 0;
			if (SubTypeId & ~asTYPEID_MASK_SEQNBR)
			{
				cache = reinterpret_cast<SCache*>(ObjType->GetUserData(ARRAY_CACHE));
				if (!cache || (cache->CmpFunc == 0 && cache->EqFunc == 0))
				{
					VMCContext *ctx = asGetActiveContext();
					VMCTypeInfo* subType = ObjType->GetEngine()->GetTypeInfoById(SubTypeId);
					if (ctx)
					{
						char tmp[512];
						if (cache && cache->EqFuncReturnCode == asMULTIPLE_FUNCTIONS)
#if defined(_MSC_VER) && _MSC_VER >= 1500 && !defined(__S3E__)
							sprintf_s(tmp, 512, "Type '%s' has multiple matching opEquals or opCmp methods", subType->GetName());
#else
							sprintf(tmp, "Type '%s' has multiple matching opEquals or opCmp methods", subType->GetName());
#endif
						else
#if defined(_MSC_VER) && _MSC_VER >= 1500 && !defined(__S3E__)
							sprintf_s(tmp, 512, "Type '%s' does not have a matching opEquals or opCmp method", subType->GetName());
#else
							sprintf(tmp, "Type '%s' does not have a matching opEquals or opCmp method", subType->GetName());
#endif
						ctx->SetException(tmp);
					}

					return -1;
				}
			}

			VMCContext *cmpContext = 0;
			bool isNested = false;

			if (SubTypeId & ~asTYPEID_MASK_SEQNBR)
			{
				cmpContext = asGetActiveContext();
				if (cmpContext)
				{
					if (cmpContext->GetEngine() == ObjType->GetEngine() && cmpContext->PushState() >= 0)
						isNested = true;
					else
						cmpContext = 0;
				}

				if (cmpContext == 0)
					cmpContext = ObjType->GetEngine()->CreateContext();
			}

			int ret = -1;
			as_size_t size = GetSize();
			for (as_size_t i = startAt; i < size; i++)
			{
				if (Equals(At(i), value, cmpContext, cache))
				{
					ret = (int)i;
					break;
				}
			}

			if (cmpContext)
			{
				if (isNested)
				{
					asEContextState state = cmpContext->GetState();
					cmpContext->PopState();
					if (state == asEXECUTION_ABORTED)
						cmpContext->Abort();
				}
				else
					cmpContext->Release();
			}

			return ret;
		}
		void VMCArray::Copy(void *dst, void *src)
		{
			memcpy(dst, src, ElementSize);
		}
		void *VMCArray::GetArrayItemPointer(int index)
		{
			return Buffer->Data + index * ElementSize;
		}
		void *VMCArray::GetDataPointer(void *buf)
		{
			if ((SubTypeId & asTYPEID_MASK_OBJECT) && !(SubTypeId & asTYPEID_OBJHANDLE))
				return reinterpret_cast<void*>(*(size_t*)buf);
			else
				return buf;
		}
		void VMCArray::SortAsc()
		{
			Sort(0, GetSize(), true);
		}
		void VMCArray::SortAsc(as_size_t startAt, as_size_t count)
		{
			Sort(startAt, count, true);
		}
		void VMCArray::SortDesc()
		{
			Sort(0, GetSize(), false);
		}
		void VMCArray::SortDesc(as_size_t startAt, as_size_t count)
		{
			Sort(startAt, count, false);
		}
		void VMCArray::Sort(as_size_t startAt, as_size_t count, bool asc)
		{
			SCache *cache = reinterpret_cast<SCache*>(ObjType->GetUserData(ARRAY_CACHE));
			if (SubTypeId & ~asTYPEID_MASK_SEQNBR)
			{
				if (!cache || cache->CmpFunc == 0)
				{
					VMCContext *ctx = asGetActiveContext();
					VMCTypeInfo* subType = ObjType->GetEngine()->GetTypeInfoById(SubTypeId);
					if (ctx)
					{
						char tmp[512];
						if (cache && cache->CmpFuncReturnCode == asMULTIPLE_FUNCTIONS)
#if defined(_MSC_VER) && _MSC_VER >= 1500 && !defined(__S3E__)
							sprintf_s(tmp, 512, "Type '%s' has multiple matching opCmp methods", subType->GetName());
#else
							sprintf(tmp, "Type '%s' has multiple matching opCmp methods", subType->GetName());
#endif
						else
#if defined(_MSC_VER) && _MSC_VER >= 1500 && !defined(__S3E__)
							sprintf_s(tmp, 512, "Type '%s' does not have a matching opCmp method", subType->GetName());
#else
							sprintf(tmp, "Type '%s' does not have a matching opCmp method", subType->GetName());
#endif
						ctx->SetException(tmp);
					}

					return;
				}
			}

			if (count < 2)
				return;

			int start = startAt;
			int end = startAt + count;

			if (start >= (int)Buffer->NumElements || end > (int)Buffer->NumElements)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Index out of bounds");

				return;
			}

			unsigned char tmp[16];
			VMCContext *cmpContext = 0;
			bool isNested = false;

			if (SubTypeId & ~asTYPEID_MASK_SEQNBR)
			{
				cmpContext = asGetActiveContext();
				if (cmpContext)
				{
					if (cmpContext->GetEngine() == ObjType->GetEngine() && cmpContext->PushState() >= 0)
						isNested = true;
					else
						cmpContext = 0;
				}

				if (cmpContext == 0)
					cmpContext = ObjType->GetEngine()->RequestContext();
			}

			for (int i = start + 1; i < end; i++)
			{
				Copy(tmp, GetArrayItemPointer(i));
				int j = i - 1;

				while (j >= start && Less(GetDataPointer(tmp), At(j), asc, cmpContext, cache))
				{
					Copy(GetArrayItemPointer(j + 1), GetArrayItemPointer(j));
					j--;
				}

				Copy(GetArrayItemPointer(j + 1), tmp);
			}

			if (cmpContext)
			{
				if (isNested)
				{
					asEContextState state = cmpContext->GetState();
					cmpContext->PopState();
					if (state == asEXECUTION_ABORTED)
						cmpContext->Abort();
				}
				else
					ObjType->GetEngine()->ReturnContext(cmpContext);
			}
		}
		void VMCArray::Sort(asIScriptFunction *func, as_size_t startAt, as_size_t count)
		{
			if (count < 2)
				return;

			as_size_t start = startAt;
			as_size_t end = as_uint64_t(startAt) + as_uint64_t(count) >= (as_uint64_t(1) << 32) ? 0xFFFFFFFF : startAt + count;
			if (end > Buffer->NumElements)
				end = Buffer->NumElements;

			if (start >= Buffer->NumElements)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Index out of bounds");

				return;
			}

			unsigned char tmp[16];
			VMCContext *cmpContext = 0;
			bool isNested = false;

			cmpContext = asGetActiveContext();
			if (cmpContext)
			{
				if (cmpContext->GetEngine() == ObjType->GetEngine() && cmpContext->PushState() >= 0)
					isNested = true;
				else
					cmpContext = 0;
			}

			if (cmpContext == 0)
				cmpContext = ObjType->GetEngine()->RequestContext();

			if (!cmpContext)
				return;

			for (as_size_t i = start + 1; i < end; i++)
			{
				Copy(tmp, GetArrayItemPointer(i));
				as_size_t j = i - 1;

				while (j != 0xFFFFFFFF && j >= start)
				{
					cmpContext->Prepare(func);
					cmpContext->SetArgAddress(0, GetDataPointer(tmp));
					cmpContext->SetArgAddress(1, At(j));
					int r = cmpContext->Execute();
					if (r != asEXECUTION_FINISHED)
						break;

					if (*(bool*)(cmpContext->GetAddressOfReturnValue()))
					{
						Copy(GetArrayItemPointer(j + 1), GetArrayItemPointer(j));
						j--;
					}
					else
						break;
				}

				Copy(GetArrayItemPointer(j + 1), tmp);
			}

			if (isNested)
			{
				asEContextState state = cmpContext->GetState();
				cmpContext->PopState();
				if (state == asEXECUTION_ABORTED)
					cmpContext->Abort();
			}
			else
				ObjType->GetEngine()->ReturnContext(cmpContext);
		}
		void VMCArray::CopyBuffer(SBuffer *dst, SBuffer *src)
		{
			VMCManager *engine = ObjType->GetEngine();
			if (SubTypeId & asTYPEID_OBJHANDLE)
			{
				if (dst->NumElements > 0 && src->NumElements > 0)
				{
					int count = dst->NumElements > src->NumElements ? src->NumElements : dst->NumElements;
					void **max = (void**)(dst->Data + count * sizeof(void*));
					void **d = (void**)dst->Data;
					void **s = (void**)src->Data;

					for (; d < max; d++, s++)
					{
						void *tmp = *d;
						*d = *s;

						if (*d)
							engine->AddRefScriptObject(*d, ObjType->GetSubType());

						if (tmp)
							engine->ReleaseScriptObject(tmp, ObjType->GetSubType());
					}
				}
			}
			else
			{
				if (dst->NumElements > 0 && src->NumElements > 0)
				{
					int count = dst->NumElements > src->NumElements ? src->NumElements : dst->NumElements;
					if (SubTypeId & asTYPEID_MASK_OBJECT)
					{
						void **max = (void**)(dst->Data + count * sizeof(void*));
						void **d = (void**)dst->Data;
						void **s = (void**)src->Data;

						VMCTypeInfo *subType = ObjType->GetSubType();
						for (; d < max; d++, s++)
							engine->AssignScriptObject(*d, *s, subType);
					}
					else
						memcpy(dst->Data, src->Data, (size_t)count * (size_t)ElementSize);
				}
			}
		}
		void VMCArray::Precache()
		{
			SubTypeId = ObjType->GetSubTypeId();
			if (!(SubTypeId & ~asTYPEID_MASK_SEQNBR))
				return;

			SCache *cache = reinterpret_cast<SCache*>(ObjType->GetUserData(ARRAY_CACHE));
			if (cache)
				return;

			asAcquireExclusiveLock();
			cache = reinterpret_cast<SCache*>(ObjType->GetUserData(ARRAY_CACHE));
			if (cache)
			{
				asReleaseExclusiveLock();
				return;
			}

			cache = reinterpret_cast<SCache*>(asAllocMem(sizeof(SCache)));
			if (!cache)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");

				asReleaseExclusiveLock();
				return;
			}

			memset(cache, 0, sizeof(SCache));
			bool mustBeConst = (SubTypeId & asTYPEID_HANDLETOCONST) ? true : false;

			VMCTypeInfo *subType = ObjType->GetEngine()->GetTypeInfoById(SubTypeId);
			if (subType)
			{
				for (as_size_t i = 0; i < subType->GetMethodCount(); i++)
				{
					asIScriptFunction *func = subType->GetMethodByIndex(i);
					if (func->GetParamCount() == 1 && (!mustBeConst || func->IsReadOnly()))
					{
						asDWORD flags = 0;
						int returnTypeId = func->GetReturnTypeId(&flags);
						if (flags != asTM_NONE)
							continue;

						bool isCmp = false, isEq = false;
						if (returnTypeId == asTYPEID_INT32 && strcmp(func->GetName(), "opCmp") == 0)
							isCmp = true;
						if (returnTypeId == asTYPEID_BOOL && strcmp(func->GetName(), "opEquals") == 0)
							isEq = true;

						if (!isCmp && !isEq)
							continue;

						int paramTypeId;
						func->GetParam(0, &paramTypeId, &flags);

						if ((paramTypeId & ~(asTYPEID_OBJHANDLE | asTYPEID_HANDLETOCONST)) != (SubTypeId &  ~(asTYPEID_OBJHANDLE | asTYPEID_HANDLETOCONST)))
							continue;

						if ((flags & asTM_INREF))
						{
							if ((paramTypeId & asTYPEID_OBJHANDLE) || (mustBeConst && !(flags & asTM_CONST)))
								continue;
						}
						else if (paramTypeId & asTYPEID_OBJHANDLE)
						{
							if (mustBeConst && !(paramTypeId & asTYPEID_HANDLETOCONST))
								continue;
						}
						else
							continue;

						if (isCmp)
						{
							if (cache->CmpFunc || cache->CmpFuncReturnCode)
							{
								cache->CmpFunc = 0;
								cache->CmpFuncReturnCode = asMULTIPLE_FUNCTIONS;
							}
							else
								cache->CmpFunc = func;
						}
						else if (isEq)
						{
							if (cache->EqFunc || cache->EqFuncReturnCode)
							{
								cache->EqFunc = 0;
								cache->EqFuncReturnCode = asMULTIPLE_FUNCTIONS;
							}
							else
								cache->EqFunc = func;
						}
					}
				}
			}

			if (cache->EqFunc == 0 && cache->EqFuncReturnCode == 0)
				cache->EqFuncReturnCode = asNO_FUNCTION;
			if (cache->CmpFunc == 0 && cache->CmpFuncReturnCode == 0)
				cache->CmpFuncReturnCode = asNO_FUNCTION;

			ObjType->SetUserData(cache, ARRAY_CACHE);
			asReleaseExclusiveLock();
		}
		void VMCArray::EnumReferences(VMCManager *engine)
		{
			if (SubTypeId & asTYPEID_MASK_OBJECT)
			{
				void **d = (void**)Buffer->Data;
				VMCTypeInfo *subType = engine->GetTypeInfoById(SubTypeId);
				if ((subType->GetFlags() & asOBJ_REF))
				{
					for (as_size_t n = 0; n < Buffer->NumElements; n++)
					{
						if (d[n])
							engine->GCEnumCallback(d[n]);
					}
				}
				else if ((subType->GetFlags() & asOBJ_VALUE) && (subType->GetFlags() & asOBJ_GC))
				{
					for (as_size_t n = 0; n < Buffer->NumElements; n++)
					{
						if (d[n])
							engine->ForwardGCEnumReferences(d[n], subType);
					}
				}
			}
		}
		void VMCArray::ReleaseAllHandles(VMCManager*)
		{
			Resize(0);
		}
		void VMCArray::AddRef() const
		{
			GCFlag = false;
			asAtomicInc(RefCount);
		}
		void VMCArray::Release() const
		{
			GCFlag = false;
			if (asAtomicDec(RefCount) == 0)
			{
				this->~VMCArray();
				asFreeMem(const_cast<VMCArray*>(this));
			}
		}
		int VMCArray::GetRefCount()
		{
			return RefCount;
		}
		void VMCArray::SetFlag()
		{
			GCFlag = true;
		}
		bool VMCArray::GetFlag()
		{
			return GCFlag;
		}
		VMCArray* VMCArray::Create(VMCTypeInfo *TI, as_size_t length)
		{
			void *mem = asAllocMem(sizeof(VMCArray));
			if (mem == 0)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");

				return 0;
			}

			VMCArray *a = new(mem) VMCArray(length, TI);
			return a;
		}
		VMCArray* VMCArray::Create(VMCTypeInfo *TI, void *initList)
		{
			void *mem = asAllocMem(sizeof(VMCArray));
			if (mem == 0)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");

				return 0;
			}

			VMCArray *a = new(mem) VMCArray(TI, initList);
			return a;
		}
		VMCArray* VMCArray::Create(VMCTypeInfo *TI, as_size_t length, void *defVal)
		{
			void *mem = asAllocMem(sizeof(VMCArray));
			if (mem == 0)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");

				return 0;
			}

			VMCArray *a = new(mem) VMCArray(length, defVal, TI);
			return a;
		}
		VMCArray* VMCArray::Create(VMCTypeInfo *TI)
		{
			return VMCArray::Create(TI, as_size_t(0));
		}
		void VMCArray::CleanupTypeInfoCache(VMCTypeInfo* Type)
		{
			VMCArray::SCache* Cache = reinterpret_cast<VMCArray::SCache*>(Type->GetUserData(ARRAY_CACHE));
			if (Cache != nullptr)
			{
				Cache->~SCache();
				asFreeMem(Cache);
			}
		}
		bool VMCArray::TemplateCallback(VMCTypeInfo* T, bool& DontGarbageCollect)
		{
			int TypeId = T->GetSubTypeId();
			if (TypeId == asTYPEID_VOID)
				return false;

			if ((TypeId & asTYPEID_MASK_OBJECT) && !(TypeId & asTYPEID_OBJHANDLE))
			{
				VMCManager* Engine = T->GetEngine();
				VMCTypeInfo* Subtype = Engine->GetTypeInfoById(TypeId);
				asDWORD Flags = Subtype->GetFlags();

				if ((Flags & asOBJ_VALUE) && !(Flags & asOBJ_POD))
				{
					bool Found = false;
					for (as_size_t n = 0; n < Subtype->GetBehaviourCount(); n++)
					{
						asEBehaviours Beh;
						asIScriptFunction* Func = Subtype->GetBehaviourByIndex(n, &Beh);
						if (Beh != asBEHAVE_CONSTRUCT)
							continue;

						if (Func->GetParamCount() == 0)
						{
							Found = true;
							break;
						}
					}

					if (!Found)
					{
						Engine->WriteMessage("Array", 0, 0, asMSGTYPE_ERROR, "The subtype has no default constructor");
						return false;
					}
				}
				else if ((Flags & asOBJ_REF))
				{
					bool Found = false;
					if (!Engine->GetEngineProperty(asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE))
					{
						for (as_size_t n = 0; n < Subtype->GetFactoryCount(); n++)
						{
							asIScriptFunction* Func = Subtype->GetFactoryByIndex(n);
							if (Func->GetParamCount() == 0)
							{
								Found = true;
								break;
							}
						}
					}

					if (!Found)
					{
						Engine->WriteMessage("Array", 0, 0, asMSGTYPE_ERROR, "The subtype has no default factory");
						return false;
					}
				}

				if (!(Flags & asOBJ_GC))
					DontGarbageCollect = true;
			}
			else if (!(TypeId & asTYPEID_OBJHANDLE))
			{
				DontGarbageCollect = true;
			}
			else
			{
				assert(TypeId & asTYPEID_OBJHANDLE);
				VMCTypeInfo* Subtype = T->GetEngine()->GetTypeInfoById(TypeId);
				asDWORD Flags = Subtype->GetFlags();

				if (!(Flags & asOBJ_GC))
				{
					if ((Flags & asOBJ_SCRIPT_OBJECT))
					{
						if ((Flags & asOBJ_NOINHERIT))
							DontGarbageCollect = true;
					}
					else
						DontGarbageCollect = true;
				}
			}

			return true;
		}

		VMCMapKey::VMCMapKey()
		{
			ValueObj = 0;
			TypeId = 0;
		}
		VMCMapKey::VMCMapKey(VMCManager *engine, void *value, int typeId)
		{
			ValueObj = 0;
			TypeId = 0;
			Set(engine, value, typeId);
		}
		VMCMapKey::~VMCMapKey()
		{
			if (ValueObj && TypeId)
			{
				VMCContext *ctx = asGetActiveContext();
				if (!ctx)
				{
					assert((TypeId & asTYPEID_MASK_OBJECT) == 0);
				}
				else
					FreeValue(ctx->GetEngine());
			}
		}
		void VMCMapKey::FreeValue(VMCManager *engine)
		{
			if (TypeId & asTYPEID_MASK_OBJECT)
			{
				engine->ReleaseScriptObject(ValueObj, engine->GetTypeInfoById(TypeId));
				ValueObj = 0;
				TypeId = 0;
			}
		}
		void VMCMapKey::EnumReferences(VMCManager *inEngine)
		{
			if (ValueObj)
				inEngine->GCEnumCallback(ValueObj);

			if (TypeId)
				inEngine->GCEnumCallback(inEngine->GetTypeInfoById(TypeId));
		}
		void VMCMapKey::Set(VMCManager *engine, void *value, int typeId)
		{
			FreeValue(engine);
			TypeId = typeId;

			if (typeId & asTYPEID_OBJHANDLE)
			{
				ValueObj = *(void**)value;
				engine->AddRefScriptObject(ValueObj, engine->GetTypeInfoById(typeId));
			}
			else if (typeId & asTYPEID_MASK_OBJECT)
			{
				ValueObj = engine->CreateScriptObjectCopy(value, engine->GetTypeInfoById(typeId));
				if (ValueObj == 0)
				{
					VMCContext *ctx = asGetActiveContext();
					if (ctx)
						ctx->SetException("Cannot create copy of object");
				}
			}
			else
			{
				int size = engine->GetSizeOfPrimitiveType(typeId);
				memcpy(&ValueInt, value, size);
			}
		}
		void VMCMapKey::Set(VMCManager *engine, VMCMapKey &value)
		{
			if (value.TypeId & asTYPEID_OBJHANDLE)
				Set(engine, (void*)&value.ValueObj, value.TypeId);
			else if (value.TypeId & asTYPEID_MASK_OBJECT)
				Set(engine, (void*)value.ValueObj, value.TypeId);
			else
				Set(engine, (void*)&value.ValueInt, value.TypeId);
		}
		bool VMCMapKey::Get(VMCManager *engine, void *value, int typeId) const
		{
			if (typeId & asTYPEID_OBJHANDLE)
			{
				if ((TypeId & asTYPEID_MASK_OBJECT))
				{
					if ((TypeId & asTYPEID_HANDLETOCONST) && !(typeId & asTYPEID_HANDLETOCONST))
						return false;

					engine->RefCastObject(ValueObj, engine->GetTypeInfoById(TypeId), engine->GetTypeInfoById(typeId), reinterpret_cast<void**>(value));
					return true;
				}
			}
			else if (typeId & asTYPEID_MASK_OBJECT)
			{
				bool isCompatible = false;
				if ((TypeId & ~(asTYPEID_OBJHANDLE | asTYPEID_HANDLETOCONST)) == typeId && ValueObj != 0)
					isCompatible = true;

				if (isCompatible)
				{
					engine->AssignScriptObject(value, ValueObj, engine->GetTypeInfoById(typeId));
					return true;
				}
			}
			else
			{
				if (TypeId == typeId)
				{
					int size = engine->GetSizeOfPrimitiveType(typeId);
					memcpy(value, &ValueInt, size);
					return true;
				}

				if (typeId == asTYPEID_DOUBLE)
				{
					if (TypeId == asTYPEID_INT64)
					{
						*(double*)value = double(ValueInt);
					}
					else if (TypeId == asTYPEID_BOOL)
					{
						char localValue;
						memcpy(&localValue, &ValueInt, sizeof(char));
						*(double*)value = localValue ? 1.0 : 0.0;
					}
					else if (TypeId > asTYPEID_DOUBLE && (TypeId & asTYPEID_MASK_OBJECT) == 0)
					{
						int localValue;
						memcpy(&localValue, &ValueInt, sizeof(int));
						*(double*)value = double(localValue);
					}
					else
					{
						*(double*)value = 0;
						return false;
					}

					return true;
				}
				else if (typeId == asTYPEID_INT64)
				{
					if (TypeId == asTYPEID_DOUBLE)
					{
						*(as_int64_t*)value = as_int64_t(ValueFlt);
					}
					else if (TypeId == asTYPEID_BOOL)
					{
						char localValue;
						memcpy(&localValue, &ValueInt, sizeof(char));
						*(as_int64_t*)value = localValue ? 1 : 0;
					}
					else if (TypeId > asTYPEID_DOUBLE && (TypeId & asTYPEID_MASK_OBJECT) == 0)
					{
						int localValue;
						memcpy(&localValue, &ValueInt, sizeof(int));
						*(as_int64_t*)value = localValue;
					}
					else
					{
						*(as_int64_t*)value = 0;
						return false;
					}

					return true;
				}
				else if (typeId > asTYPEID_DOUBLE && (TypeId & asTYPEID_MASK_OBJECT) == 0)
				{
					if (TypeId == asTYPEID_DOUBLE)
					{
						*(int*)value = int(ValueFlt);
					}
					else if (TypeId == asTYPEID_INT64)
					{
						*(int*)value = int(ValueInt);
					}
					else if (TypeId == asTYPEID_BOOL)
					{
						char localValue;
						memcpy(&localValue, &ValueInt, sizeof(char));
						*(int*)value = localValue ? 1 : 0;
					}
					else if (TypeId > asTYPEID_DOUBLE && (TypeId & asTYPEID_MASK_OBJECT) == 0)
					{
						int localValue;
						memcpy(&localValue, &ValueInt, sizeof(int));
						*(int*)value = localValue;
					}
					else
					{
						*(int*)value = 0;
						return false;
					}

					return true;
				}
				else if (typeId == asTYPEID_BOOL)
				{
					if (TypeId & asTYPEID_OBJHANDLE)
					{
						*(bool*)value = ValueObj ? true : false;
					}
					else if (TypeId & asTYPEID_MASK_OBJECT)
					{
						*(bool*)value = true;
					}
					else
					{
						as_uint64_t zero = 0;
						int size = engine->GetSizeOfPrimitiveType(TypeId);
						*(bool*)value = memcmp(&ValueInt, &zero, size) == 0 ? false : true;
					}

					return true;
				}
			}

			return false;
		}
		const void * VMCMapKey::GetAddressOfValue() const
		{
			if ((TypeId & asTYPEID_MASK_OBJECT) && !(TypeId & asTYPEID_OBJHANDLE))
				return ValueObj;

			return reinterpret_cast<const void*>(&ValueObj);
		}
		int VMCMapKey::GetTypeId() const
		{
			return TypeId;
		}

		VMCMap::Iterator::Iterator(const VMCMap &Dict, Map::const_iterator it) : It(it), Dict(Dict)
		{
		}
		void VMCMap::Iterator::operator++()
		{
			++It;
		}
		void VMCMap::Iterator::operator++(int)
		{
			++It;
		}
		VMCMap::Iterator &VMCMap::Iterator::operator*()
		{
			return *this;
		}
		bool VMCMap::Iterator::operator==(const Iterator &other) const
		{
			return It == other.It;
		}
		bool VMCMap::Iterator::operator!=(const Iterator &other) const
		{
			return It != other.It;
		}
		const std::string &VMCMap::Iterator::GetKey() const
		{
			return It->first;
		}
		int VMCMap::Iterator::GetTypeId() const
		{
			return It->second.TypeId;
		}
		bool VMCMap::Iterator::GetValue(void *value, int typeId) const
		{
			return It->second.Get(Dict.Engine, value, typeId);
		}
		const void *VMCMap::Iterator::GetAddressOfValue() const
		{
			return It->second.GetAddressOfValue();
		}

		VMCMap::VMCMap(VMCManager *engine)
		{
			Init(engine);
		}
		VMCMap::VMCMap(unsigned char *buffer)
		{
			VMCContext *ctx = asGetActiveContext();
			Init(ctx->GetEngine());

			VMCMap::SCache& cache = *reinterpret_cast<VMCMap::SCache*>(Engine->GetUserData(MAP_CACHE));
			bool keyAsRef = cache.KeyType->GetFlags() & asOBJ_REF ? true : false;

			as_size_t length = *(as_size_t*)buffer;
			buffer += 4;

			while (length--)
			{
				if (asPWORD(buffer) & 0x3)
					buffer += 4 - (asPWORD(buffer) & 0x3);

				std::string name;
				if (keyAsRef)
				{
					name = **(std::string**)buffer;
					buffer += sizeof(std::string*);
				}
				else
				{
					name = *(std::string*)buffer;
					buffer += sizeof(std::string);
				}

				int typeId = *(int*)buffer;
				buffer += sizeof(int);

				void *ref = (void*)buffer;
				if (typeId >= asTYPEID_INT8 && typeId <= asTYPEID_DOUBLE)
				{
					as_int64_t i64;
					double d;

					switch (typeId)
					{
						case asTYPEID_INT8:
							i64 = *(char*)ref;
							break;
						case asTYPEID_INT16:
							i64 = *(short*)ref;
							break;
						case asTYPEID_INT32:
							i64 = *(int*)ref;
							break;
						case asTYPEID_UINT8:
							i64 = *(unsigned char*)ref;
							break;
						case asTYPEID_UINT16:
							i64 = *(unsigned short*)ref;
							break;
						case asTYPEID_UINT32:
							i64 = *(unsigned int*)ref;
							break;
						case asTYPEID_INT64:
						case asTYPEID_UINT64:
							i64 = *(as_int64_t*)ref;
							break;
						case asTYPEID_FLOAT:
							d = *(float*)ref;
							break;
						case asTYPEID_DOUBLE:
							d = *(double*)ref;
							break;
					}

					if (typeId >= asTYPEID_FLOAT)
						Set(name, &d, asTYPEID_DOUBLE);
					else
						Set(name, &i64, asTYPEID_INT64);
				}
				else
				{
					if ((typeId & asTYPEID_MASK_OBJECT) && !(typeId & asTYPEID_OBJHANDLE) && (Engine->GetTypeInfoById(typeId)->GetFlags() & asOBJ_REF))
						ref = *(void**)ref;

					Set(name, ref, VMManager::Get(Engine)->IsNullable(typeId) ? 0 : typeId);
				}

				if (typeId & asTYPEID_MASK_OBJECT)
				{
					VMCTypeInfo *TI = Engine->GetTypeInfoById(typeId);
					if (TI->GetFlags() & asOBJ_VALUE)
						buffer += TI->GetSize();
					else
						buffer += sizeof(void*);
				}
				else if (typeId == 0)
				{
					TH_WARN("[memerr] use nullptr instead of null for initialization lists");
					buffer += sizeof(void*);
				}
				else
					buffer += Engine->GetSizeOfPrimitiveType(typeId);
			}
		}
		VMCMap::VMCMap(const VMCMap& other)
		{
			Init(other.Engine);

			Map::const_iterator it;
			for (it = other.Dict.begin(); it != other.Dict.end(); ++it)
			{
				auto& Key = it->second;
				if (Key.TypeId & asTYPEID_OBJHANDLE)
					Set(it->first, (void*)&Key.ValueObj, Key.TypeId);
				else if (Key.TypeId & asTYPEID_MASK_OBJECT)
					Set(it->first, (void*)Key.ValueObj, Key.TypeId);
				else
					Set(it->first, (void*)&Key.ValueInt, Key.TypeId);
			}
		}
		VMCMap::~VMCMap()
		{
			DeleteAll();
		}
		void VMCMap::AddRef() const
		{
			GCFlag = false;
			asAtomicInc(RefCount);
		}
		void VMCMap::Release() const
		{
			GCFlag = false;
			if (asAtomicDec(RefCount) == 0)
			{
				this->~VMCMap();
				asFreeMem(const_cast<VMCMap*>(this));
			}
		}
		int VMCMap::GetRefCount()
		{
			return RefCount;
		}
		void VMCMap::SetGCFlag()
		{
			GCFlag = true;
		}
		bool VMCMap::GetGCFlag()
		{
			return GCFlag;
		}
		void VMCMap::EnumReferences(VMCManager *inEngine)
		{
			Map::iterator it;
			for (it = Dict.begin(); it != Dict.end(); ++it)
			{
				auto& Key = it->second;
				if (Key.TypeId & asTYPEID_MASK_OBJECT)
				{
					VMCTypeInfo *subType = Engine->GetTypeInfoById(Key.TypeId);
					if ((subType->GetFlags() & asOBJ_VALUE) && (subType->GetFlags() & asOBJ_GC))
						Engine->ForwardGCEnumReferences(Key.ValueObj, subType);
					else
						inEngine->GCEnumCallback(Key.ValueObj);
				}
			}
		}
		void VMCMap::ReleaseAllReferences(VMCManager*)
		{
			DeleteAll();
		}
		VMCMap &VMCMap::operator =(const VMCMap &other)
		{
			DeleteAll();

			Map::const_iterator it;
			for (it = other.Dict.begin(); it != other.Dict.end(); ++it)
			{
				auto& Key = it->second;
				if (Key.TypeId & asTYPEID_OBJHANDLE)
					Set(it->first, (void*)&Key.ValueObj, Key.TypeId);
				else if (Key.TypeId & asTYPEID_MASK_OBJECT)
					Set(it->first, (void*)Key.ValueObj, Key.TypeId);
				else
					Set(it->first, (void*)&Key.ValueInt, Key.TypeId);
			}

			return *this;
		}
		VMCMapKey *VMCMap::operator[](const std::string &key)
		{
			return &Dict[key];
		}
		const VMCMapKey *VMCMap::operator[](const std::string &key) const
		{
			Map::const_iterator it;
			it = Dict.find(key);
			if (it != Dict.end())
				return &it->second;

			VMCContext *ctx = asGetActiveContext();
			if (ctx)
				ctx->SetException("Invalid access to non-existing value");

			return 0;
		}
		void VMCMap::Set(const std::string &key, void *value, int typeId)
		{
			Map::iterator it;
			it = Dict.find(key);
			if (it == Dict.end())
				it = Dict.insert(Map::value_type(key, VMCMapKey())).first;

			it->second.Set(Engine, value, typeId);
		}
		bool VMCMap::Get(const std::string &key, void *value, int typeId) const
		{
			Map::const_iterator it;
			it = Dict.find(key);
			if (it != Dict.end())
				return it->second.Get(Engine, value, typeId);

			return false;
		}
		bool VMCMap::GetIndex(size_t Index, std::string* Key, void** Value, int* TypeId) const
		{
			if (Index >= Dict.size())
				return false;

			auto It = Begin();
			size_t Offset = 0;

			while (Offset != Index)
			{
				++Offset;
				++It;
			}

			if (Key != nullptr)
				*Key = It.GetKey();

			if (Value != nullptr)
				*Value = (void*)It.GetAddressOfValue();

			if (TypeId != nullptr)
				*TypeId = It.GetTypeId();

			return true;
		}
		int VMCMap::GetTypeId(const std::string &key) const
		{
			Map::const_iterator it;
			it = Dict.find(key);
			if (it != Dict.end())
				return it->second.TypeId;

			return -1;
		}
		bool VMCMap::Exists(const std::string &key) const
		{
			Map::const_iterator it;
			it = Dict.find(key);
			if (it != Dict.end())
				return true;

			return false;
		}
		bool VMCMap::IsEmpty() const
		{
			if (Dict.size() == 0)
				return true;

			return false;
		}
		as_size_t VMCMap::GetSize() const
		{
			return as_size_t(Dict.size());
		}
		bool VMCMap::Delete(const std::string &key)
		{
			Map::iterator it;
			it = Dict.find(key);
			if (it != Dict.end())
			{
				it->second.FreeValue(Engine);
				Dict.erase(it);
				return true;
			}

			return false;
		}
		void VMCMap::DeleteAll()
		{
			Map::iterator it;
			for (it = Dict.begin(); it != Dict.end(); ++it)
				it->second.FreeValue(Engine);

			Dict.clear();
		}
		VMCArray* VMCMap::GetKeys() const
		{
			VMCMap::SCache *cache = reinterpret_cast<VMCMap::SCache*>(Engine->GetUserData(MAP_CACHE));
			VMCTypeInfo *TI = cache->ArrayType;

			VMCArray *Array = VMCArray::Create(TI, as_size_t(Dict.size()));
			Map::const_iterator it;
			long current = -1;

			for (it = Dict.begin(); it != Dict.end(); ++it)
			{
				current++;
				*(std::string*)Array->At(current) = it->first;
			}

			return Array;
		}
		VMCMap::Iterator VMCMap::Begin() const
		{
			return Iterator(*this, Dict.begin());
		}
		VMCMap::Iterator VMCMap::End() const
		{
			return Iterator(*this, Dict.end());
		}
		VMCMap::Iterator VMCMap::Find(const std::string &key) const
		{
			return Iterator(*this, Dict.find(key));
		}
		VMCMap *VMCMap::Create(VMCManager *engine)
		{
			VMCMap *obj = (VMCMap*)asAllocMem(sizeof(VMCMap));
			new(obj) VMCMap(engine);
			return obj;
		}
		VMCMap *VMCMap::Create(unsigned char *buffer)
		{
			VMCMap *obj = (VMCMap*)asAllocMem(sizeof(VMCMap));
			new(obj) VMCMap(buffer);
			return obj;
		}
		void VMCMap::Init(VMCManager *e)
		{
			RefCount = 1;
			GCFlag = false;
			Engine = e;

			VMCMap::SCache* Cache = reinterpret_cast<VMCMap::SCache*>(Engine->GetUserData(MAP_CACHE));
			Engine->NotifyGarbageCollectorOfNewObject(this, Cache->DictType);
		}
		void VMCMap::Cleanup(VMCManager *engine)
		{
			VMCMap::SCache* Cache = reinterpret_cast<VMCMap::SCache*>(engine->GetUserData(MAP_CACHE));
			TH_DELETE(SCache, Cache);
		}
		void VMCMap::Setup(VMCManager *engine)
		{
			VMCMap::SCache* cache = reinterpret_cast<VMCMap::SCache*>(engine->GetUserData(MAP_CACHE));
			if (cache == 0)
			{
				cache = TH_NEW(VMCMap::SCache);
				engine->SetUserData(cache, MAP_CACHE);
				engine->SetEngineUserDataCleanupCallback(Cleanup, MAP_CACHE);

				cache->DictType = engine->GetTypeInfoByName("Map");
				cache->ArrayType = engine->GetTypeInfoByDecl("Array<String>");
				cache->KeyType = engine->GetTypeInfoByDecl("String");
			}
		}
		void VMCMap::Factory(VMCGeneric *gen)
		{
			*(VMCMap**)gen->GetAddressOfReturnLocation() = VMCMap::Create(gen->GetEngine());
		}
		void VMCMap::ListFactory(VMCGeneric *gen)
		{
			unsigned char *buffer = (unsigned char*)gen->GetArgAddress(0);
			*(VMCMap**)gen->GetAddressOfReturnLocation() = VMCMap::Create(buffer);
		}
		void VMCMap::KeyConstruct(void *mem)
		{
			new(mem) VMCMapKey();
		}
		void VMCMap::KeyDestruct(VMCMapKey *obj)
		{
			VMCContext *ctx = asGetActiveContext();
			if (ctx)
			{
				VMCManager *engine = ctx->GetEngine();
				obj->FreeValue(engine);
			}
			obj->~VMCMapKey();
		}
		VMCMapKey& VMCMap::KeyopAssign(void *ref, int typeId, VMCMapKey *obj)
		{
			VMCContext *ctx = asGetActiveContext();
			if (ctx)
			{
				VMCManager *engine = ctx->GetEngine();
				obj->Set(engine, ref, typeId);
			}
			return *obj;
		}
		VMCMapKey& VMCMap::KeyopAssign(const VMCMapKey &other, VMCMapKey *obj)
		{
			VMCContext *ctx = asGetActiveContext();
			if (ctx)
			{
				VMCManager *engine = ctx->GetEngine();
				obj->Set(engine, const_cast<VMCMapKey&>(other));
			}

			return *obj;
		}
		VMCMapKey& VMCMap::KeyopAssign(double val, VMCMapKey *obj)
		{
			return KeyopAssign(&val, asTYPEID_DOUBLE, obj);
		}
		VMCMapKey& VMCMap::KeyopAssign(as_int64_t val, VMCMapKey *obj)
		{
			return VMCMap::KeyopAssign(&val, asTYPEID_INT64, obj);
		}
		void VMCMap::KeyopCast(void *ref, int typeId, VMCMapKey *obj)
		{
			VMCContext *ctx = asGetActiveContext();
			if (ctx)
			{
				VMCManager *engine = ctx->GetEngine();
				obj->Get(engine, ref, typeId);
			}
		}
		as_int64_t VMCMap::KeyopConvInt(VMCMapKey *obj)
		{
			as_int64_t value;
			KeyopCast(&value, asTYPEID_INT64, obj);
			return value;
		}
		double VMCMap::KeyopConvDouble(VMCMapKey *obj)
		{
			double value;
			KeyopCast(&value, asTYPEID_DOUBLE, obj);
			return value;
		}

		VMCGrid::VMCGrid(VMCTypeInfo *TI, void *buf)
		{
			RefCount = 1;
			GCFlag = false;
			ObjType = TI;
			ObjType->AddRef();
			Buffer = 0;
			SubTypeId = ObjType->GetSubTypeId();

			VMCManager *engine = TI->GetEngine();
			if (SubTypeId & asTYPEID_MASK_OBJECT)
				ElementSize = sizeof(asPWORD);
			else
				ElementSize = engine->GetSizeOfPrimitiveType(SubTypeId);

			as_size_t Height = *(as_size_t*)buf;
			as_size_t Width = Height ? *(as_size_t*)((char*)(buf)+4) : 0;

			if (!CheckMaxSize(Width, Height))
				return;

			buf = (as_size_t*)(buf)+1;
			if ((TI->GetSubTypeId() & asTYPEID_MASK_OBJECT) == 0)
			{
				CreateBuffer(&Buffer, Width, Height);
				for (as_size_t y = 0; y < Height; y++)
				{
					buf = (as_size_t*)(buf)+1;
					if (Width > 0)
						memcpy(At(0, y), buf, (size_t)Width * (size_t)ElementSize);

					buf = (char*)(buf)+Width * (as_size_t)ElementSize;
					if (asPWORD(buf) & 0x3)
						buf = (char*)(buf)+4 - (asPWORD(buf) & 0x3);
				}
			}
			else if (TI->GetSubTypeId() & asTYPEID_OBJHANDLE)
			{
				CreateBuffer(&Buffer, Width, Height);
				for (as_size_t y = 0; y < Height; y++)
				{
					buf = (as_size_t*)(buf)+1;
					if (Width > 0)
						memcpy(At(0, y), buf, (size_t)Width * (size_t)ElementSize);

					memset(buf, 0, (size_t)Width * (size_t)ElementSize);
					buf = (char*)(buf)+Width * (as_size_t)ElementSize;

					if (asPWORD(buf) & 0x3)
						buf = (char*)(buf)+4 - (asPWORD(buf) & 0x3);
				}
			}
			else if (TI->GetSubType()->GetFlags() & asOBJ_REF)
			{
				SubTypeId |= asTYPEID_OBJHANDLE;
				CreateBuffer(&Buffer, Width, Height);
				SubTypeId &= ~asTYPEID_OBJHANDLE;

				for (as_size_t y = 0; y < Height; y++)
				{
					buf = (as_size_t*)(buf)+1;
					if (Width > 0)
						memcpy(At(0, y), buf, (size_t)Width * (size_t)ElementSize);

					memset(buf, 0, (size_t)Width * (size_t)ElementSize);
					buf = (char*)(buf)+Width * (as_size_t)ElementSize;

					if (asPWORD(buf) & 0x3)
						buf = (char*)(buf)+4 - (asPWORD(buf) & 0x3);
				}
			}
			else
			{
				CreateBuffer(&Buffer, Width, Height);

				VMCTypeInfo *subType = TI->GetSubType();
				as_size_t subTypeSize = subType->GetSize();
				for (as_size_t y = 0; y < Height; y++)
				{
					buf = (as_size_t*)(buf)+1;
					for (as_size_t x = 0; x < Width; x++)
					{
						void *obj = At(x, y);
						unsigned char *srcObj = (unsigned char*)(buf)+x * subTypeSize;
						engine->AssignScriptObject(obj, srcObj, subType);
					}

					buf = (char*)(buf)+Width * subTypeSize;
					if (asPWORD(buf) & 0x3)
						buf = (char*)(buf)+4 - (asPWORD(buf) & 0x3);
				}
			}

			if (ObjType->GetFlags() & asOBJ_GC)
				ObjType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, ObjType);
		}
		VMCGrid::VMCGrid(as_size_t Width, as_size_t Height, VMCTypeInfo *TI)
		{
			RefCount = 1;
			GCFlag = false;
			ObjType = TI;
			ObjType->AddRef();
			Buffer = 0;
			SubTypeId = ObjType->GetSubTypeId();

			if (SubTypeId & asTYPEID_MASK_OBJECT)
				ElementSize = sizeof(asPWORD);
			else
				ElementSize = ObjType->GetEngine()->GetSizeOfPrimitiveType(SubTypeId);

			if (!CheckMaxSize(Width, Height))
				return;

			CreateBuffer(&Buffer, Width, Height);
			if (ObjType->GetFlags() & asOBJ_GC)
				ObjType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, ObjType);
		}
		void VMCGrid::Resize(as_size_t Width, as_size_t Height)
		{
			if (!CheckMaxSize(Width, Height))
				return;

			SBuffer *tmpBuffer = 0;
			CreateBuffer(&tmpBuffer, Width, Height);
			if (tmpBuffer == 0)
				return;

			if (Buffer)
			{
				as_size_t w = Width > Buffer->Width ? Buffer->Width : Width;
				as_size_t h = Height > Buffer->Height ? Buffer->Height : Height;
				for (as_size_t y = 0; y < h; y++)
				{
					for (as_size_t x = 0; x < w; x++)
						SetValue(tmpBuffer, x, y, At(Buffer, x, y));
				}

				DeleteBuffer(Buffer);
			}

			Buffer = tmpBuffer;
		}
		VMCGrid::VMCGrid(as_size_t Width, as_size_t Height, void *defVal, VMCTypeInfo *TI)
		{
			RefCount = 1;
			GCFlag = false;
			ObjType = TI;
			ObjType->AddRef();
			Buffer = 0;
			SubTypeId = ObjType->GetSubTypeId();

			if (SubTypeId & asTYPEID_MASK_OBJECT)
				ElementSize = sizeof(asPWORD);
			else
				ElementSize = ObjType->GetEngine()->GetSizeOfPrimitiveType(SubTypeId);

			if (!CheckMaxSize(Width, Height))
				return;

			CreateBuffer(&Buffer, Width, Height);
			if (ObjType->GetFlags() & asOBJ_GC)
				ObjType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, ObjType);

			for (as_size_t y = 0; y < GetHeight(); y++)
			{
				for (as_size_t x = 0; x < GetWidth(); x++)
					SetValue(x, y, defVal);
			}
		}
		void VMCGrid::SetValue(as_size_t x, as_size_t y, void *value)
		{
			SetValue(Buffer, x, y, value);
		}
		void VMCGrid::SetValue(SBuffer *buf, as_size_t x, as_size_t y, void *value)
		{
			void *ptr = At(buf, x, y);
			if (ptr == 0)
				return;

			if ((SubTypeId & ~asTYPEID_MASK_SEQNBR) && !(SubTypeId & asTYPEID_OBJHANDLE))
				ObjType->GetEngine()->AssignScriptObject(ptr, value, ObjType->GetSubType());
			else if (SubTypeId & asTYPEID_OBJHANDLE)
			{
				void *tmp = *(void**)ptr;
				*(void**)ptr = *(void**)value;
				ObjType->GetEngine()->AddRefScriptObject(*(void**)value, ObjType->GetSubType());
				if (tmp)
					ObjType->GetEngine()->ReleaseScriptObject(tmp, ObjType->GetSubType());
			}
			else if (SubTypeId == asTYPEID_BOOL ||
				SubTypeId == asTYPEID_INT8 ||
				SubTypeId == asTYPEID_UINT8)
				*(char*)ptr = *(char*)value;
			else if (SubTypeId == asTYPEID_INT16 ||
				SubTypeId == asTYPEID_UINT16)
				*(short*)ptr = *(short*)value;
			else if (SubTypeId == asTYPEID_INT32 ||
				SubTypeId == asTYPEID_UINT32 ||
				SubTypeId == asTYPEID_FLOAT ||
				SubTypeId > asTYPEID_DOUBLE)
				*(int*)ptr = *(int*)value;
			else if (SubTypeId == asTYPEID_INT64 ||
				SubTypeId == asTYPEID_UINT64 ||
				SubTypeId == asTYPEID_DOUBLE)
				*(double*)ptr = *(double*)value;
		}
		VMCGrid::~VMCGrid()
		{
			if (Buffer)
			{
				DeleteBuffer(Buffer);
				Buffer = 0;
			}

			if (ObjType)
				ObjType->Release();
		}
		as_size_t VMCGrid::GetWidth() const
		{
			if (Buffer)
				return Buffer->Width;

			return 0;
		}
		as_size_t VMCGrid::GetHeight() const
		{
			if (Buffer)
				return Buffer->Height;

			return 0;
		}
		bool VMCGrid::CheckMaxSize(as_size_t Width, as_size_t Height)
		{
			as_size_t maxSize = 0xFFFFFFFFul - sizeof(SBuffer) + 1;
			if (ElementSize > 0)
				maxSize /= (as_size_t)ElementSize;

			as_uint64_t numElements = (as_uint64_t)Width * (as_uint64_t)Height;
			if ((numElements >> 32) || numElements > (as_uint64_t)maxSize)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Too large grid size");

				return false;
			}
			return true;
		}
		VMCTypeInfo *VMCGrid::GetGridObjectType() const
		{
			return ObjType;
		}
		int VMCGrid::GetGridTypeId() const
		{
			return ObjType->GetTypeId();
		}
		int VMCGrid::GetElementTypeId() const
		{
			return SubTypeId;
		}
		void *VMCGrid::At(as_size_t x, as_size_t y)
		{
			return At(Buffer, x, y);
		}
		void *VMCGrid::At(SBuffer *buf, as_size_t x, as_size_t y)
		{
			if (buf == 0 || x >= buf->Width || y >= buf->Height)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Index out of bounds");
				return 0;
			}

			as_size_t index = x + y * buf->Width;
			if ((SubTypeId & asTYPEID_MASK_OBJECT) && !(SubTypeId & asTYPEID_OBJHANDLE))
				return *(void**)(buf->Data + (as_size_t)ElementSize * index);
			else
				return buf->Data + (as_size_t)ElementSize * index;
		}
		const void *VMCGrid::At(as_size_t x, as_size_t y) const
		{
			return const_cast<VMCGrid*>(this)->At(const_cast<SBuffer*>(Buffer), x, y);
		}
		void VMCGrid::CreateBuffer(SBuffer **buf, as_size_t w, as_size_t h)
		{
			as_size_t numElements = w * h;
			*buf = reinterpret_cast<SBuffer*>(asAllocMem(sizeof(SBuffer) - 1 + (size_t)ElementSize * (size_t)numElements));

			if (*buf)
			{
				(*buf)->Width = w;
				(*buf)->Height = h;
				Construct(*buf);
			}
			else
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");
			}
		}
		void VMCGrid::DeleteBuffer(SBuffer *buf)
		{
			assert(buf);
			Destruct(buf);
			asFreeMem(buf);
		}
		void VMCGrid::Construct(SBuffer *buf)
		{
			assert(buf);
			if (SubTypeId & asTYPEID_OBJHANDLE)
			{
				void *d = (void*)(buf->Data);
				memset(d, 0, (buf->Width*buf->Height) * sizeof(void*));
			}
			else if (SubTypeId & asTYPEID_MASK_OBJECT)
			{
				void **max = (void**)(buf->Data + (buf->Width*buf->Height) * sizeof(void*));
				void **d = (void**)(buf->Data);

				VMCManager *engine = ObjType->GetEngine();
				VMCTypeInfo *subType = ObjType->GetSubType();

				for (; d < max; d++)
				{
					*d = (void*)engine->CreateScriptObject(subType);
					if (*d == 0)
					{
						memset(d, 0, sizeof(void*)*(max - d));
						return;
					}
				}
			}
		}
		void VMCGrid::Destruct(SBuffer *buf)
		{
			assert(buf);
			if (SubTypeId & asTYPEID_MASK_OBJECT)
			{
				VMCManager *engine = ObjType->GetEngine();
				void **max = (void**)(buf->Data + (buf->Width*buf->Height) * sizeof(void*));
				void **d = (void**)(buf->Data);

				for (; d < max; d++)
				{
					if (*d)
						engine->ReleaseScriptObject(*d, ObjType->GetSubType());
				}
			}
		}
		void VMCGrid::EnumReferences(VMCManager *engine)
		{
			if (Buffer == 0)
				return;

			if (SubTypeId & asTYPEID_MASK_OBJECT)
			{
				as_size_t numElements = Buffer->Width * Buffer->Height;
				void **d = (void**)Buffer->Data;
				VMCTypeInfo *subType = engine->GetTypeInfoById(SubTypeId);

				if ((subType->GetFlags() & asOBJ_REF))
				{
					for (as_size_t n = 0; n < numElements; n++)
					{
						if (d[n])
							engine->GCEnumCallback(d[n]);
					}
				}
				else if ((subType->GetFlags() & asOBJ_VALUE) && (subType->GetFlags() & asOBJ_GC))
				{
					for (as_size_t n = 0; n < numElements; n++)
					{
						if (d[n])
							engine->ForwardGCEnumReferences(d[n], subType);
					}
				}
			}
		}
		void VMCGrid::ReleaseAllHandles(VMCManager*)
		{
			if (Buffer == 0)
				return;

			DeleteBuffer(Buffer);
			Buffer = 0;
		}
		void VMCGrid::AddRef() const
		{
			GCFlag = false;
			asAtomicInc(RefCount);
		}
		void VMCGrid::Release() const
		{
			GCFlag = false;
			if (asAtomicDec(RefCount) == 0)
			{
				this->~VMCGrid();
				asFreeMem(const_cast<VMCGrid*>(this));
			}
		}
		int VMCGrid::GetRefCount()
		{
			return RefCount;
		}
		void VMCGrid::SetFlag()
		{
			GCFlag = true;
		}
		bool VMCGrid::GetFlag()
		{
			return GCFlag;
		}
		VMCGrid *VMCGrid::Create(VMCTypeInfo *TI)
		{
			return VMCGrid::Create(TI, 0, 0);
		}
		VMCGrid *VMCGrid::Create(VMCTypeInfo *TI, as_size_t w, as_size_t h)
		{
			void *mem = asAllocMem(sizeof(VMCGrid));
			if (mem == 0)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");

				return 0;
			}

			VMCGrid *a = new(mem) VMCGrid(w, h, TI);
			return a;
		}
		VMCGrid *VMCGrid::Create(VMCTypeInfo *TI, void *initList)
		{
			void *mem = asAllocMem(sizeof(VMCGrid));
			if (mem == 0)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");

				return 0;
			}

			VMCGrid *a = new(mem) VMCGrid(TI, initList);
			return a;
		}
		VMCGrid *VMCGrid::Create(VMCTypeInfo *TI, as_size_t w, as_size_t h, void *defVal)
		{
			void *mem = asAllocMem(sizeof(VMCGrid));
			if (mem == 0)
			{
				VMCContext *ctx = asGetActiveContext();
				if (ctx)
					ctx->SetException("Out of memory");

				return 0;
			}

			VMCGrid *a = new(mem) VMCGrid(w, h, defVal, TI);
			return a;
		}
		bool VMCGrid::TemplateCallback(VMCTypeInfo *TI, bool &DontGarbageCollect)
		{
			int typeId = TI->GetSubTypeId();
			if (typeId == asTYPEID_VOID)
				return false;

			VMCManager* Engine = TI->GetEngine();
			if ((typeId & asTYPEID_MASK_OBJECT) && !(typeId & asTYPEID_OBJHANDLE))
			{
				VMCTypeInfo *subtype = Engine->GetTypeInfoById(typeId);
				asDWORD flags = subtype->GetFlags();

				if ((flags & asOBJ_VALUE) && !(flags & asOBJ_POD))
				{
					bool found = false;
					for (as_size_t n = 0; n < subtype->GetBehaviourCount(); n++)
					{
						asEBehaviours beh;
						asIScriptFunction *func = subtype->GetBehaviourByIndex(n, &beh);
						if (beh != asBEHAVE_CONSTRUCT)
							continue;

						if (func->GetParamCount() == 0)
						{
							found = true;
							break;
						}
					}

					if (!found)
					{
						Engine->WriteMessage("Array", 0, 0, asMSGTYPE_ERROR, "The subtype has no default constructor");
						return false;
					}
				}
				else if ((flags & asOBJ_REF))
				{
					bool found = false;
					if (!Engine->GetEngineProperty(asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE))
					{
						for (as_size_t n = 0; n < subtype->GetFactoryCount(); n++)
						{
							asIScriptFunction *func = subtype->GetFactoryByIndex(n);
							if (func->GetParamCount() == 0)
							{
								found = true;
								break;
							}
						}
					}

					if (!found)
					{
						Engine->WriteMessage("Array", 0, 0, asMSGTYPE_ERROR, "The subtype has no default factory");
						return false;
					}
				}

				if (!(flags & asOBJ_GC))
					DontGarbageCollect = true;
			}
			else if (!(typeId & asTYPEID_OBJHANDLE))
			{
				DontGarbageCollect = true;
			}
			else
			{
				assert(typeId & asTYPEID_OBJHANDLE);
				VMCTypeInfo *subtype = Engine->GetTypeInfoById(typeId);
				asDWORD flags = subtype->GetFlags();

				if (!(flags & asOBJ_GC))
				{
					if ((flags & asOBJ_SCRIPT_OBJECT))
					{
						if ((flags & asOBJ_NOINHERIT))
							DontGarbageCollect = true;
					}
					else
						DontGarbageCollect = true;
				}
			}

			return true;
		}

		VMCRef::VMCRef()
		{
			Ref = 0;
			Type = 0;
		}
		VMCRef::VMCRef(const VMCRef &other)
		{
			Ref = other.Ref;
			Type = other.Type;

			AddRefHandle();
		}
		VMCRef::VMCRef(void *ref, VMCTypeInfo *type)
		{
			Ref = ref;
			Type = type;

			AddRefHandle();
		}
		VMCRef::VMCRef(void *ref, int typeId)
		{
			Ref = 0;
			Type = 0;

			Assign(ref, typeId);
		}
		VMCRef::~VMCRef()
		{
			ReleaseHandle();
		}
		void VMCRef::ReleaseHandle()
		{
			if (Ref && Type)
			{
				VMCManager *engine = Type->GetEngine();
				engine->ReleaseScriptObject(Ref, Type);
				engine->Release();

				Ref = 0;
				Type = 0;
			}
		}
		void VMCRef::AddRefHandle()
		{
			if (Ref && Type)
			{
				VMCManager *engine = Type->GetEngine();
				engine->AddRefScriptObject(Ref, Type);
				engine->AddRef();
			}
		}
		VMCRef &VMCRef::operator =(const VMCRef &other)
		{
			Set(other.Ref, other.Type);
			return *this;
		}
		void VMCRef::Set(void *ref, VMCTypeInfo *type)
		{
			if (Ref == ref)
				return;

			ReleaseHandle();
			Ref = ref;
			Type = type;
			AddRefHandle();
		}
		void *VMCRef::GetRef()
		{
			return Ref;
		}
		VMCTypeInfo *VMCRef::GetType() const
		{
			return Type;
		}
		int VMCRef::GetTypeId() const
		{
			if (Type == 0) return 0;

			return Type->GetTypeId() | asTYPEID_OBJHANDLE;
		}
		VMCRef &VMCRef::Assign(void *ref, int typeId)
		{
			if (typeId == 0)
			{
				Set(0, 0);
				return *this;
			}

			if (typeId & asTYPEID_OBJHANDLE)
			{
				ref = *(void**)ref;
				typeId &= ~asTYPEID_OBJHANDLE;
			}

			VMCContext* ctx = asGetActiveContext();
			VMCManager* engine = ctx->GetEngine();
			VMCTypeInfo* type = engine->GetTypeInfoById(typeId);

			if (type && strcmp(type->GetName(), "ref") == 0)
			{
				VMCRef *r = (VMCRef*)ref;
				ref = r->Ref;
				type = r->Type;
			}

			Set(ref, type);
			return *this;
		}
		bool VMCRef::operator==(const VMCRef &o) const
		{
			if (Ref == o.Ref && Type == o.Type)
				return true;

			return false;
		}
		bool VMCRef::operator!=(const VMCRef &o) const
		{
			return !(*this == o);
		}
		bool VMCRef::Equals(void *ref, int typeId) const
		{
			if (typeId == 0)
				ref = 0;

			if (typeId & asTYPEID_OBJHANDLE)
			{
				void** Sub = (void**)ref;
				if (!Sub)
					return false;

				ref = *Sub;
				typeId &= ~asTYPEID_OBJHANDLE;
			}

			if (ref == Ref)
				return true;

			return false;
		}
		void VMCRef::Cast(void **outRef, int typeId)
		{
			if (Type == 0)
			{
				*outRef = 0;
				return;
			}

			assert(typeId & asTYPEID_OBJHANDLE);
			typeId &= ~asTYPEID_OBJHANDLE;

			VMCManager* engine = Type->GetEngine();
			VMCTypeInfo* type = engine->GetTypeInfoById(typeId);
			*outRef = 0;

			engine->RefCastObject(Ref, Type, type, outRef);
		}
		void VMCRef::EnumReferences(VMCManager *inEngine)
		{
			if (Ref)
				inEngine->GCEnumCallback(Ref);

			if (Type)
				inEngine->GCEnumCallback(Type);
		}
		void VMCRef::ReleaseReferences(VMCManager *inEngine)
		{
			Set(0, 0);
		}
		void VMCRef::Construct(VMCRef *self)
		{
			new(self) VMCRef();
		}
		void VMCRef::Construct(VMCRef *self, const VMCRef &o)
		{
			new(self) VMCRef(o);
		}
		void VMCRef::Construct(VMCRef *self, void *ref, int typeId)
		{
			new(self) VMCRef(ref, typeId);
		}
		void VMCRef::Destruct(VMCRef *self)
		{
			self->~VMCRef();
		}

		VMCWeakRef::VMCWeakRef(VMCTypeInfo *type)
		{
			Ref = 0;
			Type = type;
			Type->AddRef();
			WeakRefFlag = 0;
		}
		VMCWeakRef::VMCWeakRef(const VMCWeakRef &other)
		{
			Ref = other.Ref;
			Type = other.Type;
			Type->AddRef();
			WeakRefFlag = other.WeakRefFlag;
			if (WeakRefFlag)
				WeakRefFlag->AddRef();
		}
		VMCWeakRef::VMCWeakRef(void *ref, VMCTypeInfo *type)
		{
			Ref = ref;
			Type = type;
			Type->AddRef();

			assert(strcmp(type->GetName(), "WeakRef") == 0 ||
				strcmp(type->GetName(), "ConstWeakRef") == 0);

			WeakRefFlag = Type->GetEngine()->GetWeakRefFlagOfScriptObject(Ref, Type->GetSubType());
			if (WeakRefFlag)
				WeakRefFlag->AddRef();
		}
		VMCWeakRef::~VMCWeakRef()
		{
			if (Type)
				Type->Release();

			if (WeakRefFlag)
				WeakRefFlag->Release();
		}
		VMCWeakRef &VMCWeakRef::operator =(const VMCWeakRef &other)
		{
			if (Ref == other.Ref &&
				WeakRefFlag == other.WeakRefFlag)
				return *this;

			if (Type != other.Type)
			{
				if (!(strcmp(Type->GetName(), "ConstWeakRef") == 0 &&
					strcmp(other.Type->GetName(), "WeakRef") == 0 &&
					Type->GetSubType() == other.Type->GetSubType()))
				{
					assert(false);
					return *this;
				}
			}

			Ref = other.Ref;
			if (WeakRefFlag)
				WeakRefFlag->Release();

			WeakRefFlag = other.WeakRefFlag;
			if (WeakRefFlag)
				WeakRefFlag->AddRef();

			return *this;
		}
		VMCWeakRef &VMCWeakRef::Set(void *newRef)
		{
			if (WeakRefFlag)
				WeakRefFlag->Release();

			Ref = newRef;
			if (newRef)
			{
				WeakRefFlag = Type->GetEngine()->GetWeakRefFlagOfScriptObject(newRef, Type->GetSubType());
				WeakRefFlag->AddRef();
			}
			else
				WeakRefFlag = 0;

			Type->GetEngine()->ReleaseScriptObject(newRef, Type->GetSubType());
			return *this;
		}
		VMCTypeInfo *VMCWeakRef::GetRefType() const
		{
			return Type->GetSubType();
		}
		bool VMCWeakRef::operator==(const VMCWeakRef &o) const
		{
			if (Ref == o.Ref &&
				WeakRefFlag == o.WeakRefFlag &&
				Type == o.Type)
				return true;

			return false;
		}
		bool VMCWeakRef::operator!=(const VMCWeakRef &o) const
		{
			return !(*this == o);
		}
		void *VMCWeakRef::Get() const
		{
			if (Ref == 0 || WeakRefFlag == 0)
				return 0;

			WeakRefFlag->Lock();
			if (!WeakRefFlag->Get())
			{
				Type->GetEngine()->AddRefScriptObject(Ref, Type->GetSubType());
				WeakRefFlag->Unlock();
				return Ref;
			}
			WeakRefFlag->Unlock();

			return 0;
		}
		bool VMCWeakRef::Equals(void *ref) const
		{
			if (Ref != ref)
				return false;

			asILockableSharedBool *flag = Type->GetEngine()->GetWeakRefFlagOfScriptObject(ref, Type->GetSubType());
			if (WeakRefFlag != flag)
				return false;

			return true;
		}
		void VMCWeakRef::Construct(VMCTypeInfo *type, void *mem)
		{
			new(mem) VMCWeakRef(type);
		}
		void VMCWeakRef::Construct2(VMCTypeInfo *type, void *ref, void *mem)
		{
			new(mem) VMCWeakRef(ref, type);
			VMCContext *ctx = asGetActiveContext();
			if (ctx && ctx->GetState() == asEXECUTION_EXCEPTION)
				reinterpret_cast<VMCWeakRef*>(mem)->~VMCWeakRef();
		}
		void VMCWeakRef::Destruct(VMCWeakRef *obj)
		{
			obj->~VMCWeakRef();
		}
		bool VMCWeakRef::TemplateCallback(VMCTypeInfo *TI, bool&)
		{
			VMCTypeInfo *subType = TI->GetSubType();
			if (subType == 0)
				return false;

			if (!(subType->GetFlags() & asOBJ_REF))
				return false;

			if (TI->GetSubTypeId() & asTYPEID_OBJHANDLE)
				return false;

			as_size_t cnt = subType->GetBehaviourCount();
			for (as_size_t n = 0; n < cnt; n++)
			{
				asEBehaviours beh;
				subType->GetBehaviourByIndex(n, &beh);
				if (beh == asBEHAVE_GET_WEAKREF_FLAG)
					return true;
			}

			TI->GetEngine()->WriteMessage("WeakRef", 0, 0, asMSGTYPE_ERROR, "The subtype doesn't support weak references");
			return false;
		}

		VMCComplex::VMCComplex()
		{
			R = 0;
			I = 0;
		}
		VMCComplex::VMCComplex(const VMCComplex &other)
		{
			R = other.R;
			I = other.I;
		}
		VMCComplex::VMCComplex(float _r, float _i)
		{
			R = _r;
			I = _i;
		}
		bool VMCComplex::operator==(const VMCComplex &o) const
		{
			return (R == o.R) && (I == o.I);
		}
		bool VMCComplex::operator!=(const VMCComplex &o) const
		{
			return !(*this == o);
		}
		VMCComplex &VMCComplex::operator=(const VMCComplex &other)
		{
			R = other.R;
			I = other.I;
			return *this;
		}
		VMCComplex &VMCComplex::operator+=(const VMCComplex &other)
		{
			R += other.R;
			I += other.I;
			return *this;
		}
		VMCComplex &VMCComplex::operator-=(const VMCComplex &other)
		{
			R -= other.R;
			I -= other.I;
			return *this;
		}
		VMCComplex &VMCComplex::operator*=(const VMCComplex &other)
		{
			*this = *this * other;
			return *this;
		}
		VMCComplex &VMCComplex::operator/=(const VMCComplex &other)
		{
			*this = *this / other;
			return *this;
		}
		float VMCComplex::SquaredLength() const
		{
			return R * R + I * I;
		}
		float VMCComplex::Length() const
		{
			return sqrtf(SquaredLength());
		}
		VMCComplex VMCComplex::operator+(const VMCComplex &other) const
		{
			return VMCComplex(R + other.R, I + other.I);
		}
		VMCComplex VMCComplex::operator-(const VMCComplex &other) const
		{
			return VMCComplex(R - other.R, I + other.I);
		}
		VMCComplex VMCComplex::operator*(const VMCComplex &other) const
		{
			return VMCComplex(R*other.R - I * other.I, R*other.I + I * other.R);
		}
		VMCComplex VMCComplex::operator/(const VMCComplex &other) const
		{
			float squaredLen = other.SquaredLength();
			if (squaredLen == 0)
				return VMCComplex(0, 0);

			return VMCComplex((R*other.R + I * other.I) / squaredLen, (I*other.R - R * other.I) / squaredLen);
		}
		VMCComplex VMCComplex::GetRI() const
		{
			return *this;
		}
		VMCComplex VMCComplex::GetIR() const
		{
			return VMCComplex(R, I);
		}
		void VMCComplex::SetRI(const VMCComplex &o)
		{
			*this = o;
		}
		void VMCComplex::SetIR(const VMCComplex &o)
		{
			R = o.I;
			I = o.R;
		}
		void VMCComplex::DefaultConstructor(VMCComplex *self)
		{
			new(self) VMCComplex();
		}
		void VMCComplex::CopyConstructor(const VMCComplex &other, VMCComplex *self)
		{
			new(self) VMCComplex(other);
		}
		void VMCComplex::ConvConstructor(float r, VMCComplex *self)
		{
			new(self) VMCComplex(r);
		}
		void VMCComplex::InitConstructor(float r, float i, VMCComplex *self)
		{
			new(self) VMCComplex(r, i);
		}
		void VMCComplex::ListConstructor(float *list, VMCComplex *self)
		{
			new(self) VMCComplex(list[0], list[1]);
		}

		VMCRandom::VMCRandom()
		{
			SeedFromTime();
		}
		void VMCRandom::AddRef()
		{
			asAtomicInc(Ref);
		}
		void VMCRandom::Release()
		{
			if (asAtomicDec(Ref) <= 0)
			{
				this->~VMCRandom();
				asFreeMem((void*)this);
			}
		}
		void VMCRandom::SeedFromTime()
		{
			Seed(static_cast<uint32_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		}
		uint32_t VMCRandom::GetU()
		{
			return Twister();
		}
		int32_t VMCRandom::GetI()
		{
			return Twister();
		}
		double VMCRandom::GetD()
		{
			return DDist(Twister);
		}
		void VMCRandom::Seed(uint32_t Seed)
		{
			Twister.seed(Seed);
		}
		void VMCRandom::Seed(VMCArray* Array)
		{
			if (!Array || Array->GetElementTypeId() != asTYPEID_UINT32)
			{
				VMCContext* Context = asGetActiveContext();
				if (Context != nullptr)
					Context->SetException("random::seed Array element type not uint32");

				return;
			}

			std::vector<uint32_t> Vector;
			Vector.reserve(Array->GetSize());

			for (unsigned int i = 0; i < Array->GetSize(); i++)
				Vector.push_back(static_cast<uint32_t*>(Array->GetBuffer())[i]);

			std::seed_seq Sq(Vector.begin(), Vector.end());
			Twister.seed(Sq);
		}
		void VMCRandom::Assign(VMCRandom* From)
		{
			if (From != nullptr)
				Twister = From->Twister;
		}
		VMCRandom* VMCRandom::Create()
		{
			void* Data = asAllocMem(sizeof(VMCRandom));
			return new(Data) VMCRandom();
		}

		VMCThread::VMCThread(VMCManager* Engine, VMCFunction* Func) : Function(Func), Manager(VMManager::Get(Engine)), Context(nullptr), GCFlag(false), Ref(1)
		{
			Engine->NotifyGarbageCollectorOfNewObject(this, Engine->GetTypeInfoByName("Thread"));
		}
		void VMCThread::Routine()
		{
			Mutex.lock();
			if (!Function)
			{
				Release();
				return Mutex.unlock();
			}

			if (Context == nullptr)
				Context = Manager->CreateContext();

			if (Context == nullptr)
			{
				Manager->GetEngine()->WriteMessage("", 0, 0, asMSGTYPE_ERROR, "failed to start a thread: no available context");
				Release();

				return Mutex.unlock();
			}

			if (Context->GetState() != VMExecState::SUSPENDED)
			{
				Context->Prepare(Function);
				Context->SetArgObject(0, this);
				Context->SetUserData(this, ContextUD);
			}
			Mutex.unlock();

			int Result = Context->Execute();
			Mutex.lock();

			if (Result != (int)VMExecState::SUSPENDED)
			{
				Context->SetUserData(nullptr, ContextUD);
				TH_CLEAR(Context);
			}

			CV.notify_all();
			Mutex.unlock();
			Release();
			asThreadCleanup();
		}
		void VMCThread::AddRef()
		{
			GCFlag = false;
			asAtomicInc(Ref);
		}
		void VMCThread::Suspend()
		{
			Mutex.lock();
			if (Context && Context->GetState() != VMExecState::SUSPENDED)
				Context->Suspend();
			Mutex.unlock();
		}
		void VMCThread::Release()
		{
			GCFlag = false;
			if (asAtomicDec(Ref) <= 0)
			{
				if (Thread.joinable())
					Thread.join();

				ReleaseReferences(nullptr);
				this->~VMCThread();
				asFreeMem((void*)this);
			}
		}
		void VMCThread::SetGCFlag()
		{
			GCFlag = true;
		}
		bool VMCThread::GetGCFlag()
		{
			return GCFlag;
		}
		int VMCThread::GetRefCount()
		{
			return Ref;
		}
		void VMCThread::EnumReferences(VMCManager* Engine)
		{
			for (int i = 0; i < 2; i++)
			{
				for (auto Any : Pipe[i].Queue)
				{
					if (Any != nullptr)
						Engine->GCEnumCallback(Any);
				}
			}

			Engine->GCEnumCallback(Engine);
			if (Context != nullptr)
				Engine->GCEnumCallback(Context);

			if (Function != nullptr)
				Engine->GCEnumCallback(Function);
		}
		int VMCThread::Join(uint64_t Timeout)
		{
			{
				std::lock_guard<std::mutex> Guard(Mutex);
				if (std::this_thread::get_id() == Thread.get_id())
					return -1;

				if (!Thread.joinable())
					return -1;
			}
			{
				std::unique_lock<std::mutex> Guard(Mutex);
				if (CV.wait_for(Guard, std::chrono::milliseconds(Timeout), [&]
				{
					return !((Context && Context->GetState() != VMExecState::SUSPENDED));
				}))
				{
					Thread.join();
					return 1;
				}
			}

			return 0;
		}
		int VMCThread::Join()
		{
			if (std::this_thread::get_id() == Thread.get_id())
				return -1;

			while (true)
			{
				int R = Join(1000);
				if (R == -1 || R == 1)
					return R;
			}

			return 0;
		}
		void VMCThread::Push(void* fRef, int TypeId)
		{
			auto* fThread = GetThread();
			int Id = (fThread == this ? 1 : 0);

			void* Data = asAllocMem(sizeof(VMCAny));
			VMCAny* Any = new(Data) VMCAny(fRef, TypeId, VMManager::Get()->GetEngine());
			Pipe[Id].Mutex.lock();
			Pipe[Id].Queue.push_back(Any);
			Pipe[Id].Mutex.unlock();
			Pipe[Id].CV.notify_one();
		}
		bool VMCThread::Pop(void* fRef, int TypeId)
		{
			bool Resolved = false;
			while (!Resolved)
				Resolved = Pop(fRef, TypeId, 1000);

			return true;
		}
		bool VMCThread::Pop(void* fRef, int TypeId, uint64_t Timeout)
		{
			auto* fThread = GetThread();
			int Id = (fThread == this ? 0 : 1);

			std::unique_lock<std::mutex> Guard(Pipe[Id].Mutex);
			if (!CV.wait_for(Guard, std::chrono::milliseconds(Timeout), [&]
			{
				return Pipe[Id].Queue.size() != 0;
			}))
				return false;

			VMCAny* Result = Pipe[Id].Queue.front();
			if (!Result->Retrieve(fRef, TypeId))
				return false;

			Pipe[Id].Queue.erase(Pipe[Id].Queue.begin());
			Result->Release();

			return true;
		}
		bool VMCThread::IsActive()
		{
			Mutex.lock();
			bool State = (Context && Context->GetState() != VMExecState::SUSPENDED);
			Mutex.unlock();

			return State;
		}
		bool VMCThread::Start()
		{
			Mutex.lock();
			if (!Function)
			{
				Mutex.unlock();
				return false;
			}

			if (Context != nullptr)
			{
				if (Context->GetState() != VMExecState::SUSPENDED)
				{
					Mutex.unlock();
					return false;
				}
				else
				{
					Mutex.unlock();
					Join();
					Mutex.lock();
				}
			}
			else if (Thread.joinable())
			{
				if (std::this_thread::get_id() == Thread.get_id())
				{
					Mutex.unlock();
					return false;
				}

				Thread.join();
			}

			AddRef();
			Thread = std::thread(&VMCThread::Routine, this);
			Mutex.unlock();

			return true;
		}
		void VMCThread::ReleaseReferences(VMCManager*)
		{
			if (Join() >= 0)
				TH_ERROR("[memerr] thread was forced to join");

			for (int i = 0; i < 2; i++)
			{
				Pipe[i].Mutex.lock();
				for (auto Any : Pipe[i].Queue)
				{
					if (Any != nullptr)
						Any->Release();
				}
				Pipe[i].Queue.clear();
				Pipe[i].Mutex.unlock();
			}

			Mutex.lock();
			if (Function)
				Function->Release();

			TH_CLEAR(Context);
			Manager = nullptr;
			Function = nullptr;
			Mutex.unlock();
		}
		void VMCThread::Create(VMCGeneric* Generic)
		{
			VMCManager* Engine = Generic->GetEngine();
			VMCFunction* Function = *(VMCFunction**)Generic->GetAddressOfArg(0);
			void* Data = asAllocMem(sizeof(VMCThread));
			*(VMCThread**)Generic->GetAddressOfReturnLocation() = new(Data) VMCThread(Engine, Function);
		}
		VMCThread* VMCThread::GetThread()
		{
			VMCContext* Context = asGetActiveContext();
			if (!Context)
				return nullptr;

			return static_cast<VMCThread*>(Context->GetUserData(ContextUD));
		}
		void VMCThread::ThreadSleep(uint64_t Timeout)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(Timeout));
		}
		uint64_t VMCThread::GetThreadId()
		{
			return (uint64_t)std::hash<std::thread::id>()(std::this_thread::get_id());
		}
		int VMCThread::ContextUD = 550;
		int VMCThread::EngineListUD = 551;

		VMCPromise::VMCPromise(VMCContext* Base, VMCTypeInfo* Info) : Context(Base), Any(nullptr), Type(Info), Ref(2), Stored(false), GCFlag(false)
		{
			if (!Context)
				return;

			VMContext* Next = VMContext::Get(Context);
			if (Next != nullptr)
				Next->PushCoroutine();

			VMCManager* Engine = Context->GetEngine();
			Context->AddRef();
			Engine->NotifyGarbageCollectorOfNewObject(this, Engine->GetTypeInfoByName("Promise"));
		}
		void VMCPromise::Release()
		{
			GCFlag = false;
			if (asAtomicDec(Ref) <= 0)
			{
				ReleaseReferences(nullptr);
				this->~VMCPromise();
				asFreeMem((void*)this);
			}
		}
		void VMCPromise::AddRef()
		{
			GCFlag = false;
			asAtomicInc(Ref);
		}
		void VMCPromise::EnumReferences(VMCManager* Engine)
		{
			Safe.lock();
			if (Context != nullptr)
				Engine->GCEnumCallback(Context);

			if (Any != nullptr)
				Engine->GCEnumCallback(Any);
			Safe.unlock();
		}
		void VMCPromise::ReleaseReferences(VMCManager*)
		{
			Safe.lock();
			if (Any != nullptr)
			{
				Any->Release();
				Any = nullptr;
			}

			if (Context != nullptr)
			{
				Context->Release();
				Context = nullptr;
			}
			Safe.unlock();
		}
		void VMCPromise::SetGCFlag()
		{
			GCFlag = true;
		}
		bool VMCPromise::GetGCFlag()
		{
			return GCFlag;
		}
		int VMCPromise::GetRefCount()
		{
			return Ref;
		}
		int VMCPromise::Acquire(VMCAny* Value)
		{
			if (!Context || Stored)
				return asEXECUTION_UNINITIALIZED;

			Safe.lock();
			if (Any != nullptr)
				Any->Release();

			if (Value != nullptr)
				Value->AddRef();

			Stored = true;
			Any = Value;
			Safe.unlock();
			Release();

			VMContext* Base = VMContext::Get(Context);
			if (!Base)
				return asEXECUTION_ERROR;

			return Core::Schedule::Get()->SetTask([Base]()
			{
				Base->PopCoroutine();
				Base->Resume();
			});
		}
		int VMCPromise::Set(void* fRef, int TypeId)
		{
			if (!Context)
				return -1;

			void* Data = asAllocMem(sizeof(VMCAny));
			VMCAny* Result = new(Data) VMCAny(fRef, TypeId, Context->GetEngine());
			Result->Release();

			if (TypeId & asTYPEID_OBJHANDLE)
			{
				VMCManager* Manager = Context->GetEngine();
				Manager->ReleaseScriptObject(*(void**)fRef, Manager->GetTypeInfoById(TypeId));
			}

			return Acquire(Result);
		}
		int VMCPromise::Set(void* fRef, const char* TypeName)
		{
			if (!Context)
				return -1;

			return Set(fRef, Context->GetEngine()->GetTypeIdByDecl(TypeName));
		}
		bool VMCPromise::Retrieve(void* fRef, int TypeId)
		{
			if (!Any)
				return Stored;

			return Any->Retrieve(fRef, TypeId);
		}
		void* VMCPromise::Get()
		{
			if (!Any)
				return nullptr;

			int TypeId = Any->GetTypeId(), RefTypeId = (Type ? Type->GetTypeId() : -1);
			if (RefTypeId != -1 && RefTypeId != TypeId)
			{
				VMCManager* Engine = Context->GetEngine();
				const char* From = Engine->GetTypeDeclaration(TypeId);
				const char* To = Engine->GetTypeDeclaration(RefTypeId);

				Context->SetException(Core::Form("cannot convert from %s to %s", (From ? From : "[undefined]"), (To ? To : "[undefined]")).Get());
				return nullptr;
			}

			if (TypeId & asTYPEID_OBJHANDLE)
				return &Any->Value.ValueObj;
			else if (TypeId & asTYPEID_MASK_OBJECT)
				return Any->Value.ValueObj;
			else if (TypeId <= asTYPEID_DOUBLE || TypeId & asTYPEID_MASK_SEQNBR)
				return &Any->Value.ValueInt;

			Context->SetException("retrieve this object explicitly with To(T& out)");
			return nullptr;
		}
		VMCAny* VMCPromise::GetSrc()
		{
			return Any;
		}
		VMCPromise* VMCPromise::Await()
		{
			if (!Context)
				return this;

			if (!Any && !Stored)
				Context->Suspend();

			return this;
		}
		VMCPromise* VMCPromise::Create(VMCTypeInfo* Info)
		{
			VMCContext* Context = asGetActiveContext();
			if (!Context)
				return nullptr;

			VMCManager* Engine = Context->GetEngine();
			if (!Engine)
				return nullptr;

			return new(asAllocMem(sizeof(VMCPromise))) VMCPromise(Context, Info);
		}
		int VMCPromise::GetTypeId(const char* Name)
		{
			if (!Name)
				return -1;

			VMCContext* Context = asGetActiveContext();
			if (!Context)
				return -1;

			return Context->GetEngine()->GetTypeIdByDecl(Name);
		}

		bool RegisterAnyAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("Any", sizeof(VMCAny), asOBJ_REF | asOBJ_GC);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_FACTORY, "Any@ f()", asFUNCTION(VMCAny::Factory1), asCALL_GENERIC);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_FACTORY, "Any@ f(?&in) explicit", asFUNCTION(VMCAny::Factory2), asCALL_GENERIC);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_FACTORY, "Any@ f(const int64&in) explicit", asFUNCTION(VMCAny::Factory2), asCALL_GENERIC);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_FACTORY, "Any@ f(const double&in) explicit", asFUNCTION(VMCAny::Factory2), asCALL_GENERIC);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_ADDREF, "void f()", asMETHOD(VMCAny, AddRef), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_RELEASE, "void f()", asMETHOD(VMCAny, Release), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(VMCAny, GetRefCount), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(VMCAny, SetFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(VMCAny, GetFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(VMCAny, EnumReferences), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Any", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(VMCAny, ReleaseAllHandles), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Any", "Any &opAssign(Any&in)", asFUNCTION(VMCAny::Assignment), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("Any", "void Store(?&in)", asMETHODPR(VMCAny, Store, (void*, int), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Any", "bool Retrieve(?&out)", asMETHODPR(VMCAny, Retrieve, (void*, int) const, bool), asCALL_THISCALL);
			return true;
		}
		bool RegisterArrayAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->SetTypeInfoUserDataCleanupCallback(VMCArray::CleanupTypeInfoCache, ARRAY_CACHE);
			Engine->RegisterObjectType("Array<class T>", 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(VMCArray::TemplateCallback), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_FACTORY, "Array<T>@ f(int&in)", asFUNCTIONPR(VMCArray::Create, (VMCTypeInfo*), VMCArray*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_FACTORY, "Array<T>@ f(int&in, uint length) explicit", asFUNCTIONPR(VMCArray::Create, (VMCTypeInfo*, as_size_t), VMCArray*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_FACTORY, "Array<T>@ f(int&in, uint length, const T &in value)", asFUNCTIONPR(VMCArray::Create, (VMCTypeInfo*, as_size_t, void *), VMCArray*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_LIST_FACTORY, "Array<T>@ f(int&in type, int&in list) {repeat T}", asFUNCTIONPR(VMCArray::Create, (VMCTypeInfo*, void*), VMCArray*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_ADDREF, "void f()", asMETHOD(VMCArray, AddRef), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_RELEASE, "void f()", asMETHOD(VMCArray, Release), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "T &opIndex(uint index)", asMETHODPR(VMCArray, At, (as_size_t), void*), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "const T &opIndex(uint index) const", asMETHODPR(VMCArray, At, (as_size_t) const, const void*), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "Array<T> &opAssign(const Array<T>&in)", asMETHOD(VMCArray, operator=), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void InsertAt(uint index, const T&in value)", asMETHODPR(VMCArray, InsertAt, (as_size_t, void *), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void InsertAt(uint index, const Array<T>& arr)", asMETHODPR(VMCArray, InsertAt, (as_size_t, const VMCArray &), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void InsertLast(const T&in value)", asMETHOD(VMCArray, InsertLast), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void RemoveAt(uint index)", asMETHOD(VMCArray, RemoveAt), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void RemoveLast()", asMETHOD(VMCArray, RemoveLast), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void RemoveRange(uint start, uint count)", asMETHOD(VMCArray, RemoveRange), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "uint Size() const", asMETHOD(VMCArray, GetSize), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void Reserve(uint length)", asMETHOD(VMCArray, Reserve), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void Resize(uint length)", asMETHODPR(VMCArray, Resize, (as_size_t), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void SortAsc()", asMETHODPR(VMCArray, SortAsc, (), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void SortAsc(uint startAt, uint count)", asMETHODPR(VMCArray, SortAsc, (as_size_t, as_size_t), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void SortDesc()", asMETHODPR(VMCArray, SortDesc, (), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void SortDesc(uint startAt, uint count)", asMETHODPR(VMCArray, SortDesc, (as_size_t, as_size_t), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "void Reverse()", asMETHOD(VMCArray, Reverse), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "int Find(const T&in if_handle_then_const value) const", asMETHODPR(VMCArray, Find, (void*) const, int), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "int Find(uint startAt, const T&in if_handle_then_const value) const", asMETHODPR(VMCArray, Find, (as_size_t, void*) const, int), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "int FindByRef(const T&in if_handle_then_const value) const", asMETHODPR(VMCArray, FindByRef, (void*) const, int), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "int FindByRef(uint startAt, const T&in if_handle_then_const value) const", asMETHODPR(VMCArray, FindByRef, (as_size_t, void*) const, int), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "bool opEquals(const Array<T>&in) const", asMETHOD(VMCArray, operator==), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Array<T>", "bool IsEmpty() const", asMETHOD(VMCArray, IsEmpty), asCALL_THISCALL);
			Engine->RegisterFuncdef("bool Array<T>::Less(const T&in if_handle_then_const a, const T&in if_handle_then_const b)");
			Engine->RegisterObjectMethod("Array<T>", "void Sort(const Less &in, uint startAt = 0, uint count = uint(-1))", asMETHODPR(VMCArray, Sort, (asIScriptFunction*, as_size_t, as_size_t), void), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(VMCArray, GetRefCount), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(VMCArray, SetFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(VMCArray, GetFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(VMCArray, EnumReferences), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Array<T>", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(VMCArray, ReleaseAllHandles), asCALL_THISCALL);
			Engine->RegisterDefaultArrayType("Array<T>");
			return true;
		}
		bool RegisterComplexAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("Complex", sizeof(VMCComplex), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<VMCComplex>() | asOBJ_APP_CLASS_ALLFLOATS);
			Engine->RegisterObjectProperty("Complex", "float R", asOFFSET(VMCComplex, R));
			Engine->RegisterObjectProperty("Complex", "float I", asOFFSET(VMCComplex, I));
			Engine->RegisterObjectBehaviour("Complex", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(VMCComplex::DefaultConstructor), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("Complex", asBEHAVE_CONSTRUCT, "void f(const Complex &in)", asFUNCTION(VMCComplex::CopyConstructor), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("Complex", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(VMCComplex::ConvConstructor), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("Complex", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(VMCComplex::InitConstructor), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("Complex", asBEHAVE_LIST_CONSTRUCT, "void f(const int &in) {float, float}", asFUNCTION(VMCComplex::ListConstructor), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("Complex", "Complex &opAddAssign(const Complex &in)", asMETHODPR(VMCComplex, operator+=, (const VMCComplex &), VMCComplex&), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex &opSubAssign(const Complex &in)", asMETHODPR(VMCComplex, operator-=, (const VMCComplex &), VMCComplex&), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex &opMulAssign(const Complex &in)", asMETHODPR(VMCComplex, operator*=, (const VMCComplex &), VMCComplex&), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex &opDivAssign(const Complex &in)", asMETHODPR(VMCComplex, operator/=, (const VMCComplex &), VMCComplex&), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "bool opEquals(const Complex &in) const", asMETHODPR(VMCComplex, operator==, (const VMCComplex &) const, bool), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex opAdd(const Complex &in) const", asMETHODPR(VMCComplex, operator+, (const VMCComplex &) const, VMCComplex), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex opSub(const Complex &in) const", asMETHODPR(VMCComplex, operator-, (const VMCComplex &) const, VMCComplex), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex opMul(const Complex &in) const", asMETHODPR(VMCComplex, operator*, (const VMCComplex &) const, VMCComplex), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex opDiv(const Complex &in) const", asMETHODPR(VMCComplex, operator/, (const VMCComplex &) const, VMCComplex), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "float Abs() const", asMETHOD(VMCComplex, Length), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex get_RI() const property", asMETHOD(VMCComplex, GetRI), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "Complex get_IR() const property", asMETHOD(VMCComplex, GetIR), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "void set_RI(const Complex &in) property", asMETHOD(VMCComplex, SetRI), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Complex", "void set_IR(const Complex &in) property", asMETHOD(VMCComplex, SetIR), asCALL_THISCALL);
			return true;
		}
		bool RegisterMapAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("MapKey", sizeof(VMCMapKey), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_GC | asGetTypeTraits<VMCMapKey>());
			Engine->RegisterObjectBehaviour("MapKey", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(VMCMap::KeyConstruct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("MapKey", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(VMCMap::KeyDestruct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("MapKey", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(VMCMapKey, EnumReferences), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("MapKey", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(VMCMapKey, FreeValue), asCALL_THISCALL);
			Engine->RegisterObjectMethod("MapKey", "MapKey &opAssign(const MapKey &in)", asFUNCTIONPR(VMCMap::KeyopAssign, (const VMCMapKey &, VMCMapKey *), VMCMapKey &), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "MapKey &opHndlAssign(const ?&in)", asFUNCTIONPR(VMCMap::KeyopAssign, (void *, int, VMCMapKey*), VMCMapKey &), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "MapKey &opHndlAssign(const MapKey &in)", asFUNCTIONPR(VMCMap::KeyopAssign, (const VMCMapKey &, VMCMapKey *), VMCMapKey &), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "MapKey &opAssign(const ?&in)", asFUNCTIONPR(VMCMap::KeyopAssign, (void *, int, VMCMapKey*), VMCMapKey &), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "MapKey &opAssign(double)", asFUNCTIONPR(VMCMap::KeyopAssign, (double, VMCMapKey*), VMCMapKey &), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "MapKey &opAssign(int64)", asFUNCTIONPR(VMCMap::KeyopAssign, (as_int64_t, VMCMapKey*), VMCMapKey &), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "void opCast(?&out)", asFUNCTIONPR(VMCMap::KeyopCast, (void *, int, VMCMapKey*), void), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "void opConv(?&out)", asFUNCTIONPR(VMCMap::KeyopCast, (void *, int, VMCMapKey*), void), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "int64 opConv()", asFUNCTIONPR(VMCMap::KeyopConvInt, (VMCMapKey*), as_int64_t), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("MapKey", "double opConv()", asFUNCTIONPR(VMCMap::KeyopConvDouble, (VMCMapKey*), double), asCALL_CDECL_OBJLAST);

			Engine->RegisterObjectType("Map", sizeof(VMCMap), asOBJ_REF | asOBJ_GC);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_FACTORY, "Map@ f()", asFUNCTION(VMCMap::Factory), asCALL_GENERIC);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_LIST_FACTORY, "Map @f(int &in) {repeat {String, ?}}", asFUNCTION(VMCMap::ListFactory), asCALL_GENERIC);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_ADDREF, "void f()", asMETHOD(VMCMap, AddRef), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_RELEASE, "void f()", asMETHOD(VMCMap, Release), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "Map &opAssign(const Map &in)", asMETHODPR(VMCMap, operator=, (const VMCMap &), VMCMap&), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "void Set(const String &in, const ?&in)", asMETHODPR(VMCMap, Set, (const std::string&, void*, int), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "bool Get(const String &in, ?&out) const", asMETHODPR(VMCMap, Get, (const std::string&, void*, int) const, bool), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "bool Exists(const String &in) const", asMETHOD(VMCMap, Exists), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "bool IsEmpty() const", asMETHOD(VMCMap, IsEmpty), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "uint GetSize() const", asMETHOD(VMCMap, GetSize), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "bool Delete(const String &in)", asMETHOD(VMCMap, Delete), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "void DeleteAll()", asMETHOD(VMCMap, DeleteAll), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "Array<String>@ GetKeys() const", asMETHOD(VMCMap, GetKeys), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "MapKey &opIndex(const String &in)", asMETHODPR(VMCMap, operator[], (const std::string &), VMCMapKey*), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Map", "const MapKey &opIndex(const String &in) const", asMETHODPR(VMCMap, operator[], (const std::string &) const, const VMCMapKey*), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(VMCMap, GetRefCount), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(VMCMap, SetGCFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(VMCMap, GetGCFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(VMCMap, EnumReferences), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Map", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(VMCMap, ReleaseAllReferences), asCALL_THISCALL);

			VMCMap::Setup(Engine);
			return true;
		}
		bool RegisterGridAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("Grid<class T>", 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(VMCGrid::TemplateCallback), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_FACTORY, "Grid<T>@ f(int&in)", asFUNCTIONPR(VMCGrid::Create, (VMCTypeInfo*), VMCGrid*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_FACTORY, "Grid<T>@ f(int&in, uint, uint)", asFUNCTIONPR(VMCGrid::Create, (VMCTypeInfo*, as_size_t, as_size_t), VMCGrid*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_FACTORY, "Grid<T>@ f(int&in, uint, uint, const T &in)", asFUNCTIONPR(VMCGrid::Create, (VMCTypeInfo*, as_size_t, as_size_t, void *), VMCGrid*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_LIST_FACTORY, "Grid<T>@ f(int&in type, int&in list) {repeat {repeat_same T}}", asFUNCTIONPR(VMCGrid::Create, (VMCTypeInfo*, void*), VMCGrid*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_ADDREF, "void f()", asMETHOD(VMCGrid, AddRef), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_RELEASE, "void f()", asMETHOD(VMCGrid, Release), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Grid<T>", "T &opIndex(uint, uint)", asMETHODPR(VMCGrid, At, (as_size_t, as_size_t), void*), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Grid<T>", "const T &opIndex(uint, uint) const", asMETHODPR(VMCGrid, At, (as_size_t, as_size_t) const, const void*), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Grid<T>", "void Resize(uint, uint)", asMETHODPR(VMCGrid, Resize, (as_size_t, as_size_t), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Grid<T>", "uint Width() const", asMETHOD(VMCGrid, GetWidth), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Grid<T>", "uint Height() const", asMETHOD(VMCGrid, GetHeight), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(VMCGrid, GetRefCount), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(VMCGrid, SetFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(VMCGrid, GetFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(VMCGrid, EnumReferences), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Grid<T>", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(VMCGrid, ReleaseAllHandles), asCALL_THISCALL);

			return true;
		}
		bool RegisterRefAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("Ref", sizeof(VMCRef), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_GC | asGetTypeTraits<VMCRef>());
			Engine->RegisterObjectBehaviour("Ref", asBEHAVE_CONSTRUCT, "void f()", asFUNCTIONPR(VMCRef::Construct, (VMCRef *), void), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectBehaviour("Ref", asBEHAVE_CONSTRUCT, "void f(const Ref &in)", asFUNCTIONPR(VMCRef::Construct, (VMCRef *, const VMCRef &), void), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectBehaviour("Ref", asBEHAVE_CONSTRUCT, "void f(const ?&in)", asFUNCTIONPR(VMCRef::Construct, (VMCRef *, void *, int), void), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectBehaviour("Ref", asBEHAVE_DESTRUCT, "void f()", asFUNCTIONPR(VMCRef::Destruct, (VMCRef *), void), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectBehaviour("Ref", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(VMCRef, EnumReferences), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Ref", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(VMCRef, ReleaseReferences), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Ref", "void opCast(?&out)", asMETHODPR(VMCRef, Cast, (void **, int), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Ref", "Ref &opHndlAssign(const Ref &in)", asMETHOD(VMCRef, operator=), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Ref", "Ref &opHndlAssign(const ?&in)", asMETHOD(VMCRef, Assign), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Ref", "bool opEquals(const Ref &in) const", asMETHODPR(VMCRef, operator==, (const VMCRef &) const, bool), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Ref", "bool opEquals(const ?&in) const", asMETHODPR(VMCRef, Equals, (void*, int) const, bool), asCALL_THISCALL);
			return true;
		}
		bool RegisterWeakRefAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("WeakRef<class T>", sizeof(VMCWeakRef), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_TEMPLATE | asOBJ_APP_CLASS_DAK);
			Engine->RegisterObjectBehaviour("WeakRef<T>", asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(VMCWeakRef::Construct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("WeakRef<T>", asBEHAVE_CONSTRUCT, "void f(int&in, T@+)", asFUNCTION(VMCWeakRef::Construct2), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("WeakRef<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(VMCWeakRef::Destruct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("WeakRef<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(VMCWeakRef::TemplateCallback), asCALL_CDECL);
			Engine->RegisterObjectMethod("WeakRef<T>", "T@ opImplCast()", asMETHOD(VMCWeakRef, Get), asCALL_THISCALL);
			Engine->RegisterObjectMethod("WeakRef<T>", "T@ Get() const", asMETHODPR(VMCWeakRef, Get, () const, void*), asCALL_THISCALL);
			Engine->RegisterObjectMethod("WeakRef<T>", "WeakRef<T> &opHndlAssign(const WeakRef<T> &in)", asMETHOD(VMCWeakRef, operator=), asCALL_THISCALL);
			Engine->RegisterObjectMethod("WeakRef<T>", "WeakRef<T> &opAssign(const WeakRef<T> &in)", asMETHOD(VMCWeakRef, operator=), asCALL_THISCALL);
			Engine->RegisterObjectMethod("WeakRef<T>", "bool opEquals(const WeakRef<T> &in) const", asMETHODPR(VMCWeakRef, operator==, (const VMCWeakRef &) const, bool), asCALL_THISCALL);
			Engine->RegisterObjectMethod("WeakRef<T>", "WeakRef<T> &opHndlAssign(T@)", asMETHOD(VMCWeakRef, Set), asCALL_THISCALL);
			Engine->RegisterObjectMethod("WeakRef<T>", "bool opEquals(const T@+) const", asMETHOD(VMCWeakRef, Equals), asCALL_THISCALL);
			Engine->RegisterObjectType("ConstWeakRef<class T>", sizeof(VMCWeakRef), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_TEMPLATE | asOBJ_APP_CLASS_DAK);
			Engine->RegisterObjectBehaviour("ConstWeakRef<T>", asBEHAVE_CONSTRUCT, "void f(int&in)", asFUNCTION(VMCWeakRef::Construct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("ConstWeakRef<T>", asBEHAVE_CONSTRUCT, "void f(int&in, const T@+)", asFUNCTION(VMCWeakRef::Construct2), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("ConstWeakRef<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(VMCWeakRef::Destruct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("ConstWeakRef<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(VMCWeakRef::TemplateCallback), asCALL_CDECL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "const T@ opImplCast() const", asMETHOD(VMCWeakRef, Get), asCALL_THISCALL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "const T@ Get() const", asMETHODPR(VMCWeakRef, Get, () const, void*), asCALL_THISCALL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "ConstWeakRef<T> &opHndlAssign(const ConstWeakRef<T> &in)", asMETHOD(VMCWeakRef, operator=), asCALL_THISCALL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "ConstWeakRef<T> &opAssign(const ConstWeakRef<T> &in)", asMETHOD(VMCWeakRef, operator=), asCALL_THISCALL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "bool opEquals(const ConstWeakRef<T> &in) const", asMETHODPR(VMCWeakRef, operator==, (const VMCWeakRef &) const, bool), asCALL_THISCALL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "ConstWeakRef<T> &opHndlAssign(const T@)", asMETHOD(VMCWeakRef, Set), asCALL_THISCALL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "bool opEquals(const T@+) const", asMETHOD(VMCWeakRef, Equals), asCALL_THISCALL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "ConstWeakRef<T> &opHndlAssign(const WeakRef<T> &in)", asMETHOD(VMCWeakRef, operator=), asCALL_THISCALL);
			Engine->RegisterObjectMethod("ConstWeakRef<T>", "bool opEquals(const WeakRef<T> &in) const", asMETHODPR(VMCWeakRef, operator==, (const VMCWeakRef &) const, bool), asCALL_THISCALL);
			return true;
		}
		bool RegisterMathAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterGlobalFunction("float FpFromIEEE(uint)", asFUNCTIONPR(VMCMath::FpFromIEEE, (as_size_t), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("uint FpToIEEE(float)", asFUNCTIONPR(VMCMath::FpToIEEE, (float), as_size_t), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double FpFromIEEE(uint64)", asFUNCTIONPR(VMCMath::FpFromIEEE, (as_uint64_t), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("uint64 FpToIEEE(double)", asFUNCTIONPR(VMCMath::FpToIEEE, (double), as_uint64_t), asCALL_CDECL);
			Engine->RegisterGlobalFunction("bool CloseTo(float, float, float = 0.00001f)", asFUNCTIONPR(VMCMath::CloseTo, (float, float, float), bool), asCALL_CDECL);
			Engine->RegisterGlobalFunction("bool CloseTo(double, double, double = 0.0000000001)", asFUNCTIONPR(VMCMath::CloseTo, (double, double, double), bool), asCALL_CDECL);
#if !defined(_WIN32_WCE)
			Engine->RegisterGlobalFunction("float Cos(float)", asFUNCTIONPR(cosf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Sin(float)", asFUNCTIONPR(sinf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Tan(float)", asFUNCTIONPR(tanf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Acos(float)", asFUNCTIONPR(acosf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Asin(float)", asFUNCTIONPR(asinf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Atan(float)", asFUNCTIONPR(atanf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Atan2(float,float)", asFUNCTIONPR(atan2f, (float, float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Cosh(float)", asFUNCTIONPR(coshf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Sinh(float)", asFUNCTIONPR(sinhf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Tanh(float)", asFUNCTIONPR(tanhf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Log(float)", asFUNCTIONPR(logf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Log10(float)", asFUNCTIONPR(log10f, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Pow(float, float)", asFUNCTIONPR(powf, (float, float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Sqrt(float)", asFUNCTIONPR(sqrtf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Ceil(float)", asFUNCTIONPR(ceilf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Abs(float)", asFUNCTIONPR(fabsf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Floor(float)", asFUNCTIONPR(floorf, (float), float), asCALL_CDECL);
			Engine->RegisterGlobalFunction("float Fraction(float)", asFUNCTIONPR(fracf, (float), float), asCALL_CDECL);
#else
			Engine->RegisterGlobalFunction("double Cos(double)", asFUNCTIONPR(cos, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Sin(double)", asFUNCTIONPR(sin, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Tan(double)", asFUNCTIONPR(tan, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Acos(double)", asFUNCTIONPR(acos, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Asin(double)", asFUNCTIONPR(asin, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Atan(double)", asFUNCTIONPR(atan, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Atan2(double,double)", asFUNCTIONPR(atan2, (double, double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Cosh(double)", asFUNCTIONPR(cosh, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Sinh(double)", asFUNCTIONPR(sinh, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Tanh(double)", asFUNCTIONPR(tanh, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Log(double)", asFUNCTIONPR(log, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Log10(double)", asFUNCTIONPR(log10, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Pow(double, double)", asFUNCTIONPR(pow, (double, double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Sqrt(double)", asFUNCTIONPR(sqrt, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Ceil(double)", asFUNCTIONPR(ceil, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Abs(double)", asFUNCTIONPR(fabs, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Floor(double)", asFUNCTIONPR(floor, (double), double), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double Fraction(double)", asFUNCTIONPR(frac, (double), double), asCALL_CDECL);
#endif
			return true;
		}
		bool RegisterStringAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("String", sizeof(std::string), asOBJ_VALUE | asGetTypeTraits<std::string>());
			Engine->RegisterStringFactory("String", StringFactory::Get());
			Engine->RegisterObjectBehaviour("String", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(VMCString::Construct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("String", asBEHAVE_CONSTRUCT, "void f(const String &in)", asFUNCTION(VMCString::CopyConstruct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectBehaviour("String", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(VMCString::Destruct), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAssign(const String &in)", asMETHODPR(std::string, operator =, (const std::string&), std::string&), asCALL_THISCALL);
			Engine->RegisterObjectMethod("String", "String &opAddAssign(const String &in)", asFUNCTION(VMCString::AddAssignTo), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "bool opEquals(const String &in) const", asFUNCTIONPR(VMCString::Equals, (const std::string &, const std::string &), bool), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectMethod("String", "int opCmp(const String &in) const", asFUNCTION(VMCString::Cmp), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectMethod("String", "String opAdd(const String &in) const", asFUNCTIONPR(std::operator +, (const std::string &, const std::string &), std::string), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectMethod("String", "uint Size() const", asFUNCTION(VMCString::Length), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "void Resize(uint)", asFUNCTION(VMCString::Resize), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "bool IsEmpty() const", asFUNCTION(VMCString::IsEmpty), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "uint8 &opIndex(uint)", asFUNCTION(VMCString::CharAt), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "const uint8 &opIndex(uint) const", asFUNCTION(VMCString::CharAt), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAssign(double)", asFUNCTION(VMCString::AssignDoubleTo), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAddAssign(double)", asFUNCTION(VMCString::AddAssignDoubleTo), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String opAdd(double) const", asFUNCTION(VMCString::AddDouble1), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectMethod("String", "String opAdd_r(double) const", asFUNCTION(VMCString::AddDouble2), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAssign(float)", asFUNCTION(VMCString::AssignFloatTo), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAddAssign(float)", asFUNCTION(VMCString::AddAssignFloatTo), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String opAdd(float) const", asFUNCTION(VMCString::AddFloat1), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectMethod("String", "String opAdd_r(float) const", asFUNCTION(VMCString::AddFloat2), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAssign(int64)", asFUNCTION(VMCString::AssignInt64To), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAddAssign(int64)", asFUNCTION(VMCString::AddAssignInt64To), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String opAdd(int64) const", asFUNCTION(VMCString::AddInt641), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectMethod("String", "String opAdd_r(int64) const", asFUNCTION(VMCString::AddInt642), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAssign(uint64)", asFUNCTION(VMCString::AssignUInt64To), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAddAssign(uint64)", asFUNCTION(VMCString::AddAssignUInt64To), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String opAdd(uint64) const", asFUNCTION(VMCString::AddUInt641), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectMethod("String", "String opAdd_r(uint64) const", asFUNCTION(VMCString::AddUInt642), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAssign(bool)", asFUNCTION(VMCString::AssignBoolTo), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String &opAddAssign(bool)", asFUNCTION(VMCString::AddAssignBoolTo), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String opAdd(bool) const", asFUNCTION(VMCString::AddBool1), asCALL_CDECL_OBJFIRST);
			Engine->RegisterObjectMethod("String", "String opAdd_r(bool) const", asFUNCTION(VMCString::AddBool2), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String Substr(uint start = 0, int count = -1) const", asFUNCTION(VMCString::Sub), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "int FindFirst(const String &in, uint start = 0) const", asFUNCTION(VMCString::FindFirst), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "int FindFirstOf(const String &in, uint start = 0) const", asFUNCTION(VMCString::FindFirstOf), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "int FindFirstNotOf(const String &in, uint start = 0) const", asFUNCTION(VMCString::FindFirstNotOf), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "int FindLast(const String &in, int start = -1) const", asFUNCTION(VMCString::FindLast), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "int FindLastOf(const String &in, int start = -1) const", asFUNCTION(VMCString::FindLastOf), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "int FindLastNotOf(const String &in, int start = -1) const", asFUNCTION(VMCString::FindLastNotOf), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "void Insert(uint pos, const String &in other)", asFUNCTION(VMCString::Insert), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "void Erase(uint pos, int count = -1)", asFUNCTION(VMCString::Erase), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String Replace(const String &in, const String &in, uint64 o = 0)", asFUNCTION(VMCString::Replace), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "Array<String>@ Split(const String &in) const", asFUNCTION(VMCString::Split), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String ToLower() const", asFUNCTION(VMCString::ToLower), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String ToUpper() const", asFUNCTION(VMCString::ToUpper), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "String Reverse() const", asFUNCTION(VMCString::Reverse), asCALL_CDECL_OBJLAST);
			Engine->RegisterObjectMethod("String", "Address@ GetAddress() const", asFUNCTION(VMCString::ToPtr), asCALL_CDECL_OBJLAST);
			Engine->RegisterGlobalFunction("int64 ToInt(const String &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(VMCString::IntStore), asCALL_CDECL);
			Engine->RegisterGlobalFunction("uint64 ToUInt(const String &in, uint base = 10, uint &out byteCount = 0)", asFUNCTION(VMCString::UIntStore), asCALL_CDECL);
			Engine->RegisterGlobalFunction("double ToFloat(const String &in, uint &out byteCount = 0)", asFUNCTION(VMCString::FloatStore), asCALL_CDECL);
			Engine->RegisterGlobalFunction("uint8 ToChar(const String &in)", asFUNCTION(VMCString::ToChar), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(const Array<String> &in, const String &in)", asFUNCTION(VMCString::Join), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(int8)", asFUNCTION(VMCString::ToInt8), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(int16)", asFUNCTION(VMCString::ToInt16), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(int)", asFUNCTION(VMCString::ToInt), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(int64)", asFUNCTION(VMCString::ToInt64), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(uint8)", asFUNCTION(VMCString::ToUInt8), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(uint16)", asFUNCTION(VMCString::ToUInt16), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(uint)", asFUNCTION(VMCString::ToUInt), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(uint64)", asFUNCTION(VMCString::ToUInt64), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(float)", asFUNCTION(VMCString::ToFloat), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String ToString(double)", asFUNCTION(VMCString::ToDouble), asCALL_CDECL);

			return true;
		}
		bool RegisterExceptionAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterGlobalFunction("void Throw(const String &in = \"\")", asFUNCTION(VMCException::Throw), asCALL_CDECL);
			Engine->RegisterGlobalFunction("String GetException()", asFUNCTION(VMCException::GetException), asCALL_CDECL);
			return true;
		}
		bool RegisterMutexAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("Mutex", sizeof(VMCMutex), asOBJ_REF);
			Engine->RegisterObjectBehaviour("Mutex", asBEHAVE_FACTORY, "Mutex@ f()", asFUNCTION(VMCMutex::Factory), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Mutex", asBEHAVE_ADDREF, "void f()", asMETHOD(VMCMutex, AddRef), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Mutex", asBEHAVE_RELEASE, "void f()", asMETHOD(VMCMutex, Release), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Mutex", "bool TryLock()", asMETHOD(VMCMutex, TryLock), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Mutex", "void Lock()", asMETHOD(VMCMutex, Lock), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Mutex", "void Unlock()", asMETHOD(VMCMutex, Unlock), asCALL_THISCALL);
			return true;
		}
		bool RegisterThreadAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("Thread", 0, asOBJ_REF | asOBJ_GC);
			Engine->RegisterFuncdef("void ThreadEvent(Thread@+)");
			Engine->RegisterObjectBehaviour("Thread", asBEHAVE_FACTORY, "Thread@ f(ThreadEvent@)", asFUNCTION(VMCThread::Create), asCALL_GENERIC);
			Engine->RegisterObjectBehaviour("Thread", asBEHAVE_ADDREF, "void f()", asMETHOD(VMCThread, AddRef), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Thread", asBEHAVE_RELEASE, "void f()", asMETHOD(VMCThread, Release), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Thread", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(VMCThread, SetGCFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Thread", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(VMCThread, GetGCFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Thread", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(VMCThread, GetRefCount), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Thread", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(VMCThread, EnumReferences), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Thread", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(VMCThread, ReleaseReferences), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Thread", "bool IsActive()", asMETHOD(VMCThread, IsActive), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Thread", "bool Invoke()", asMETHOD(VMCThread, Start), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Thread", "void Suspend()", asMETHOD(VMCThread, Suspend), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Thread", "void Push(const ?&in)", asMETHOD(VMCThread, Push), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Thread", "bool Pop(?&out)", asMETHODPR(VMCThread, Pop, (void*, int), bool), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Thread", "bool Pop(?&out, uint64)", asMETHODPR(VMCThread, Pop, (void*, int, uint64_t), bool), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Thread", "int Join(uint64)", asMETHODPR(VMCThread, Join, (uint64_t), int), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Thread", "int Join()", asMETHODPR(VMCThread, Join, (), int), asCALL_THISCALL);
			Engine->RegisterGlobalFunction("Thread@+ GetThread()", asFUNCTION(VMCThread::GetThread), asCALL_CDECL);
			Engine->RegisterGlobalFunction("uint64 GetThreadId()", asFUNCTION(VMCThread::GetThreadId), asCALL_CDECL);
			Engine->RegisterGlobalFunction("void Sleep(uint64)", asFUNCTION(VMCThread::ThreadSleep), asCALL_CDECL);
			return true;
		}
		bool RegisterRandomAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("Random", 0, asOBJ_REF);
			Engine->RegisterObjectBehaviour("Random", asBEHAVE_FACTORY, "Random@ f()", asFUNCTION(VMCRandom::Create), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Random", asBEHAVE_ADDREF, "void f()", asMETHOD(VMCRandom, AddRef), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Random", asBEHAVE_RELEASE, "void f()", asMETHOD(VMCRandom, Release), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Random", "void opAssign(const Random&)", asMETHODPR(VMCRandom, Assign, (VMCRandom *), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Random", "void Seed(uint)", asMETHODPR(VMCRandom, Seed, (uint32_t), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Random", "void Seed(uint[]&)", asMETHODPR(VMCRandom, Seed, (VMCArray *), void), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Random", "int GetI()", asMETHOD(VMCRandom, GetI), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Random", "uint GetU()", asMETHOD(VMCRandom, GetU), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Random", "double GetD()", asMETHOD(VMCRandom, GetD), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Random", "void SeedFromTime()", asMETHOD(VMCRandom, SeedFromTime), asCALL_THISCALL);
			return true;
		}
		bool RegisterPromiseAPI(VMManager* Manager)
		{
			VMCManager* Engine = Manager->GetEngine();
			if (!Engine)
				return false;

			Engine->RegisterObjectType("Promise<class T>", 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(VMCArray::TemplateCallback), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_FACTORY, "Promise<T>@ f(int&in)", asFUNCTIONPR(VMCPromise::Create, (VMCTypeInfo*), VMCPromise*), asCALL_CDECL);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_ADDREF, "void f()", asMETHOD(VMCPromise, AddRef), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_RELEASE, "void f()", asMETHOD(VMCPromise, Release), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(VMCPromise, SetGCFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(VMCPromise, GetGCFlag), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(VMCPromise, GetRefCount), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(VMCPromise, EnumReferences), asCALL_THISCALL);
			Engine->RegisterObjectBehaviour("Promise<T>", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(VMCPromise, ReleaseReferences), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Promise<T>", "void Set(const ?&in)", asMETHODPR(VMCPromise, Set, (void*, int), int), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Promise<T>", "Any@+ GetSrc()", asMETHOD(VMCPromise, GetSrc), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Promise<T>", "const T& Get()", asMETHOD(VMCPromise, Get), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Promise<T>", "bool To(?&out)", asMETHODPR(VMCPromise, Retrieve, (void*, int), bool), asCALL_THISCALL);
			Engine->RegisterObjectMethod("Promise<T>", "Promise<T>@+ Await()", asMETHOD(VMCPromise, Await), asCALL_THISCALL);
			return true;
		}
		bool FreeCoreAPI()
		{
			StringFactory::Free();
			return false;
		}
	}
}
