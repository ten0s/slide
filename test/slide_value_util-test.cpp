#include <gtest/gtest.h>
#include "../lib/slide_endian.hpp"
#include "../lib/slide_value_util.hpp"

using namespace libslide;

TEST(SlideValueUtilSuite, read_little) {
    uint8_t buf[] = { 0x12, 0x34 };
    auto out = read<uint16_t>(buf, Endian::LE);
    EXPECT_EQ(out, 0x3412);
}

TEST(SlideValueUtilSuite, read_big) {
    uint8_t buf[] = { 0x12, 0x34 };
    auto out = read<uint16_t>(buf, Endian::BE);
    EXPECT_EQ(out, 0x1234);
}

TEST(SlideValueUtilSuite, write_little) {
    uint8_t buf[2];
    uint16_t in = 0x1234;
    write(buf, in, Endian::LE);
    EXPECT_EQ(0x34, buf[0]);
    EXPECT_EQ(0x12, buf[1]);
}

TEST(SlideValueUtilSuite, write_big) {
    uint8_t buf[2];
    uint16_t in = 0x1234;
    write(buf, in, Endian::BE);
    EXPECT_EQ(0x12, buf[0]);
    EXPECT_EQ(0x34, buf[1]);
}

TEST(SlideValueUtilSuite, read_write_little) {
    uint8_t buf[4];
    uint32_t in = 0x1234;
    write(buf, in, Endian::LE);
    auto out = read<uint32_t>(buf, Endian::LE);
    EXPECT_EQ(out, in);
}

TEST(SlideValueUtilSuite, read_write_big) {
    uint8_t buf[4];
    uint32_t in = 0x1234;
    write(buf, in, Endian::BE);
    auto out = read<uint32_t>(buf, Endian::BE);
    EXPECT_EQ(out, in);
}

TEST(SlideValueUtilSuite, make) {
    EXPECT_EQ(0x1234, make<uint16_t>({0x12, 0x34}));
}

TEST(SlideValueUtilSuite, high_order_byte) {
    EXPECT_EQ(0x12, high_order_byte<uint16_t>(0x1234));
}

TEST(SlideValueUtilSuite, low_order_byte) {
    EXPECT_EQ(0x34, low_order_byte<uint16_t>(0x1234));
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
