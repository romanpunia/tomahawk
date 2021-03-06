#ifndef TH_SCRIPT_STD_API_H
#define TH_SCRIPT_STD_API_H

#include "../core/script.h"
#define TH_PROMISIFY(MemberFunction, TypeId) Tomahawk::Script::VMCPromise::Ify<decltype(&MemberFunction), &MemberFunction>::Id<TypeId>
#define TH_PROMISIFY_DECL(MemberFunction, TypeName) Tomahawk::Script::VMCPromise::Ify<decltype(&MemberFunction), &MemberFunction>::Decl<TypeName>
#define TH_ARRAYIFY(MemberFunction, TypeId) Tomahawk::Script::VMCArray::Ify<decltype(&MemberFunction), &MemberFunction>::Id<TypeId>
#define TH_ARRAYIFY_DECL(MemberFunction, TypeName) Tomahawk::Script::VMCArray::Ify<decltype(&MemberFunction), &MemberFunction>::Decl<TypeName>
#define TH_ANYIFY(MemberFunction, TypeId) Tomahawk::Script::VMCAny::Ify<decltype(&MemberFunction), &MemberFunction>::Id<TypeId>
#define TH_ANYIFY_DECL(MemberFunction, TypeName) Tomahawk::Script::VMCAny::Ify<decltype(&MemberFunction), &MemberFunction>::Decl<TypeName>

namespace Tomahawk
{
	namespace Script
	{
		class VMCArray;

		class TH_OUT VMCException
		{
		public:
			static void Throw(const std::string& In);
			static std::string GetException();
		};

		class TH_OUT VMCString
		{
		public:
			static void Construct(std::string *thisPointer);
			static void CopyConstruct(const std::string &other, std::string *thisPointer);
			static void Destruct(std::string *thisPointer);
			static std::string &AddAssignTo(const std::string &str, std::string &dest);
			static bool IsEmpty(const std::string &str);
			static void* ToPtr(const std::string& Value);
			static std::string Reverse(const std::string& Value);
			static std::string& AssignUInt64To(as_uint64_t i, std::string &dest);
			static std::string& AddAssignUInt64To(as_uint64_t i, std::string &dest);
			static std::string AddUInt641(const std::string &str, as_uint64_t i);
			static std::string AddInt641(as_int64_t i, const std::string &str);
			static std::string& AssignInt64To(as_int64_t i, std::string &dest);
			static std::string& AddAssignInt64To(as_int64_t i, std::string &dest);
			static std::string AddInt642(const std::string &str, as_int64_t i);
			static std::string AddUInt642(as_uint64_t i, const std::string &str);
			static std::string& AssignDoubleTo(double f, std::string &dest);
			static std::string& AddAssignDoubleTo(double f, std::string &dest);
			static std::string& AssignFloatTo(float f, std::string &dest);
			static std::string& AddAssignFloatTo(float f, std::string &dest);
			static std::string& AssignBoolTo(bool b, std::string &dest);
			static std::string& AddAssignBoolTo(bool b, std::string &dest);
			static std::string AddDouble1(const std::string &str, double f);
			static std::string AddDouble2(double f, const std::string &str);
			static std::string AddFloat1(const std::string &str, float f);
			static std::string AddFloat2(float f, const std::string &str);
			static std::string AddBool1(const std::string &str, bool b);
			static std::string AddBool2(bool b, const std::string &str);
			static char *CharAt(unsigned int i, std::string &str);
			static int Cmp(const std::string &a, const std::string &b);
			static int FindFirst(const std::string &sub, as_size_t start, const std::string &str);
			static int FindFirstOf(const std::string &sub, as_size_t start, const std::string &str);
			static int FindLastOf(const std::string &sub, as_size_t start, const std::string &str);
			static int FindFirstNotOf(const std::string &sub, as_size_t start, const std::string &str);
			static int FindLastNotOf(const std::string &sub, as_size_t start, const std::string &str);
			static int FindLast(const std::string &sub, int start, const std::string &str);
			static void Insert(unsigned int pos, const std::string &other, std::string &str);
			static void Erase(unsigned int pos, int count, std::string &str);
			static as_size_t Length(const std::string &str);
			static void Resize(as_size_t l, std::string &str);
			static std::string Replace(const std::string& a, const std::string& b, uint64_t o, const std::string& base);
			static as_int64_t IntStore(const std::string &val, as_size_t base, as_size_t *byteCount);
			static as_uint64_t UIntStore(const std::string &val, as_size_t base, as_size_t *byteCount);
			static double FloatStore(const std::string &val, as_size_t *byteCount);
			static std::string Sub(as_size_t start, int count, const std::string &str);
			static bool Equals(const std::string& lhs, const std::string& rhs);
			static std::string ToLower(const std::string& Symbol);
			static std::string ToUpper(const std::string& Symbol);
			static std::string ToInt8(char Value);
			static std::string ToInt16(short Value);
			static std::string ToInt(int Value);
			static std::string ToInt64(int64_t Value);
			static std::string ToUInt8(unsigned char Value);
			static std::string ToUInt16(unsigned short Value);
			static std::string ToUInt(unsigned int Value);
			static std::string ToUInt64(uint64_t Value);
			static std::string ToFloat(float Value);
			static std::string ToDouble(double Value);
			static VMCArray* Split(const std::string &delim, const std::string &str);
			static std::string Join(const VMCArray &array, const std::string &delim);
			static char ToChar(const std::string& Symbol);
		};

