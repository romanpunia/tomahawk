#ifndef TH_NETWORK_MONGODB_H
#define TH_NETWORK_MONGODB_H

#include "../core/network.h"

struct _bson_t;
struct _mongoc_client_t;
struct _mongoc_uri_t;
struct _mongoc_database_t;
struct _mongoc_collection_t;
struct _mongoc_cursor_t;
struct _mongoc_bulk_operation_t;
struct _mongoc_client_pool_t;
struct _mongoc_change_stream_t;
struct _mongoc_client_session_t;

namespace Tomahawk
{
	namespace Network
	{
		namespace MDB
		{
			typedef _bson_t TDocument;
			typedef _mongoc_client_pool_t TConnectionPool;
			typedef _mongoc_bulk_operation_t TStream;
			typedef _mongoc_cursor_t TCursor;
			typedef _mongoc_collection_t TCollection;
			typedef _mongoc_database_t TDatabase;
			typedef _mongoc_uri_t TAddress;
			typedef _mongoc_client_t TConnection;
			typedef _mongoc_change_stream_t TWatcher;
			typedef _mongoc_client_session_t TTransaction;

			class Connection;

			class Queue;

			enum class Query
			{
				None = 0,
				Tailable_Cursor = 1 << 1,
				Slave_Ok = 1 << 2,
				Oplog_Replay = 1 << 3,
				No_Cursor_Timeout = 1 << 4,
				Await_Data = 1 << 5,
				Exhaust = 1 << 6,
				Partial = 1 << 7,
			};

			enum class Type
			{
				Unknown,
				Uncastable,
				Null,
				Document,
				Array,
				String,
				Integer,
				Number,
				Boolean,
				ObjectId,
				Decimal
			};

			enum class TransactionState
			{
				OK,
				Retry_Commit,
				Retry,
				Fatal
			};

			inline Query operator |(Query A, Query B)
			{
				return static_cast<Query>(static_cast<uint64_t>(A) | static_cast<uint64_t>(B));
			}

			struct TH_OUT Property
			{
				std::string Name;
				std::string String;
				TDocument* Object;
				TDocument* Array;
				Type Mod;
				int64_t Integer;
				uint64_t High;
				uint64_t Low;
				double Number;
				unsigned char ObjectId[12] = { 0 };
				bool Boolean;
				bool IsValid;

				Property();
				~Property();
				void Release();
				std::string& ToString();
			};

			class TH_OUT Util
			{
			public:
				static bool GetId(unsigned char* Id12);
				static bool GetDecimal(const char* Value, int64_t* High, int64_t* Low);
				static unsigned int GetHashId(unsigned char* Id12);
				static int64_t GetTimeId(unsigned char* Id12);
				static std::string IdToString(unsigned char* Id12);
				static std::string StringToId(const std::string& Id24);
			};

			class TH_OUT Document
			{
			private:
				TDocument* Base;
				bool Store;

			public:
				Document();
				Document(TDocument* NewBase);
				void Release() const;
				void Join(const Document& Value);
				void Loop(const std::function<bool(Property*)>& Callback) const;
				bool SetDocument(const char* Key, const Document& Value, uint64_t ArrayId = 0);
				bool SetArray(const char* Key, const Document& Array, uint64_t ArrayId = 0);
				bool SetString(const char* Key, const char* Value, uint64_t ArrayId = 0);
				bool SetBlob(const char* Key, const char* Value, uint64_t Length, uint64_t ArrayId = 0);
				bool SetInteger(const char* Key, int64_t Value, uint64_t ArrayId = 0);
				bool SetNumber(const char* Key, double Value, uint64_t ArrayId = 0);
				bool SetDecimal(const char* Key, uint64_t High, uint64_t Low, uint64_t ArrayId = 0);
				bool SetDecimalString(const char* Key, const std::string& Value, uint64_t ArrayId = 0);
				bool SetDecimalInteger(const char* Key, int64_t Value, uint64_t ArrayId = 0);
				bool SetDecimalNumber(const char* Key, double Value, uint64_t ArrayId = 0);
				bool SetBoolean(const char* Key, bool Value, uint64_t ArrayId = 0);
				bool SetObjectId(const char* Key, unsigned char Value[12], uint64_t ArrayId = 0);
				bool SetNull(const char* Key, uint64_t ArrayId = 0);
				bool Set(const char* Key, Property* Value, uint64_t ArrayId = 0);
				bool Has(const char* Key) const;
				bool Get(const char* Key, Property* Output) const;
				bool Find(const char* Key, Property* Output) const;
				uint64_t Count() const;
				std::string ToRelaxedJSON() const;
				std::string ToExtendedJSON() const;
				std::string ToJSON() const;
				std::string ToIndices() const;
				Core::Document* ToDocument(bool IsArray = false) const;
				TDocument* Get() const;
				Document Copy() const;
				Document& Persist(bool Keep = true);
				operator bool() const
				{
					return Base != nullptr;
				}

