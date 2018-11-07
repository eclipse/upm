/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2018 Intel Corporation.
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


#include "gtest/gtest.h"
#include "external/json/json.hpp"

namespace
{
    class source
    {
        public:
            std::string unit;
            float max;
            float min;
            float accuracy;
    };

    void to_json(nlohmann::json& j, const source& p) {
        j = nlohmann::json{
            {"unit", p.unit},
                {"min", p.min},
                {"max", p.max},
                {"accuracy", p.accuracy}};
    }

    void from_json(const nlohmann::json& j, source& p) {
        p.unit = j.at("unit").get<std::string>();
        p.min = j.at("min").get<float>();
        p.max = j.at("max").get<float>();
        p.accuracy = j.at("accuracy").get<float>();
    }

    bool operator==(const source& lhs, const source& rhs)
    {
        return lhs.unit == rhs.unit && lhs.min == rhs.min;
    }
}

/* Json test fixture */
class json_unit : public ::testing::Test
{
    protected:
        /* One-time setup logic if needed */
        json_unit() {}

        /* One-time tear-down logic if needed */
        virtual ~json_unit() {}

        /* Per-test setup logic if needed */
        virtual void SetUp() {}

        /* Per-test tear-down logic if needed */
        virtual void TearDown() {}
};

/* Test basic json serialization */
TEST_F(json_unit, test_basic_class_serialization)
{
    source s_temp {"C", -40.0, 85.0, 1.0};
    source s_humi {"%", 0.0, 100.0, 1.0};

    /* Create the json objects */
    nlohmann::json j_temp = s_temp;
    nlohmann::json j_humi = s_humi;

    /* Serialize/deserialize the originals */
    source clone_temp = j_temp;
    source clone_humi = j_humi;
    ASSERT_EQ(s_temp, clone_temp);

    /* Populate an stl container */
    std::map<std::string, source> sources;
    sources["temperature"] = s_temp;
    sources["humidity-relative"] = s_humi;

    /* Create the json object */
    nlohmann::json j_sources = sources;

    /* Serialize/deserialize the container */
    std::map<std::string, source> clone_sources = j_sources;
    ASSERT_EQ(sources, j_sources);
}
