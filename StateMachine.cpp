#include "StateMachine.hpp"

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

const int StateMachine::NullState = -1;

///////////////////////////////////////////////////////////////////////

StateMachine::StateMachine( )
{
  currentState = new State(*this,NullState);
  states[NullState]= currentState;// ensure there's a null state in place
}

///////////////////////////////////////////////////////////////////////

StateMachine::~StateMachine( )
{
  for (std::map<int, State*>::const_iterator i= states.begin(); i!=states.end(); ++i)
  {
    delete i->second;
  }
  
}

///////////////////////////////////////////////////////////////////////

void StateMachine::addState( StateMachine::State* s )
{
  int newid = s->id();
  
  if (newid == currentState->id() && s != currentState )
  {
    // replacing the currently-executing state is undefined, but it shouldn't
    // cause an access fault.
    currentState = s;
  }

  std::map<int,State*>::iterator place = states.find(s->id());
  if ( place != states.end())
  { // state with this id already present. Replace it.
    delete place->second;
    place->second = s;
  }
  else
  {
    states[s->id()] = s;
  }

}

///////////////////////////////////////////////////////////////////////

void StateMachine::processPendingStateChanges()
{
  for (std::vector<int>::const_iterator i=pendingStates.begin(); i!=pendingStates.end(); ++i )
  {
    if (*i != currentState->id())
    {
      currentState->onExit();
      
      std::map<int,State*>::iterator place = states.find(*i);
      if ( place != states.end())
      {
        currentState = place->second;
      }
      else
      { // caller-supplied state doesn't exist. Enter Null state
        currentState = states[NullState];
        // note,this logic may cause call of <Null state>.onEnter() if we were in null state 
        // already, but as that function does nothing, it's begign  
      }
      currentState->onEnter();  
    }
  }
  pendingStates.clear();
}

///////////////////////////////////////////////////////////////////////

void StateMachine::action()
{
  currentState->action();
  processPendingStateChanges();
}

