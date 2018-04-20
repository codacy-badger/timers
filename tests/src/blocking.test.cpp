#include <chrono>
#include <future>
#include <thread>

#include <gtest/gtest.h>

#include "make_all_members_public.h"
#include <timers/blocking.h>
#include <timers/exceptions.h>

#include "static_assertions.h"
#include "time_utils.h"

namespace
{
using namespace std::chrono_literals;
namespace timers = burda::timers;

class blocking_test : public ::testing::Test
{
protected:
    timers::blocking m_blocking_timer;
};

TEST(blocking_test_static, static_assertions)
{
    assert_properties<timers::blocking>();

    SUCCEED();
}

TEST(blocking_test_construction_destruction, basic_construction_destruction)
{
    ASSERT_NO_THROW(timers::blocking blocking_timer);

    timers::blocking blocking_timer;
    EXPECT_NO_THROW(blocking_timer.timers::blocking::~blocking());
}

TEST_F(blocking_test, default_values)
{
    EXPECT_FALSE(m_blocking_timer.m_terminated);
}

TEST_F(blocking_test, block_not_throwing)
{
    EXPECT_NO_THROW(m_blocking_timer.block(1ms));
}

TEST_F(blocking_test, block_throwing)
{
    EXPECT_THROW(m_blocking_timer.block(0h), timers::exceptions::time_period_zero);
    EXPECT_THROW(m_blocking_timer.block(-1ms), timers::exceptions::time_period_negative);
}

TEST_F(blocking_test, block_time)
{
    // TODO: move this measuring into some common macro
    const auto start = timers::testing::clock::now();
    EXPECT_TRUE(m_blocking_timer.block(2s));
    const auto end = timers::testing::clock::now();
    const auto elapsed = timers::testing::round_to_seconds(end - start);

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 2.0, 100.0);
}

TEST_F(blocking_test, block_and_stop)
{
    auto terminator = std::async(std::launch::async, [this]()
    {
        std::this_thread::sleep_for(2s);
        m_blocking_timer.stop();
    });

    EXPECT_FALSE(m_blocking_timer.block(10s));
    terminator.get();

    EXPECT_TRUE(m_blocking_timer.m_terminated);
}

TEST_F(blocking_test, block_multiple_times)
{
    const auto start = timers::testing::clock::now();
    EXPECT_TRUE(m_blocking_timer.block(1s));
    EXPECT_TRUE(m_blocking_timer.block(4s));
    const auto end = timers::testing::clock::now();
    const auto elapsed = timers::testing::round_to_seconds(end - start);

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 5.0, 100.0);
    EXPECT_TRUE(m_blocking_timer.m_terminated);
}

TEST_F(blocking_test, block_multiple_times_in_parallel)
{
    const auto start = timers::testing::clock::now();

    auto caller1 = std::async(std::launch::async, [this]()
    {
        EXPECT_TRUE(m_blocking_timer.block(5s));
    });

    auto caller2 = std::async(std::launch::async, [this]()
    {
        EXPECT_TRUE(m_blocking_timer.block(5s));
    });

    caller1.get();
    caller2.get();

    const auto end = timers::testing::clock::now();
    const auto elapsed = timers::testing::round_to_seconds(end - start);

    timers::testing::assert_that_elapsed_time_in_tolerance(elapsed, 10.0, 100.0);
}

TEST_F(blocking_test, stop)
{
    EXPECT_NO_THROW(m_blocking_timer.stop());

    m_blocking_timer.m_terminated = false;
    m_blocking_timer.stop();
    EXPECT_TRUE(m_blocking_timer.m_terminated);
}
}
