#include <gtest/gtest.h>

extern "C"
{
#include <led_driver.h>
#include <runtime_error_stub.h>
}

class LedDriverTest : public ::testing::Test
{
protected:
  uint16_t virtual_leds;

  LedDriverTest()
  {
    led_driver_init(&virtual_leds);
  }

  ~LedDriverTest()
  {
  }

  // virtual void SetUp()
  // {
  // }

  // virtual void TearDown()
  // {
  // }
};

TEST_F(LedDriverTest, LedsOffOnInit)
{
  led_driver_turn_on_all();
  led_driver_init(&virtual_leds);
  ASSERT_EQ(ALL_LEDS_OFF, led_driver_get_state());
}

TEST_F(LedDriverTest, TurnOnSingleLed)
{
  led_driver_turn_on_led(1);
  ASSERT_EQ(0x1, led_driver_get_state());
}

TEST_F(LedDriverTest, TurnOffSingleLed)
{
  led_driver_turn_on_led(1);
  led_driver_turn_off_led(1);
  ASSERT_EQ(ALL_LEDS_OFF, led_driver_get_state());
}

TEST_F(LedDriverTest, TurnOnMultipleLeds)
{
  led_driver_turn_on_led(8);
  led_driver_turn_on_led(9);
  ASSERT_EQ(0x0180, led_driver_get_state());
}

TEST_F(LedDriverTest, TurnOffMultipleLeds)
{
  led_driver_turn_on_all();
  led_driver_turn_off_led(8);
  led_driver_turn_off_led(9);
  ASSERT_EQ(ALL_LEDS_ON & (~0x0180), led_driver_get_state());
}

TEST_F(LedDriverTest, TurnOnAllLeds)
{
  led_driver_turn_on_all();
  ASSERT_EQ(ALL_LEDS_ON, led_driver_get_state());
}

TEST_F(LedDriverTest, TurnOffAllLeds)
{
  led_driver_turn_on_all();
  led_driver_turn_off_all();
  ASSERT_EQ(ALL_LEDS_OFF, led_driver_get_state());
}

TEST_F(LedDriverTest, QueryLedIsOn)
{
  ASSERT_FALSE(led_driver_led_is_on(16));
  led_driver_turn_on_led(16);
  ASSERT_TRUE(led_driver_led_is_on(16));
}

TEST_F(LedDriverTest, LedMemoryIsNotReadDirectly)
{
  virtual_leds = ALL_LEDS_ON;
  led_driver_turn_on_led(8);
  ASSERT_EQ(0x80, virtual_leds);
}

TEST_F(LedDriverTest, OutOfBoundsProducesRuntimeError)
{
  led_driver_turn_on_led(-1);
  ASSERT_STREQ("LED Driver: out-of-bounds LED", runtime_error_get_last_message());
  ASSERT_EQ(-1, runtime_error_get_last_parameter());
}

/* Parameterized out of bounds tests */
class LedDriverTestParameterized : public LedDriverTest,
                                   public testing::WithParamInterface<int>
{
};

INSTANTIATE_TEST_CASE_P(LedDriverBoundsTests, LedDriverTestParameterized, ::testing::Values(-200, -1, 0, 17, 3081));

TEST_P(LedDriverTestParameterized, OutOfBoundsTurnOnDoesNoHarm)
{
  led_driver_turn_on_led(GetParam());
  ASSERT_EQ(ALL_LEDS_OFF, led_driver_get_state());
}

TEST_P(LedDriverTestParameterized, OutOfBoundsTurnOffDoesNoHarm)
{
  led_driver_turn_on_all();
  led_driver_turn_off_led(GetParam());
  ASSERT_EQ(ALL_LEDS_ON, led_driver_get_state());
}

TEST_P(LedDriverTestParameterized, OutOfBoundsLedsAlwaysOff)
{
  ASSERT_FALSE(led_driver_led_is_on(GetParam()));
}
