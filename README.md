# GRPC nginx proxy demo

- grpc C++ build
- grpc C++ hello demo
- grpc C++ robot demo
- nginx conf work with grpc
- cmake build
- grpc client work with nginx

# Work around
- cd `myrpc`
- Build grpc `./build_grpc.sh`
- Build demo `./build_prj.sh hello` then `./build_prj.sh robot`
- cd `prjs`
- Run grpc service `./run.sh`
- Run nginx server `nginx -s stop & nginx -c ~/myrpc/prjs/nginx_grpc.conf`
- Run grpc client and curl `./client.sh`
- Loopup server port status `netstat -ntlp`
- Kill servers `./kill_servers.sh`

# test result
- grpc server and client: passed
- nginx proxy grpc server: passed (without ssl)
- grpc client vist nginx grpc addr: passed (without ssl)
- curl vist http2 static files: passed (without ssl)
- Chrome vist http2 static files: failed `broser not support http2 without ssl`

So, if user need grpc server and http server on same port, need use ssl support both for grpc client and nginx.

# Author
BBDXF

