cmake_minimum_required(VERSION 3.21)
project(tomfox)

# tomfox 需要 C++ 17 的支持
set(CMAKE_CXX_STANDARD 17)

# boost 配置
find_package(Boost 1.77.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${BOOST_LIBS_DIR})

# 日志框架 spdlog
if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()

# tomfox 依赖
set(tomfox
        tomfox/tomfox.hpp
        tomfox/tomfox/client_factory.hpp
        tomfox/tomfox/connection.hpp
        tomfox/tomfox/cookie.hpp
        tomfox/tomfox/define.h
        tomfox/tomfox/dh1024.pem
        tomfox/tomfox/function_traits.hpp
        tomfox/tomfox/gzip.hpp
        tomfox/tomfox/http_cache.hpp
        tomfox/tomfox/http_client.hpp
        tomfox/tomfox/http_parser.hpp
        tomfox/tomfox/http_router.hpp
        tomfox/tomfox/http_server.hpp
        tomfox/tomfox/io_service_pool.hpp
        tomfox/tomfox/itoa.hpp
        tomfox/tomfox/itoa_jeaiii.hpp
        tomfox/tomfox/mime_types.hpp
        tomfox/tomfox/modern_callback.h
        tomfox/tomfox/multipart_parser.hpp
        tomfox/tomfox/multipart_reader.hpp
        tomfox/tomfox/picohttpparser.h
        tomfox/tomfox/request.hpp
        tomfox/tomfox/response.hpp
        tomfox/tomfox/response_cv.hpp
        tomfox/tomfox/response_parser.hpp
        tomfox/tomfox/router.hpp
        tomfox/tomfox/server.crt
        tomfox/tomfox/server.key
        tomfox/tomfox/session.hpp
        tomfox/tomfox/session_manager.hpp
        tomfox/tomfox/sha1.hpp
        tomfox/tomfox/smtp_client.hpp
        tomfox/tomfox/upload_file.hpp
        tomfox/tomfox/uri.hpp
        tomfox/tomfox/url_encode_decode.hpp
        tomfox/tomfox/use_asio.hpp
        tomfox/tomfox/utils.hpp
        tomfox/tomfox/websocket.hpp
        tomfox/tomfox/ws_define.h
        tomfox/tomfox/nlohmann_json.hpp
        tomfox/tomfox/render.h
        )


add_executable(tomfox main.cpp ${cinatra})

# 链接日志框架
target_link_libraries(tomfox PRIVATE spdlog::spdlog)