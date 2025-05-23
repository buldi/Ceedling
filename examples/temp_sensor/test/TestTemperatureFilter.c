/* =========================================================================
    Ceedling - Test-Centered Build System for C
    ThrowTheSwitch.org
    Copyright (c) 2010-25 Mike Karlesky, Mark VanderVoord, & Greg Williams
    SPDX-License-Identifier: MIT
========================================================================= */

#include "unity.h"
#include "Types.h"
#include "TemperatureFilter.h"
#include <math.h>

void setUp(void)
{
  TemperatureFilter_Init();
}

void tearDown(void)
{
}

void testShouldInitializeTemeratureToInvalidValue(void)
{
  TemperatureFilter_Init();
  TEST_ASSERT_FLOAT_IS_NEG_INF(TemperatureFilter_GetTemperatureInCelcius());
}

void testShouldInitializeTemperatureAfterCallToInit(void)
{
  TemperatureFilter_Init();
  TemperatureFilter_ProcessInput(17.8f);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 17.8f, TemperatureFilter_GetTemperatureInCelcius());

  TemperatureFilter_Init();
  TemperatureFilter_ProcessInput(32.6f);
  TEST_ASSERT_FLOAT_WITHIN(0.0001f, 32.6f, TemperatureFilter_GetTemperatureInCelcius());
}

void setValueAndVerifyResponse(float input, float response)
{
  float actual;
  TemperatureFilter_ProcessInput(input);
  actual = TemperatureFilter_GetTemperatureInCelcius();

  if (input == +INFINITY ||
      input == -INFINITY ||
      isnan(input))
  {
    TEST_ASSERT_FLOAT_IS_NEG_INF(actual);
  }
  else
  {
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, response, actual);
  }
}

//These ugly line-continuations are here just to show that you can use them:
void testShouldWeightEachSubsequentValue\
By25Percent\
AfterInitialValue(void)
{
  TemperatureFilter_Init();
  setValueAndVerifyResponse(0.0f, 0.0f);
  setValueAndVerifyResponse(10.0f, 2.5f);
  setValueAndVerifyResponse(10.0f, 4.375f);
  setValueAndVerifyResponse(10.0f, 5.78125f);

  TemperatureFilter_Init();
  setValueAndVerifyResponse(100.0f, 100.0f);
  setValueAndVerifyResponse(0.0f, 75.0f);
  setValueAndVerifyResponse(0.0f, 56.25f);
  setValueAndVerifyResponse(0.0f, 42.1875f);
}

void setInvalidTemperatureAndVerifyReinitialized(float invalidTemperature)
{
  TemperatureFilter_Init();
  setValueAndVerifyResponse(100.0f, 100.0f);
  setValueAndVerifyResponse(invalidTemperature, -INFINITY);
  setValueAndVerifyResponse(14.3f, 14.3f);
}

void testShouldResetAverageIfPassedInfinityOrInvalidValue(void)
{
  setInvalidTemperatureAndVerifyReinitialized(-INFINITY);
  setInvalidTemperatureAndVerifyReinitialized(+INFINITY);
  setInvalidTemperatureAndVerifyReinitialized(+NAN);
  setInvalidTemperatureAndVerifyReinitialized(-NAN);
}
