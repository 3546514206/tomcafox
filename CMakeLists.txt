cmake_minimum_required(VERSION 3.21)
project(cinatra)

# cinatra 需要 C++ 17 的支持
set(CMAKE_CXX_STANDARD 17)

# boost 配置
find_package(Boost 1.77.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOOST_LIBS_DIR})

# cinatra 依赖
set(cinatra
        cinatra/cinatra.hpp
        cinatra/cinatra/client_factory.hpp
        cinatra/cinatra/connection.hpp
        cinatra/cinatra/cookie.hpp
        cinatra/cinatra/define.h
        cinatra/cinatra/dh1024.pem
        cinatra/cinatra/function_traits.hpp
        cinatra/cinatra/gzip.hpp
        cinatra/cinatra/http_cache.hpp
        cinatra/cinatra/http_client.hpp
        cinatra/cinatra/http_parser.hpp
        cinatra/cinatra/http_router.hpp
        cinatra/cinatra/http_server.hpp
        cinatra/cinatra/io_service_pool.hpp
        cinatra/cinatra/itoa.hpp
        cinatra/cinatra/itoa_jeaiii.hpp
        cinatra/cinatra/mime_types.hpp
        cinatra/cinatra/modern_callback.h
        cinatra/cinatra/multipart_parser.hpp
        cinatra/cinatra/multipart_reader.hpp
        cinatra/cinatra/picohttpparser.h
        cinatra/cinatra/render.h
        cinatra/cinatra/request.hpp
        cinatra/cinatra/response.hpp
        cinatra/cinatra/response_cv.hpp
        cinatra/cinatra/response_parser.hpp
        cinatra/cinatra/router.hpp
        cinatra/cinatra/server.crt
        cinatra/cinatra/server.key
        cinatra/cinatra/session.hpp
        cinatra/cinatra/session_manager.hpp
        cinatra/cinatra/sha1.hpp
        cinatra/cinatra/smtp_client.hpp
        cinatra/cinatra/upload_file.hpp
        cinatra/cinatra/uri.hpp
        cinatra/cinatra/url_encode_decode.hpp
        cinatra/cinatra/use_asio.hpp
        cinatra/cinatra/utils.hpp
        cinatra/cinatra/websocket.hpp
        cinatra/cinatra/ws_define.h
        )


add_executable(tomfox main.cpp ${cinatra})
