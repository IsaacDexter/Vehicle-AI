#pragma once
#include <functional>
#include <list>

struct Task
{
private:
	/// <summary>The task to perform.</summary>
	std::function<void()> task;
	/// <summary>The end condition for the task. Set to true for the task to only complete once</summary>
	std::function<bool()> end;
	/// <summary>How many times the task has been executed within this instance.</summary>
	int executions;

public:
	Task(std::function<void()> task, std::function<bool()> endCondition)
	{
		this->task = task;
		this->end = endCondition;
		this->executions = 0;
	}
	Task()
	{
		Clear();
	}
	void Execute()
	{
		task();
		executions++;
		if (end())
		{
			Clear();
		}
	}
	/// <returns>Whether or not the task has ended. The same as calling task.end().</returns>
	bool IsComplete()
	{
		return end();
	}
	/// <summary>Forces the task's end condition to be true, ending the task.</summary>
	void Cease()
	{
		this->end = [] { return true; };
	}
	/// <summary>
	/// 
	/// </summary>
	void Clear()
	{
		this->task = [] {; };	//Set task to a blank, empty lambda
		this->end = [] { return true; };	//Set end condition to be always true as there is no task to complete
	}
};

class TaskManager
{
public:
	TaskManager();
	~TaskManager();
	void AddTask(Task task);
	void ExecuteTasks(const float deltaTime);
	void Clear();
	void Break();
private:
	/// <summary>A queue containing paramaterless lambdas that contain the function calls with parameters in them.
	/// <para>Essentially allows the class to queue tasks to be called at another time, in this case in the update function.</para>
	/// <para>Definitely an unconventional way of doing this, but this is the most versatile and reusable way I could think of:</para>
	/// <para>I want these classes to see more life as more than just cars. It also saves having to do a check each frame to see if we are seeking something.</para></summary>
	std::list<Task*> m_tasks;
	//I want to thank fredoverflow's answer( https://stackoverflow.com/a/22109991 ) for helping me understand the concept of a queue of function pointers.
	//I want to thank Johannes Schaub - litb's answer ( https://stackoverflow.com/a/2402607 ) for helping me undestand function pointers to member functions.
};

