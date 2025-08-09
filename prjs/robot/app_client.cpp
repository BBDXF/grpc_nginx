
#include "robot.grpc.pb.h"
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
    auto stub = ::andy::Robot::NewStub(chan);

    auto req = ::andy::Position();
    req.set_x(100);
    req.set_y(22);
    req.set_z(-30);
    auto resp = ::andy::BoolMsg();

    {
        // method 1
        auto context = ::grpc::ClientContext();
        context.set_deadline(std::chrono::system_clock::now() + std::chrono::seconds(3)); // 3s timeout
        auto status = stub->MoveTo(&context, req, &resp);
        if(status.ok()){
            printf("call MoveTo ok! => %d\r\n", resp.success());
        }else{
            printf("call MoveTo error: %s\r\n", status.error_message().c_str());
            printf("connection loss! please check server status!\r\n");
            return -1;
        }
    }
    {
        // method 2
        auto context = ::grpc::ClientContext();
        auto empty = ::google::protobuf::Empty();
        auto status = stub->getPostion(&context, empty, &req);
        if(status.ok()){
            printf("call getPostion ok! => (%d, %d, %d)\r\n", req.x(),req.y(),req.z());
        }else{
            printf("call getPostion error: %s\r\n", status.error_message().c_str());
        }
    }

    return 0;
}