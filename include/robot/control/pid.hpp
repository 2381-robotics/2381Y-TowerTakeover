#ifndef PID_HPP_
#define PID_HPP_

class Pid {
 public:
  /**
   * Constructor
   * Accepts the kp, ki, and kd to determine the correction value
   * Defaults to (0,0,0)
   */
   Pid(double kp = 0, double ki = 0, double kd = 0);
  /**
   * Resets the error counts. It should be called when the PID loop is not
   * active to prevent integral windup.
   */
  void ResetError();
  double last_error_;
  double error_sum_;
  /**
   * Returns the output of the PID controller correcting the input.
   * @param setpoint The current setpoint value
   * @param current_value The current value that will be compared with the setpoint
   */
  double Update(double setpoint, double current_value);

  void Set_Error(double error);


  double kp_;
  double ki_;
  double kd_;
 private:
  // PID constants (Proportional (P), Integral (I), Derivative (D))

  // Summation of errors used in the integral term


  // The last error value to find the difference with the current error value
  // for the derivative term.

};

#endif
