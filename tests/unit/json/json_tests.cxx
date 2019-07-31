/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2018 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
