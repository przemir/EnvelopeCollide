#ifndef ISDERIVEDFROM_H
#define ISDERIVEDFROM_H

namespace EnvelopeCollide { namespace System {

//////////////////////////////////////////////////////////////////
// code from: http://www.gotw.ca/publications/mxc++-item-4.htm
template<typename D, typename B>
class TreeIsDerivedFrom
{
  static void Constraints(D* p)
  {
    B* pb = p;
    pb = p; // suppress warnings about unused variables
  }

protected:
  TreeIsDerivedFrom()
  {
      void(*p)(D*) = Constraints;
  }
};

// Force it to fail in the case where B is void
template<typename D>
class TreeIsDerivedFrom<D, void>
{
  TreeIsDerivedFrom() { char* p = (int*)0; /* error */ }
};
// code from: http://www.gotw.ca/publications/mxc++-item-4.htm
////////////////////////////////////////////////////////////////

}}

#endif // ISDERIVEDFROM_H
