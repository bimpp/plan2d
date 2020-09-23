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
/*!
 * @file plan2d.hpp
 * 
 * All about the plan in 2D, and are inspired by [IFC](https://technical.buildingsmart.org/standards/ifc/)
 */
#pragma once

#include <cstdint>
#include <cmath>
#include <cassert>
#include <stdexcept>

#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>

namespace bimpp
{
    namespace plan2d
    {
        template<typename T = double>
        class point
        {
        public:
            typedef T   precision_type;

        public:
            point(precision_type _x = 0, precision_type _y = 0)
                : data({ _x, _y })
            {}

        public:
            inline precision_type x() const
            {
                return data[0];
            }

            inline point& x(precision_type _v)
            {
                data[0] = _v;
                return *this;
            }

            inline precision_type y() const
            {
                return data[1];
            }

            inline point& y(precision_type _v)
            {
                data[1] = _v;
                return *this;
            }

        public:
            bool operator==(const point& _a) const
            {
                return (data == _a.data);
            }

            bool operator<(const point& _a) const
            {
                if (data[0] < _a.data[0])
                {
                    return true;
                }
                if (data[0] > _a.data[0])
                {
                    return false;
                }
                return (data[1] < _a.data[1]);
            }

        public:
            std::array<precision_type, 2> data;
        };

        /*!
         * Define some classes and declare some constant values
         */
        template<typename TPrecision = double, typename TPoint = point<TPrecision>>
        class constant
        {
        public:
            typedef TPrecision      precision_type;
            /// Define the point type
            typedef TPoint          point_type;
            typedef size_t          id_type;

        public:
            static const point_type zero_point;     ///< Origin point
            static const point_type unit_point;     ///< Unit point
            static const id_type    none_id;        ///< Invalid id

        public:
            /*!
             * Is id valid?
             */
            static bool isValid(id_type _id)
            {
                return (_id != none_id);
            }

            template<typename TV>
            static TPrecision convert(TV _v)
            {
                return static_cast<TPrecision>(_v);
            }
        };

        template<typename TPrecision, typename TPoint>
        const typename constant<TPrecision, TPoint>::point_type     constant<TPrecision, TPoint>::zero_point(0, 0);
        template<typename TPrecision, typename TPoint>
        const typename constant<TPrecision, TPoint>::point_type     constant<TPrecision, TPoint>::unit_point(1, 1);
        template<typename TPrecision, typename TPoint>
        const typename constant<TPrecision, TPoint>::id_type        constant<TPrecision, TPoint>::none_id(-1);

        /*!
         * A node represents a point or a joint with two walls in the 2D plan
         * 
         * @brief A point or a joint in the plan
         */
        template<typename TConstant = constant<>>
        class node
        {
        public:
            typedef typename TConstant::precision_type  precision_type;
            typedef typename TConstant::point_type      point_type;

        public:
            /*!
             * A constructor by a 2D coordinate
             * 
             * @param _x The value in x-axis, default is 0.
             * @param _y The value in y-axis, default is 0.
             */
            node(precision_type _x = 0, precision_type _y = 0)
                : point(_x, _y)
            {}

            /*!
             * A constructor by a 2D point
             * 
             * @param _point A 2D point, default is origin point.
             */
            node(const point_type& _point = TConstant::zero_point)
                : point(_point)
            {}

        public:
            inline precision_type x() const
            {
                return point.x();
            }

            inline node& x(precision_type _x)
            {
                point.x(_x);
                return *this;
            }

            inline precision_type y() const
            {
                return point.y();
            }

            inline node& y(precision_type _x)
            {
                point.y(_x);
                return *this;
            }

        private:
            /*!
             * The point or position of the node
             */
            point_type point;
        };

        /*!
         * A wall represents a wall in the 2D plan
         */
        template<typename TConstant = constant<>>
        class wall
        {
        public:
            typedef typename TConstant::precision_type  precision_type;

        public:
            wall(size_t _start_node_id = TConstant::none_id
                , size_t _end_node_id = TConstant::none_id
                , precision_type _thickness = 0)
                : kind("")
                , start_node_id(_start_node_id)
                , end_node_id(_end_node_id)
                , thickness(_thickness)
            {}

        public:
            inline bool isValid() const
            {
                return (TConstant::isValid(start_node_id)
                    && TConstant::isValid(end_node_id)
                    && start_node_id != end_node_id
                    && thickness >= 0);
            }

