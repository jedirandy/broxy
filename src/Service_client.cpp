#include <iostream>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <chrono>
#include "Service.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace ::broxy;

int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "args host url" << endl;
		return EXIT_FAILURE;
	}
	std::string host = std::string(argv[1]);
	std::string url = std::string(argv[2]);

	boost::shared_ptr<TTransport> socket(new TSocket(host, 9090));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	ServiceClient client(protocol);

	try {
		Request req;
		Response res;
		req.url = url;
		transport->open();
		auto start = std::chrono::system_clock::now();
		client.get(res, req);
		auto end = std::chrono::system_clock::now();
		cout << chrono::duration <double, milli> (end - start).count() << " ms" << endl;
		if (req.url == "@stats") {
			// print stats
			cout << res.body << endl;
		}
		transport->close();
	} catch (TException& tx) {
		cout << "ERROR: " << tx.what() << endl;
	}
}
