/*****************************************************************
 * \file   SafeStack.h
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

template<typename T>
class SafeStack
	{
	private:
		SafeList<T> List;

	public:

		SafeStack()
			{
			}

		~SafeStack()
			{
			}

		SafeStack(const SafeStack<T>&) = default;
		SafeStack<T> &operator=(const SafeStack<T>&) = default;

		void Push(const T &value)
			{
			List.Append(value);
			}

		void Pop()
			{
			List.RemoveLast();
			}

		T Top()
			{
			return List.GetLast();
			}

		bool IsEmpty()
			{
			return List.IsEmpty();
			}

		uint32_t Size()
			{
			return List.Size();
			}

		void Clear()
			{
			List.Clear();
			}

		void Print()
			{
			List.Print();
			}

		friend std::ostream &operator<<(std::ostream &stream, SafeStack<T> &stack)
			{
			while (!stack.IsEmpty())
				{
				T element = stack.Top();
				stack.Pop();
				stream << element << std::endl;
				}

			return stream;
			}
	};

