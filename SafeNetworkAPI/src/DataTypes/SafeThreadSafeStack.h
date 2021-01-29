/*****************************************************************
 * \file   SafeThreadSafeStack.h
 * \brief  
 * 
 * \author Can Karka
 * \date   January 2021
 * 
 * Copyright (C) 2021 Can Karka
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 *********************************************************************/

#pragma once


#include "SafeList.h"

#include <iostream>
#include <mutex>

template<typename T>
class SafeThreadSafeStack
	{
	private:
		SafeList<T> List;
		std::mutex Mutex;

		std::mutex MUXMutex;
		std::condition_variable CVBlocking;

	public:

		SafeThreadSafeStack()
			{
			}

		~SafeThreadSafeStack()
			{
			}

		SafeThreadSafeStack(const SafeThreadSafeStack<T> &) = default;
		SafeThreadSafeStack<T> &operator=(const SafeThreadSafeStack<T>&) = default;

		void Push(const T &value)
			{
			std::scoped_lock lock(Mutex);
			List.Append(value);

			std::unique_lock<std::mutex> ul(MUXMutex);
			CVBlocking.notify_one();
			}

		void Pop()
			{
			std::scoped_lock lock(Mutex);
			List.RemoveLast();
			}

		T Top()
			{
			std::scoped_lock lock(Mutex);
			return List.GetLast();
			}

		bool IsEmpty()
			{
			std::scoped_lock lock(Mutex);
			return List.IsEmpty();
			}

		uint32_t Size()
			{
			std::scoped_lock lock(Mutex);
			return List.Size();
			}

		void Clear()
			{
			std::scoped_lock lock(Mutex);
			List.Clear();
			}

		void Print()
			{
			std::scoped_lock lock(Mutex);
			List.Print();
			}

		void Wait()
			{
			while (IsEmpty())
				{
				std::unique_lock<std::mutex> ul(MUXMutex);
				CVBlocking.wait(ul);
				}
			}

		friend std::ostream &operator<<(std::ostream &stream, SafeThreadSafeStack<T> &stack)
			{
			std::scoped_lock lock(stack.Mutex);

			while (!stack.IsEmpty())
				{
				T element = stack.Top();
				stack.Pop();
				stream << element << std::endl;
				}

			return stream;
			}
	};

