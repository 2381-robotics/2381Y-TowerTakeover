#ifndef GLOBAL_AUTON_HPP
#define GLOBAL_AUTON_HPP
#include "./auto_sequence.hpp"

namespace Auton
{
    extern AutoSequence* AT_Skills;
    extern AutoSequence *AT_Blue5, *AT_Red5, *AT_Blue7, *AT_Red7;

    extern AutoSequence* CUS_FULL, *CUS_Q1, *CUS_Q2, *CUS_Q3, *CUS_Q4;

    extern AutoSequence* AT_Test_Ultras;

    extern AutoTask StackTask();
    extern AutoTask ArmTask();
    extern AutoTask DeployTask();
    extern AutoTask Turn90Deg();
    extern AutoTask InvertTurn90Deg();
} // namespace Auton

#endif