			public:
				static Document FromEmpty();
				static Document FromDocument(Core::Document* Document);
				static Document FromJSON(const std::string& JSON);
				static Document FromBuffer(const unsigned char* Buffer, uint64_t Length);
				static Document FromSource(TDocument* Src);

			private:
				static bool Clone(void* It, Property* Output);
			};

			class TH_OUT Address
			{
			private:
				TAddress* Base;

			public:
				Address(TAddress* NewBase);
				void Release();
				void SetOption(const char* Name, int64_t Value);
				void SetOption(const char* Name, bool Value);
				void SetOption(const char* Name, const char* Value);
				void SetAuthMechanism(const char* Value);
				void SetAuthSource(const char* Value);
				void SetCompressors(const char* Value);
				void SetDatabase(const char* Value);
				void SetUsername(const char* Value);
				void SetPassword(const char* Value);
				TAddress* Get() const;
				operator bool() const
				{
					return Base != nullptr;
				}

			public:
				static Address FromURI(const char* Value);
			};

			class TH_OUT Stream
			{
			private:
				TStream* Base;

			public:
				Stream(TStream* NewBase);
				void Release();
				bool RemoveMany(const Document& Select, const Document& Options);
				bool RemoveOne(const Document& Select, const Document& Options);
				bool ReplaceOne(const Document& Select, const Document& Replacement, const Document& Options);
				bool Insert(const Document& Result, const Document& Options);
				bool UpdateOne(const Document& Select, const Document& Result, const Document& Options);
				bool UpdateMany(const Document& Select, const Document& Result, const Document& Options);
				Core::Async<Document> ExecuteWithReply();
				Core::Async<bool> Execute();
				uint64_t GetHint() const;
				TStream* Get() const;
				operator bool() const
				{
					return Base != nullptr;
				}

			public:
				static Stream FromEmpty(bool IsOrdered);
			};

			class TH_OUT Cursor
			{
			private:
				TCursor* Base;

			public:
				Cursor();
				Cursor(TCursor* NewBase);
				void Release();
				void SetMaxAwaitTime(uint64_t MaxAwaitTime);
				void SetBatchSize(uint64_t BatchSize);
				bool SetLimit(int64_t Limit);
				bool SetHint(uint64_t Hint);
				bool HasError() const;
				bool HasMoreData() const;
				bool NextSync() const;
				Core::Async<bool> Next() const;
				int64_t GetId() const;
				int64_t GetLimit() const;
				uint64_t GetMaxAwaitTime() const;
				uint64_t GetBatchSize() const;
				uint64_t GetHint() const;
				Document GetCurrent() const;
				Cursor Clone();
				TCursor* Get() const;
				operator bool() const
				{
					return Base != nullptr;
				}
			};

			class TH_OUT Collection
			{
			private:
				TCollection* Base;

