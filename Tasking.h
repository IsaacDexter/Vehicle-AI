#pragma once

#include <functional>
#include <list>

struct Task
{
private:
	/// <summary>The task to perform when the task is instanciated. The equivalent of battlecry.</summary>
	std::function<void()> execute;
	/// <summary>The task to perform each frame. The equivalent of at start of turn.</summary>
	std::function<void(float dt)> maintain;
	/// <summary>The task to perform upon completion. The equivalent of deathrattle.</summary>
	std::function<void()> complete;
	/// <summary>The end condition for the task. Set to true for the task to only complete once</summary>
	std::function<bool()> check;

public:
	/// <summary>A task is a collection of functions that conclude, check and clear themsleves.</summary>
	/// <param name="execute">Battlecry:</param>
	/// <param name="maintain">At the start of each turn:</param>
	/// <param name="complete">Deathrattle:</param>
	/// <param name="check">The check to see if the task has completed itself.</param>
	Task(
		std::function<void()> execute,
		std::function<void(float dt)> maintain,
		std::function<void()> complete,
		std::function<bool()> check
	)
	{
		this->execute = execute;
		this->maintain = maintain;
		this->complete = complete;
		this->check = check;

		Execute();
	}
	/// <summary>A task is a collection of functions that conclude, check and clear themsleves.</summary>
	Task()
	{
		Clear();

		Execute();
	}

	~Task()
	{
		Clear();
	}
	/// <summary>Called automatically when the task comes into existence. Will perform a check to see if it's complete, and will complete then clear itself if so.</summary>
	void Execute()
	{
		execute();
		check();
	}
	/// <summary>Should be called each turn by the task manager. Will perform a check to see if it's complete, and will complete then clear itself if so.</summary>
	void Maintain(float dt)
	{
		maintain(dt);
		check();
	}
	/// <summary>Called automatically when the end condition is true, or can be called manually to end the task. Will clear the task, rendering it blank and ready to delete.</summary>
	void Complete()
	{
		complete();
		Clear();
	}

	/// <returns>Called automatically through execution and maintainance. Can be called externally, where it will return whether or not the task is complete. When it is, it will call complete, then clear itself.</returns>
	bool Check()
	{
		bool check = this->check();
		if (check)
		{
			Complete();
		}
		return check;
	}

	/// <summary>Sets execute, maintain and complete to blank, empty lambdas. Check will return true, as the task will be complete. Can be used to cancel a task early.</summary>
	void Clear()
	{
		this->execute = [] {; };			//Set task to a blank, empty lambda
		this->maintain = [](float dt) {; };	//Set task to a blank, empty lambda
		this->complete = [] {; };			//Set task to a blank, empty lambda
		this->check = [] { return true; };	//Set end condition to be always true as there are no tasks left to complete
	}
};

class TaskManager
{
public:
	TaskManager();
	~TaskManager();
	void AddTask(Task task);
	void MaintainTasks(const float deltaTime);
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

