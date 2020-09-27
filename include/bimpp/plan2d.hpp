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

#if !defined(M_PI)
#define M_PI       3.14159265358979323846   // pi
#endif

namespace bimpp
{
    namespace plan2d
    {
        /*!
         * Define a point for the 2D plan, it has two dimensions.
         */
        template<typename T = double>
        class point
        {
        public:
            typedef T   precision_type;

        public:
            point(precision_type _x = 0, precision_type _y = 0)
                : data({ _x, _y })
            {}

            point(const point& _a)
                : data(_a.data)
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
            point& operator=(const point& _a)
            {
                data = _a.data;
                return *this;
            }

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

            point operator+(const point& _a) const
            {
                return point(x() + _a.x(), y() + _a.y());
            }

            point operator-(const point& _a) const
            {
                return point(x() - _a.x(), y() - _a.y());
            }

            inline precision_type dot(const point& _a) const
            {
                return (x() * _a.y() - y() * _a.x());
            }

            inline precision_type cross(const point& _a) const
            {
                return (x() * _a.x() + y() * _a.y());
            }

            inline precision_type normalize()
            {
                precision_type len = sqrt(x() * x() + y() * y());
                if (len != static_cast<precision_type>(0)
                    && len != static_cast<precision_type>(1))
                {
                    x(x() / len);
                    y(y() / len);
                }
                return len;
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
            inline const point_type& p() const
            {
                return point;
            }

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
            typedef typename TConstant::id_type         id_type;

        public:
            wall(id_type _start_node_id = TConstant::none_id
                , id_type _end_node_id = TConstant::none_id
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
            id_type         start_node_id;
            /// The end of wall
            id_type         end_node_id;
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
            typedef typename TConstant::id_type         id_type;

        public:
            hole(id_type _wall_id = TConstant::none_id
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
            id_type         wall_id;
            precision_type  distance;
            precision_type  width;
        };

        template<typename TConstant = constant<>>
        class room
        {
        public:
            typedef typename TConstant::id_type         id_type;
            typedef std::vector<id_type>                id_vector;

        public:
            room()
                : kind("")
                , wall_ids()
            {}

        public:
            std::string     kind;
            id_vector       wall_ids;
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
            typedef typename TConstant::id_type         id_type;
            typedef house<TConstant>                    house_type;
            typedef std::map<id_type, house_type>       house_map;
            typedef typename TConstant::point_type      position_type;
            typedef std::map<position_type, id_type>    potision_map;

        public:
            house_map houses;
            potision_map positions;
        };

        template<typename TConstant = constant<>>
        class site
        {
        public:
            typedef typename TConstant::id_type         id_type;
            typedef building<TConstant>                 building_type;
            typedef std::map<id_type, building_type>    building_map;

        public:
            std::string name;
            building_map buildings;
        };

        template<typename TConstant = constant<>>
        class project
        {
        public:
            typedef typename TConstant::id_type         id_type;
            typedef site<TConstant>                     site_type;
            typedef std::map<id_type, site_type>        site_map;

        public:
            std::string name;
            site_map sites;
        };

        template<typename TConstant = constant<>>
        class algorithm
        {
        public:
            typedef typename TConstant::precision_type  precision_type;
            typedef typename TConstant::point_type      point_type;
            typedef typename TConstant::id_type         id_type;
            typedef std::vector<id_type>                id_vector;
            typedef node<TConstant>                     node_type;
            typedef wall<TConstant>                     wall_type;
            typedef room<TConstant>                     room_type;
            typedef house<TConstant>                    house_type;

        public:
            class wall_ex
            {
            public:
                wall_ex(id_type _id = TConstant::none_id, bool _inversed = false)
                    : id(_id)
                    , inversed(_inversed)
                    , repeated(false)
                {}

            public:
                bool operator==(const wall_ex& _a) const
                {
                    return (id == _a.id && inversed == _a.inversed);
                }

            public:
                id_type     id;
                bool        inversed;
                bool        repeated;
            };

            /*!
             * An enum, it means the direction of the room's edges.
             */
            enum room_side
            {
                room_side_both,     ///< Don't know the room faces inside or outside
                room_side_in,       ///< The room faces inside
                room_side_out,      ///< The room faces outside
            };

            class room_ex
            {
            public:
                room_ex()
                    : id(TConstant::none_id)
                    , walls()
                    , side(room_side_both)
                {}

            public:
                id_type                 id;
                std::vector<wall_ex>    walls;
                room_side               side;
            };

            class node_ex
            {
            public:
                node_ex(id_type _id = TConstant::none_id
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
                id_type     id;
                bool        used;
                wall_ex     with_wall;
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

            /*!
             * Is the `_all` contains `_sub`?
             * 
             * **Importance**: The `_all` and `_sub` must be sorted from small to big.
             * 
             * @param _all A vector contains all items
             * @param _sub A vector contains a part of items
             */
            template<typename TItem>
            static bool isContainsForBiggerVector(const std::vector<TItem>& _all, const std::vector<TItem>& _sub)
            {
                if (_all.empty() || _sub.empty()) return false;
                if (_all.size() < _sub.size()) return false;
                size_t a_index = 0;
                size_t s_index = 0;
                const size_t a_size = _all.size();
                const size_t s_size = _sub.size();
                while (a_index < a_size
                    && s_index < s_size
                    && _all[a_index] <= _sub[s_index])
                {
                    if (_all[a_index] == _sub[s_index])
                    {
                        ++s_index;
                    }
                    else
                    {
                        ++a_index;
                    }
                }
                return (s_index == s_size);
            }

            /*!
             * Calculate the angle \f$ \alpha \f$ between \f$ \vec{OA} \f$ and \f$ \vec{OB} \f$.
             * @f[
             * f(\vec{OA}, \vec{OB}) =
             * \begin{cases}
             * \cos^{-1}(\vec{OA} \cdot \vec{OB})               & \quad \text{if } 0^{\circ} \leq \alpha \leq 180^{\circ}\\
             * \pi * 2 - \cos^{-1}(\vec{OA} \cdot \vec{OB})     & \quad \text{if } 180^{\circ} < \alpha < 360^{\circ}
             * \end{cases}
             * @f]
             *
             * @param _o The \f$ {O} \f$ node(point)
             * @param _a The \f$ {A} \f$ node(point)
             * @param _b The \f$ {B} \f$ node(point)
             */
            static precision_type calculateAngleEx(const node_type& _o, const node_type& _a, const node_type& _b)
            {
                /// Make the line \f$ \vec{OA} \f$, and normalize it.
                point_type line_a(_a.p() - _o.p()); line_a.normalize();
                /// Make the line \f$ \vec{OB} \f$, and normalize it.
                point_type line_b(_b.p() - _o.p()); line_b.normalize();
                /// Compute the dot product and the cross product between \f$ \vec{OA} \f$ and \f$ \vec{OB} \f$
                precision_type sin_res = line_a.dot(line_b);
                precision_type cos_res = line_a.cross(line_b);
                precision_type res = static_cast<precision_type>(0);
                if (sin_res >= 0)
                {
                    res = static_cast<precision_type>(acos(cos_res));
                }
                else
                {
                    res = static_cast<precision_type>(M_PI * 2.0 - acos(cos_res));
                }
                return res;
            }

            /*!
             * Calculate the increased sine of a angle \f$ \alpha \f$ between \f$ \vec{OA} \f$ and \f$ \vec{OB} \f$.
             * @f[
             * f(\vec{OA}, \vec{OB}) =
             * \begin{cases}
             * \vert\vec{OA} \times \vec{OB}\vert         & \quad \text{if } 0^{\circ} \leq \alpha \leq 90^{\circ}\\
             * 2 - \vert\vec{OA} \times \vec{OB}\vert     & \quad \text{if } 90^{\circ} < \alpha < 270^{\circ}\\
             * 4 + \vert\vec{OA} \times \vec{OB}\vert     & \quad \text{if } 270^{\circ} \leq \alpha < 360^{\circ}
             * \end{cases}
             * @f]
             * 
             * @param _o The \f$ {O} \f$ node(point)
             * @param _a The \f$ {A} \f$ node(point)
             * @param _b The \f$ {B} \f$ node(point)
             */
            static precision_type calculateSinAngleEx(const node_type& _o, const node_type& _a, const node_type& _b)
            {
                /// Make the line \f$ \vec{OA} \f$, and normalize it.
                point_type line_a(_a.p() - _o.p()); line_a.normalize();
                /// Make the line \f$ \vec{OB} \f$, and normalize it.
                point_type line_b(_b.p() - _o.p()); line_b.normalize();
                /// Compute the dot product and the cross product between \f$ \vec{OA} \f$ and \f$ \vec{OB} \f$
                precision_type sin_res = line_a.dot(line_b);
                precision_type cos_res = line_a.cross(line_b);
                precision_type res = static_cast<precision_type>(0);
                if (cos_res >= static_cast<precision_type>(0))
                {
                    if (sin_res >= static_cast<precision_type>(0))
                    {
                        res = sin_res;
                    }
                    else
                    {
                        res = static_cast<precision_type>(4) + sin_res;
                    }
                }
                else
                {
                    res = static_cast<precision_type>(2) - sin_res;
                }
                return res;
            }

            /*!
             * Calculate the increased cosine of a angle \f$ \alpha \f$ between \f$ \vec{OA} \f$ and \f$ \vec{OB} \f$.
             * @f[
             * f(\vec{OA}, \vec{OB}) =
             * \begin{cases}
             * 1 - \vec{OA} \cdot \vec{OB}      & \quad \text{if } 0^{\circ} \leq \alpha \leq 180^{\circ}\\
             * 3 + \vec{OA} \cdot \vec{OB}      & \quad \text{if } 180^{\circ} < \alpha < 360^{\circ}
             * \end{cases}
             * @f]
             *
             * @param _o The \f$ {O} \f$ node(point)
             * @param _a The \f$ {A} \f$ node(point)
             * @param _b The \f$ {B} \f$ node(point)
             */
            static precision_type calculateCosAngleEx(const node_type& _o, const node_type& _a, const node_type& _b)
            {
                /// Make the line \f$ \vec{OA} \f$, and normalize it.
                point_type line_a(_a.p() - _o.p()); line_a.normalize();
                /// Make the line \f$ \vec{OB} \f$, and normalize it.
                point_type line_b(_b.p() - _o.p()); line_b.normalize();
                /// Compute the dot product and the cross product between \f$ \vec{OA} \f$ and \f$ \vec{OB} \f$
                precision_type sin_res = line_a.dot(line_b);
                precision_type cos_res = line_a.cross(line_b);
                precision_type res = static_cast<precision_type>(0);
                if (sin_res >= static_cast<precision_type>(0))
                {
                    res = static_cast<precision_type>(1) - cos_res;
                }
                else
                {
                    res = static_cast<precision_type>(3) + cos_res;
                }
                return res;
            }

            /*!
             * Compute all room's edges by all walls, and don't use recursion.
             * 
             * @param _house The house
             * @param _room_exs Output the room's edge list
             * @param _room_id The special id of room, find all rooms if it is none
             */
            static bool computeRoomExs(const house_type& _house
                , room_ex_vector& _room_exs
                , id_type _room_id = TConstant::none_id)
            {
                id_vector bim_room_ids;
                if (_room_id != TConstant::none_id)
                {
                    const typename house_type::room_map::const_iterator cit_found_room = _house.rooms.find(_room_id);
                    if (cit_found_room == _house.rooms.cend())
                    {
                        return false;
                    }
                    bim_room_ids.push_back(_room_id);
                }
                else
                {
                    for (typename house_type::room_map::const_iterator cit = _house.rooms.cbegin(); cit != _house.rooms.cend(); ++cit)
                    {
                        bim_room_ids.push_back(cit->first);
                    }
                }

                _room_exs.clear();

                std::map<id_type, std::vector<node_ex>> bim_nodes_2_next_nodes;

                for (id_type bim_room_id : bim_room_ids)
                {
                    const room_type& bim_room = _house.rooms.find(bim_room_id)->second;
                    for (const id_type wall_id : bim_room.wall_ids)
                    {
                        const wall_type& bim_wall = _house.walls.find(wall_id)->second;
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

                /// Compute the path/edges of the room
                while (!bim_nodes_2_next_nodes.empty())
                {
                    room_ex bim_room_ex;
                    bim_room_ex.id  = _room_id;
                    bool bim_path_is_closed     = false;

                    id_type bim_start_node_id            = bim_nodes_2_next_nodes.cbegin()->first;
                    id_type bim_last_node_id             = TConstant::none_id;
                    id_type bim_first_wall_start_node_id = TConstant::none_id;
                    id_type bim_first_wall_end_node_id   = TConstant::none_id;
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
                                const node_type& bim_start_node = _house.nodes.find(bim_start_node_id)->second;
                                const node_type& bim_last_node = _house.nodes.find(bim_last_node_id)->second;
                                for (size_t i = 0; i < bim_next_nodes.size(); ++i)
                                {
                                    const node_ex& bim_next_node = bim_next_nodes[i];
                                    if (bim_next_node.used) continue;
                                    const node_type& bim_node = _house.nodes.find(bim_next_node.id)->second;
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
                            bim_room_ex.walls.push_back(bim_wall_ex);

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
                        /// mark all repeated walls
                        std::map<id_type, size_t> bim_walls_2_counts;
                        for (const wall_ex& bim_wall_ex : bim_room_ex.walls)
                        {
                            typename std::map<id_type, size_t>::iterator it_found = bim_walls_2_counts.find(bim_wall_ex.id);
                            if (it_found == bim_walls_2_counts.end())
                            {
                                bim_walls_2_counts.insert(std::make_pair<>(bim_wall_ex.id, 1));
                            }
                            else
                            {
                                ++it_found->second;
                            }
                        }
                        for (wall_ex& bim_wall_ex : bim_room_ex.walls)
                        {
                            bim_wall_ex.repeated = (bim_walls_2_counts[bim_wall_ex.id] != 1);
                        }

                        //TODO: decide wheather the room-ex is inside or outside
                        /// find the most left node from all unrepeated walls
                        {
                            size_t bim_wall_ex_index = bim_room_ex.walls.size();
                            node_type bim_left_node(TConstant::zero_point);
                            {
                                id_type bim_left_node_id = TConstant::none_id;
                                for (size_t i = 0, ic = bim_room_ex.walls.size(); i < ic; ++i)
                                {
                                    const wall_ex& bim_wall_ex = bim_room_ex.walls[i];
                                    /// ignore all repeated walls
                                    if (bim_wall_ex.repeated) continue;
                                    const wall_type& bim_wall = _house.walls.find(bim_wall_ex.id)->second;
                                    if (!TConstant::isValid(bim_left_node_id))
                                    {
                                        bim_left_node_id = bim_wall.start_node_id;
                                        bim_left_node = _house.nodes.find(bim_left_node_id)->second;
                                        bim_wall_ex_index = i;
                                    }
                                    else
                                    {
                                        const node_type& bim_left_node_temp = _house.nodes.find(bim_wall.start_node_id)->second;
                                        if (bim_left_node_temp.p() < bim_left_node.p())
                                        {
                                            bim_left_node_id = bim_wall.start_node_id;
                                            bim_left_node = bim_left_node_temp;
                                            bim_wall_ex_index = i;
                                        }
                                    }
                                }
                            }
                            if (bim_wall_ex_index < bim_room_ex.walls.size())
                            {
                                const size_t bim_walls_count = bim_room_ex.walls.size();
                                const wall_ex& bim_start_wall_ex = bim_room_ex.walls[bim_wall_ex_index];
                                const wall_ex* bim_next_wall_ex_ptr = nullptr;
                                while (bim_next_wall_ex_ptr != &bim_start_wall_ex
                                    && (bim_next_wall_ex_ptr == nullptr
                                        || bim_next_wall_ex_ptr->repeated))
                                {
                                    if (bim_start_wall_ex.inversed)
                                    {
                                        bim_wall_ex_index = bim_wall_ex_index + 1;
                                    }
                                    else
                                    {
                                        bim_wall_ex_index = bim_wall_ex_index + bim_walls_count - 1;
                                    }
                                    bim_wall_ex_index = bim_wall_ex_index % bim_walls_count;
                                    bim_next_wall_ex_ptr = &bim_room_ex.walls[bim_wall_ex_index];
                                }
                                if (bim_next_wall_ex_ptr != &bim_start_wall_ex)
                                {
                                    const wall_type& bim_start_wall = _house.walls.find(bim_start_wall_ex.id)->second;
                                    const node_type bim_start_node = _house.nodes.find(bim_start_wall.end_node_id)->second;
                                    const wall_type& bim_next_wall = _house.walls.find(bim_next_wall_ex_ptr->id)->second;
                                    const node_type bim_next_node = _house.nodes.find(bim_start_wall.start_node_id == bim_next_wall.end_node_id ? bim_next_wall.start_node_id : bim_next_wall.end_node_id)->second;
                                    const precision_type bim_sin_angle_ex = calculateCosAngleEx(bim_left_node
                                        , bim_start_wall_ex.inversed ? bim_next_node : bim_start_node
                                        , bim_start_wall_ex.inversed ? bim_start_node : bim_next_node);
                                    if (bim_sin_angle_ex == 0)
                                    {
                                        bim_room_ex.side = room_side_both;
                                    }
                                    else
                                    {
                                        bim_room_ex.side = (bim_sin_angle_ex <= static_cast<precision_type>(2)) ? room_side_in : room_side_out;
                                    }
                                }
                            }
                        }

                        _room_exs.push_back(bim_room_ex);
                    }

                    for (typename std::map<id_type, std::vector<node_ex>>::iterator it_m = bim_nodes_2_next_nodes.begin(); it_m != bim_nodes_2_next_nodes.end();)
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

                if (!TConstant::isValid(_room_id))
                {
                    /// make sure the room id
                    std::map<id_type, std::vector<id_type>> bim_rooms_2_walls;
                    for (typename house_type::room_map::const_iterator cit = _house.rooms.cbegin();
                        cit != _house.rooms.cend(); ++cit)
                    {
                        const room_type& bim_room = cit->second;
                        id_vector bim_wall_ids = bim_room.wall_ids;
                        std::sort(bim_wall_ids.begin(), bim_wall_ids.end());
                        bim_rooms_2_walls.insert(std::make_pair(cit->first, bim_wall_ids));
                    }

                    for (room_ex& each_room_ex : _room_exs)
                    {
                        id_vector bim_wall_ids;
                        for (const wall_ex& bim_wall_ex : each_room_ex.walls)
                        {
                            bim_wall_ids.push_back(bim_wall_ex.id);
                        }
                        if (bim_wall_ids.empty())
                        {
                            continue;
                        }
                        std::sort(bim_wall_ids.begin(), bim_wall_ids.end());
                        for (typename std::map<id_type, std::vector<id_type>>::const_iterator cit = bim_rooms_2_walls.cbegin();
                            cit != bim_rooms_2_walls.cend(); ++cit)
                        {
                            if (!isContainsForBiggerVector<>(cit->second, bim_wall_ids)) continue;
                            each_room_ex.id = cit->first;
                            break;
                        }
                    }
                }

                return !_room_exs.empty();
            }
        };
    }
}
