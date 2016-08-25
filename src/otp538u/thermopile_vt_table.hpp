/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// This table was taken from the '538U VT Table__20_200(v1.3).pdf'
// datasheet, but the 25C column has been removed for consistency.

static const int otp538u_vt_table_max = 23;

// Thermistor temperature (C)
// { -20 -10 0 10 20 30 40 50 60 70 80 90 100 }

static float otp538u_vt_table[otp538u_vt_table_max][13] = {
                                                  // object temp (C)  
  {0.000, -0.246, -0.523, -0.832, -1.177, -1.559, // -20C 
   -1.981, -2.446, -2.957, -3.516, -4.126, -4.791, -5.513},
  
  {0.243, 0.000, -0.274, -0.580, -0.922, -1.301, // -10
   -1.721, -2.183, -2.691, -3.247, -3.854, -4.516, -5.236},
  
  {0.511, 0.271, 0.000, -0.303, -0.642, -1.018, // 0
   -1.434, -1.894, -2.398, -2.951, -3.556, -4.215, -4.931},
  
  {0.804, 0.567, 0.300, 0.000, -0.335, -0.708, // 10
   -1.121, -1.577, -2.078, -2.628, -3.229, -3.884, -4.597},
  
  {1.125, 0.891, 0.628, 0.331, 0.000, -0.369, // 20
   -0.778, -1.230, -1.728, -2.274, -2.871, -3.523, -4.232},
  
  {1.474, 1.244, 0.985, 0.692, 0.365, 0.000, // 30
   -0.405, -0.853, -1.347, -1.889, -2.482, -3.130, -3.835},
  
  {1.852, 1.628, 1.372, 1.084, 0.761, 0.401, // 40
   0.000, -0.444, -0.933, -1.470, -2.059, -2.702, -3.403},
  
  {2.263, 2.043, 1.792, 1.509, 1.191, 0.835, // 50
   0.439, 0.000, -0.484, -1.017, -1.601, -2.240, -2.936},
  
  {2.706, 2.491, 2.246, 1.968, 1.655, 1.304, // 60
   0.913, 0.479, 0.000, -0.528, -1.107, -1.740, -2.431},
  
  {3.184, 2.975, 2.735, 2.462, 2.155, 1.809, // 70
   1.424, 0.996, 0.522, 0.000, -0.573, -1.201, -1.887},
  
  {3.698, 3.495, 3.261, 2.994, 2.692, 2.353, // 80
   1.974, 1.552, 1.084, 0.568, 0.000, -0.622, -1.301},
  
  {4.250, 4.053, 3.825, 3.565, 3.270, 2.937, // 90
   2.564, 2.148, 1.687, 1.177, 0.616, 0.000, -0.673},
  
  {4.841, 4.651, 4.430, 4.177, 3.888, 3.562, // 100
   3.196, 2.787, 2.332, 1.829, 1.275, 0.666, 0.000},
  
  {5.473, 5.290, 5.076, 4.830, 4.549, 4.231, // 110
   3.872, 3.470, 3.023, 2.527, 1.980, 1.379, 0.720},
  
  {6.147, 5.972, 5.767, 5.528, 5.255, 4.944, // 120
   4.593, 4.199, 3.760, 3.272, 2.733, 2.139, 1.488},
  
  {6.866, 6.699, 6.502, 6.272, 6.007, 5.705, // 130
   5.362, 4.976, 4.545, 4.066, 3.535, 2.950, 2.307},
  
  {7.631, 7.473, 7.285, 7.064, 6.808, 6.514, // 140
   6.180, 5.803, 5.381, 4.910, 4.388, 3.812, 3.178},
  
  {8.444, 8.295, 8.116, 7.905, 7.658, 7.373, // 150
   7.049, 6.682, 6.269, 5.807, 5.295, 4.728, 4.103},
  
  {9.306, 9.167, 8.998, 8.796, 8.560, 8.285, // 160
   7.971, 7.613, 7.211, 6.759, 6.257, 5.700, 5.085},
  
  {10.219, 10.091, 9.933, 9.741, 9.515, 9.251, // 170
   8.947, 8.601, 8.208, 7.768, 7.276, 6.729, 6.125},
  
  {11.185, 11.068, 10.921, 10.741, 10.526, 10.274, // 180
   9.981, 9.645, 9.264, 8.835, 8.354, 7.818, 7.226},
  
  {12.206, 12.101, 11.966, 11.798, 11.595, 11.354, // 190
   11.073, 10.749, 10.380, 9.962, 9.493, 8.969, 8.388},
  
  {13.284, 13.191, 13.068, 12.913, 12.722, 12.494, // 200
   12.225, 11.914, 11.557, 11.152, 10.695, 10.184, 9.616}
};

