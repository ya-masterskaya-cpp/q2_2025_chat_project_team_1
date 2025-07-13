#include <gtest/gtest.h>


std::string IP_GLB;
std::string PORT_GLB;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    if(argc == 2) {
        std::string server_param = std::string(argv[1]);
        size_t delimiter_pos = server_param.find(':');
        IP_GLB = server_param.substr(0, delimiter_pos);
        PORT_GLB =  server_param.substr(delimiter_pos + 1);
    } else {
        ::testing::GTEST_FLAG(filter) = "-MessageHandler.*";
    }

    return RUN_ALL_TESTS();
}