		class TH_OUT VMCMutex
		{
		private:
			std::mutex Base;
			int Ref;

		public:
			VMCMutex();
			void AddRef();
			void Release();
			bool TryLock();
			void Lock();
			void Unlock();
			
		public:
			static VMCMutex* Factory();
		};

		class TH_OUT VMCMath
		{
		public:
			static float FpFromIEEE(as_size_t raw);
			static as_size_t FpToIEEE(float fp);
			static double FpFromIEEE(as_uint64_t raw);
			static as_uint64_t FpToIEEE(double fp);
			static bool CloseTo(float a, float b, float epsilon);
			static bool CloseTo(double a, double b, double epsilon);
		};

		class TH_OUT VMCAny
		{
			friend class VMCPromise;

		protected:
			struct ValueStruct
			{
				union
				{
					as_int64_t ValueInt;
					double ValueFlt;
					void* ValueObj;
				};
				int TypeId;
			};

		protected:
			mutable int RefCount;
			mutable bool GCFlag;
			VMCManager* Engine;
			ValueStruct Value;

		public:
			VMCAny(VMCManager* Engine);
			VMCAny(void* Ref, int RefTypeId, VMCManager* Engine);
			VMCAny(const VMCAny&);
			int AddRef() const;
			int Release() const;
			VMCAny& operator= (const VMCAny&);
			int CopyFrom(const VMCAny* Other);
			void Store(void* Ref, int RefTypeId);
			bool Retrieve(void* Ref, int RefTypeId) const;
			int GetTypeId() const;
			int GetRefCount();
			void SetFlag();
			bool GetFlag();
			void EnumReferences(VMCManager* Engine);
			void ReleaseAllHandles(VMCManager* Engine);

		protected:
			virtual ~VMCAny();
			void FreeObject();

		public:
			static VMCAny* Create(int TypeId, void* Ref);
			static VMCAny* Create(const char* Decl, void* Ref);
			static void Factory1(VMCGeneric* G);
			static void Factory2(VMCGeneric* G);
			static VMCAny& Assignment(VMCAny* Other, VMCAny* Self);

		public:
			template <typename T, T>
			struct Ify;

			template <typename T, typename R, typename ...Args, R(T::* F)(Args...)>
			struct Ify<R(T::*)(Args...), F>
			{
				template <VMTypeId TypeId>
				static VMCAny* Id(T* Base, Args... Data)
				{
					R Subresult((Base->*F)(Data...));
					return VMCAny::Create((int)TypeId, &Subresult);
				}
				template <const char* TypeName>
				static VMCAny* Id(T* Base, Args... Data)
				{
					R Subresult((Base->*F)(Data...));
					return VMCAny::Create(TypeName, &Subresult);
				}
			};
		};

		class TH_OUT VMCArray
		{
		public:
			struct SBuffer
			{
				as_size_t MaxElements;
				as_size_t NumElements;
				unsigned char Data[1];
			};

