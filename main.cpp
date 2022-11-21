#include <iostream>

#include <boost/version.hpp>
#include <boost/config.hpp>


#include "./tomfox/tomfox.hpp"

// 测试 boost 是否安装成功
void test_boost() {
    std::cout << BOOST_VERSION << std::endl;        //	boost版本号
    std::cout << BOOST_LIB_VERSION << std::endl;    //	boost版本号
    std::cout << BOOST_PLATFORM << std::endl;        //	操作系统
    std::cout << BOOST_COMPILER << std::endl;        //	编译器
    std::cout << BOOST_STDLIB << std::endl;          //	标准库
}

void test_start_server() {
    int max_thread_num = std::thread::hardware_concurrency();

    tomfox::http_server server(max_thread_num);

    std::string_view address = "127.0.0.1";
    std::string_view port = "8017";
    server.listen(address, port);
    std::cout << "服务线程数：" << max_thread_num << std::endl;
    std::cout << "服务监听端口：" << port << std::endl;
    server.set_http_handler<tomfox::GET, tomfox::POST>("/http", [](tomfox::request &req, tomfox::response &res) {
        char hello[] = "hello setsuna,i am tomfox!";
        res.set_status_and_content(tomfox::status_type::ok, hello);
        std::cout << "请求头: " << req.head() << std::endl;
        std::cout << "请求体: " << req.body() << std::endl;
    });

    server.run();
}

int main() {
    // 测试 boost 是否成功安装，CMake 相关配置是否正确
    test_boost();
    // web 容器测试
    test_start_server();
    return 0;
}







