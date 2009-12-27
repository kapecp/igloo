/* 
 * File:   stringendingwithconstraint.h
 * Author: joakim
 *
 * Created on December 25, 2009, 10:43 PM
 */

#ifndef _STRINGENDINGWITHCONSTRAINT_H
#define	_STRINGENDINGWITHCONSTRAINT_H

namespace igloo {

  class StringEndingWithConstraint : public Constraint<std::string>, public IEvaluateConstraint<std::string>
  {
  public:

    StringEndingWithConstraint(std::string expected) : Constraint<std::string>(expected)
    {
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
  };
}

#endif	/* _STRINGENDINGWITHCONSTRAINT_H */
