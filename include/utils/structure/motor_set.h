#ifndef MOTOR_SET_H
#define MOTOR_SET_H

#include <list>
#include <map>



class Motor_Set{
public:
  Motor_Set(std::list<char>* parameters);

  template <typename T1, typename T2>
  std::map<char, T1> Create_Fields (std::list<T2> arguments, T1 (*f)(T2));

private:
  std::list<char>* _parameters;
};



#endif