			struct SCache
			{
				asIScriptFunction* CmpFunc;
				asIScriptFunction* EqFunc;
				int CmpFuncReturnCode;
				int EqFuncReturnCode;
			};

		protected:
			mutable int RefCount;
			mutable bool GCFlag;
			VMCTypeInfo* ObjType;
			SBuffer* Buffer;
			int ElementSize;
			int SubTypeId;

		public:
			void AddRef() const;
			void Release() const;
			VMCTypeInfo* GetArrayObjectType() const;
			int GetArrayTypeId() const;
			int GetElementTypeId() const;
			as_size_t GetSize() const;
			bool IsEmpty() const;
			void Reserve(as_size_t MaxElements);
			void Resize(as_size_t NumElements);
			void* At(as_size_t Index);
			const void* At(as_size_t Index) const;
			void SetValue(as_size_t Index, void* Value);
			VMCArray& operator= (const VMCArray&);
			bool operator== (const VMCArray&) const;
			void InsertAt(as_size_t Index, void* Value);
			void InsertAt(as_size_t Index, const VMCArray& Other);
			void InsertLast(void* Value);
			void RemoveAt(as_size_t Index);
			void RemoveLast();
			void RemoveRange(as_size_t start, as_size_t Count);
			void SortAsc();
			void SortDesc();
			void SortAsc(as_size_t StartAt, as_size_t Count);
			void SortDesc(as_size_t StartAt, as_size_t Count);
			void Sort(as_size_t StartAt, as_size_t Count, bool Asc);
			void Sort(VMCFunction* Less, as_size_t StartAt, as_size_t Count);
			void Reverse();
			int Find(void* Value) const;
			int Find(as_size_t StartAt, void* Value) const;
			int FindByRef(void* Ref) const;
			int FindByRef(as_size_t StartAt, void* Ref) const;
			void* GetBuffer();
			int GetRefCount();
			void SetFlag();
			bool GetFlag();
			void EnumReferences(VMCManager* Engine);
			void ReleaseAllHandles(VMCManager* Engine);

		protected:
			VMCArray(VMCTypeInfo* T, void* InitBuf);
			VMCArray(as_size_t Length, VMCTypeInfo* T);
			VMCArray(as_size_t Length, void* DefVal, VMCTypeInfo* T);
			VMCArray(const VMCArray& Other);
			virtual ~VMCArray();
			bool Less(const void* A, const void* B, bool Asc, VMCContext* Ctx, SCache* Cache);
			void* GetArrayItemPointer(int Index);
			void* GetDataPointer(void* Buffer);
			void Copy(void* Dst, void* Src);
			void Precache();
			bool CheckMaxSize(as_size_t NumElements);
			void Resize(int Delta, as_size_t At);
			void CreateBuffer(SBuffer** Buf, as_size_t NumElements);
			void DeleteBuffer(SBuffer* Buf);
			void CopyBuffer(SBuffer* Dst, SBuffer* Src);
			void Construct(SBuffer* Buf, as_size_t Start, as_size_t End);
			void Destruct(SBuffer* Buf, as_size_t Start, as_size_t End);
			bool Equals(const void* A, const void* B, VMCContext* Ctx, SCache* Cache) const;

		public:
			static VMCArray* Create(VMCTypeInfo* T);
			static VMCArray* Create(VMCTypeInfo* T, as_size_t Length);
			static VMCArray* Create(VMCTypeInfo* T, as_size_t Length, void* DefaultValue);
			static VMCArray* Create(VMCTypeInfo* T, void* ListBuffer);
			static void CleanupTypeInfoCache(VMCTypeInfo* Type);
			static bool TemplateCallback(VMCTypeInfo* T, bool& DontGarbageCollect);

