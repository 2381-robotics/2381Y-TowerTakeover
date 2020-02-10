#ifndef GLOBAL_AUTON_HPP
#define GLOBAL_AUTON_HPP
#include "./auto_sequence.hpp"

namespace Auton
{
    extern AutoSequence* AT_Skills;
    extern AutoSequence *AT_Blue5, *AT_Red5, *AT_Blue7, *AT_Red7;

    extern AutoSequence* AT_Test_Ultras;

    extern AutoTask StackTask;
    extern AutoTask DeployTask;
    extern AutoTask Turn90Deg, InvertTurn90Deg;
} // namespace Auton

#endif