#include "Plato_SIMP.hpp"

#include <gtest/gtest.h>

namespace PlatoTestSIMP
{
TEST(SIMP, DefaultCtor)
{
  // Default constructed is line with slope 1
  const Plato::SIMP tSimp;
  
  EXPECT_EQ(tSimp.eval(0.0), 0.0);
  EXPECT_EQ(tSimp.eval(0.5), 0.5);
  EXPECT_EQ(tSimp.eval(1.0), 1.0);

  EXPECT_EQ(tSimp.grad(0.0), 1.0);
  EXPECT_EQ(tSimp.grad(0.5), 1.0);
  EXPECT_EQ(tSimp.grad(1.0), 1.0);
}

TEST(SIMP, Constant)
{
  const Plato::SIMP tSimp(0.0, 0.25);
  
  EXPECT_EQ(tSimp.eval(0.0), 1.0);
  EXPECT_EQ(tSimp.eval(0.5), 1.0);
  EXPECT_EQ(tSimp.eval(1.0), 1.0);

  EXPECT_EQ(tSimp.grad(0.0), 0.0);
  EXPECT_EQ(tSimp.grad(0.5), 0.0);
  EXPECT_EQ(tSimp.grad(1.0), 0.0);
}

TEST(SIMP, Linear)
{
  const Plato::SIMP tSimp(1.0, 0.5);
  
  EXPECT_EQ(tSimp.eval(0.0), 0.5);
  EXPECT_EQ(tSimp.eval(0.5), 0.75);
  EXPECT_EQ(tSimp.eval(1.0), 1.0);

  EXPECT_EQ(tSimp.grad(0.0), 0.5);
  EXPECT_EQ(tSimp.grad(0.5), 0.5);
  EXPECT_EQ(tSimp.grad(1.0), 0.5);
}

TEST(SIMP, Quadratic)
{
  const Plato::SIMP tSimp(2.0, 0.0);
  
  EXPECT_EQ(tSimp.eval(0.0), 0.0);
  EXPECT_EQ(tSimp.eval(0.5), 0.25);
  EXPECT_EQ(tSimp.eval(1.0), 1.0);

  EXPECT_EQ(tSimp.grad(0.0), 0.0);
  EXPECT_EQ(tSimp.grad(0.5), 1.0);
  EXPECT_EQ(tSimp.grad(1.0), 2.0);
}
}
