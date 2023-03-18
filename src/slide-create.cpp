/**
 *   AutoCAD slide command line utility
 *
 *   Copyright (C) 2023 Dmitry Klionsky aka ten0s <dm.klionsky@gmail.com>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* SPDX-License-Identifier: GPL-3.0-or-later */

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <boost/program_options.hpp>

#include "../lib/slide.hpp"
#include "../lib/slide_binary_writer.hpp"
#include "../lib/slide_records.hpp"
#include "../lib/slide_record_text_parser.hpp"
#include "../lib/slide_records_visitor_stat.hpp"
#include "../lib/slide_endian.hpp"
#include "../lib/slide_util.hpp"
#include "../lib/slide_version.hpp"

namespace po = boost::program_options;
using namespace libslide;

// https://cairo.cairographics.narkive.com/H3TgPxCc/
// invalid-value-typically-too-big-for-the-size-of-the-input-surface-pattern-etc
static constexpr int16_t MAX_IMAGE_SIZE = 32767;

template<typename T>
static void
print_usage(std::ostream& os, const std::string& prog, const T& options)
{
    os << "Usage: " << prog << " [options] <FILE.sld> [FILE-RECORDS | -]\n"
       << options << "\n";
}

static std::vector<std::string>
read_lines(std::istream& is)
{
    std::vector<std::string> lines;
    for (std::string line; getline(is, line);) {
        lines.push_back(line);
    }
    return lines;
}

static std::vector<std::shared_ptr<SlideRecord>>
parse_records(const std::vector<std::string>& lines)
{
    std::vector<std::shared_ptr<SlideRecord>> records;
    for (const auto& line : lines) {
        auto record = parse_slide_record_text(line);
        if (record.get()) {
            records.push_back(record);
            if (dynamic_cast<SlideRecordEndOfFile*>(record.get()))
                break;
        }
    }
    // If there's no EOF record, add it explicitly
    if (!records.size() || !dynamic_cast<SlideRecordEndOfFile*>(records.back().get())) {
        records.push_back(std::make_shared<SlideRecordEndOfFile>());
    }
    return records;
}

static int
create_slide(const std::string& file,
             uint8_t version,
             uint16_t width,
             uint16_t height,
             double ratio,
             Endian endian,
             const std::vector<std::shared_ptr<SlideRecord>>& records)
{
    SlideHeader header{
        version,
        width,
        height,
        ratio,
        endian
    };

    Slide slide{
        basename(file),
        header,
        records,
        0
    };

    make_backup(file);
    std::ofstream ofs{file, std::ios::binary};
    write_slide_binary(ofs, slide);
    return 0;
}

int main(int argc, char* argv[])
{
    auto prog = basename(argv[0]);

    po::options_description generic("Generic options");
    generic.add_options()
        ("help", "print help")
        ("version", "print version")
        ;

    po::options_description config("Configuration");
    config.add_options()
        ("v1", "version 1.0")
        ("v2", "version 2.0, default")
        ("width,w",
         po::value<unsigned>(),
         ("width [1, " + std::to_string(MAX_IMAGE_SIZE) + "],\n"
          "automatic by default").c_str())
        ("height,h",
         po::value<unsigned>(),
         ("height [1, " + std::to_string(MAX_IMAGE_SIZE) + "],\n"
          "automatic by default").c_str())
        ("ratio,r",
         po::value<double>(),
         "aspect ratio,\n"
         "width/height by default")
        ("endian,e",
         po::value<std::string>(),
         "endian (native, little, big),\n"
         "native by default")
        ;

    po::options_description hidden("Hidden options");
    hidden.add_options()
       ("names",
         po::value<std::vector<std::string>>(),
         "FILE.sld [FILE-RECORDS]")
        ;

    po::options_description all_options;
    all_options.add(generic).add(config).add(hidden);

    po::options_description visible_options("Allowed options");
    visible_options.add(generic).add(config);

    po::positional_options_description p;
    p.add("names", -1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv)
                  .options(all_options)
                  .positional(p)
                  .run(), vm);
        po::notify(vm);
    } catch (const po::unknown_option& e) {
        std::cerr << "Error: Unknown option: " << e.get_option_name() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    if (vm.count("help")) {
        print_usage(std::cout, prog, visible_options);
        return 0;
    }

    if (vm.count("version")) {
        std::cout << VERSION << "\n";
        return 0;
    }

    uint8_t version = 2;
    if (vm.count("v1")) {
        version = 1;
    }

    uint16_t width = 0;
    if (vm.count("width")) {
        width = vm["width"].as<unsigned>();
        if (width == 0) {
            std::cerr << "Error: Invalid width: " << width << "\n";
        }
    }

    uint16_t height = 0;
    if (vm.count("height")) {
        height = vm["height"].as<unsigned>();
        if (height == 0) {
            std::cerr << "Error: Invalid height: " << height << "\n";
        }
    }

    double ratio = 0.0;
    if (vm.count("ratio")) {
        ratio = vm["ratio"].as<double>();
        if (ratio <= 0.0) {
            std::cerr << "Error: Invalid aspect ratio: " << ratio << "\n";
        }
    }

    Endian endian = Endian::native;
    if (vm.count("endian")) {
        auto opt = vm["endian"].as<std::string>();
        if (auto val = to_upper(opt); val == "NATIVE") {
            endian = Endian::native;
        } else if (val == "LITTLE") {
            endian = Endian::little;
        } else if (val == "BIG") {
            endian = Endian::big;
        } else {
            std::cerr << "Error: Invalid endian: " << opt << "\n";
        }
    }
    if (endian == Endian::native) {
        endian = native_endian();
    }

    if (vm.count("names")) {
        auto names = vm["names"].as<std::vector<std::string>>();
        if (names.size() >= 1) {
            auto file = names[0];
            auto ext = to_upper(get_ext(file));
            if (ext == ".SLD") {
                try {
                    std::vector<std::string> lines;
                    if (names.size() > 1) {
                        if (names[1] != "-") {
                            std::ifstream ifs{names[1]};
                            lines = read_lines(ifs);
                        } else {
                            lines = read_lines(std::cin);
                        }
                    }

                    auto records = parse_records(lines);

                    if (width == 0 || height == 0 || ratio == 0.0) {
                        SlideRecordsVisitorStat visitor;
                        for (auto& record : records) {
                            record->visit(visitor);
                        }
                        if (width == 0) {
                            width = visitor.max_x() + visitor.min_x() /* right pad */;
                        }
                        if (height == 0) {
                            height = visitor.max_y() + visitor.min_y() /* bottom pad */;
                        }
                        if (ratio == 0.0) {
                            ratio = 1.0 * width / height;
                        }
                    }

                    return create_slide(
                        file,
                        version,
                        width,
                        height,
                        ratio,
                        endian,
                        records
                    );
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                    return 1;
                }
            } else {
                std::cerr << "Error: Invalid slide extension: " << file << "\n";
                return 1;
            }
        }
    }

    print_usage(std::cerr, prog, visible_options);
    return 1;
}
