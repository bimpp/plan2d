.. Plan 2D documentation master file, created by
   sphinx-quickstart on Fri Sep 25 00:04:19 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Plan 2D's documentation!
===================================

This project is for plan in 2D. And it is inspired by IFC_.

Features
========

* Header-Only
* Define some data structure for the 2D plan
   * Node
   * Wall
   * Hole - Door or Window or Hole
   * Room
* Implement some algorithms for the 2D plan
   * Compute the edges of the room by some dispersed walls

.. toctree::
   :maxdepth: 2
   :caption: Table of Contents:
   :numbered:

   datastructures
   algorithms
   usage

License
=======

.. code-block:: c
   :linenos:

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

.. _IFC: https://technical.buildingsmart.org/standards/ifc/