		public:
			template <typename T>
			static VMCArray* Compose(VMCTypeInfo* ArrayType, const std::vector<T>& Objects)
			{
				VMCArray* Array = Create(ArrayType, (unsigned int)Objects.size());
				for (size_t i = 0; i < Objects.size(); i++)
					Array->SetValue((as_size_t)i, (void*)&Objects[i]);

				return Array;
			}
			template <typename T>
			static typename std::enable_if<std::is_pointer<T>::value, std::vector<T>>::type Decompose(VMCArray* Array)
			{
				std::vector<T> Result;
				if (!Array)
					return Result;

				unsigned int Size = Array->GetSize();
				Result.reserve(Size);

				for (unsigned int i = 0; i < Size; i++)
					Result.push_back((T)Array->At(i));

				return Result;
			}
			template <typename T>
			static typename std::enable_if<!std::is_pointer<T>::value, std::vector<T>>::type Decompose(VMCArray* Array)
			{
				std::vector<T> Result;
				if (!Array)
					return Result;

				unsigned int Size = Array->GetSize();
				Result.reserve(Size);

				for (unsigned int i = 0; i < Size; i++)
					Result.push_back(*((T*)Array->At(i)));

				return Result;
			}

		public:
			template <typename T, T>
			struct Ify;

			template <typename T, typename R, typename ...Args, std::vector<R>(T::* F)(Args...)>
			struct Ify<std::vector<R>(T::*)(Args...), F>
			{
				template <VMTypeId TypeId>
				static VMCArray* Id(T* Base, Args... Data)
				{
					VMManager* Manager = VMManager::Get();
					if (!Manager)
						return nullptr;

					VMCTypeInfo* Info = Manager->Global().GetTypeInfoById((int)TypeId).GetTypeInfo();
					if (!Info)
						return nullptr;

					std::vector<R> Source((Base->*F)(Data...));
					return VMCArray::Compose(Info, Source);
				}
				template <const char* TypeName>
				static VMCArray* Decl(T* Base, Args... Data)
				{
					VMManager* Manager = VMManager::Get();
					if (!Manager)
						return nullptr;

					VMCTypeInfo* Info = Manager->Global().GetTypeInfoByDecl(TypeName).GetTypeInfo();
					if (!Info)
						return nullptr;

					std::vector<R> Source((Base->*F)(Data...));
					return VMCArray::Compose(Info, Source);
				}
			};
		};

		class TH_OUT VMCMapKey
		{
		protected:
			friend class VMCMap;

		protected:
			union
			{
				as_int64_t ValueInt;
				double ValueFlt;
				void* ValueObj;
			};
			int TypeId;

		public:
			VMCMapKey();
			VMCMapKey(VMCManager* Engine, void* Value, int TypeId);
			~VMCMapKey();
			void Set(VMCManager* Engine, void* Value, int TypeId);
			void Set(VMCManager* Engine, VMCMapKey& Value);
			bool Get(VMCManager* Engine, void* Value, int TypeId) const;
			const void* GetAddressOfValue() const;
			int GetTypeId() const;
			void FreeValue(VMCManager* Engine);
			void EnumReferences(VMCManager* Engine);
		};

		class TH_OUT VMCMap
		{
		public:
			typedef std::unordered_map<std::string, VMCMapKey> Map;

		public:
			class Iterator
			{
			protected:
				friend class VMCMap;

			protected:
				Map::const_iterator It;
				const VMCMap& Dict;

			public:
				void operator++();
				void operator++(int);
				Iterator& operator*();
				bool operator==(const Iterator& Other) const;
				bool operator!=(const Iterator& Other) const;
				const std::string& GetKey() const;
				int GetTypeId() const;
				bool GetValue(void* Value, int TypeId) const;
				const void* GetAddressOfValue() const;

			protected:
				Iterator();
				Iterator(const VMCMap& Dict, Map::const_iterator It);
				Iterator& operator= (const Iterator&)
				{
					return *this;
				}
			};

			struct SCache
			{
				VMCTypeInfo* DictType;
				VMCTypeInfo* ArrayType;
				VMCTypeInfo* KeyType;
			};

		protected:
			VMCManager* Engine;
			mutable int RefCount;
			mutable bool GCFlag;
			Map Dict;

		public:
			void AddRef() const;
			void Release() const;
			VMCMap& operator= (const VMCMap& Other);
			void Set(const std::string& Key, void* Value, int TypeId);
			bool Get(const std::string& Key, void* Value, int TypeId) const;
			bool GetIndex(size_t Index, std::string* Key, void** Value, int* TypeId) const;
			VMCMapKey* operator[](const std::string& Key);
			const VMCMapKey* operator[](const std::string& Key) const;
			int GetTypeId(const std::string& Key) const;
			bool Exists(const std::string& Key) const;
			bool IsEmpty() const;
			as_size_t GetSize() const;
			bool Delete(const std::string& Key);
			void DeleteAll();
			VMCArray* GetKeys() const;
			Iterator Begin() const;
			Iterator End() const;
			Iterator Find(const std::string& Key) const;
			int GetRefCount();
			void SetGCFlag();
			bool GetGCFlag();
			void EnumReferences(VMCManager* Engine);
			void ReleaseAllReferences(VMCManager* Engine);

