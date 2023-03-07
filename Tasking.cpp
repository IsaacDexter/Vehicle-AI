#include "Tasking.h"

TaskManager::TaskManager()
{
	m_tasks = std::list<Task*>();
}

TaskManager::~TaskManager()
{
	Clear();
}

void TaskManager::AddTask(Task task)
{
	m_tasks.push_back(new Task(task));
}

void TaskManager::ExecuteTasks(const float deltaTime)
{
	for (Task* task : m_tasks)		//For each task...
	{
		task->Execute();				//Execute that task
		if (task->IsComplete())		//If that task is finished,
		{
			m_tasks.remove(task);	//Remove it from the list of tasks
		}
	}
}

void TaskManager::Clear()
{
	for (Task* task : m_tasks)		//For each task...
	{
		task->Clear();				//Execute that task
		m_tasks.remove(task);	//Remove it from the list of tasks
		delete task;
	}
}
