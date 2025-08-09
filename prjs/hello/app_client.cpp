#include "hello.grpc.pb.h"
#include "grpcpp/grpcpp.h"
#include "absl/flags/parse.h"
#include "absl/flags/flag.h"
#include "absl/log/initialize.h"

ABSL_FLAG(std::string, target, "localhost:9900", "Server address");

int main(int argc, char** argv)
{
    absl::ParseCommandLine(argc, argv);
    absl::InitializeLog();

    auto addr = absl::GetFlag(FLAGS_target);
    printf("client request to %s\r\n", addr.c_str());

    auto chan = ::grpc::CreateChannel(addr, ::grpc::InsecureChannelCredentials());
    auto stub = ::andy::Greeter::NewStub(chan);

    // check connection
    // auto chan_status = chan->GetState(true);
    // if( chan_status != GRPC_CHANNEL_READY){
    //     printf("connection loss! please check server status!\r\n");
    //     return -1;
    // }

    auto req = ::andy::HelloRequest();
    req.set_name("client-1");
    auto resp = ::andy::HelloReply();

    {
        // method 1
        auto context = ::grpc::ClientContext();
        context.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(3)); // 3s timeout
        auto status = stub->SayHello(&context, req, &resp);
        if(status.ok()){
            printf("call SayHello ok! => %s\r\n", resp.message().c_str());
        }else{
            printf("call SayHello error: %s\r\n", status.error_message().c_str());
            printf("connection loss! please check server status!\r\n");
            return -1;
        }
    }

    // method 2, stream
    {
        auto context = ::grpc::ClientContext();        
        auto streamr = stub->SayHelloStreamReply(&context, req);
        while(streamr->Read(&resp)) {
            printf("call SayHelloStreamReply : %s\r\n", resp.message().c_str());
        }
        printf("call SayHelloStreamReply stream end.\r\n");
    }

    // method 3, both stream
    {
        auto context = ::grpc::ClientContext();        
        auto stream = stub->SayHelloBidiStream(&context);
        int i = 100;
        while(i--) {
            req.set_name("client msg:"+std::to_string(i));
            if(stream->Write(req)==false){
                stream->Read(&resp);
                printf("write fail, read from server: %s\r\n", resp.message().c_str());
            }
        }
        printf("call SayHelloBidiStream stream end.\r\n");
    }

    return 0;
}