		protected:
			VMCMap(VMCManager* Engine);
			VMCMap(unsigned char* Buffer);
			VMCMap(const VMCMap&);
			virtual ~VMCMap();
			void Init(VMCManager* Engine);

		public:
			static VMCMap* Create(VMCManager* Engine);
			static VMCMap* Create(unsigned char* Buffer);
			static void Cleanup(VMCManager *engine);
			static void Setup(VMCManager *engine);
			static void Factory(VMCGeneric *gen);
			static void ListFactory(VMCGeneric *gen);
			static void KeyConstruct(void *mem);
			static void KeyDestruct(VMCMapKey *obj);
			static VMCMapKey &KeyopAssign(void *ref, int typeId, VMCMapKey *obj);
			static VMCMapKey &KeyopAssign(const VMCMapKey &other, VMCMapKey *obj);
			static VMCMapKey &KeyopAssign(double val, VMCMapKey *obj);
			static VMCMapKey &KeyopAssign(as_int64_t val, VMCMapKey *obj);
			static void KeyopCast(void *ref, int typeId, VMCMapKey *obj);
			static as_int64_t KeyopConvInt(VMCMapKey *obj);
			static double KeyopConvDouble(VMCMapKey *obj);
		};

		class TH_OUT VMCGrid
		{
		public:
			struct SBuffer
			{
				as_size_t Width;
				as_size_t Height;
				unsigned char Data[1];
			};

		protected:
			mutable int RefCount;
			mutable bool GCFlag;
			VMCTypeInfo* ObjType;
			SBuffer* Buffer;
			int ElementSize;
			int SubTypeId;

		public:
			void AddRef() const;
			void Release() const;
			VMCTypeInfo* GetGridObjectType() const;
			int GetGridTypeId() const;
			int GetElementTypeId() const;
			as_size_t GetWidth() const;
			as_size_t GetHeight() const;
			void Resize(as_size_t Width, as_size_t Height);
			void* At(as_size_t X, as_size_t Y);
			const void* At(as_size_t X, as_size_t Y) const;
			void  SetValue(as_size_t X, as_size_t Y, void* Value);
			int GetRefCount();
			void SetFlag();
			bool GetFlag();
			void EnumReferences(VMCManager* Engine);
			void ReleaseAllHandles(VMCManager* Engine);

		protected:
			VMCGrid(VMCTypeInfo* T, void* InitBuf);
			VMCGrid(as_size_t W, as_size_t H, VMCTypeInfo* T);
			VMCGrid(as_size_t W, as_size_t H, void* DefVal, VMCTypeInfo* T);
			virtual ~VMCGrid();
			bool CheckMaxSize(as_size_t X, as_size_t Y);
			void CreateBuffer(SBuffer** Buf, as_size_t W, as_size_t H);
			void DeleteBuffer(SBuffer* Buf);
			void Construct(SBuffer* Buf);
			void Destruct(SBuffer* Buf);
			void SetValue(SBuffer* Buf, as_size_t X, as_size_t Y, void* Value);
			void* At(SBuffer* Buf, as_size_t X, as_size_t Y);

		public:
			static VMCGrid* Create(VMCTypeInfo* T);
			static VMCGrid* Create(VMCTypeInfo* T, as_size_t Width, as_size_t Height);
			static VMCGrid* Create(VMCTypeInfo* T, as_size_t Width, as_size_t Height, void* DefaultValue);
			static VMCGrid* Create(VMCTypeInfo* T, void* ListBuffer);
			static bool TemplateCallback(VMCTypeInfo *TI, bool &DontGarbageCollect);
		};

		class TH_OUT VMCRef
		{
		protected:
			VMCTypeInfo* Type;
			void* Ref;

