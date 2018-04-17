//FSM.h
#ifndef STATEMACHINE_HPP_N08VM1TY
#define STATEMACHINE_HPP_N08VM1TY

#include <vector>
#include <map>

class StateMachine
{

public:
  static const int NullState;

  class State
  {
  public:
    State(StateMachine& machine_, int id_= StateMachine::NullState) : machine(machine_), stateID(id_) { };
    virtual ~State() { };
    virtual void action(){ };
    virtual void onEnter(){};
    virtual void onExit(){};
    
    int id() const { return stateID; };

  protected:
    StateMachine& machine;
    int stateID;
  };

private:
  State* currentState;
  std::vector<int> pendingStates;
  std::map<int, State*> states;

protected:
  void processPendingStateChanges();

public:
  StateMachine();
  ~StateMachine();

  void action( );
  void moveTo( int newStateID ) { pendingStates.push_back( newStateID ); };
  void moveImmediatelyTo( int newStateID ) { pendingStates.clear(); pendingStates.push_back( newStateID ); processPendingStateChanges(); };
  State& state() { return *currentState; };
  const State& state() const { return *currentState; };

  void addState( State* state___retained_and_deleted );
};

#endif /* end of include guard: STATEMACHINE_HPP_N08VM1TY */
