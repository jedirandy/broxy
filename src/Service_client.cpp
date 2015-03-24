#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <chrono>
#include "../src/Service.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::broxy;

int main(int argc, char** argv) {
	boost::shared_ptr<TTransport> socket(new TSocket("localhost", 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	ServiceClient client(protocol);

	try {
		transport->open();

		auto start = std::chrono::system_clock::now();
		Request req;
//		req.url = "www.google.com";

		req.url = std::string(argv[1]);
		Response res;
		client.get(res, req);
		auto end = std::chrono::system_clock::now();
		cout << chrono::duration <double, milli> (end - start).count() << " ms" << endl;
		transport->close();
	} catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}
}
