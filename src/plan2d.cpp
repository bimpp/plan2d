/*
 * The MIT License (MIT)
 * Copyright © 2020 BIM++
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the “Software”),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include <bimpp/plan2d.hpp>

#if defined(WIN32) && !defined(NDEBUG)
#include <crtdbg.h>
#endif

int main(int argc, char* argv[])
{
#if defined(WIN32) && !defined(NDEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    /// Make a house
    bimpp::plan2d::house<double> bimpp_house;

    for (size_t y = 0; y < 5; ++y)
    {
        for (size_t x = 0; x < 5; ++x)
        {
            bimpp_house.nodes.insert(std::make_pair<>(x * 10 + y, bimpp::plan2d::node<>(x, y)));
        }
    }

    for (size_t y = 0; y < 5; ++y)
    {
        for (size_t x = 0; x < 5; ++x)
        {
            if (x > 0)
            {
                bimpp_house.walls.insert(std::make_pair<>(1000 + y * 100 + (x - 1) * 10 + x, bimpp::plan2d::wall<>((x - 1) * 10 + y, x * 10 + y)));
            }
            if (y > 0)
            {
                bimpp_house.walls.insert(std::make_pair<>(2000 + x * 100 + (y - 1) * 10 + y, bimpp::plan2d::wall<>(x * 10 + (y - 1), x * 10 + y)));
            }
        }
    }

    /// Make a room that has some complex edges
    bimpp::plan2d::room<> bimpp_room;
    bimpp_room.wall_ids = {
        1012, 1034, 1101, 1112, 1123, 1201, 1234, 1312, 1323, 1423, 1434,
        2001, 2012, 2023, 2034, 2101, 2112, 2123, 2134, 2201, 2223, 2234, 2301, 2334, 2401, 2412, 2423, 2434
    };
    bimpp_house.rooms.insert(std::make_pair<>(0, bimpp_room));

    bimpp::plan2d::algorithm<>::room_ex_vector bimpp_room_exs;
    bimpp::plan2d::algorithm<>::calculateRoomExs(bimpp_house, bimpp_room_exs);
    return 0;
}