			public:
                Collection();
				Collection(TCollection* NewBase);
				void Release();
				Core::Async<bool> Rename(const std::string& NewDatabaseName, const std::string& NewCollectionName);
				Core::Async<bool> RenameWithOptions(const std::string& NewDatabaseName, const std::string& NewCollectionName, const Document& Options);
				Core::Async<bool> RenameWithRemove(const std::string& NewDatabaseName, const std::string& NewCollectionName);
				Core::Async<bool> RenameWithOptionsAndRemove(const std::string& NewDatabaseName, const std::string& NewCollectionName, const Document& Options);
				Core::Async<bool> Remove(const Document& Options);
				Core::Async<bool> RemoveIndex(const std::string& Name, const Document& Options);
				Core::Async<Document> RemoveMany(const Document& Select, const Document& Options);
				Core::Async<Document> RemoveOne(const Document& Select, const Document& Options);
				Core::Async<Document> ReplaceOne(const Document& Select, const Document& Replacement, const Document& Options);
				Core::Async<Document> InsertMany(std::vector<Document>& List, const Document& Options);
				Core::Async<Document> InsertOne(const Document& Result, const Document& Options);
				Core::Async<Document> UpdateMany(const Document& Select, const Document& Update, const Document& Options);
				Core::Async<Document> UpdateOne(const Document& Select, const Document& Update, const Document& Options);
				Core::Async<Document> FindAndModify(const Document& Select, const Document& Sort, const Document& Update, const Document& Fields, bool Remove, bool Upsert, bool New);
				Core::Async<uint64_t> CountElementsInArray(const Document& Match, const Document& Filter, const Document& Options) const;
				Core::Async<uint64_t> CountDocuments(const Document& Select, const Document& Options) const;
				Core::Async<uint64_t> CountDocumentsEstimated(const Document& Options) const;
				Core::Async<Cursor> FindIndexes(const Document& Options) const;
				Core::Async<Cursor> FindMany(const Document& Select, const Document& Options) const;
				Core::Async<Cursor> FindOne(const Document& Select, const Document& Options) const;
				Core::Async<Cursor> Aggregate(Query Flags, const Document& Pipeline, const Document& Options) const;
				const char* GetName() const;
				Stream CreateStream(const Document& Options);
				TCollection* Get() const;
				operator bool() const
				{
					return Base != nullptr;
				}
			};

			class TH_OUT Database
			{
			private:
				TDatabase* Base;

			public:
				Database(TDatabase* NewBase);
				void Release();
				Core::Async<bool> RemoveAllUsers();
				Core::Async<bool> RemoveUser(const std::string& Name);
				Core::Async<bool> Remove();
				Core::Async<bool> RemoveWithOptions(const Document& Options);
				Core::Async<bool> AddUser(const std::string& Username, const std::string& Password, const Document& Roles, const Document& Custom);
				Core::Async<bool> HasCollection(const std::string& Name) const;
				Core::Async<Collection> CreateCollection(const std::string& Name, const Document& Options);
				Core::Async<Cursor> FindCollections(const Document& Options) const;
				std::vector<std::string> GetCollectionNames(const Document& Options) const;
				const char* GetName() const;
				Collection GetCollection(const std::string& Name) const;
				TDatabase* Get() const;
				operator bool() const
				{
					return Base != nullptr;
				}
			};

			class TH_OUT Watcher
			{
			private:
				TWatcher * Base;

			public:
				Watcher(TWatcher* NewBase);
				void Release();
				Core::Async<bool> Next(const Document& Result) const;
				Core::Async<bool> Error(const Document& Result) const;
				bool NextSync(const Document& Result) const;
				bool ErrorSync(const Document& Result) const;
				TWatcher* Get() const;
				operator bool() const
				{
					return Base != nullptr;
				}

			public:
				static Watcher FromConnection(Connection* Connection, const Document& Pipeline, const Document& Options);
				static Watcher FromDatabase(const Database& Src, const Document& Pipeline, const Document& Options);
				static Watcher FromCollection(const Collection& Src, const Document& Pipeline, const Document& Options);
			};

			class TH_OUT Transaction
			{
			private:
				TTransaction* Base;