		public:
			VMCRef();
			VMCRef(const VMCRef& Other);
			VMCRef(void* Ref, VMCTypeInfo* Type);
			VMCRef(void* Ref, int TypeId);
			~VMCRef();
			VMCRef& operator=(const VMCRef& Other);
			void Set(void* Ref, VMCTypeInfo* Type);
			bool operator== (const VMCRef& Other) const;
			bool operator!= (const VMCRef& Other) const;
			bool Equals(void* Ref, int TypeId) const;
			void Cast(void** OutRef, int TypeId);
			VMCTypeInfo* GetType() const;
			int GetTypeId() const;
			void* GetRef();
			void EnumReferences(VMCManager* Engine);
			void ReleaseReferences(VMCManager* Engine);
			VMCRef& Assign(void* Ref, int TypeId);

		protected:
			void ReleaseHandle();
			void AddRefHandle();

		public:
			static void Construct(VMCRef *self);
			static void Construct(VMCRef *self, const VMCRef &o);
			static void Construct(VMCRef *self, void *ref, int typeId);
			static void Destruct(VMCRef *self);
		};

		class TH_OUT VMCWeakRef
		{
		protected:
			VMCLockableSharedBool* WeakRefFlag;
			VMCTypeInfo* Type;
			void* Ref;

		public:
			VMCWeakRef(VMCTypeInfo* Type);
			VMCWeakRef(const VMCWeakRef& Other);
			VMCWeakRef(void* Ref, VMCTypeInfo* Type);
			~VMCWeakRef();
			VMCWeakRef& operator= (const VMCWeakRef& Other);
			bool operator== (const VMCWeakRef& Other) const;
			bool operator!= (const VMCWeakRef& Other) const;
			VMCWeakRef& Set(void* NewRef);
			void* Get() const;
			bool Equals(void* Ref) const;
			VMCTypeInfo* GetRefType() const;

		public:
			static void Construct(VMCTypeInfo *type, void *mem);
			static void Construct2(VMCTypeInfo *type, void *ref, void *mem);
			static void Destruct(VMCWeakRef *obj);
			static bool TemplateCallback(VMCTypeInfo *TI, bool&);
		};

		class TH_OUT VMCComplex
		{
		public:
			float R;
			float I;

		public:
			VMCComplex();
			VMCComplex(const VMCComplex& Other);
			VMCComplex(float R, float I = 0);
			VMCComplex& operator= (const VMCComplex& Other);
			VMCComplex& operator+= (const VMCComplex& Other);
			VMCComplex& operator-= (const VMCComplex& Other);
			VMCComplex& operator*= (const VMCComplex& Other);
			VMCComplex& operator/= (const VMCComplex& Other);
			float Length() const;
			float SquaredLength() const;
			VMCComplex GetRI() const;
			void SetRI(const VMCComplex& In);
			VMCComplex GetIR() const;
			void SetIR(const VMCComplex& In);
			bool operator== (const VMCComplex& Other) const;
			bool operator!= (const VMCComplex& Other) const;
			VMCComplex operator+ (const VMCComplex& Other) const;
			VMCComplex operator- (const VMCComplex& Other) const;
			VMCComplex operator* (const VMCComplex& Other) const;
			VMCComplex operator/ (const VMCComplex& Other) const;

		public:
			static void DefaultConstructor(VMCComplex *self);
			static void CopyConstructor(const VMCComplex &other, VMCComplex *self);
			static void ConvConstructor(float r, VMCComplex *self);
			static void InitConstructor(float r, float i, VMCComplex *self);
			static void ListConstructor(float *list, VMCComplex *self);
		};

		class TH_OUT VMCThread
		{
		private:
			static int ContextUD;
			static int EngineListUD;

		private:
			struct
			{
				std::vector<VMCAny*> Queue;
				std::condition_variable CV;
				std::mutex Mutex;
			} Pipe[2];

		private:
			std::condition_variable CV;
			std::thread Thread;
			std::mutex Mutex;
			VMCFunction* Function;
			VMManager* Manager;
			VMContext* Context;
			bool GCFlag;
			int Ref;

