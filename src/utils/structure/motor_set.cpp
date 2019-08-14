#include "utils/structure/motor_set.h"

Motor_Set::Motor_Set(std::list<char>* parameters){
  _parameters = parameters;
}

template <typename T1, typename T2>
std::map<char, T1> Motor_Set::Create_Fields (std::list<T2> arguments, T1 (*f)(T2)){
  std::map<char, T1> result;

  std::list<char>::iterator parameters_it = _parameters->begin();
  __gnu_cxx::iterator arguments_it = arguments.begin();
  for(; parameters_it!=_parameters->end()&&arguments_it!= arguments.end(); parameters_it++, arguments_it++){
    result[parameters_it] = (*f)(arguments_it);
  }
  return result;
}