			public:
				Transaction(TTransaction* NewBase);
                bool Push(Document& QueryOptions) const;
				Core::Async<bool> Start();
				Core::Async<bool> Abort();
				Core::Async<Document> RemoveMany(const Collection& Base, const Document& Select, const Document& Options);
				Core::Async<Document> RemoveOne(const Collection& Base, const Document& Select, const Document& Options);
				Core::Async<Document> ReplaceOne(const Collection& Base, const Document& Select, const Document& Replacement, const Document& Options);
				Core::Async<Document> InsertMany(const Collection& Base, std::vector<Document>& List, const Document& Options);
				Core::Async<Document> InsertOne(const Collection& Base, const Document& Result, const Document& Options);
				Core::Async<Document> UpdateMany(const Collection& Base, const Document& Select, const Document& Update, const Document& Options);
				Core::Async<Document> UpdateOne(const Collection& Base, const Document& Select, const Document& Update, const Document& Options);
				Core::Async<Cursor> FindMany(const Collection& Base, const Document& Select, const Document& Options) const;
				Core::Async<Cursor> FindOne(const Collection& Base, const Document& Select, const Document& Options) const;
				Core::Async<Cursor> Aggregate(const Collection& Base, Query Flags, const Document& Pipeline, const Document& Options) const;
				Core::Async<TransactionState> Commit();
				TTransaction* Get() const;
				operator bool() const
				{
					return Base != nullptr;
				}
			};

			class TH_OUT Connection : public Core::Object
			{
				friend Queue;
				friend Transaction;

			private:
				std::atomic<bool> Connected;
				Transaction Session;
				TConnection* Base;
				Queue* Master;

			public:
				Connection();
				virtual ~Connection() override;
				Core::Async<bool> Connect(const std::string& Address);
				Core::Async<bool> Connect(Address* URI);
				Core::Async<bool> Disconnect();
				Core::Async<bool> MakeTransaction(const std::function<Core::Async<bool>(Transaction&)>& Callback);
				Core::Async<bool> MakeCotransaction(const std::function<bool(Transaction&)>& Callback);
				Core::Async<Cursor> FindDatabases(const Document& Options) const;
				void SetProfile(const std::string& Name);
				bool SetServer(bool Writeable);
				Transaction GetSession();
				Database GetDatabase(const std::string& Name) const;
				Database GetDefaultDatabase() const;
				Collection GetCollection(const char* DatabaseName, const char* Name) const;
				Address GetAddress() const;
				Queue* GetMaster() const;
				TConnection* Get() const;
				std::vector<std::string> GetDatabaseNames(const Document& Options) const;
				bool IsConnected() const;
			};

			class TH_OUT Queue : public Core::Object
			{
			private:
				std::atomic<bool> Connected;
				TConnectionPool* Pool;
				Address SrcAddress;

			public:
				Queue();
				virtual ~Queue() override;
				Core::Async<bool> Connect(const std::string& Address);
				Core::Async<bool> Connect(Address* URI);
				Core::Async<bool> Disconnect();
				void SetProfile(const char* Name);
				void Push(Connection** Client);
				Connection* Pop();
				TConnectionPool* Get() const;
				Address GetAddress() const;
			};

			class TH_OUT Driver
			{
			private:
				struct Pose
				{
					std::string Key;
					size_t Offset;
					bool Escape;
				};

				struct Sequence
				{
					std::vector<Pose> Positions;
					std::string Request;
					Document Cache;

					Sequence();
				};

			private:
				static std::unordered_map<std::string, Sequence>* Queries;
				static std::mutex* Safe;
				static std::atomic<int> State;

			public:
				static void Create();
				static void Release();
				static bool AddQuery(const std::string& Name, const char* Buffer, size_t Size);
				static bool AddDirectory(const std::string& Directory, const std::string& Origin = "");
				static bool RemoveQuery(const std::string& Name);
				static Document GetQuery(const std::string& Name, Core::DocumentArgs* Map, bool Once = true);
				static std::vector<std::string> GetQueries();

			private:
				static std::string GetJSON(Core::Document* Source, bool Escape);
			};
		}
	}
}
#endif
