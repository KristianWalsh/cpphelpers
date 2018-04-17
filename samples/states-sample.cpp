#include <iostream>
#include "StateMachine.hpp"


/* 
    The state machine has two states, Moo and Baa
    
    In state Moo, "action" will say "moo" for three times, then move to 

*/

    // state names
enum { kMoo=1, kBaa=2 };

    // State definition for Moo state
class MooState : public StateMachine::State
{
  public:
    MooState(StateMachine& s) : StateMachine::State( s, kMoo ), actionCount(0),exitCount(0),enterCount(0) { }
    int actionCount;
    int exitCount;
    int enterCount;

    virtual void onExit() { std::cout<<"Exiting state MOO"<<std::endl; actionCount=0; }
    virtual void onEnter() { std::cout<<"Entering state MOO"<<std::endl; }
    virtual void action() { ++actionCount; std::cout<<"MOO! "<<actionCount<<std::endl; if (actionCount==3) { machine.moveTo(kBaa);} } 
};


    // State definition for Baa state.
class BaaState : public StateMachine::State
{
  public:
    BaaState(StateMachine& s) : StateMachine::State( s, kBaa ), actionCount(0) { }
    int actionCount;

    virtual void onEnter() { std::cout<<"Entering state BAA"<<std::endl; actionCount=0; }
    virtual void action() { ++actionCount; std::cout<<"BAA! "<<actionCount<<std::endl; if (actionCount==2) { machine.moveTo(kMoo);} }
    virtual void onExit() { std::cout<<"Exiting state BAA"<<std::endl; }
};


int main(int argc, const char** argv)
{
  StateMachine m;

    // initial state is -1, undefined.
  if (m.state().id()==StateMachine::NullState)
  {
    std::cout<<"State machine starts in undefined state"<<std::endl;
  }

    // create the two states
  MooState* moo = new MooState(m);
  m.addState( moo );
  BaaState* baa = new BaaState(m);
  m.addState( baa );

  // often it's enough to do m.addState(new MooState(m));  but we want to remember the state object,
  // rather than push it into the statemachine and forget about it.

    // request move to "Moo" state
  m.moveTo(kMoo);
    // the state-machine won't change state until it has been "cranked" again,
    // by calling m.action(). By contrast, calling moveImmediatelyTo() effects
    // the change there and then.
  if (m.state().id()==StateMachine::NullState)
  {
    std::cout<<"State machine still in undefined state after call to moveTo() "<<std::endl;
  }
  
  std::cout<<"bring state-machine out of NullState "<<std::endl;
  m.action(); // exit NullState, enter Moo.

  std::cout<<"crank state-machine three times "<<std::endl;
  m.action();
  m.action();  
  m.action(); // third call of action() while in Moo state means change to Baa

  // after two calls in Baa, Baa::action will request change to Moo
  std::cout<<"crank state-machine twice "<<std::endl;
  m.action(); 
  m.action(); 

  std::cout<<"crank state-machine once "<<std::endl;
  m.action(); // "Moo"
  

  std::cout<<"request immediate move to BAA "<<std::endl;
  m.moveImmediatelyTo(kBaa);
  // moveImmediatelyTo() calls all pending state-change requests without calling action() in between

  std::cout<<"crank state-machine once "<<std::endl;
  m.action(); // "Baa"

}