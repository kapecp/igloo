/* 
 * File:   stringendingwithconstraint.h
 * Author: joakim
 *
 * Created on December 25, 2009, 10:43 PM
 */

#ifndef _STRINGENDINGWITHCONSTRAINT_H
#define	_STRINGENDINGWITHCONSTRAINT_H

namespace igloo {

  class StringEndingWithConstraint : public Constraint<std::string> {
  public:

    //    StringEndingWithConstraint(const char* expected) : _expected(expected) {}

    StringEndingWithConstraint(std::string expected) : _expected(expected)
    {
    }

    virtual bool IsLogicalOperator() const
    {
      return false;
    }

    virtual bool Evaluate(std::string actual) const
    {
      if(_expected.size() > actual.size())
      {
        return false;
      }

      std::string endOfActual = actual.substr(actual.size()-_expected.size());

      return _expected.compare(endOfActual) == 0;
    }

    std::string ExpectationText() const
    {
      std::ostringstream str;
      str << "string ending with " << _expected;
      return str.str();
    }

  private:
    std::string _expected;
  };
}

#endif	/* _STRINGENDINGWITHCONSTRAINT_H */
