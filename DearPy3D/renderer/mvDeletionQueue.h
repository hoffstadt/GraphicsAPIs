#pragma once

#include <functional>
#include <deque>

namespace DearPy3D {

	class mvDeletionQueue
	{

	public:

		void pushDeletor(std::function<void()>&&);
		void flush();

	private:

		std::deque<std::function<void()>> _deletors;
	};

}