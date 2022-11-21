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

void start_test_server() {
    int max_thread_num = std::thread::hardware_concurrency();
    tomfox::http_server server(max_thread_num);
    server.listen("0.0.0.0", "8017");
    server.set_http_handler<tomfox::GET, tomfox::POST>("/http",
                                                       [](tomfox::request &req, tomfox::response &res) {
                                                           char hello[] = "hello,i am tomfox!";
                                                           res.set_status_and_content(tomfox::status_type::ok,
                                                                                      hello);
                                                       });

    server.run();
}

int main() {
    test_boost();
    start_test_server();
    return 0;
}