        public:
            /// The kind of wall
            std::string     kind;
            /// The start of wall
            size_t          start_node_id;
            /// The end of wall
            size_t          end_node_id;
            /// The thickness of wall
            precision_type  thickness;
        };

        /*!
         * A hole represents a hole in the 2D plan,
         * it might be a window or a door or a hole in a wall.
         */
        template<typename TConstant = constant<>>
        class hole
        {
        public:
            typedef typename TConstant::precision_type  precision_type;

        public:
            hole(size_t _wall_id = TConstant::none_id
                , precision_type _distance = 0
                , precision_type _width = 0)
                : kind("")
                , direction("")
                , wall_id(_wall_id)
                , distance(_distance)
                , width(_width)
            {}

        public:
            inline bool isValid() const
            {
                return (TConstant::isValid(wall_id)
                    && distance != 0
                    && width != 0);
            }

        public:
            std::string     kind;
            std::string     direction;
            size_t          wall_id;
            precision_type  distance;
            precision_type  width;
        };

        template<typename TConstant = constant<>>
        class room
        {
        public:
            room()
                : kind("")
                , wall_ids()
            {}

        public:
            std::string kind;
            std::vector<size_t> wall_ids;
        };

        template<typename TConstant = constant<>>
        class house
        {
        public:
            typedef node<TConstant>                 node_type;
            typedef std::map<size_t, node_type>     node_map;
            typedef std::pair<size_t, node_type>    node_pair;
            typedef wall<TConstant>                 wall_type;
            typedef std::map<size_t, wall_type>     wall_map;
            typedef std::pair<size_t, wall_type>    wall_pair;
            typedef hole<TConstant>                 hole_type;
            typedef std::map<size_t, hole_type>     hole_map;
            typedef std::pair<size_t, hole_type>    hole_pair;
            typedef room<TConstant>                 room_type;
            typedef std::map<size_t, room_type>     room_map;
            typedef std::pair<size_t, room_type>    room_pair;

        public:
            house()
                : name("")
                , nodes()
                , walls()
                , holes()
                , rooms()
            {}

        public:
            inline void reset()
            {
                name = "";
                nodes.clear();
                walls.clear();
                holes.clear();
                rooms.clear();
            }

        public:
            std::string name;
            node_map nodes;
            wall_map walls;
            hole_map holes;
            room_map rooms;
        };

        template<typename TConstant = constant<>>
        class building
        {
        public:
            typedef house<TConstant>                house_type;
            typedef std::map<size_t, house_type>    house_map;
            typedef typename TConstant::point_type  position_type;
            typedef std::map<position_type, size_t> potision_map;

        public:
            house_map houses;
            potision_map positions;
        };

        template<typename TConstant = constant<>>
        class site
        {
        public:
            typedef building<TConstant>             building_type;
            typedef std::map<size_t, building_type> building_map;

        public:
            std::string name;
            building_map buildings;
        };

        template<typename TConstant = constant<>>
        class project
        {
        public:
            typedef site<TConstant>                 site_type;
            typedef std::map<size_t, site_type>     site_map;

        public:
            std::string name;
            site_map sites;
        };

        template<typename TConstant = constant<>>
        class algorithm
        {
        public:
            typedef typename TConstant::precision_type  precision_type;
            typedef house<TConstant>                    house_type;
            typedef typename house_type::node_type      node_type;

        public:
            class wall_ex
            {
            public:
                wall_ex(size_t _id = TConstant::none_id, bool _inversed = false)
                    : id(_id)
                    , inversed(_inversed)
                {}

            public:
                bool operator==(const wall_ex& _a) const
                {
                    return (id == _a.id && inversed == _a.inversed);
                }

            public:
                size_t id;
                bool inversed;
            };

            class room_ex
            {
            public:
                room_ex()
                    : room_id(TConstant::none_id)
                    , walls()
                    , inside(false)
                {}

            public:
                size_t room_id;
                std::vector<wall_ex> walls;
                bool inside;
            };

            class node_ex
            {
            public:
                node_ex(size_t _id = TConstant::none_id
                    , bool _used = false)
                    : id(_id)
                    , used(_used)
                    , with_wall()
                {}

            public:
                bool operator==(const node_ex& _a) const
                {
                    return (id == _a.id
                        && used == _a.used
                        && with_wall == _a.with_wall);
                }