		public:
			VMCThread(VMCManager* Engine, VMCFunction* Function);
			void EnumReferences(VMCManager* Engine);
			void SetGCFlag();
			void ReleaseReferences(VMCManager* Engine);
			void AddRef();
			void Release();
			void Suspend();
			void Resume();
			void Push(void* Ref, int TypeId);
			bool Pop(void* Ref, int TypeId);
			bool Pop(void* Ref, int TypeId, uint64_t Timeout);
			bool IsActive();
			bool Start();
			bool GetGCFlag();
			int GetRefCount();
			int Join(uint64_t Timeout);
			int Join();

		private:
			void Routine();

		public:
			static void Create(VMCGeneric* Generic);
			static VMCThread* GetThread();
			static uint64_t GetThreadId();
			static void ThreadSleep(uint64_t Timeout);
		};

		class TH_OUT VMCRandom
		{
		private:
			std::mt19937 Twister;
			std::uniform_real_distribution<double> DDist = std::uniform_real_distribution<double>(0.0, 1.0);
			int Ref = 1;

		public:
			VMCRandom();
			void AddRef();
			void Release();
			void SeedFromTime();
			uint32_t GetU();
			int32_t GetI();
			double GetD();
			void Seed(uint32_t Seed);
			void Seed(VMCArray* Array);
			void Assign(VMCRandom* From);

		public:
			static VMCRandom* Create();
		};

		class TH_OUT VMCPromise
		{
		private:
			VMCContext* Context;
			VMCAny* Any;
			VMCTypeInfo* Type;
			std::mutex Safe;
			int Ref;
			bool Stored;
			bool GCFlag;

		private:
			VMCPromise(VMCContext* Base, VMCTypeInfo* Info);

		public:
			void EnumReferences(VMCManager* Engine);
			void ReleaseReferences(VMCManager* Engine);
			void AddRef();
			void Release();
			void SetGCFlag();
			bool GetGCFlag();
			int GetRefCount();
			int Acquire(VMCAny* Value);
			int Set(void* Ref, int TypeId);
			int Set(void* Ref, const char* TypeId);
			bool Retrieve(void* Ref, int TypeId);
			void* Get();
			VMCAny* GetSrc();
			VMCPromise* Await();

		private:
			static int GetTypeId(const char* Name);

		public:
			static VMCPromise* Create(VMCTypeInfo* Type = nullptr);
			
		public:
			template <typename T, T>
			struct Ify;

			template <typename T, typename R, typename ...Args, Core::Async<R>(T::*F)(Args...)>
			struct Ify<Core::Async<R>(T::*)(Args...), F>
			{
				template <VMTypeId TypeId>
				static VMCPromise* Id(T* Base, Args... Data)
				{
					VMCPromise* Future = VMCPromise::Create();
					((Base->*F)(Data...)).Await([Future](R&& Result)
					{
						Future->Set((void*)&Result, (int)TypeId);
					});

					return Future;
				}
				template <const char* TypeName>
				static VMCPromise* Decl(T* Base, Args... Data)
				{
					VMCPromise* Future = VMCPromise::Create();
					((Base->*F)(Data...)).Await([Future](R&& Result)
					{
						Future->Set((void*)&Result, TypeName);
					});

					return Future;
				}
			};
		};

		TH_OUT bool RegisterAnyAPI(VMManager* Manager);
		TH_OUT bool RegisterArrayAPI(VMManager* Manager);
		TH_OUT bool RegisterComplexAPI(VMManager* Manager);
		TH_OUT bool RegisterMapAPI(VMManager* Manager);
		TH_OUT bool RegisterGridAPI(VMManager* Manager);
		TH_OUT bool RegisterRefAPI(VMManager* Manager);
		TH_OUT bool RegisterWeakRefAPI(VMManager* Manager);
		TH_OUT bool RegisterMathAPI(VMManager* Manager);
		TH_OUT bool RegisterStringAPI(VMManager* Manager);
		TH_OUT bool RegisterExceptionAPI(VMManager* Manager);
		TH_OUT bool RegisterMutexAPI(VMManager* Manager);
		TH_OUT bool RegisterThreadAPI(VMManager* Manager);
		TH_OUT bool RegisterRandomAPI(VMManager* Manager);
		TH_OUT bool RegisterPromiseAPI(VMManager* Manager);
		TH_OUT bool FreeCoreAPI();
	}
}
#endif
