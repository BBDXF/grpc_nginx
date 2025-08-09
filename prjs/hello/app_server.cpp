#include "hello.grpc.pb.h"
#include "grpcpp/grpcpp.h"
#include "absl/flags/parse.h"
#include "absl/flags/flag.h"
#include "absl/log/initialize.h"

class MyRobotService : public ::andy::Greeter::Service
{
    public:
    MyRobotService(){}
    ~MyRobotService(){}

    public:
    // Sends a greeting
    ::grpc::Status SayHello(::grpc::ServerContext* context, const ::andy::HelloRequest* request, ::andy::HelloReply* response)
    {
        auto req_msg = request->name();
        response->set_message("Hello ["+req_msg+"]");
        printf("SayHello: req name: %s\r\n", req_msg.c_str());
        return ::grpc::Status::OK;
    }
    ::grpc::Status SayHelloStreamReply(::grpc::ServerContext* context, const ::andy::HelloRequest* request, ::grpc::ServerWriter< ::andy::HelloReply>* writer)
    {
        auto req_msg = request->name();
        printf("SayHelloStreamReply: req name: %s\r\n", req_msg.c_str());
        ::andy::HelloReply rep;
        for(auto i=0; i< 10; i++){
            rep.set_message(req_msg+" =>"+std::to_string(i+1));
            writer->Write(rep);
            sleep(2);
        }
        return ::grpc::Status::OK;
    }
    ::grpc::Status SayHelloBidiStream(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::andy::HelloReply, ::andy::HelloRequest>* stream)
    {
        printf("SayHelloBidiStream req.\r\n");
        int count = 0;
        ::andy::HelloRequest req;
        ::andy::HelloReply resp;
        while(stream->Read(&req)) {
            printf("read %s - %d ...\r\n", req.name().c_str(), count);
            count += 1;
            if(count%3 == 0){
                resp.set_message("i got it 3 times!\r\n");
                stream->Write(resp);
            }
        }
        printf("SayHelloBidiStream end.\r\n");
        return ::grpc::Status::OK;
    }
};

ABSL_FLAG(std::string, target, "localhost:9900", "Server address");

int main(int argc, char** argv)
{
    absl::ParseCommandLine(argc, argv);
    absl::InitializeLog();

    auto addr = absl::GetFlag(FLAGS_target);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(addr, grpc::InsecureServerCredentials());

    MyRobotService myService;
    builder.RegisterService(&myService);

    printf("serv Hello run [%s]\r\n", addr.c_str());
    auto serv = builder.BuildAndStart();
    serv->Wait();

    return 0;
}