            public:
                size_t id;
                bool used;
                wall_ex with_wall;
            };

        public:
            typedef std::vector<room_ex>               room_ex_vector;

        public:
            template<typename TItem>
            static void addUnique(std::vector<TItem>& _v, const TItem& _i)
            {
                typename std::vector<TItem>::iterator it_found = std::find(_v.begin(), _v.end(), _i);
                if (it_found != _v.end()) return;
                _v.push_back(_i);
            }

            static precision_type calculateSinAngleEx(const node<TConstant>& _o, const node<TConstant>& _a, const node<TConstant>& _b)
            {
                node<TConstant> line_a(_a.x() - _o.x(), _a.y() - _o.y());
                precision_type len_a = sqrt(line_a.x() * line_a.x() + line_a.y() * line_a.y());
                if (len_a != 0)
                {
                    line_a.x(line_a.x() / len_a);
                    line_a.y(line_a.y() / len_a);
                }
                node<TConstant> line_b(_b.x() - _o.x(), _b.y() - _o.y());
                precision_type len_b = sqrt(line_b.x() * line_b.x() + line_b.y() * line_b.y());
                if (len_b != 0)
                {
                    line_b.x(line_b.x() / len_b);
                    line_b.y(line_b.y() / len_b);
                }
                precision_type sin_res = line_a.x() * line_b.y() - line_a.y() * line_b.x();
                precision_type cos_res = line_a.x() * line_b.x() + line_a.y() * line_b.y();
                precision_type res = sin_res;
                if (cos_res < static_cast<precision_type>(0))
                {
                    if (sin_res >= static_cast<precision_type>(0))
                    {
                        res = static_cast<precision_type>(2) - res;
                    }
                    else
                    {
                        res = static_cast<precision_type>(-2) + res;
                    }
                }
                if (res < static_cast<precision_type>(0))
                {
                    res = static_cast<precision_type>(4) + res;
                }
                return res;
            }

