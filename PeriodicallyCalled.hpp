#ifndef __PERIODICALLYCALLED_HPP_
#define __PERIODICALLYCALLED_HPP_

  /*!
    A mix-in protocol class for objects that have a function that's 
    called periodically by an event loop
  */
class PeriodicallyCalled 
  {
  public:
    /*!
      the adopting class supplies an implementation of this function.
    */
    virtual void periodic( void )= 0;
  
    virtual ~PeriodicallyCalled( ) =0;
  };



#endif
