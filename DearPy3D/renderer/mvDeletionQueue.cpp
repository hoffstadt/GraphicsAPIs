#include "mvDeletionQueue.h"

namespace DearPy3D {

	void mvDeletionQueue::pushDeletor(std::function<void()>&& function)
	{
		_deletors.push_back(function);
	}

	void mvDeletionQueue::flush()
	{
		for (auto it = _deletors.rbegin(); it != _deletors.rend(); it++)
			(*it)();
		_deletors.clear();
	}

}