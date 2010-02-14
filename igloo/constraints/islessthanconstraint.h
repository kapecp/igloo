#ifndef IGLOO_ISLESSTHANCONSTRAINT_H
#define IGLOO_ISLESSTHANCONSTRAINT_H

#include <igloo/constraints/expressions/expression.h>

namespace igloo {

  template< typename ExpectedType >
  struct IsLessThanConstraint : Expression< IsLessThanConstraint<ExpectedType> >
  {
    IsLessThanConstraint(const ExpectedType& expected)
      : m_expected(expected)
    {
    }

    template<typename ActualType>
    bool operator()(const ActualType& actual) const
    {
      return (actual < m_expected);
    }

    ExpectedType m_expected;
  };

  template< typename ExpectedType >
  inline IsLessThanConstraint<ExpectedType> IsLessThan(const ExpectedType& expected)
  {
    return IsLessThanConstraint<ExpectedType>(expected);
  }

  inline IsLessThanConstraint<std::string> IsLessThan(const char* expected)
  {
    return IsLessThanConstraint<std::string>(expected);
  }

  template< typename ExpectedType >
  struct Stringizer< IsLessThanConstraint< ExpectedType > >
  {
    static std::string ToString(const IsLessThanConstraint<ExpectedType>& constraint)
    {
      std::ostringstream builder;
	  builder << "less than " << igloo::Stringize(constraint.m_expected);

      return builder.str();
    }
  };
}
#endif