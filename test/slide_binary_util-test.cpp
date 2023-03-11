#include <gtest/gtest.h>
#include "../lib/slide_endian.hpp"
#include "../lib/slide_binary_util.hpp"

using namespace libslide;

TEST(SlideBinaryUtilSuite, read_little) {
    uint8_t buf[] = { 0x12, 0x34 };
    auto out = read<uint16_t>(buf, Endian::LE);
    EXPECT_EQ(out, 0x3412);
}

TEST(SlideBinaryUtilSuite, read_big) {
    uint8_t buf[] = { 0x12, 0x34 };
    auto out = read<uint16_t>(buf, Endian::BE);
    EXPECT_EQ(out, 0x1234);
}

TEST(SlideBinaryUtilSuite, write_little) {
    uint8_t buf[2];
    uint16_t in = 0x1234;
    write(buf, in, Endian::LE);
    EXPECT_EQ(0x34, buf[0]);
    EXPECT_EQ(0x12, buf[1]);
}

TEST(SlideBinaryUtilSuite, write_big) {
    uint8_t buf[2];
    uint16_t in = 0x1234;
    write(buf, in, Endian::BE);
    EXPECT_EQ(0x12, buf[0]);
    EXPECT_EQ(0x34, buf[1]);
}

TEST(SlideBinaryUtilSuite, read_write_little) {
    uint8_t buf[4];
    uint32_t in = 0x1234;
    write(buf, in, Endian::LE);
    auto out = read<uint32_t>(buf, Endian::LE);
    EXPECT_EQ(out, in);
}

TEST(SlideBinaryUtilSuite, read_write_big) {
    uint8_t buf[4];
    uint32_t in = 0x1234;
    write(buf, in, Endian::BE);
    auto out = read<uint32_t>(buf, Endian::BE);
    EXPECT_EQ(out, in);
}

TEST(SlideBinaryUtilSuite, make) {
    EXPECT_EQ(0x1234, make<uint16_t>({0x12, 0x34}));
}

TEST(SlideBinaryUtilSuite, high_order_byte) {
    EXPECT_EQ(0x12, high_order_byte<uint16_t>(0x1234));
}

TEST(SlideBinaryUtilSuite, low_order_byte) {
    EXPECT_EQ(0x34, low_order_byte<uint16_t>(0x1234));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
