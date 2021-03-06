#ifndef	_PIACTIONSUPERVISORIPC_H
#define	_PIACTIONSUPERVISORIPC_H

#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

#include "PIAction.h"
#include "PIObject.h"
#include "PIClientStub.h"

class CPIActionSupervisorIPC : public IPIAction, public CPIObject {
	public:
		CPIActionSupervisorIPC();
		virtual ~CPIActionSupervisorIPC();

		// variables
	protected:
		int serverFd;
		struct sockaddr_in serverAddr;
		unsigned int serverPort;
		pthread_t serverThread;
		pthread_attr_t serverThreadAttr;
		pthread_mutex_t mutexClient;
		pthread_cond_t condClient;
		enum { STATE_WAIT, STATE_SIG } stateClient;
		pthread_t clientThread;
		pthread_attr_t clientThreadAttr;
		bool isContinue;
		bool runningServerThread;
		bool runningClientThread;

	public:
		CPIClientStub::LIST client;

		// method
	protected:	
		bool initializeTCP(void);
		bool startThreads(void);
		void waitThreads(void);
		bool isRunningServerThread(void);
		bool isRunningClientThread(void);
		void clearClient(void);
    
		bool parseClientPacket( CPIClientStub* pClient, CPIPacket& packet);
		bool sendResponse(CPIClientStub* pClient, const int command, const int resultCode, const std::string& resultValue);
		bool processPacket(CPIPacket& packet, int& resultCode, std::string& resultValue);
    
	public:
		virtual bool initialize(void);
		virtual bool finalize(void);
		virtual void clear(void);

	public:
		virtual int run(const std::string& param);
		virtual int stop(void);
	
	public:
		static CPIActionSupervisorIPC& getInstance(void);
		static void* fnCommunicateClient(void* pzArg);
		static void* fnWaitClient(void* pzArg);
};
#endif // #ifndef _PIACTIONSUPERVISORIPC_H
