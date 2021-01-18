/*****************************************************************
 * \file   Queue.h
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

#include "Core/Core.h"
#include "DataTypes/List.h"

/**
 * 
 * This is a basic implementation of a Queue.
 * 
 */
template<class T>
class SafeQueue
	{
	private:
		SafeList<T> List;

	public:

		/**
		 * 
		 * Empty constructor.
		 * 
		 */
		SafeQueue()
			{
			}

		/**
		 * 
		 * Empty destructor.
		 * 
		 */
		~SafeQueue()
			{
			}

		/**
		 * 
		 * Appends a new element into the queue.
		 * 
		 * @param value The Value which should be added to the queue.
		 */
		void Enqueue(const T &value)
			{
			List.Append(value);
			}

		/**
		 * 
		 * Removes the first element of the queue.
		 * 
		 */
		void Dequeue()
			{
			List.RemoveFirst();
			}

		/**
		 * 
		 * Returns the first element of the queue.
		 * 
		 * @return Returns the Value of the queue.
		 */
		T Front()
			{
			return List.GetFirst();
			}

		/**
		 * 
		 * Checks whether the queue is empty.
		 * 
		 * @return Returns true if no elements are queued.
		 */
		bool IsEmpty()
			{
			return List.IsEmpty();
			}

		/**
		 * 
		 * Returns the current element count of the queue.
		 * 
		 * @return Returns the current element count of the queue.
		 */
		uint32 Size()
			{
			return List.Size();
			}

		/**
		 * 
		 * Prints the Queue to the console.
		 * 
		 */
		void Print()
			{
			List.Print();
			}

		/**
		 * 
		 * Prints the Queue to a stream.
		 * 
		 * @param stream The output stream where the queue should be printed into.
		 * @param queue The queue which should be printed.
		 * @return Returns a reference to the output stream with the printed queue.
		 */
		friend std::ostream &operator<<(std::ostream &stream, SafeQueue<T> &queue)
			{
			while (!queue.IsEmpty())
				{
				T content = queue.Front();
				queue.Dequeue();
				stream << content << std::endl;
				}

			return stream << "\n";
			}
	};

