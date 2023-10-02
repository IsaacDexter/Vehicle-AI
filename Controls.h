#pragma once

namespace Controls
{
/// <summary>1: Activates movement mode.</summary>
static const WPARAM KEY_MOVEMENT			=0x31;
/// <summary>2: Activates automated decision making mode.</summary>
static const WPARAM KEY_DECISION			=0x32;
/// <summary>3: Activates alternative state moving mode.</summary>
static const WPARAM KEY_STATES				=0x33;

/// <summary><para>(Movement mode) A: Selected car moves to the last clicked location and comes to a slow stop.</para>
/// <para>(State Mode) A: Blue car arrives at a random waypoint on the map. This version of arrive uses a hierarchical state instead of a task. </para></summary>
static const WPARAM KEY_ARRIVE				=0x41;
/// <summary>(Movement mode) E: Selected car flees from unselected cars predicted position.</summary>
static const WPARAM KEY_EVADE				=0x45;
/// <summary>(Movement mode) F: Selected car flees from unselected cars current position.</summary>
static const WPARAM KEY_FLEE				=0x46;
/// <summary>(Movement mode) I: Selected car seeks to unselected cars predicted position.</summary>
static const WPARAM KEY_INTERCEPT			=0x49;
/// <summary>(Decision mode) M: Output both cars' money to the window.</summary>
static const WPARAM KEY_MONEY				=0x4D;
/// <summary>(Movement mode) O: Selected car arrivesat last clicked location, avoiding other vehicles through a whisker.</summary>
static const WPARAM KEY_OBSTACLE_AVOIDANCE	=0x4F;
/// <summary>(Movement mode) P: Selected car seeks to unselected cars previous position, a tailing behavior.</summary>
static const WPARAM KEY_PURSUIT				=0x50;
/// <summary>(Movement mode) R: Selected car rambles, moving to a random waypoint, queuing up a new random waypoint upon arrival.</summary>
static const WPARAM KEY_RAMBLE				=0x52;
/// <summary><para>(Movement mode) S: Selected car seeks to unselected cars current position.</para>
/// <para>(State Mode) S: Blue car seeks to a random waypoint on the map. This version of seek uses a state instead of a task. </para></summary>
static const WPARAM KEY_SEEK				=0x53;
/// <summary>(Movement mode) W: Selected car wanders, selecting a random direction and setting a waypoint a certain distance ahead of the car, offset in a random direction by a set radius </summary>
static const WPARAM KEY_WANDER				=0x57;

/// <summary>(All modes) SPACE: Clear all tasks and deactivate all states</summary>
static const WPARAM KEY_CLEAR				=0x20;
/// <summary><para>(Movement and State modes): LEFT ARROW: Select red car.</para>
/// <para>(Decision mode) LEFT ARROW: Activate red car.</para></summary>
static const WPARAM KEY_SELECT_RED_CAR		=VK_LEFT;
/// <summary><para>(Movement and State modes): RIGHT ARROW: Select blue car.</para>
/// <para>(Decision mode) RIGHT ARROW: Activate blue car.</para></summary>
static const WPARAM KEY_SELECT_BLUE_CAR		=VK_RIGHT;
}