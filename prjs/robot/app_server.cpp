#include "robot.grpc.pb.h"
#include "grpcpp/grpcpp.h"
#include "absl/flags/parse.h"
#include "absl/flags/flag.h"
#include "absl/log/initialize.h"

class MyRobotService : public ::andy::Robot::Service
{
    public:
    MyRobotService(){}
    ~MyRobotService(){}
    ::andy::Position pos;

    public:
    ::grpc::Status MoveTo(::grpc::ServerContext* context, const ::andy::Position* request, ::andy::BoolMsg* response)
    {
        pos = *request;
        printf("Call MoveTo [%d, %d, %d].\r\n", request->x(), request->y(), request->z());
        response->set_success(true);
        return ::grpc::Status::OK;
    }
    ::grpc::Status getPostion(::grpc::ServerContext* context, const ::google::protobuf::Empty* request, ::andy::Position* response)
    {
        *response = pos;
        printf("Call getPostion\r\n");
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

    printf("serv Robot run [%s]\r\n", addr.c_str());
    auto serv = builder.BuildAndStart();
    serv->Wait();

    return 0;
}