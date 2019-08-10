#ifndef PID_H_
#define PID_H_

class Pid {
 public:
  /**
   * Constructor
   * Accepts the kp, ki, and kd to determine the correction value
   * Defaults to (0,0,0)
   */
   Pid(double* kp = 0, double* ki = 0, double* kd = 0);
  /**
   * Resets the error counts. It should be called when the PID loop is not
   * active to prevent integral windup.
   */
  void ResetError();

  /**
   * Returns the output of the PID controller correcting the input.
   * @param goal The current setpoint value
   * @param current_value The current value that will be compared with the setpoint
   */
  double Update(double setpoint, double current_value);

 private:
  // PID constants (Proportional (P), Integral (I), Derivative (D))
  double* kp_;
  double* ki_;
  double* kd_;

  // Summation of errors used in the integral term
  double error_sum_;

  // The last error value to find the difference with the current error value
  // for the derivative term.
  double last_error_;
};

#endif
