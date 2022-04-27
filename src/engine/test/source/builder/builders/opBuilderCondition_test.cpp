/* Copyright (C) 2015-2021, Wazuh Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#include <gtest/gtest.h>
#include "testUtils.hpp"

#include "opBuilderCondition.hpp"
#include "opBuilderConditionReference.hpp"
#include "opBuilderConditionValue.hpp"
#include "opBuilderHelperFilter.hpp"

using namespace base;
namespace bld = builder::internals::builders;

using FakeTrFn = std::function<void(std::string)>;
static FakeTrFn tr = [](std::string msg){};

auto createEvent = [](const char * json){
    return std::make_shared<EventHandler>(std::make_shared<Document>(json));
};

TEST(opBuilderCondition, BuildsAllNonRegistered)
{
    Document doc{R"({
        "check": [
            {"string": "value"},
            {"int": 1},
            {"bool": true},
            {"reference": "$field"},
            {"helper1": "+exists"},
            {"helper2": "+not_exists"}
        ]
    })"};
    const auto & arr = doc.begin()->value.GetArray();
    for (auto it = arr.Begin(); it != arr.end(); ++it)
    {
        ASSERT_THROW(bld::opBuilderCondition(*it, tr), invalid_argument);
    }
}

TEST(opBuilderCondition, BuildsValue)
{
    BuilderVariant c = bld::opBuilderConditionValue;
    Registry::registerBuilder("condition.value", c);
    Document doc{R"({
        "check": [
            {"string": "value"},
            {"int": 1},
            {"bool": true}
        ]
    })"};
    const auto & arr = doc.begin()->value.GetArray();
    for (auto it = arr.Begin(); it != arr.end(); ++it)
    {
        ASSERT_NO_THROW(bld::opBuilderCondition(*it, tr));
    }
}

TEST(opBuilderCondition, BuildsReference)
{
    BuilderVariant c = bld::opBuilderConditionReference;
    Registry::registerBuilder("condition.reference", c);
    Document doc{R"({"check": {"ref": "$ref"}})"};
    ASSERT_NO_THROW(bld::opBuilderCondition(doc.get("/check"), tr));
}

TEST(opBuilderCondition, BuildsHelperExists)
{
    BuilderVariant c = bld::opBuilderHelperExists;
    Registry::registerBuilder("helper.exists", c);
    Document doc{R"({"check": {"ref": "+exists"}})"};
    ASSERT_NO_THROW(bld::opBuilderCondition(doc.get("/check"), tr));
}

TEST(opBuilderCondition, BuildsHelperNotExists)
{
    BuilderVariant c = bld::opBuilderHelperNotExists;
    Registry::registerBuilder("helper.not_exists", c);
    Document doc{R"({"check": {"ref": "+not_exists"}})"};
    ASSERT_NO_THROW(bld::opBuilderCondition(doc.get("/check"), tr));
}