            static bool computeRoomExs(const house_type& _house
                , room_ex_vector& _room_exs
                , size_t _room_id = TConstant::none_id)
            {
                std::vector<size_t> bim_room_ids;
                if (_room_id != TConstant::none_id)
                {
                    const typename house<TConstant>::room_map::const_iterator cit_found_room = _house.rooms.find(_room_id);
                    if (cit_found_room == _house.rooms.cend())
                    {
                        return false;
                    }
                }
                else
                {
                    for (typename house<TConstant>::room_map::const_iterator cit = _house.rooms.cbegin(); cit != _house.rooms.cend(); ++cit)
                    {
                        bim_room_ids.push_back(cit->first);
                    }
                }

                std::map<size_t, std::vector<node_ex>> bim_nodes_2_next_nodes;

                for (size_t bim_room_id : bim_room_ids)
                {
                    const room<TConstant>& bim_room = _house.rooms.find(bim_room_id)->second;
                    for (const size_t wall_id : bim_room.wall_ids)
                    {
                        const wall<TConstant>& bim_wall = _house.walls.find(wall_id)->second;
                        node_ex bim_next_node;
                        bim_next_node.with_wall.id = wall_id;
                        {
                            bim_next_node.with_wall.inversed = false;
                            bim_next_node.id = bim_wall.end_node_id;
                            if (bim_nodes_2_next_nodes.find(bim_wall.start_node_id) == bim_nodes_2_next_nodes.end())
                            {
                                std::vector<node_ex> bim_next_nodes;
                                bim_next_nodes.push_back(bim_next_node);
                                bim_nodes_2_next_nodes.insert(std::make_pair<>(bim_wall.start_node_id, bim_next_nodes));
                            }
                            else
                            {
                                addUnique<>(bim_nodes_2_next_nodes[bim_wall.start_node_id], bim_next_node);
                            }
                        }
                        {
                            bim_next_node.with_wall.inversed = true;
                            bim_next_node.id = bim_wall.start_node_id;
                            if (bim_nodes_2_next_nodes.find(bim_wall.end_node_id) == bim_nodes_2_next_nodes.end())
                            {
                                std::vector<node_ex> bim_next_nodes;
                                bim_next_nodes.push_back(bim_next_node);
                                bim_nodes_2_next_nodes.insert(std::make_pair<>(bim_wall.end_node_id, bim_next_nodes));
                            }
                            else
                            {
                                addUnique<>(bim_nodes_2_next_nodes[bim_wall.end_node_id], bim_next_node);
                            }
                        }
                    }
                }

                if (bim_nodes_2_next_nodes.empty())
                {
                    return false;
                }

                /// compute the path/edges of the room
                while (!bim_nodes_2_next_nodes.empty())
                {
                    room_ex bim_closed_room_ex;
                    bim_closed_room_ex.room_id  = _room_id;
                    bool bim_path_is_closed     = false;

                    size_t bim_start_node_id            = bim_nodes_2_next_nodes.cbegin()->first;
                    size_t bim_last_node_id             = TConstant::none_id;
                    size_t bim_first_wall_start_node_id = TConstant::none_id;
                    size_t bim_first_wall_end_node_id   = TConstant::none_id;
                    wall_ex bim_first_wall_ex;
                    while (true)
                    {
                        std::vector<node_ex>& bim_next_nodes = bim_nodes_2_next_nodes.find(bim_start_node_id)->second;
                        if (bim_next_nodes.empty())
                        {
                            break;
                        }
                        if (bim_last_node_id == TConstant::none_id)
                        {
                            bim_last_node_id = bim_start_node_id;
                            node_ex& bim_next_node = bim_next_nodes[0];
                            bim_start_node_id = bim_next_node.id;
                            bim_first_wall_ex = bim_next_node.with_wall;
                        }
                        else
                        {
                            assert(bim_start_node_id != bim_last_node_id);
                            if (bim_start_node_id == bim_last_node_id)
                            {
                                throw std::invalid_argument("contains invalid wall!");
                            }

                            std::map<precision_type, size_t> sin_angle_ex_2_index;
                            {
                                /// sort the next node by the `sin-angle-ex`
                                const node<TConstant>& bim_start_node = _house.nodes.find(bim_start_node_id)->second;
                                const node<TConstant>& bim_last_node = _house.nodes.find(bim_last_node_id)->second;
                                for (size_t i = 0; i < bim_next_nodes.size(); ++i)
                                {
                                    const node_ex& bim_next_node = bim_next_nodes[i];
                                    if (bim_next_node.used) continue;
                                    const node<TConstant>& bim_node = _house.nodes.find(bim_next_node.id)->second;
                                    precision_type sin_angle_ex = calculateSinAngleEx(bim_start_node, bim_last_node, bim_node);
                                    sin_angle_ex_2_index.insert(std::make_pair<>(sin_angle_ex, i));
                                }
                            }
                            if (sin_angle_ex_2_index.empty())
                            {
                                break;
                            }

                            typename std::map<precision_type, size_t>::const_iterator sin_angle_ex_2_index_last = --sin_angle_ex_2_index.cend();
                            node_ex& bim_next_node = bim_next_nodes[sin_angle_ex_2_index_last->second];
                            bim_next_node.used = true;

                            wall_ex bim_wall_ex;
                            bim_wall_ex = bim_next_node.with_wall;
                            bim_closed_room_ex.walls.push_back(bim_wall_ex);

                            if (bim_first_wall_ex == bim_next_node.with_wall)
                            {
                                bim_path_is_closed = true;
                                break;
                            }

                            bim_last_node_id = bim_start_node_id;
                            bim_start_node_id = bim_next_node.id;
                        }
                    }

                    if (bim_path_is_closed)
                    {
                        _room_exs.push_back(bim_closed_room_ex);
                    }

                    for (typename std::map<size_t, std::vector<node_ex>>::iterator it_m = bim_nodes_2_next_nodes.begin(); it_m != bim_nodes_2_next_nodes.end();)
                    {
                        std::vector<node_ex>& bim_next_nodes = it_m->second;
                        for (typename std::vector<node_ex>::iterator it_v = bim_next_nodes.begin(); it_v != bim_next_nodes.end();)
                        {
                            if (it_v->used)
                            {
                                it_v = bim_next_nodes.erase(it_v);
                            }
                            else
                            {
                                ++it_v;
                            }
                        }

                        if (it_m->second.empty())
                        {
                            it_m = bim_nodes_2_next_nodes.erase(it_m);
                        }
                        else
                        {
                            ++it_m;
                        }
                    }
                }

                //TODO: decide wheather the closed path is inside or outside
                for (room_ex& closed_room_ex : _room_exs)
                {
                    //
                }

                return !_room_exs.empty();
            }
        };
    }
}
