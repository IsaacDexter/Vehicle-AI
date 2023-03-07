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

void TaskManager::MaintainTasks(const float deltaTime)
{
	std::list<Task*>::iterator it = m_tasks.begin();
	// loop while the iterator is not at the end
	while (it != m_tasks.end())
	{
		Task* task = (*it);
		task->Maintain(deltaTime);
		if (task->Check())
		{
			// delete the task. This will also assign(increment) the iterator to be the next item in the list
			it = m_tasks.erase(it);
			continue; // continue the next loop (we don't want to increment below as this will skip an item)
		}
		it++; // increment the iterator
	}
}

void TaskManager::Clear()
{
	std::list<Task*>::iterator it = m_tasks.begin();
	// loop while the iterator is not at the end
	while (it != m_tasks.end())
	{
		// delete the task. This will also assign(increment) the iterator to be the next item in the list
		it = m_tasks.erase(it);
		continue; // continue the next loop (we don't want to increment below as this will skip an item)
		it++; // increment the iterator
	}
}

void TaskManager::Break()
{
	bool broken = true;
}
