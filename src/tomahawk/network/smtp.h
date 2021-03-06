#ifndef TH_NETWORK_SMTP_H
#define TH_NETWORK_SMTP_H

#include "../core/network.h"

namespace Tomahawk
{
	namespace Network
	{
		namespace SMTP
		{
			typedef std::function<void()> ReplyCallback;

			enum class Priority
			{
				High = 2,
				Normal = 3,
				Low = 4
			};

			struct TH_OUT Recipient
			{
				std::string Name;
				std::string Address;
			};

			struct TH_OUT Attachment
			{
				std::string Path;
				uint64_t Length = 0;
			};

			struct TH_OUT RequestFrame
			{
				std::vector<Recipient> Recipients;
				std::vector<Recipient> CCRecipients;
				std::vector<Recipient> BCCRecipients;
				std::vector<Attachment> Attachments;
				std::vector<std::string> Messages;
				std::string SenderName;
				std::string SenderAddress;
				std::string Subject;
				std::string Charset = "utf-8";
				std::string Mailer;
				std::string Receiver;
				std::string Login;
				std::string Password;
				std::string Hostname;
				Priority Prior = Priority::Normal;
				bool Authenticate = true;
				bool NoNotification = false;
				bool AllowHTML = false;
				char RemoteAddress[48] = { 0 };
			};

			class TH_OUT Client final : public SocketClient
			{
			private:
				FILE* AttachmentFile;
				std::string Buffer;
				std::string Command;
				std::string Boundary;
				RequestFrame Request;
				int64_t Pending;
				bool Staging;
				bool Authorized;

			public:
				Client(int64_t ReadTimeout);
				virtual ~Client() override;
				Core::Async<int> Send(RequestFrame* Root);
				RequestFrame* GetRequest();

			private:
				bool OnResolveHost(Host* Address) override;
				bool OnConnect() override;
				bool OnClose() override;
				bool Authorize(const ReplyCallback& Callback);
				bool SendAttachment();
				bool ProcessAttachment();
				bool ReadResponses(int Code, const ReplyCallback& Callback);
				bool ReadResponse(int Code, const ReplyCallback& Callback);
				bool SendRequest(int Code, const std::string& Content, const ReplyCallback& Callback);
				bool CanRequest(const char* Keyword);
				unsigned char* Unicode(const char* String);
			};
		}
	}
}
#endif